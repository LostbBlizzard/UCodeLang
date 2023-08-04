#include "LanguageSever.hpp"
#include <unordered_map>
#include <functional>
LanguageSeverStart

#define InitializeCheck() if (IsInitialized== false){ResponseError e;\
	e.code = (integer)ErrorCodes::ServerNotInitialized;\
	SendResponseErrorToClient(requestid, e); return;}\

#define ShutdownCheck() if (IsShutdown == true) { \
	ResponseError e; \
	e.code = (integer)ErrorCodes::InvalidRequest; \
	SendResponseErrorToClient(requestid, e); return;}\

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

	using RequestFunc = void(LanguageSever::*)(integer  requestid, const json& Params);
	using NotificationFunc = void(LanguageSever::*)(const json& Params);
	inline static const std::unordered_map<String, RequestFunc> RequestFuncs
	{
		{"initialize",&LanguageSever::Sever_initialize},
		{"shutdown",&LanguageSever::Sever_Shutdown},
		{"textDocument/definition",&LanguageSever::textDocument_definition},
		{"textDocument/hover",&LanguageSever::textDocument_hover},
		{"textDocument/rename",&LanguageSever::textDocument_rename},
	};
	inline static const std::unordered_map<String, NotificationFunc> NotificationFuncs
	{
		{"exit",&LanguageSever::Sever_Exit},
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
	auto dataOp = Item.Parse();

	if (dataOp.has_value())
	{
		auto& data = dataOp.value();

		if (auto Val = data.Type.Get_If<ClientPacket::RequestMessage_t>())
		{
			if (LanguageSeverFuncMap::RequestFuncs.count(Val->method))
			{
				auto FuncPtr = LanguageSeverFuncMap::RequestFuncs.at(Val->method);
				(*this.*FuncPtr)(Val->id, Val->params);
			}
			else
			{
				ResponseError err;
				err.code = (integer)ErrorCodes::MethodNotFound;
				SendResponseErrorToClient(Val->id,err);
			}
		}
		else  if (auto Val = data.Type.Get_If<ClientPacket::NotificationMessage_t>())
		{
			if (LanguageSeverFuncMap::NotificationFuncs.count(Val->method))
			{
				auto FuncPtr = LanguageSeverFuncMap::NotificationFuncs.at(Val->method);
				(*this.*FuncPtr)(Val->params);
			}
		}
		else if (auto Val = data.Type.Get_If<ClientPacket::ResponseMessage_t>())
		{

		}


		
	}

}

void LanguageSever::Sever_Shutdown(integer requestid, const json& params)
{
	IsShutdown = true;
	 

	SendResponseMessageToClient(requestid, TsNull());
}
void LanguageSever::Sever_Exit(const json& params)
{
	StopRuning();
	ProcessExitCode = 0;

}
void LanguageSever::textDocument_definition(integer  requestid,const json& Params)
{
	InitializeCheck(); ShutdownCheck();
}
void LanguageSever::textDocument_hover(integer  requestid, const json& params)
{

	InitializeCheck(); ShutdownCheck();
}
void LanguageSever::textDocument_rename(integer  requestid, const json& params)
{

	InitializeCheck(); ShutdownCheck();
	
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
	InitializeParams params;
	ns::from_json(Params, params);
	
	InitializeResult V;
	V.capabilities.positionEncoding = PositionEncodingkind::PositionEncodingKind8;
	V.capabilities.hoverProvider = true;


	
	SendResponseMessageToClient(requestid,V);

	IsInitialized = false;
	//window_logMessage(MessageType::Log, "Hello World Sever Side");
}
LanguageSeverEnd