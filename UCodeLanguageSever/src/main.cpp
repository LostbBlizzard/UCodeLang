
#include <iostream>
#include "LanguageSever.hpp"
#include <iostream>
#include <string>
#include <thread>

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

				while (true);

				std::thread SeverThread([]()
					{
						UCodeLanguageSever::LanguageSever Sever;
						SeverPtr = &Sever;

						

						while (Sever.Step())
						{

						}
						SeverPtr = nullptr;
					}
				);

				std::this_thread::sleep_for(std::chrono::seconds(3));//so thread can start up

				std::thread OutThread([](std::thread* SeverThread)
				{
						while (SeverThread->joinable())
						{
							if (SeverPtr)
							{
								auto List = SeverPtr->GetPackets();
								for (auto& Item : List)
								{

									std::cout << Item._Data;
								}
							}
						}
				},&SeverThread);


				bool ReadingPacketSize = false;
				std::string Buffer;
				std::string NumberBuffer;
				size_t PacketSize = 0;
				while (SeverThread.joinable())
				{
					char V;
					std::cin.read(&V, 1);
					Buffer += V;


					if (PacketSize == 0) 
					{
						if (ReadingPacketSize == false)
						{
							if (Buffer == "Content-Length:")
							{
								Buffer.clear();
								ReadingPacketSize = true;
							}
						}
						else
						{
							bool IsNum = IsInNumCharList(V);
							if (NumberBuffer.size())
							{
								if (!IsNum)
								{
									PacketSize = std::stoi(NumberBuffer);
									PacketSize += 1;
									Buffer.clear();
									NumberBuffer.clear();
								}
								else
								{
									NumberBuffer += V;
								}
							}
							else
							{
								if (IsNum)
								{
									NumberBuffer += V;
								}
							}

						}

					}
					else
					{
						PacketSize--;


						if (PacketSize == 0)
						{
							UCodeLanguageSever::ClientPacket p;
							p._Data = std::move(Buffer);
							p._Data = p._Data.substr(1);

							SeverPtr->AddPacket(std::move(p));


							{
								//reset
								PacketSize = 0;
								ReadingPacketSize = false;
							}
						}
					}
				}
			}

			/*
			else
			{
				int Port = ReadNumber(View, View);

				{
					UCodeLanguageSever::Sever sever = {};
					sever.RunOnSever(SeverIp, Port);

					while (sever.Step());
				}
			}
			*/
		}
		else
		{
			std::cout << "Bad Arg\n";
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
	RunArg("--Start stdio");
	std::cout << "Done\n";


	char V;
	std::cin >> V;

	return 0;
}

