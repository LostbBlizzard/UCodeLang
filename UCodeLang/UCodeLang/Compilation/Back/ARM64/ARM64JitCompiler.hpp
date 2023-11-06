#pragma once

#include "../Jit/Jit.hpp"
#include "UCodeLang/RunTime/RunTimeLangState.hpp"

#include "ARM64Builder.hpp"
#include "ARM64IR.hpp"
UCodeLangStart
class ARM64JitCompiler :public JitCompiler
{
public:
	void Reset();
	bool BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& ArmOutput);
	void SubCall(FuncType Value, uintptr_t CPPOffset, void* ArmOutput);
private:
};

UCodeLangEnd