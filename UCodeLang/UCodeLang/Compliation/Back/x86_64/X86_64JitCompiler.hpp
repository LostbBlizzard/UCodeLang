#pragma once
#include "../Jit/Jit.hpp"

UCodeLangStart
class X86_64JitCompiler
{
public:
	X86_64JitCompiler();
	~X86_64JitCompiler();
	void Reset();


	bool BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X64Output);


	

	void SubCall(JitInfo::FuncType Value, uintptr_t CPPOffset, Vector<UInt8>& X64Output);

	Vector<NullJitCalls> NullCalls;
private:
	Vector<UInt8>* Output = nullptr;
	Vector<Instruction>* _Ins =nullptr;
	size_t Index = 0;
	bool IsGood =false;

	inline auto& Get_Ins() { return _Ins->operator[](Index); }
	inline void Next_Ins() { Index++; }


	void BuildBody();
	
	
	template<typename T>
	void PushBytes(UInt8 Byte, T& Value)
	{
		PushBytes(Byte);
		PushBytes(Value);
	}

	
	void PushByte(UInt8 Byte)
	{
		PushBytes(Byte);
	}

	template<typename T>
	void PushBytes(const char* Bytes,T& Value)
	{
		PushBytes(Bytes);
		PushBytes(Value);
	}
	template<typename T>
	void PushBytes(const char* Bytes, T& Value, const char* Bytes2)
	{
		PushBytes(Bytes);
		PushBytes(Value);
		PushBytes(Bytes2);
	}

	void PushBytes(const char* Bytes)
	{
		PushBytes((const UInt8*)Bytes, strlen(Bytes));
	}
	
	void PushBytes(const UInt8* Bytes,size_t Bits)
	{
		for (size_t i = 0; i < Bits; i++)
		{
			Output->push_back(Bytes[i]);
		}
	}
	template<typename T>
	void PushBytes(const T& Bytes)
	{

		for (size_t i = 0; i < sizeof(T); i++)
		{
			Output->push_back( ((UInt8*)&Bytes)[i] );
		}
	}
};
UCodeLangEnd