#pragma once

#include "Jit.hpp"
#include "UCodeLang/RunTime/Interpreters/Interpreter.hpp"
#include "JitCompilers.h"
UCodeLangStart

//UCodeLang_RemoveInterpreterFallback
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
		#if UCodeLang_RemoveInterpreterFallback
		_Interpreter.Init(State);
		#else
		this->State = State;
		#endif
	}
	void UnLoad()
	{
		UFuncToCPPFunc.clear();
		#if UCodeLang_RemoveInterpreterFallback
		_Interpreter.UnLoad();
		#endif
	}
	UCodeLangForceinline auto Get_State()
	{
		#if UCodeLang_RemoveInterpreterFallback
		return _Interpreter.Get_State();
		#else
		return State;
		#endif
	}

	

	Return_t Call(const String& FunctionName);
	Return_t Call(UAddress address);
	
	Return_t ThisCall(UAddress This,const String& FunctionName);
	Return_t ThisCall(UAddress This, UAddress address);
	UCodeLangForceinline Return_t ThisCall(UAddress This, const ClassMethod& Function)
	{
		return ThisCall(This, Function.FullName);
	}


	//
	template<typename... Args> Return_t Call(const String& FunctionName, Args&&... parameters)
	{
		if (CheckIfFunctionExist(FunctionName)) 
		{
			PushParameters(parameters...);
			return Call(FunctionName);
		}
		return Return_t(RetState::Error_Function_doesnt_exist);
	}
	template<typename... Args> Return_t Call(UAddress address, Args&&... parameters)
	{
		PushParameters(parameters...);
		return Call(address);
	}
	template<typename... Args> Return_t ThisCall(UAddress This, const String& FunctionName, Args&&... parameters)
	{
		if (CheckIfFunctionExist(FunctionName))
		{
			PushParameter(This);
			PushParameters(parameters...);
			return Call(FunctionName);
		}
		return Return_t(RetState::Error_Function_doesnt_exist);
	}
	template<typename... Args>	Return_t ThisCall(UAddress This, UAddress address, Args&&... parameters)
	{
		PushParameter(This);
		PushParameters(parameters...);
		return Call(address);
	}
	template<typename... Args> UCodeLangForceinline Return_t ThisCall(UAddress This, const ClassMethod& Function, Args&&... parameters)
	{
		return ThisCall(This, Function.FullName, parameters...);
	}


	template<typename... Args> void PushParameters(Args&&... parameters)
	{
		([&]
		{
				PushParameter(parameters);
		} (), ...);
	}
	template<typename T> UCodeLangForceinline void PushParameter(const T& Value)
	{
		PushParameter((const void*)&Value, sizeof(Value));
	}
	void PushParameter(const void* Value, size_t ValueSize);
	
	
	bool CheckIfFunctionExist(const String& FunctionName);
private:
	#if UCodeLang_RemoveInterpreterFallback
	Interpreter _Interpreter;
	#else
	RunTimeLangState* State = nullptr;
	#endif
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