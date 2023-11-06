#pragma once
#include "../ARM/ARMBuilder.hpp"
#include "ARM64Data.hpp"
UCodeLangStart


//resources
//https://www.amd.com/system/files/TechDocs/24592.pdf

class ARM64Builder
{
public:
	using Value8 = NativeCodeBuilder::Value8;
	using Value16 = NativeCodeBuilder::Value16;
	using Value32 = NativeCodeBuilder::Value32;
	using Value64 = NativeCodeBuilder::Value64;

	ARM64Builder() {}
	~ARM64Builder() {}

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

	NativeCodeBuilder _Output;
};
UCodeLangEnd