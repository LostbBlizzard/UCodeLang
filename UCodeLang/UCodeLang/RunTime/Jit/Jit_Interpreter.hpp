#pragma once

#include "Jit.hpp"
#include "UCodeLang/RunTime/Interpreters/Interpreter.hpp"
#include "JitCompilers.h"
#include "Jit_State.hpp"
#include "UCodeLang/LangCore/DataType/BinaryVectorMap.hpp"
UCodeLangStart


/*
Trys to convert UCodeInstruction to CPP call's and fall's back on the Interpreter if it cant.
if you need more speed use the NativeInterpreter and NativeULangCompilerBackEnd.
*/
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
	Jit_Interpreter& operator=(const Jit_Interpreter& Other) = default;

	void Init(RunTimeLangState* State,Jit_State* JitState = nullptr)
	{
		#if UCodeLang_KeepJitInterpreterFallback
		_Interpreter.Init(State);
		#else
		this->State = State;
		#endif
		#if HasSupportforJit
		this->jitState = JitState;
		#endif
	}
	void UnLoad()
	{
		#if HasSupportforJit
		UFuncToCPPFunc.clear();
		#endif
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
		return ThisCall(This, Function.DecorationName);
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
		return ThisCall(This, Function.DecorationName, parameters...);
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
	T RCall(const ClassMethod& Function, Args&&... parameters)
	{
		return RCall<T, Args...>(Function.DecorationName,parameters...);
	}
	template<typename T, typename... Args>
	T RThisCall(PtrType This, const ClassMethod& Function, Args&&... parameters)
	{
		return RThisCall(This, Function.DecorationName, Args&&... parameters)
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


	bool IsDebug = false;
	bool AlwaysJit = false;
private:
	
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
		void* NativeFunc = nullptr;
	};
//	
	#if UCodeLang_KeepJitInterpreterFallback
	Interpreter _Interpreter;
	#else
	RunTimeLangState* State = nullptr;
	#endif


	#if HasSupportforJit
	Jit_State* jitState = nullptr;

	
	NativeJitAssembler _Assembler;
	

	AsmBuffer ExBuffer = AsmBuffer(Get_EnvironmentData().PageSize);
	size_t Insoffset = 0;

	
	BinaryVectorMap<UAddress, JitFuncData> UFuncToCPPFunc;
	Vector<Byte> TepOutBuffer;
	#endif


	static void OnUAddressCall(UAddress addresstojit);
	CPPCallRet Call_CPPFunc(JitFunc ToCall);


	void BuildCheck(Jit_Interpreter::JitFuncData& Item, const UCodeLang::UAddress& address);
	void LogASM();
	Optional<String> GetNameForHex(const String& Hex);

	bool ShouldJit(UAddress address, const Vector<Instruction>& Insts);
};

UCodeLangEnd