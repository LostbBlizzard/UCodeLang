#pragma once
#include <UCodeLang/LangCore/UCodeLangNameSpace.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
UCodeLangStart


//Type the Interpreter,Jit-Interpreter,Native Interpreter Uses this to do File operations
struct UFileHandle
{
	using Handle = void*;
	enum class PosType : Byte
	{
		Set,
		Start,
		End,
	};
	enum class IoMode
	{
		Both,
		In,
		Out,
	};
	enum class FileType
	{
		Text,
		Bytes,
	};

	struct FileOpenMode
	{
		IoMode Io;
		FileType Type;
	};

	static UCodeLangAPIExport Handle Open(const Span<char> path, FileOpenMode Mode);
	static UCodeLangAPIExport Handle Open(const Span<PathChar> path, FileOpenMode Mode);

	static UCodeLangAPIExport Handle Open(const char* path, size_t Size, FileOpenMode Mode);
	static UCodeLangAPIExport Handle Open(const PathChar* path, size_t Size, FileOpenMode Mode);

	static UCodeLangAPIExport bool Is_open(Handle Handle);
	static UCodeLangAPIExport void Close(Handle Handle);

	static UCodeLangAPIExport void WriteBytes(Handle Handle, const BytesView bytes);
	static UCodeLangAPIExport void ReadBytes(Handle Handle, BytesView Outbuffer);

	static UCodeLangAPIExport void WriteBytes(Handle Handle, const Byte* buffer, size_t bufferSize);
	static UCodeLangAPIExport void ReadBytes(Handle Handle, Byte* Outbuffer, size_t OutbufferSize);

	static UCodeLangAPIExport void SetPos(Handle Handle, size_t Size, PosType Type);
	static UCodeLangAPIExport size_t GetPos(Handle Handle);

	static UCodeLangAPIExport bool FileExist(const Span<char> path);
	static UCodeLangAPIExport bool FileExist(const Span<PathChar> path);
	static UCodeLangAPIExport bool FileExist(const char* path, size_t Size);
	static UCodeLangAPIExport bool FileExist(const PathChar* path, size_t Size);

	static UCodeLangAPIExport bool FileRemove(const Span<char> path);
	static UCodeLangAPIExport bool FileRemove(const Span<PathChar> path);
	static UCodeLangAPIExport bool FileRemove(const char* path, size_t Size);
	static UCodeLangAPIExport bool FileRemove(const PathChar* path, size_t Size);
};

//Type the Interpreter,Jit-Interpreter,Native Interpreter Uses this to do Networking operations
//API is base on C# Sockets
//https://learn.microsoft.com/en-us/dotnet/fundamentals/networking/sockets/sockets-overview
struct USocketHandle
{
	using Handle = void*;
	enum class SocketType
	{
		Dgram,
		Raw,
		Rdm,
		Seqpacket,
		Stream,
		Unknown,
	};
	enum class ProtocolType
	{
		IP,
		IPv4,
		IPv6,
		Tcp,
		Udp,
	};
	struct IPEndPoint
	{

	};
	struct IPAddress
	{

	};
	static UCodeLangAPIExport Handle Open(SocketType socketType, ProtocolType protocolType);
	static UCodeLangAPIExport void Close(Handle Handle);

	static UCodeLangAPIExport void Connect(IPEndPoint Ip);

	static UCodeLangAPIExport int Send(Handle Handle, const Span<Byte> bytes);

	static UCodeLangAPIExport int Send(Handle Handle, const Byte* byte, size_t size);

	static UCodeLangAPIExport int Receive(Handle Handle, Span<Byte> output);
	static UCodeLangAPIExport int Receive(Handle Handle, const Byte* byte, size_t size);

	static UCodeLangAPIExport IPAddress GetHostEntry(const Span<char> String);

};

UCodeLangEnd

