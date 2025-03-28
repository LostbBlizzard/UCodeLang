
#include <iostream>
#include "LSPSever.hpp"
#include <iostream>
#include <string>
#include <string_view>
#include <thread>
#include <fstream>
#include <condition_variable>
#include <UCodeLang/LangCore/StringHelper.hpp>
#if UCodeLang_Platform_Windows
#include <windows.h>
#endif
const char NumberCharList[] = "1234567890";
bool IsInNumCharList(char Value)
{
	for (size_t i = 0; i < sizeof(NumberCharList); i++)
	{
		if (Value == NumberCharList[i])
		{
			return true;
		}
	}
	return false;
}

const char NameCharList[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890,.':/\\";
bool IsInNameCharList(char Value)
{
	for (size_t i = 0; i < sizeof(NameCharList); i++)
	{
		if (Value == NameCharList[i])
		{
			return true;
		}
	}
	return false;
}


UCodeLang::String_view ReadString(UCodeLang::String_view View, UCodeLang::String_view& ToUpdate)
{
	size_t Size =0;
	size_t Start = 0;
	bool IsReading = false;
	for (size_t i = 0; i < View.size(); i++)
	{
		char V = View[i];

		if (IsInNameCharList(V))
		{
			if (IsReading == false)
			{
				Start = i;
			}
			IsReading = true;
			
		}
		else
		{
			if (IsReading) {
				break;
			}
		}
		Size++;
	}
	ToUpdate = ToUpdate.substr(Size);
	return { View.data()+ Start,Size };
}

int ReadNumber(UCodeLang::String_view View, UCodeLang::String_view& ToUpdate)
{
	auto Str = ReadString(View, ToUpdate);


	UCodeLang::String TepStr = (UCodeLang::String)Str;
	auto Value = std::atoi(TepStr.c_str()); 

	return Value;
}

std::ofstream File("uclanglsplogs.txt");
void LogMSG(const UCodeLang::String& Str)
{
	File << Str << std::endl;
}

//Args
//start [SeverIp] [Port] 
//start stdio

void RunArg(UCodeLang::String_view View)
{


	using StrHelp = UCodeLang::StringHelper;
	if (StrHelp::StartWith(View, "start"))
	{
		View = View.substr(sizeof("start") - 1);

		for (size_t i = 0; i < View.size(); i++)
		{
			if (View[i] != ' ')
			{
				View = View.substr(i);
				break;
			}
		}

		if (StrHelp::StartWith(View, "stdio"))
		{
			View = View.substr(sizeof("stdio") - 1);

			{
				static UCodeLanguageSever::LSPSever* SeverPtr = nullptr;
				SeverPtr = nullptr;

				#if UCodeLangDebug
				LogMSG("Starting ULang Sever");
				#endif

				std::mutex severm;
				std::condition_variable cv;

				std::mutex severout;
				std::condition_variable outcv;
				size_t count = 0;
				size_t count2 = 0;

				std::thread SeverThread([&count2,&count,&cv,&severm,&outcv]()
					{
						UCodeLanguageSever::LSPSever Sever;
						Sever.LspDebugLogCallback = [](std::string_view msg)
						{
							std::string str = "LspSever Log[";
							str += msg;
							str += "]";
								
							LogMSG(str);
						};

						SeverPtr = &Sever;
						size_t mycount = 0;
						size_t oldsize = Sever.PacketCount();

						bool keepgoing = true;
						while (keepgoing)
						{
							std::unique_lock lk(severm);
							cv.wait(lk, [&] { return count != mycount; });
							mycount = count;

							keepgoing = Sever.Step();

							if (oldsize != Sever.PacketCount())
							{
								count2++;
								outcv.notify_one();
							}
						}
						SeverPtr = nullptr;
					}
				);


				std::thread OutThread([&count2,&severout,&outcv](std::thread* SeverThread)
					{
						size_t mycount =0;
						while (SeverThread->joinable())
						{
							std::unique_lock lk(severout);
							outcv.wait(lk, [&] { return mycount != count2; });
							mycount = count2;
							if (SeverPtr)
							{
								auto List = SeverPtr->GetPackets();
								for (auto& Item : List)
								{
									#if UCodeLangDebug
									LogMSG("Sent Packet:" + Item._Data);
									#endif

									auto s = Item.ToLanguageServerString();

									std::cout << s << std::flush;
								}
							}
						}
					}, &SeverThread);


				UCodeLanguageSever::ClientPacket::StreamState state;
				while (SeverThread.joinable())
				{
					char V;
					std::cin >> V;
					auto packet_op = UCodeLanguageSever::ClientPacket::Stream(state, V);

					if (packet_op.has_value())
					{
						UCodeLanguageSever::ClientPacket& p = packet_op.value();


						#if UCodeLangDebug
						LogMSG("Got Packet:" + p._Data + "\n");
						#endif

						SeverPtr->AddPacket(std::move(p));
						count++;
						cv.notify_one();
					}

				}
				#if UCodeLangDebug
				LogMSG("Sever End");
				#endif

			}

		}
	}
}


int main(int argc, char* argv[])
{

	bool IsDebuging = false;
	#if UCodeLang_Platform_Windows
	IsDebuging = IsDebuggerPresent();
	#endif
	#if UCodeLang_Platform_Windows
	#if UCodeLangDebug
	if (IsDebuging)
	{
		namespace fs = std::filesystem;
		auto ucodebinpath = UCodeLang::LangInfo::GetUCodeGlobalBin();
		auto Ulangexepath = ucodebinpath / UCodeLang::Path("uclanglsp.exe");
		UCodeLang::Path ThisRuningExePath = fs::absolute(argv[0]);
		
		if (ThisRuningExePath != Ulangexepath)
		{
			fs::copy_file(ThisRuningExePath, Ulangexepath,fs::copy_options::overwrite_existing);

			auto p1 = ThisRuningExePath.replace_extension(".pdb");
			auto p2 = Ulangexepath.replace_extension(".pdb");
			fs::copy_file(p1,p2, fs::copy_options::overwrite_existing);
		}
	}
	#endif // DEBUG
	#endif
	LogMSG("Sever main");
	for (size_t i = 1; i < argc; i++)
	{
		char* Arg = argv[i];
		RunArg(UCodeLang::String_view(Arg));
	}

	return 0;
}

