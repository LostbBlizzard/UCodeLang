#pragma once
#include "../LangCore/UCodeLangNameSpace.hpp"
#include "RunTimeLib.hpp"
UCodeLangStart

struct SandBoxedIOLink
{
	struct LinkSettings
	{
		bool AddCInAndOut = true;
		bool AddFileIO = true;
	};	
	static void Link(RunTimeLib& lib,const LinkSettings& settings ={});

	using FileHandle = size_t;
	using FilePos = size_t;

	//API

	//File access
	static FileHandle fopen(String_view filename, String_view mode);
	static int fclose(FileHandle stream);

	//Direct input/output
	static size_t fread(void* buffer, size_t size, size_t count, FileHandle stream);

	static size_t fwrite(void* buffer, size_t size, size_t count, FileHandle stream);
	//File positioning
	static FilePos ftell(FileHandle stream);
	static int fseek(FileHandle stream, FilePos offset, int origin);

	//Operations on files
	static int remove(String_view filename);

	static int rename(String_view oldfilename, String_view newfilename);

private:
	struct VirtualFile
	{
		String Fullname;
		Vector<Byte> Bytes;
		size_t Pos=0;
	};
	UnorderedMap<FileHandle, VirtualFile> Files;
};

UCodeLangEnd