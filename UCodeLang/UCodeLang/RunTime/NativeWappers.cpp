#include "NativeWappers.hpp"
#include <fstream>
UCodeLangStart

 

UCodeLangAPIExport UFileHandle::Handle UFileHandle::Open(const Span<char> path)
{
	return (Handle)new std::fstream((String)String_view(path.Data(), path.Size()));
}

UCodeLangAPIExport UFileHandle::Handle UFileHandle::Open(const Span<PathChar> path)
{
	auto Str = Path::string_type();
	Str.resize(path.Size());
	memcpy(Str.data(),path.Data(), sizeof(Path) * path.Size());
	return (Handle)new std::fstream(Path(Str));
}

UCodeLangAPIExport UFileHandle::Handle UFileHandle::Open1(const char* path, size_t Size)
{
	return Open(Span<char>::Make(path, Size));
}

UCodeLangAPIExport UFileHandle::Handle UFileHandle::Open2(const PathChar* path, size_t Size)
{
	return Open(Span<PathChar>::Make(path, Size));
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

UCodeLangAPIExport void UFileHandle::WriteBytes2(Handle Handle, const Byte* buffer, size_t bufferSize)
{
	WriteBytes(Handle, BytesView::Make(buffer, bufferSize));
}

UCodeLangAPIExport void UFileHandle::ReadBytes2(Handle Handle, Byte* Outbuffer, size_t OutbufferSize)
{
	ReadBytes(Handle, BytesView::Make(Outbuffer, OutbufferSize));
}

UCodeLangEnd
