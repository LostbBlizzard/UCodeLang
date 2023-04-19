#include "Language_Server.hpp"
UCodeAnalyzerStart

/*
using LangSeverFunc = void(Language_Server::*)(Language_ID FromWho,const BytesView _Bits);

const Unordered_map<InputPacketType, LangSeverFunc> _FuncCalls
{
	{InputPacketType::Openfile,&Language_Server::Action_OpenFile},

};


void Language_Server::Step()
{

}



void Language_Server::Data_received(const Language_Input& Input)
{

	if (_FuncCalls.count(Input.PacketType))
	{
		auto& Item = _FuncCalls.at(Input.PacketType);

		auto Bits = BytesView((Byte*)Input._Bits.data(), Input._Bits.size());
		(this->*Item)(Input.FromWho, Bits);
	}
	
}

void Language_Server::Set_data_Output(Language_OutPut& Item)
{
	Language_ID NewID = _NextOutputID;
	_NextOutputID++;

	Item._ID = NewID;


	Language_Listener* Listener = new Language_Listener();
	Listener->OutputData = Item;

	_Listeners.AddValue(NewID,Unique_ptr<Language_Listener>(Listener));
}

void Language_Server::AddFileToListener(Language_Listener& Listener, const Path& path)
{

}

void Language_Server::ReadPath(BitReader& This, Path& out, const Path fallback)
{

}

void Language_Server::WritePath(BitMaker& This, const Path& Value)
{

}

void Language_Server::ReadString(BitReader& This, String& out, const String fallback)
{

}

void Language_Server::WriteString(BitMaker& This, const String& Value)
{
}

void Language_Server::ReadLangID(BitReader& This, Language_ID& out, const Language_ID fallback)
{

}

void Language_Server::WriteLangID(BitMaker& This, const Language_ID& Value)
{

}

void Language_Server::SendData(Language_ID ToWho, OutputPacketType PacketType, const BytesView _Bits)
{
	auto& User = Get_Listener(ToWho);

	if (User.CanListen[(OutputPacketType_t)PacketType])
	{
		User.OutputData.InvokeBitOutput(PacketType, _Bits);
	}
}

void Language_Server::Action_OpenFile(Language_ID FromWho, const BytesView _Bits)
{
	OpenFile_Packet V;
	{
		BitReader bits = BitReader(_Bits.Bytes, _Bits.Size);
		ReadPath(bits, V.FilePath, V.FilePath);
	}

	
	Action_OpenFile(FromWho, V);
}

void Language_Server::Action_OpenFile(Language_ID FromWho, const OpenFile_Packet& Packet)
{
	auto& User = Get_Listener(FromWho);
	if (!User._FilePathMap.HasValue(Packet.FilePath))
	{
		AddFileToListener(User, Packet.FilePath);
	}
}

//Query

void Language_Server::Query_FileText(Language_ID FromWho, const BytesView _Bits)
{
	Query_FileText_Packet V;
	{
		BitReader bits = BitReader(_Bits.Bytes, _Bits.Size);
		bits.ReadType(V.ClientSideID, V.ClientSideID);
	}
	Query_FileText(FromWho, V);
}

void Language_Server::Query_FileText(Language_ID FromWho, const Query_FileText_Packet& Packet)
{
}

//return

void Language_Server::Return_FileText(Language_ID ToWho, const Return_FileText_Packet& Packet)
{
	BitMaker bits = BitMaker();
	WriteLangID(bits, Packet.ClientSideID);
	WriteString(bits, Packet.FileText);
	SendData(ToWho,OutputPacketType::Return_FileText, bits);
}

*/

UCodeAnalyzerEnd