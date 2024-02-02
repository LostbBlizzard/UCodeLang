#include "SandBoxedIOLink.hpp"
#include "Interpreters/Interpreter.hpp"
#include <mutex>
UCodeLangStart


template<typename T, typename... Pars>
using NativeCall = RunTimeLib::NativeCall<T, Pars...>;

using FileHandle = SandBoxedIOLink::FileHandle;
using FilePos = SandBoxedIOLink::FilePos;


void SandBoxedIOLink::Link(RunTimeLib& lib, const LinkSettings& settings)
{

	if (settings.AddCInAndOut)
	{
		lib.Add_CPPCall("putchar", [](InterpreterCPPinterface& Input)
			{
				int Par0 = Input.GetParameter<int>();
				Input.Set_Return(std::putchar(Par0));
			}, std::putchar);

		lib.Add_CPPCall("putspan", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<const char*>();
				auto Par1 = Input.GetParameter<size_t>();

				Input.Set_Return(std::fwrite(Par0, 1, Par1, stdout));
			}, (NativeCall<size_t, const char*, size_t>)[](const char* pointer, size_t size)
				{
					return std::fwrite(pointer, 1, size, stdout);
				});
	}
	if (settings.AddFileIO)
	{
		lib.Add_CPPCall("fopen", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<String_view>();
				auto Par1 = Input.GetParameter<String_view>();

				Input.Set_Return(SandBoxedIOLink::fopen(Par0, Par1));
			}, (NativeCall<FileHandle, String_view, String_view>)[](String_view Par0, String_view Par1)
				{
					return SandBoxedIOLink::fopen(Par0, Par1);
				});

		lib.Add_CPPCall("fclose", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<FileHandle>();

				Input.Set_Return(SandBoxedIOLink::fclose(Par0));
			}, (NativeCall<int, FileHandle>)[](FileHandle Par0)
				{
					return SandBoxedIOLink::fclose(Par0);
				});

		lib.Add_CPPCall("fread", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<void*>();
				auto Par1 = Input.GetParameter<size_t>();
				auto Par2 = Input.GetParameter<size_t>();
				auto Par3 = Input.GetParameter<FileHandle>();

				Input.Set_Return(SandBoxedIOLink::fread(Par0, Par1, Par2, Par3));
			}, (NativeCall<size_t, void*,size_t, size_t, FileHandle>)[](void* buffer, size_t size, size_t count, FileHandle stream)
				{
					return SandBoxedIOLink::fread(buffer,size,count, stream);
				});

		lib.Add_CPPCall("fwrite", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<void*>();
				auto Par1 = Input.GetParameter<size_t>();
				auto Par2 = Input.GetParameter<size_t>();
				auto Par3 = Input.GetParameter<FileHandle>();

				Input.Set_Return(SandBoxedIOLink::fwrite(Par0, Par1, Par2, Par3));
			}, (NativeCall<size_t, void*, size_t, size_t, FileHandle>)[](void* buffer, size_t size, size_t count, FileHandle stream)
				{
					return SandBoxedIOLink::fwrite(buffer, size, count, stream);
				});
		lib.Add_CPPCall("ftell", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<FileHandle>();

				Input.Set_Return(SandBoxedIOLink::ftell(Par0));
			}, (NativeCall<FilePos,FileHandle>)[](FileHandle stream)
				{
					return SandBoxedIOLink::ftell(stream);
				});
		lib.Add_CPPCall("fseek", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<FileHandle>();
				auto Par1 = Input.GetParameter<int>();
				auto Par2 = Input.GetParameter<int>();

				Input.Set_Return(SandBoxedIOLink::fseek(Par0, Par1, Par2));
			}, (NativeCall<int, FileHandle, FilePos,int>)[](FileHandle stream, FilePos offset, int origin)
				{
					return SandBoxedIOLink::fseek(stream, offset, origin);
				});

		lib.Add_CPPCall("rename", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<String_view>();
				auto Par1 = Input.GetParameter<String_view>();

				Input.Set_Return(SandBoxedIOLink::rename(Par0,Par1));
			}, (NativeCall<int, String_view, String_view>)[](String_view Par0, String_view Par1)
				{
					return SandBoxedIOLink::rename(Par0, Par1);
				});

		lib.Add_CPPCall("remove", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<String_view>();

				Input.Set_Return(SandBoxedIOLink::remove(Par0));
			}, (NativeCall<int, String_view>)[](String_view Par0)
				{
					return SandBoxedIOLink::remove(Par0);
				});
	}
}


