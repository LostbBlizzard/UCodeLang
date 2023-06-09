#include "LanguageSever.hpp"
#include <unordered_map>
LanguageSeverStart


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

    return true;
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
void LanguageSever::Sever_initialize(integer requestid, const json& Params)
{
	InitializeResult V;
	V.capabilities.positionEncoding = PositionEncodingkind::PositionEncodingKind8;


	SendResponseMessageToClient(requestid, V);
}
LanguageSeverEnd