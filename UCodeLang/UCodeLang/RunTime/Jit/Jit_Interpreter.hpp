#pragma once

#include "Jit.hpp"
#include "UCodeLang/Compliation/BackEnd/NativeAssembler.hpp"
#include "UCodeLang/RunTime/Interpreters/Interpreter.hpp"
UCodeLangStart
class Jit_Interpreter
{
public:
	Jit_Interpreter(){}
	~Jit_Interpreter(){}

	void Init(RunTimeLangState* State)
	{
		_Interpreter.Init(State);
	}
	void UnLoad()
	{
		_Interpreter.UnLoad();
	}

	Interpreter::Return_t Call(const String& FunctionName, parameters Pars = NullParameters);
	Interpreter::Return_t Call(UAddress address, parameters Pars = NullParameters);
private:
	Interpreter _Interpreter;
	NativeAssembler _Assembler;
};

UCodeLangEnd