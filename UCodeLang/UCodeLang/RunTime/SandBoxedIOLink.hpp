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
		bool AddNetworking = true;
		LinkSettings()
		{
			
		}
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

	//

	using DateTime = size_t;

	struct RealFileLink
	{
		Path path;
		FILE* nativefile = nullptr;
		bool read = false;
		bool write = false;
	};

	struct VirtualFile
	{
		String Fullname;
		Vector<Byte> Bytes;
		size_t Pos = 0;
		DateTime lastread =0;
		DateTime lastwrite =0;
		bool FileOpened = false;
		Optional<RealFileLink> RealFile;
	};
	struct VirtualDirectory
	{

	};
	struct FileSystem
	{
		inline static UnorderedMap<FileHandle, VirtualFile> Files;
		inline static Vector<VirtualDirectory> Directorys;
		inline static FileHandle _NextID = 0;
	};

	void Reset();
	void Access(std::function<void(FileSystem& sytem)> callback);
	void GiveAccessToRealFile(const Path& file,const String& virtualfilepath, bool read = true, bool write = false);
	void GiveAccessToRealDirectory(const Path& directory, const String& virtualdirectorypath, bool read = true, bool write = false);
private:
	
	inline static size_t MaxVirtualStorageSize = (255 * 1024 * 1024) * 20;//20 MB
	inline static FileSystem system;

	static Optional<FileHandle> GetfileusingName(String_view filename);
};

UCodeLangEnd