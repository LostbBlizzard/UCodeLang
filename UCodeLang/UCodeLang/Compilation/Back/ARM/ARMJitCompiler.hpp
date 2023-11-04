#pragma once
#include "../Jit/Jit.hpp"
#include "UCodeLang/RunTime/RunTimeLangState.hpp"

#include "ARMBuilder.hpp"
#include "ARMIR.hpp"
UCodeLangStart
class ARMJitCompiler :public JitCompiler
{
public:
	void Reset();
	bool BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& ArmOutput);
	void SubCall(FuncType Value, uintptr_t CPPOffset, void* ArmOutput);
private:
};

UCodeLangEnd