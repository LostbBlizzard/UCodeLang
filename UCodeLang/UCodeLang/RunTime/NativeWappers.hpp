#pragma once
#include <UCodeLang/LangCore/UCodeLangNameSpace.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
UCodeLangStart


//Type the Interpreter,Jit-Interpreter,Native Interpreter Uses this to do file operations
struct UFileHandle
{
	using Handle = void*;
	using PathChar = Path::string_type::allocator_type;

	static UCodeLangAPIExport Handle Open(const Span<char> path);
	static UCodeLangAPIExport Handle Open(const Span<PathChar> path);

	static UCodeLangAPIExport Handle Open1(const char* path, size_t Size);
	static UCodeLangAPIExport Handle Open2(const PathChar* path, size_t Size);

	static UCodeLangAPIExport bool Is_open(Handle Handle);
	static UCodeLangAPIExport void Close(Handle Handle);

	static UCodeLangAPIExport void WriteBytes(Handle Handle, const BytesView bytes);
	static UCodeLangAPIExport void ReadBytes(Handle Handle, BytesView Outbuffer);

	static UCodeLangAPIExport void WriteBytes2(Handle Handle, const Byte* buffer, size_t bufferSize);

	static UCodeLangAPIExport void ReadBytes2(Handle Handle, Byte* Outbuffer, size_t OutbufferSize);
};

UCodeLangEnd

