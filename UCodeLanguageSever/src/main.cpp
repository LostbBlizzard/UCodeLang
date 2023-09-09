
#include <iostream>
#include "LSPSever.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <UCodeLang/LangCore/StringHelper.hpp>

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

UCodeLang::String FilePath = "C:/CoolStuff/CoolCodeingStuff/cpp/UCodeLang/UCodeLanguageSever/Msg.txt";
std::ofstream File = std::ofstream(FilePath);
std::mutex Lock = {};
void LogMSG(const UCodeLang::String& Str)
{
	Lock.lock();
	//std::cerr << Str << std::endl;
	File << Str << std::endl;
	Lock.unlock();
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


				LogMSG("Starting ULang Sever");


				std::thread SeverThread([]()
					{
						UCodeLanguageSever::LSPSever Sever;
						SeverPtr = &Sever;


						while (Sever.Step());
						SeverPtr = nullptr;
					}
				);


				std::thread OutThread([](std::thread* SeverThread)
					{
						while (SeverThread->joinable())
						{
							if (SeverPtr)
							{
								auto List = SeverPtr->GetPackets();
								for (auto& Item : List)
								{
									LogMSG("Sent Packet:" + Item._Data);

									std::cout << Item.ToLanguageServerString();
									std::cout.flush();
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
						LogMSG("Got Packet:" + p._Data);

						SeverPtr->AddPacket(std::move(p));
					}

				}





				LogMSG("Sever End");


			}

		}
	}
}


int main(int argc, char* argv[])
{
	while (true);
	for (size_t i = 0; i < argc; i++)
	{
		char* Arg = argv[i];
		RunArg(UCodeLang::String_view(Arg));
	}

	return 0;
}

