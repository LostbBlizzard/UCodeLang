#include "UCodeLang/LangCore/BitMaker.hpp"
UCodeLangStart

struct CodeGen
{
	using Value8 = Int8;
	using Value16 = Int16;
	using Value32 = Int32;
	using Value64 = Int64;

	void PushByte(Byte Value)
	{
		ByteOutput.push_back(Value);
	}
	void PushByte(const Byte* Value, size_t Size)
	{
		for (size_t i = 0; i < Size; i++)
		{
			ByteOutput.push_back(Value[i]);
		}
	}
	void PushByteR(const Byte* Value, size_t Size)
	{
		for (int i = Size - 1; i >= 0; i--)
		{
			ByteOutput.push_back(Value[i]);
		}
	}

	void PushByte_Big_endian(const Byte* Value, size_t Size)
	{
		if (BitConverter::_CPUEndian == Endian::Big)
		{
			PushByte(Value, Size);
		}
		else
		{
			PushByteR(Value, Size);
		}
	}
	void PushByte_little_endian(const Byte* Value, size_t Size)
	{
		if (BitConverter::_CPUEndian == Endian::little)
		{
			PushByte(Value, Size);
		}
		else
		{
			PushByteR(Value, Size);
		}
	}

	template<typename T>void PushValue_t(const T& Value)
	{
		PushByte((const Byte*)&Value, sizeof(Value));
	}

	template<typename T>void PushValue_t_little_endian(const T& Value)
	{
		PushByte_little_endian((const Byte*)&Value, sizeof(Value));
	}
	template<typename T>void PushValue_t_Big_endian(const T& Value)
	{
		PushByte_Big_endian((const Byte*)&Value, sizeof(Value));
	}
	Vector<Byte> ByteOutput;
};
UCodeLangEnd