#pragma once
#include "BitConverter.hpp"

UCodeLangStart

//Endian will be BitConverter InputOutEndian

template<typename T>
struct BitData
{

}; 


class BitMaker
{
public:
	using SizeAsBits = UInt32;
	using Byte = UInt8;

	BitMaker() :_Bytes()
	{
	}
	BitMaker(size_t BitSize) :_Bytes(BitSize)
	{

	}
	
	~BitMaker()
	{

	}

	UCodeLangForceinline void resize(size_t NewSize){ _Bytes.resize(NewSize);}
	UCodeLangForceinline void clear(){ _Bytes.clear();}
	
	UCodeLangForceinline size_t size() const {return _Bytes.size(); }
	
	UCodeLangForceinline auto& Get_Bytes() { return _Bytes; }
	UCodeLangForceinline auto data() { return _Bytes.data(); }

	UCodeLangForceinline auto& Get_Bytes() const { return _Bytes; }
	UCodeLangForceinline auto data() const { return _Bytes.data(); }

	void WriteBytes(const Byte* Bits, size_t size)
	{
		for (size_t i = 0; i < size; i++)
		{
			_Bytes.push_back(Bits[i]);
		}
	}

	template<typename T>
	UCodeLangForceinline  void WriteTypeAsBytes(const T& Value){WriteBytes((const Byte*)&Value, sizeof(Value));}


	template<typename T>
	UCodeLangForceinline  void WriteType(const T& Value){ BitData<T>::ToBytes(*this, Value);}


	template<>
	UCodeLangForceinline  void WriteType(const bool& Value) { WriteTypeAsBytes(BitConverter::GetBytes(Value)); }

	template<>
	UCodeLangForceinline  void WriteType(const Byte& Value) { WriteTypeAsBytes(BitConverter::GetBytes(Value)); }

	template<>
	UCodeLangForceinline  void WriteType(const Int8& Value) { WriteTypeAsBytes(BitConverter::GetBytes(Value)); }

	template<>
	UCodeLangForceinline  void WriteType(const char& Value) { WriteTypeAsBytes(BitConverter::GetBytes(Value)); }

	template<>
	UCodeLangForceinline  void WriteType(const UInt16& Value) { WriteTypeAsBytes(BitConverter::GetBytes(Value));}

	template<>
	UCodeLangForceinline  void WriteType(const Int16& Value) { WriteTypeAsBytes(BitConverter::GetBytes(Value)); }
	
	template<>
	UCodeLangForceinline  void WriteType(const UInt32& Value) { WriteTypeAsBytes(BitConverter::GetBytes(Value)); }
	
	template<>
	UCodeLangForceinline  void WriteType(const Int32& Value) { WriteTypeAsBytes(BitConverter::GetBytes(Value)); }
	
	template<>
	UCodeLangForceinline  void WriteType(const UInt64& Value) { WriteTypeAsBytes(BitConverter::GetBytes(Value)); }

	template<>
	UCodeLangForceinline  void WriteType(const Int64& Value) { WriteTypeAsBytes(BitConverter::GetBytes(Value)); }
	
	template<>
	UCodeLangForceinline  void WriteType(const float32& Value) { WriteTypeAsBytes(BitConverter::GetBytes(Value)); }

	template<>
	UCodeLangForceinline  void WriteType(const float64& Value) { WriteTypeAsBytes(BitConverter::GetBytes(Value)); }


	//
	
	UCodeLangForceinline  void WriteType(const char*& Value)
	{ 
		size_t size = strlen(Value);
		WriteType((SizeAsBits)size);
		WriteBytes((const Byte*)Value, size);
	}
	UCodeLangForceinline  void WriteType(const char* const& Value)
	{
		size_t size = strlen(Value);
		WriteType((SizeAsBits)size);
		WriteBytes((const Byte*)Value, size);
	}

	template<>
	UCodeLangForceinline  void WriteType(const String& Value) 
	{
		size_t size = Value.size();
		WriteType((SizeAsBits)size);
		WriteBytes((const Byte*)Value.c_str(), size);
	}

	template<typename T>
	UCodeLangForceinline  void WriteType(const Vector<T>& Value) 
	{  
		size_t size = Value.size();
		WriteType((SizeAsBits)size);
		for (size_t i = 0; i < size; i++)
		{
			WriteType(Value[i]);
		}
	}

private:
	Vector<Byte> _Bytes;
};
class BitReader
{
public:
	using SizeAsBits = BitMaker::SizeAsBits;
	using Byte = BitMaker::Byte;
	BitReader()
	{

	}
	BitReader(const Byte* Bytes, size_t Size)
	{
		SetBytes(Bytes, Size);
	}
	~BitReader()
	{

	}

