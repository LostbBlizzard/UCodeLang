
#include <iostream>
#include "LanguageSever.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <fstream>

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


std::string_view ReadString(std::string_view View, std::string_view& ToUpdate)
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

int ReadNumber(std::string_view View, std::string_view& ToUpdate)
{
	auto Str = ReadString(View, ToUpdate);


	std::string TepStr = (std::string)Str;
	auto Value = std::atoi(TepStr.c_str()); 

	return Value;
}

std::string FilePath = "C:/CoolStuff/CoolCodeingStuff/C++/Projects/UCodeLang/UCodeLanguageSever/Msg.txt";
std::ofstream File = std::ofstream(FilePath);
std::mutex Lock = {};
void LogMSG(const std::string& Str)
{
	Lock.lock();
	//std::cerr << Str << std::endl;
	File << Str << std::endl;
	Lock.unlock();
}

//Args
//--Start [SeverIp] [Port] 
void RunArg(std::string_view View)
{
	if (View._Starts_with("--"))
	{
		View = View.substr(2);

		if (View._Starts_with("Start"))
		{
			size_t ArgNameSize = sizeof("Start") - 1;
			View = View.substr(ArgNameSize);

			std::string SeverIp = (std::string)ReadString(View,View);
			//if (SeverIp == (std::string)"stdio")
			{
				static UCodeLanguageSever::LanguageSever* SeverPtr = nullptr;
				SeverPtr = nullptr;


				LogMSG("Starting ULang Sever");
				while (true);

				std::thread SeverThread([]()
					{
						UCodeLanguageSever::LanguageSever Sever;
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
		else
		{
			LogMSG("Bad Arg");
		}
	}
}


int main(int argc, char* argv[])
{
	for (size_t i = 0; i < argc; i++)
	{
		char* Arg = argv[i];
		RunArg(std::string_view(Arg));
	}

	return 0;
}

