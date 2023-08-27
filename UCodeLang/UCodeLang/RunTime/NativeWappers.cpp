#include "NativeWappers.hpp"
#include <fstream>
#include <cstring> //memcpy
UCodeLangStart

 

UCodeLangAPIExport UFileHandle::Handle UFileHandle::Open(const Span<char> path, FileOpenMode Mode)
{

	std::ios_base::openmode mode;
	if (Mode.Type == FileType::Bytes)
	{
		mode = std::ios::binary;
	}
	else
	{
		mode = 0;
	}

	if (Mode.Io== IoMode::In)
	{
		mode |= std::ios_base::in;
	}
	else if (Mode.Io == IoMode::Out)
	{
		mode |= std::ios_base::out;
	}
	else
	{
		mode |= std::ios_base::in;
		mode |= std::ios_base::out;
	}


	return (Handle)new std::fstream(String_view(path.Data(), path.Size()));
}

UCodeLangAPIExport UFileHandle::Handle UFileHandle::Open(const Span<PathChar> path, FileOpenMode Mode)
{
	std::ios_base::openmode mode;
	return (Handle)new std::fstream(PathView(path.Data(), path.Size()), mode);
}

UCodeLangAPIExport UFileHandle::Handle UFileHandle::Open(const char* path, size_t Size, FileOpenMode Mode)
{
	return Open(Span<char>::Make(path, Size), Mode);
}

UCodeLangAPIExport UFileHandle::Handle UFileHandle::Open(const PathChar* path, size_t Size, FileOpenMode Mode)
{
	return Open(Span<PathChar>::Make(path, Size), Mode);
}

UCodeLangAPIExport bool UFileHandle::Is_open(Handle Handle)
{
	return ((std::fstream*)Handle)->is_open();
}
UCodeLangAPIExport void UFileHandle::Close(Handle Handle)
{
	delete ((std::fstream*)Handle);
}

UCodeLangAPIExport void UFileHandle::WriteBytes(Handle Handle, const BytesView bytes)
{
	((std::fstream*)Handle)->write((const char*)bytes.Data(), bytes.Size());
}

UCodeLangAPIExport void UFileHandle::ReadBytes(Handle Handle, BytesView Outbuffer)
{
	((std::fstream*)Handle)->read((char*)Outbuffer.Data(), Outbuffer.Size());
}

UCodeLangAPIExport void UFileHandle::WriteBytes(Handle Handle, const Byte* buffer, size_t bufferSize)
{
	WriteBytes(Handle, BytesView::Make(buffer, bufferSize));
}

UCodeLangAPIExport void UFileHandle::ReadBytes(Handle Handle, Byte* Outbuffer, size_t OutbufferSize)
{
	ReadBytes(Handle, BytesView::Make(Outbuffer, OutbufferSize));
}

UCodeLangAPIExport void UFileHandle::SetPos(Handle Handle, size_t Size, PosType Type)
{
	auto& f = *((std::fstream*)Handle);

	switch (Type)
	{
	case UCodeLang::UFileHandle::PosType::Set:
		f.seekp(Size, 0);
		break;
	case UCodeLang::UFileHandle::PosType::Start:
		f.seekp(Size, std::ios::beg);
		break;
	case UCodeLang::UFileHandle::PosType::End:
		f.seekp(Size, std::ios::end);
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}

UCodeLangAPIExport size_t UFileHandle::GetPos(Handle Handle)
{
	auto& f = *((std::fstream*)Handle);
	return f.tellg();
}

UCodeLangAPIExport bool UFileHandle::FileExist(const Span<char> path)
{
	return std::filesystem::exists(String_view(path.Data(), path.Size()));
}
UCodeLangAPIExport bool UFileHandle::FileExist(const Span<PathChar> path)
{
	return std::filesystem::exists(PathView(path.Data(), path.Size()));
}

UCodeLangAPIExport bool UFileHandle::FileExist(const char* path, size_t Size)
{
	return FileExist(Span<char>::Make(path, Size));
}

UCodeLangAPIExport bool UFileHandle::FileExist(const PathChar* path, size_t Size)
{
	return FileExist(Span<PathChar>::Make(path,Size));
}

UCodeLangAPIExport bool UFileHandle::FileRemove(const Span<char> path)
{
	return std::filesystem::remove(String_view(path.Data(), path.Size()));
}
UCodeLangAPIExport bool UFileHandle::FileRemove(const Span<PathChar> path)
{
	return std::filesystem::remove(PathView(path.Data(), path.Size()));
}

UCodeLangAPIExport bool UFileHandle::FileRemove(const char* path, size_t Size)
{
	return FileRemove(Span<char>::Make(path, Size));
}

UCodeLangAPIExport bool UFileHandle::FileRemove(const PathChar* path, size_t Size)
{
	return FileRemove(Span<PathChar>::Make(path, Size));
}


UCodeLangAPIExport USocketHandle::Handle USocketHandle::Open(SocketType socketType, ProtocolType protocolType)
{

	return {};
}

UCodeLangAPIExport void USocketHandle::Close(Handle Handle)
{
}

UCodeLangAPIExport void USocketHandle::Connect(IPEndPoint Ip)
{
}

UCodeLangAPIExport int USocketHandle::Send(Handle Handle, const Span<Byte> bytes)
{

	return {};
}

UCodeLangAPIExport int USocketHandle::Send(Handle Handle, const Byte* byte, size_t size)
{
	return Send(Handle, Span<Byte>::Make(byte, size));
}

UCodeLangAPIExport int USocketHandle::Receive(Handle Handle, Span<Byte> output)
{
	return {};
}

UCodeLangAPIExport int USocketHandle::Receive(Handle Handle, const Byte* byte, size_t size)
{
	return Receive(Handle, Span<Byte>::Make(byte, size));
}

UCodeLangAPIExport USocketHandle::IPAddress USocketHandle::GetHostEntry(const Span<char> String)
{

	return {};
}

UCodeLangEnd

