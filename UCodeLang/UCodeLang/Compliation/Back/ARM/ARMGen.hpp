#pragma once
#include "../x86/X86Gen.hpp"
#include "ARMData.hpp"
UCodeLangStart


//resources
//https://www.amd.com/system/files/TechDocs/24592.pdf

class ARM64Gen
{
public:
	using Value8 = CodeGen::Value8;
	using Value16 = CodeGen::Value16;
	using Value32 = CodeGen::Value32;
	using Value64 = CodeGen::Value64;

	ARM64Gen() {}
	~ARM64Gen() {}

	//
	void PushByte(Byte Value)
	{
		_Output.PushByte(Value);
	}
	void PushByte(const Byte* Value, size_t Size)
	{
		_Output.PushByte(Value, Size);
	}
	template<typename T>void PushValue_t(const T& Value)
	{
		PushByte((const Byte*)&Value, sizeof(Value));
	}
	template<typename T>void PushValue_t_little_endian(const T& Value)
	{
		_Output.PushValue_t_little_endian(Value);
	}
	template<typename T>void PushValue_t_Big_endian(const T& Value)
	{
		_Output.PushValue_t_Big_endian(Value);
	}

	//ARM Ins list Here

	CodeGen _Output;
};
UCodeLangEnd