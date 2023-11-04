#pragma once
#include "Interpreters/Interpreter.hpp"
#include "../Compilation/Compiler.hpp"
UCodeLangStart
class REPL_RunTime
{
public:
	REPL_RunTime()
	{

	}
	~REPL_RunTime()
	{

	}
	void Set_Compiler(Compiler* Compiler)
	{
		_Compiler = Compiler;
	}
	void Set_Interpreter(Interpreter* Interpreter)
	{
		_Interpreter = Interpreter;
	}
	

	void ReadLine(String_view Text);

private:
	Compiler* _Compiler =nullptr;
	Interpreter* _Interpreter =nullptr;
};

UCodeLangEnd