std::mutex SandBoxedIOMut;

void  SandBoxedIOLink::Access(std::function<void(FileSystem& sytem)> callback)
{
	SandBoxedIOMut.lock(); 
	callback(system);
	SandBoxedIOMut.unlock();
}
void SandBoxedIOLink::GiveAccessToRealFile(const Path& file, const String& virtualfilepath, bool read, bool write)
{
	SandBoxedIOMut.lock();

	system._NextID++;
	auto newid = system._NextID;

	VirtualFile f;
	f.Fullname = virtualfilepath;
	f.FileOpened = true;
	
	RealFileLink link;
	link.path = file;
	link.read = read;
	link.write = write;

	f.RealFile = std::move(link);

	system.Files.AddValue(newid, std::move(f));

	SandBoxedIOMut.unlock();
}
void SandBoxedIOLink::GiveAccessToRealDirectory(const Path& directory, const String& virtualdirectorypath, bool read, bool write)
{
	SandBoxedIOMut.lock();

	SandBoxedIOMut.unlock();
}
void SandBoxedIOLink::Reset()
{
	SandBoxedIOMut.lock();

	system.Files.clear();
	system.Directorys.clear();

	SandBoxedIOMut.unlock();
}

Optional<FileHandle> SandBoxedIOLink::GetfileusingName(String_view filename)
{
	for (auto& item : system.Files)
	{
		if (item.second.Fullname == filename)
		{
			return item.first;
		}
	}
	return {};
}

FileHandle SandBoxedIOLink::fopen(String_view filename, String_view mode)
{
	FileHandle r;
	SandBoxedIOMut.lock();
	auto op = GetfileusingName(filename);
	if (!op.has_value())
	{
		system._NextID++;
		auto newid = system._NextID;

		VirtualFile f;
		f.Fullname = filename;
		f.FileOpened = true;

		system.Files.AddValue(newid, std::move(f));

		r =newid;
	}
	else if (op)
	{
		auto& F = system.Files.GetValue(op.value());
		if (!F.FileOpened)
		{
			r = op.value();
		}
		else {
			//file alread open
			r = 0;
		}
	}
	SandBoxedIOMut.unlock();

	return r;
}

int SandBoxedIOLink::fclose(FileHandle stream)
{
	SandBoxedIOMut.lock();
	auto& f = system.Files.GetValue(stream);
	f.FileOpened = false;
	f.Pos = 0;

	SandBoxedIOMut.unlock();
	return 0;

}

size_t SandBoxedIOLink::fread(void* buffer, size_t size, size_t count, FileHandle stream)
{
	if (size == 0 || count ==0) { return 0; }


	SandBoxedIOMut.lock();
	auto& f = system.Files.GetValue(stream);
	memcpy(buffer, f.Bytes.data() + f.Pos, count * size);

	f.Pos += size;

	SandBoxedIOMut.unlock();

	return size;
}

size_t SandBoxedIOLink::fwrite(void* buffer, size_t size, size_t count, FileHandle stream)
{
	if (size == 0 || count==0) { return 0; }


	SandBoxedIOMut.lock();
	auto& f = system.Files.GetValue(stream);
	
	Byte* start = (Byte*)buffer;
	Byte* end = (Byte*)buffer + (count * size);
	
	f.Bytes.insert(f.Bytes.begin() + f.Pos, start, end);
	f.Pos += size;


	SandBoxedIOMut.unlock();

	return size;
}

FilePos SandBoxedIOLink::ftell(FileHandle stream)
{
	return 0;
}

int SandBoxedIOLink::fseek(FileHandle stream, FilePos offset, int origin)
{
	SandBoxedIOMut.lock();
	auto& f = system.Files.GetValue(stream);
	f.Pos = offset;
	SandBoxedIOMut.unlock();

	return 0;
}

int SandBoxedIOLink::remove(String_view filename)
{
	return 0;
}

int SandBoxedIOLink::rename(String_view oldfilename, String_view newfilename)
{
	return 0;
}

UCodeLangEnd
