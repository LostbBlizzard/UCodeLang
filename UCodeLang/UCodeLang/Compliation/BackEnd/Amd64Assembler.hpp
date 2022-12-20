#pragma once
#pragma once
#include "../../LangCore.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../Helpers/InstructionBuilder.hpp"
#include "../CompliationSettings.hpp"
#include "../Front/SemanticAnalysis.hpp"

UCodeLangStart

class Amd64Assembler
{
public:
	Amd64Assembler(){}
	~Amd64Assembler(){}
	void Reset();
	bool BuildFunc(const Vector<Instruction>& Data, size_t Offset,Vector<UInt8>& Output);

	typedef void (*VoidFunc)();

	struct NullCalls_t
	{
		UAddress UCodeAddress;
		size_t CPPoffset;
	};
	Vector<NullCalls_t> NullCalls;

	
	static void BuildCall(VoidFunc FuncOffset, Vector<UInt8>& Output)
	{
		auto offset = BuildTepCall(Output);
		SubCall(FuncOffset, offset, Output);
	}
	static void SubCall(VoidFunc FuncOffset, size_t offset, Vector<UInt8>& Output);
	static size_t BuildTepCall(Vector<UInt8>& Output);

	template<typename T> UCodeLangForceinline static void PushBytes(const T& Value, Vector<UInt8>& Output)
	{
		UInt8* V = (UInt8*)&Value;
		for (size_t i = 0; i < sizeof(T); i++)
		{
			Output.push_back(*V);
			V += 1;
		}
	}

	template<typename T> UCodeLangForceinline static void SubBytes(const T& Value, size_t offset, Vector<UInt8>& Output)
	{
		UInt8* OutV = &Output[offset];
		UInt8* V = (UInt8*)&Value;
		for (size_t i = 0; i < sizeof(T); i++)
		{
			*OutV = *V;
			
			V++; 
			OutV++;
		}
	}
private:
	
};

UCodeLangEnd

