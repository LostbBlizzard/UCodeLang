#pragma once

#include "Jit.hpp"
#include "UCodeLang/RunTime/Interpreters/Interpreter.hpp"
#include "JitCompilers.h"
UCodeLangStart

//UCodeLang_KeepJitInterpreterFallback
class Jit_Interpreter
{
public:
	using CPPInput = InterpreterCPPinterface&;
	using JitFunc = RunTimeLib::CPPCallBack;
	using Return_t = Interpreter::Return_t;
	using RetState = Interpreter::RetState;
	Jit_Interpreter(){}
	~Jit_Interpreter() { UnLoad(); }
	Jit_Interpreter(const Jit_Interpreter& Other) = delete;
	Jit_Interpreter(Jit_Interpreter&& Other) = default;

	void Init(RunTimeLangState* State)
	{
		#if UCodeLang_KeepJitInterpreterFallback
		_Interpreter.Init(State);
		#else
		this->State = State;
		#endif
	}
	void UnLoad()
	{
		UFuncToCPPFunc.clear();
		#if UCodeLang_KeepJitInterpreterFallback
		_Interpreter.UnLoad();
		#endif
	}
	UCodeLangForceinline auto Get_State()
	{
		#if UCodeLang_KeepJitInterpreterFallback
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


	template<typename T, typename... Args>
	T RCall(const String& FunctionName, Args&&... parameters)
	{
		if (CheckIfFunctionExist(FunctionName))
		{
			auto V = Call(FunctionName,parameters...);
			if (V._Succeed == RetState::Success)
			{
				return Get_Return<T>();
			}
		}
		return {};
	}
	template<typename T, typename... Args>
	T RThisCall(PtrType This, const ClassMethod& Function, Args&&... parameters)
	{
		return RThisCall(This, Function.FullName, Args&&... parameters)
	}
	template<typename T, typename... Args> T RThisCall(PtrType This, const String& Function, Args&&... parameters)
	{
		if (CheckIfFunctionExist(FunctionName))
		{
			auto V = ThisCall(This, Function, parameters);
			if (V._Succeed == RetState::Success)
			{
				return Get_Return<T>();
			}
		}
		return {};
	}

	//

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

	template<typename T> T Get_Return()
	{
		T r;
		Get_Return(&r, sizeof(T));
		return r;
	}
	void Get_Return(void* Output, size_t OutputSize);
private:
	#if UCodeLang_KeepJitInterpreterFallback
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
	Unordered_map<UAddress, JitFuncData> UFuncToCPPFunc;
	Vector<UInt8> TepOutBuffer;
	static CPPCallRet OnUAddressCall();
	CPPCallRet Call_CPPFunc(JitFunc ToCall);
	
};

UCodeLangEnd