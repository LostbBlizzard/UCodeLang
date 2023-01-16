#pragma once

#include "Jit.hpp"
#include "UCodeLang/RunTime/Interpreters/Interpreter.hpp"
#include "JitCompilers.h"
UCodeLangStart

class Jit_Interpreter
{
public:
	using CPPInput = InterpreterCPPinterface&;
	using JitFunc = RunTimeLib::CPPCallBack;
	using Return_t = Interpreter::Return_t;
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

	Return_t ThisCall(UAddress This,const String& FunctionName);
	Return_t ThisCall(UAddress This, UAddress address);
	UCodeLangForceinline Return_t ThisCall(UAddress This, const ClassMethod& Function)
	{
		return ThisCall(This, Function.FullName);
	}

	Return_t Call(const String& FunctionName);
	Return_t Call(UAddress address);
private:
	Interpreter _Interpreter;
	NativeJitAssembler _Assembler;
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
	static void OnUAddressCall(CPPInput Cpp)
	{
		//UAddress V = Cpp.GetParameters<UAddress>();
		//Call(V);
	}

	static void UCodeLangAPI Tep(CPPInput Cpp)
	{


		Cpp.Set_Return();
	}
	AnyInt64 Call_CPPFunc(JitFunc ToCall);
	
};

UCodeLangEnd