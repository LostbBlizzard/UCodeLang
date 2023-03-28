#include "UCodeLang/LangCore/BitMaker.hpp"
UCodeLangStart

struct CodeGen
{
	using Value8 = Int8;
	using Value16 = Int16;
	using Value32 = Int32;
	using Value64 = Int64;

	inline void PushByte(Byte Value)
	{
		ByteOutput.push_back(Value);
	}
	inline void PushByte(const Byte* Value, size_t Size)
	{
		for (size_t i = 0; i < Size; i++)
		{
			ByteOutput.push_back(Value[i]);
		}
	}
	inline void PushByteR(const Byte* Value, size_t Size)
	{
		for (int i = Size - 1; i >= 0; i--)
		{
			ByteOutput.push_back(Value[i]);
		}
	}

	inline void PushByte_Big_endian(const Byte* Value, size_t Size)
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
	inline void PushByte_little_endian(const Byte* Value, size_t Size)
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
	inline size_t GetIndex() { return ByteOutput.size(); }
	Vector<Byte> ByteOutput;

	//

	static inline void SubByte(Byte* BufferToUpdate, const Byte* Value, size_t Size)
	{
		for (size_t i = 0; i < Size; i++)
		{
			BufferToUpdate[i] = Value[i];
		}
	}
	static inline void SubByteR(Byte* BufferToUpdate,const Byte* Value, size_t Size)
	{
		for (int i = Size - 1; i >= 0; i--)
		{
			BufferToUpdate[i] = Value[i];
		}
	}

	static inline void SubByte_Big_endian(Byte* BufferToUpdate, const Byte* Value, size_t Size)
	{
		if (BitConverter::_CPUEndian == Endian::Big)
		{
			SubByte(BufferToUpdate,Value, Size);
		}
		else
		{
			SubByteR(BufferToUpdate,Value, Size);
		}
	}
	static inline void SubByte_little_endian(Byte* BufferToUpdate, const Byte* Value, size_t Size)
	{
		if (BitConverter::_CPUEndian == Endian::little)
		{
			SubByteR(BufferToUpdate, Value, Size);
		}
		else
		{
			SubByteR(BufferToUpdate, Value, Size);
		}
	}


	template<typename T>static void SubByte_t_little_endian(Byte* BufferToUpdate, const T& Value)
	{
		SubByte_little_endian(BufferToUpdate,(const Byte*)&Value, sizeof(Value));
	}
	template<typename T>static void SubByte_t_Big_endian(Byte* BufferToUpdate, const T& Value)
	{
		SubByte_Big_endian(BufferToUpdate,(const Byte*)&Value, sizeof(Value));
	}
};
UCodeLangEnd