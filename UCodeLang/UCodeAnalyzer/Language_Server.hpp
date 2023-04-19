#pragma once
#include "Typedef.hpp"
#include <mutex>

#include "LanguageSeverTypes/PacketTypes.hpp"
UCodeAnalyzerStart


/*
using OutputCallBack = void(*)(Language_ID ID,OutputPacketType OutputType,const BytesView bits);

struct Language_OutPut
{
	ListenerID _ID{};
	OutputCallBack _BitsOutput{};

	void InvokeBitOutput(OutputPacketType OutputType, const BytesView bits)
	{
		if (_BitsOutput) 
		{
			_BitsOutput(_ID, OutputType, bits);
		}
	}
};

struct Language_Input
{
	ListenerID FromWho{};
	InputPacketType PacketType;
	Vector<Byte> _Bits;
};

struct Language_File
{
	Path _FilePath;
	String _FileCopy;
};

struct Language_Listener
{
	Language_OutPut OutputData{};

	bool CanListen[(OutputPacketType_t)OutputPacketType::_Max];

	Vector<Unique_ptr<Language_File>> _OpenedFiles;
	BinaryVectorMap< Path, Language_File*> _FilePathMap;
};
//this Sever uses raw bytes and does not handle networking and is not thread safe.
class Language_Server
{
public:
	Language_Server(){}
	~Language_Server(){}

	void Step();


	//not thread safe
	void Data_received(const Language_Input& Input);
	void Set_data_Output(Language_OutPut& Item);
	void Remove_data_Output(Language_ID ID)
	{
		_Listeners.erase(ID);
	}
private:
	Language_ID _NextOutputID = 0;
	BinaryVectorMap<Language_ID,Unique_ptr<Language_Listener>> _Listeners;

	Language_Listener& Get_Listener(Language_ID ID)
	{
		return *_Listeners.GetValue(ID);
	}

	void AddFileToListener(Language_Listener& Listener, const Path& path);

	static void ReadPath(BitReader& This, Path& out, const Path fallback);
	static void WritePath(BitMaker& This, const Path& Value);

	static void ReadString(BitReader& This, String& out, const String fallback);
	static void WriteString(BitMaker& This, const String& Value);

	static void ReadLangID(BitReader& This, Language_ID& out, const Language_ID fallback);
	static void WriteLangID(BitMaker& This, const Language_ID& Value);

	void SendData(Language_ID ToWho, OutputPacketType PacketType, const BitMaker& _Bits)
	{
		SendData(ToWho, PacketType, BytesView((Byte*)_Bits.data(), _Bits.size()));
	}
	void SendData(Language_ID ToWho, OutputPacketType PacketType, const BytesView _Bits);
public:
	
	//Actions
	void Action_OpenFile(Language_ID FromWho, const BytesView _Bits);
	void Action_OpenFile(Language_ID FromWho, const OpenFile_Packet& Packet);

	//Query
	void Query_FileText(Language_ID FromWho, const BytesView _Bits);
	void Query_FileText(Language_ID FromWho, const Query_FileText_Packet& Packet);
	//return
	void Return_FileText(Language_ID ToWho, const Return_FileText_Packet& Packet);
};


*/


UCodeAnalyzerEnd