	UCodeLangForceinline void SetBytes(const Byte* Bytes,size_t Size)
	{
		_Bytes = Bytes;
		_BytesSize = Size;
		Resetoffset();
	}
	UCodeLangForceinline void Resetoffset()
	{
		_BitOffset = 0;
	}

	UCodeLangForceinline void Set_offset(size_t Value)
	{
		_BitOffset = Value;
	}
	UCodeLangForceinline size_t Get_offset()
	{
		return _BitOffset;
	}

	UCodeLangForceinline void Increment_offset(size_t Value)
	{
		_BitOffset += Value;
	}
	UCodeLangForceinline void Decrement_offset(size_t Value)
	{
		_BitOffset += Value;
	}
	UCodeLangForceinline Byte& GetByteWith_offset(size_t Value)
	{
		return ((Byte*)_Bytes)[_BitOffset + Value];
	}
	UCodeLangForceinline Byte& GetByteWith_Noffset(size_t Value)
	{
		return  ((Byte*)_Bytes)[Value];
	}


	template<typename T>
	void ReadType(T& Out)
	{
		BitData<T>::FromBytes(*this,Out);
	}

	template<typename T> void ReadType(T& Out,const T& fallback)
	{
		try
		{
			ReadType(Out);
		}
		catch (const std::exception&)
		{
			Out = fallback;
		}
	}	
	
	UCodeLangForceinline void SizeCheck(size_t Size)
	{
		if (_BitOffset + Size > _BytesSize)
		{
			throw std::exception("No More Values");
		}
	}
	//
	template<> void ReadType(Byte& Out)
	{
		SizeCheck(sizeof(Byte));
		Out = BitConverter::BytesToUChar(_Bytes, _BitOffset);
		_BitOffset += sizeof(Byte);
	}
	template<> void ReadType(Int8& Out)
	{
		SizeCheck(sizeof(Int8));
		Out =  BitConverter::BytesToSChar(_Bytes, _BitOffset);
		_BitOffset += sizeof(Int8);
	}
	template<> void ReadType(char& Out)
	{
		SizeCheck(sizeof(char));
		 BitConverter::BytesToChar(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(char);
	}
	template<> void ReadType(bool& Out)
	{
		SizeCheck(sizeof(bool));
		 BitConverter::BytesToBool(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(bool);
	}


	template<> void ReadType(UInt16& Out)
	{
		SizeCheck(sizeof(UInt16));
		 BitConverter::BytesToInt16(_Bytes, _BitOffset,&Out);
		_BitOffset += sizeof(UInt16);
	}
	template<> void ReadType(Int16& Out)
	{
		SizeCheck(sizeof(Int16));
		 BitConverter::BytesToInt16(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(Int16);
	}

	template<> void ReadType(UInt32& Out)
	{
		SizeCheck(sizeof(UInt32));
		 BitConverter::BytesToInt(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(UInt32);
	}
	template<> void ReadType(Int32& Out)
	{
		SizeCheck(sizeof(Int32));
		 BitConverter::BytesToInt(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(Int32);
	}

	template<> void ReadType(UInt64& Out)
	{
		SizeCheck(sizeof(UInt64));
		 BitConverter::BytesToInt64(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(UInt64);
	}
	template<> void ReadType(Int64& Out)
	{
		SizeCheck(sizeof(Int64));
		 BitConverter::BytesToInt64(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(Int64);
	}

	
	template<> void ReadType(float32& Out)
	{
		SizeCheck(sizeof(float32));
		 BitConverter::BytesTofloat(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(float32);
	}

	template<> void ReadType(float64& Out)
	{
		SizeCheck(sizeof(float64));
		BitConverter::BytesTofloat64(_Bytes, _BitOffset, &Out);
		_BitOffset += sizeof(float64);
	}
	//
	Unique_Array<char> ReadCString()
	{
		SizeAsBits StrSize;
		ReadType<SizeAsBits>(StrSize, 0);
		size_t Size = StrSize;

		auto Out =std::make_unique<char[]>(Size);
		for (size_t i = 0; i < Size; i++)
		{
			ReadType(Out[i]);
		}
		return Out;
	}
	template<> void ReadType(String& Out)
	{
		SizeAsBits StrSize;
		ReadType<SizeAsBits>(StrSize, 0);
		size_t Size = StrSize;

		Out.resize(StrSize);
		for (size_t i = 0; i < Size; i++)
		{
			ReadType(Out[i]);
		}
		
	}
	template<typename T> void ReadType(Vector<T>& Out)
	{
		SizeAsBits StrSize;
		ReadType<SizeAsBits>(StrSize, 0);
		size_t Size = StrSize;

		Out.resize(StrSize);
		for (size_t i = 0; i < Size; i++)
		{
			ReadType(Out[i]);
		}
	}
private:
	const void* _Bytes = nullptr;
	size_t _BytesSize = 0;
	size_t _BitOffset = 0;
};
UCodeLangEnd

