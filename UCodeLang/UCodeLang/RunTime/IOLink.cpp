#pragma once
#include "IOLink.hpp"
#include "Interpreters/Interpreter.hpp"
UCodeLangStart

template<typename T, typename... Pars>
using NativeCall = RunTimeLib::NativeCall<T, Pars...>;

using FileHandle = std::FILE*;
using FilePos = size_t;

const char* ToCString(String_view p,size_t Par)
{

}


void IOLink::Link(RunTimeLib& lib, const LinkSettings& settings)
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

				Input.Set_Return(std::fopen(ToCString(Par0,0), ToCString(Par1,1)));
			}, (NativeCall<FileHandle, String_view, String_view>)[](String_view Par0, String_view Par1)
				{
					return std::fopen(ToCString(Par0,0), ToCString(Par1,1));
				});

		lib.Add_CPPCall("fclose", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<FileHandle>();

				Input.Set_Return(std::fclose(Par0));
			}, (NativeCall<int, FileHandle>)[](FileHandle Par0)
				{
					return std::fclose(Par0);
				});

	
		lib.Add_CPPCall("fread", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<void*>();
				auto Par1 = Input.GetParameter<size_t>();
				auto Par2 = Input.GetParameter<size_t>();
				auto Par3 = Input.GetParameter<FileHandle>();

				Input.Set_Return(std::fread(Par0, Par1, Par2, Par3));
			}, (NativeCall<size_t, void*, size_t, size_t, FileHandle>)[](void* buffer, size_t size, size_t count, FileHandle stream)
				{
					return std::fread(buffer, size, count, stream);
				});

		lib.Add_CPPCall("fwrite", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<void*>();
				auto Par1 = Input.GetParameter<size_t>();
				auto Par2 = Input.GetParameter<size_t>();
				auto Par3 = Input.GetParameter<FileHandle>();

				Input.Set_Return(std::fwrite(Par0, Par1, Par2, Par3));
			}, (NativeCall<size_t, void*, size_t, size_t, FileHandle>)[](void* buffer, size_t size, size_t count, FileHandle stream)
				{
					return std::fwrite(buffer, size, count, stream);
				});

		lib.Add_CPPCall("ftell", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<FileHandle>();

				Input.Set_Return(std::ftell(Par0));
			}, (NativeCall<FilePos, FileHandle>)[](FileHandle stream)
				{
					return (FilePos)std::ftell(stream);
				});
		lib.Add_CPPCall("fseek", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<FileHandle>();
				auto Par1 = Input.GetParameter<int>();
				auto Par2 = Input.GetParameter<int>();

				Input.Set_Return(std::fseek(Par0, Par1, Par2));
			}, (NativeCall<int, FileHandle, FilePos, int>)[](FileHandle stream, FilePos offset, int origin)
				{
					return std::fseek(stream, offset, origin);
				});

		lib.Add_CPPCall("rename", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<String_view>();
				auto Par1 = Input.GetParameter<String_view>();

				Input.Set_Return(std::rename(ToCString(Par0,0), ToCString(Par0, 1)));
			}, (NativeCall<int, String_view, String_view>)[](String_view Par0, String_view Par1)
				{
					return std::rename(ToCString(Par0,0), ToCString(Par1, 1));
				});
		lib.Add_CPPCall("remove", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<String_view>();

				Input.Set_Return(std::remove(ToCString(Par0, 0)));
			}, (NativeCall<int, String_view>)[](String_view Par0)
				{
					return std::remove(ToCString(Par0, 0));
				});

	}
}

UCodeLangEnd