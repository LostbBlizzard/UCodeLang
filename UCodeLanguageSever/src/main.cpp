
#include <iostream>
#include "Sever.hpp"

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
	bool IsReading = false;
	for (size_t i = 0; i < View.size(); i++)
	{
		char V = View[i];

		if (IsInNameCharList(V))
		{
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
	ToUpdate.substr(Size);
	return { View.data(),Size };
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
			int Port = ReadNumber(View, View);

			{
				UCodeLanguageSever::Sever sever = {};
				sever.RunOnSever(SeverIp, Port);

				while (sever.Step());
			}

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

	//Test
	RunArg("--Start 127.0.0.1 65535");

	return 0;
}