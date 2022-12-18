#pragma once

#include "Jit.hpp"
#include "UCodeLang/Compliation/BackEnd/NativeAssembler.hpp"
#include "UCodeLang/RunTime/Interpreters/Interpreter.hpp"
UCodeLangStart
class Jit_Interpreter
{
public:
	using JitFunc = RunTimeLib::CPPCallBack;
	Jit_Interpreter(){}
	~Jit_Interpreter(){}

	void Init(RunTimeLangState* State)
	{
		_Interpreter.Init(State);
	}
	void UnLoad()
	{
		UFuncToCPPFunc.clear();
		_Interpreter.UnLoad();
	}
	UCodeLangForceinline auto Get_State()
	{
		return _Interpreter.Get_State();
	}

	Interpreter::Return_t ThisCall(PtrType This,const String& FunctionName, parameters Pars = NullParameters);
	Interpreter::Return_t ThisCall(PtrType This, UAddress address, parameters Pars = NullParameters);
	UCodeLangForceinline Interpreter::Return_t ThisCall(PtrType This, const ClassMethod& Function, parameters Pars = NullParameters)
	{
		return ThisCall((PtrType)This, Function.FullName, Pars);
	}

	Interpreter::Return_t Call(const String& FunctionName, parameters Pars = NullParameters);
	Interpreter::Return_t Call(UAddress address, parameters Pars = NullParameters);
private:
	Interpreter _Interpreter;
	NativeAssembler _Assembler;
	AsmBuffer ExBuffer = AsmBuffer(Get_EnvironmentData().PageSize);
	size_t Insoffset = 0;

	enum class JitFuncType :UInt8
	{
		Null,
		UCodeCall,
		CPPCall,
	};
	struct JitFuncData
	{
		JitFuncType Type = JitFuncType::Null;
		union 
		{
			UAddress UCodeFunc;
			JitFunc Func = nullptr;
		};
	};
	unordered_map<UAddress, JitFuncData> UFuncToCPPFunc;
	Vector<UInt8> TepOutBuffer;

	UCodeLangForceinline AnyInt64 Call_CPPFunc(JitFunc ToCall,parameters& Pars)
	{
		_Interpreter.PushParameters(Pars);
		InterpreterCPPinterface Inter = &_Interpreter; ;
		auto r = ToCall(Inter);
		return r;
	}
};

UCodeLangEnd