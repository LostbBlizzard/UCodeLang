#pragma once
#include "../Jit/Jit.hpp"
#include "X86_64Gen.hpp"
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
	UAddress OnUAddressPar=0;
private:
	Vector<UInt8>* Output = nullptr;
	Vector<Instruction>* _Ins =nullptr;
	
	X86_64Gen _Gen;
	void BuildSysCallIns(InstructionSysCall Ins, RegisterID Reg);
};
UCodeLangEnd