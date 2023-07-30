#include "LanguageSever.hpp"
#include <unordered_map>
#include <functional>
LanguageSeverStart
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
Optional<SeverPacket> SeverPacket::Stream(StreamState& State, char Char)
{
	State.Buffer += Char;


	if (State.PacketSize == 0)
	{
		if (State.ReadingPacketSize == false)
		{
			if (State.Buffer == "Content-Length:")
			{
				State.Buffer.clear();
				State.ReadingPacketSize = true;
			}
		}
		else
		{
			bool IsNum = IsInNameCharList(Char);
			if (State.NumberBuffer.size())
			{
				if (!IsNum)
				{
					State.PacketSize = std::stoi(State.NumberBuffer) - 3;//the \n,\r,\n,\r. and this char
					State.Buffer.clear();
					State.NumberBuffer.clear();

					State.Buffer += Char;
				}
				else
				{
					State.NumberBuffer += Char;
				}
			}
			else
			{
				if (IsNum)
				{
					State.NumberBuffer += Char;
				}
			}

		}

	}
	else
	{
		State.PacketSize--;
		if (State.PacketSize == 0)
		{

			UCodeLanguageSever::SeverPacket p;
			p._Data = std::move(State.Buffer);

			State = StreamState();
			return p;
		}

	}
}

struct LanguageSeverFuncMap
{

	using Func = void(LanguageSever::*)(integer  requestid, const json& Params);
	inline static const std::unordered_map<String, Func> LanguageFuncs
	{
		{"initialize",&LanguageSever::Sever_initialize},
		{"textDocument/definition",&LanguageSever::textDocument_definition},
		{"textDocument/hover",&LanguageSever::textDocument_hover},
		{"textDocument/rename",&LanguageSever::textDocument_rename},
	};
};


LanguageSever::LanguageSever()
{
	
}
LanguageSever::~LanguageSever()
{

}
bool LanguageSever::Step()
{
	//Io

	if (_ClientInputLock.try_lock())
	{
		Vector<ClientPacket> Tep = std::move(_ClientPackets);
		_ClientInputLock.unlock();

		for (auto& Item : Tep)
		{
			_ClientPacketsToRun.push_back(std::move(Item));
		}
	}

	{
		for (auto& Item : _ClientPacketsToRun)
		{
			OnReceivedPacket(Item);
		}
		_ClientPacketsToRun.clear();
	}

    return Runing;
}
void LanguageSever::OnReceivedPacket(const ClientPacket& Item)
{
	json Values = json::parse(Item._Data);
	auto jsonrpcV = Values["jsonrpc"].get<String>();

	auto IDV = Values["id"].get<integer>();

	auto Method = Values["method"].get<String>();

	json params = Values["params"];

	if (LanguageSeverFuncMap::LanguageFuncs.count(Method))
	{
		auto FuncPtr = LanguageSeverFuncMap::LanguageFuncs.at(Method);
		(*this.*FuncPtr)(IDV,params);
	}

}
void LanguageSever::textDocument_definition(integer  requestid,const json& Params)
{

}
void LanguageSever::textDocument_hover(integer  requestid, const json& params)
{
}
void LanguageSever::textDocument_rename(integer  requestid, const json& params)
{
}

//

void LanguageSever::window_logMessage(MessageType Type, String MSg)
{
	LogMessageParams V;
	V.type = Type;
	V.message = MSg;

	SendMethodToClient("window/logMessage", V);
}
void LanguageSever::Sever_initialize(integer requestid, const json& Params)
{
	InitializeResult V;
	//V.capabilities.positionEncoding = PositionEncodingkind::PositionEncodingKind8;
	V.capabilities.hoverProvider = true;


	ResponseError Err;
	Err.code = (integer)ErrorCodes::InternalError;
	Err.message = "Testing";
	SendResponseErrorToClient(requestid, Err);
	//SendResponseMessageToClient(V);

	/*
	{
		json Json;
		{
			Json["jsonrpc"] = "2.0";
			Json["method"] = "initialized";
			Json["params"] = json::object();
		}


		SeverPacket packet;
		packet._Data = Json.dump();
		SendPacketToClient(std::move(packet));

	}
	*/

	//window_logMessage(MessageType::Log, "Hello World Sever Side");
}
LanguageSeverEnd