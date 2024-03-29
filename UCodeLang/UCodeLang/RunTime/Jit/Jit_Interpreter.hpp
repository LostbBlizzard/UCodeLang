#pragma once

#include "Jit.hpp"
#include "UCodeLang/RunTime/Interpreters/Interpreter.hpp"
#include "JitCompilers.hpp"
#include "Jit_State.hpp"
#include "UCodeLang/LangCore/DataType/UnorderedMap.hpp"
UCodeLangStart


/*
Trys to convert UCodeInstruction to CPP call's and fall's back on the Interpreter if it can't.
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
	~Jit_Interpreter() 
	{
		#if UCodeLangDebug
		InDestruct = true;
		#endif 
		UnLoad(); 
	}
	Jit_Interpreter(Jit_Interpreter&& Other) = default;
	Jit_Interpreter& operator=(Jit_Interpreter&& Other) = default;
	Jit_Interpreter(const Jit_Interpreter& Other) = delete;
	Jit_Interpreter& operator=(const Jit_Interpreter& Other) = delete;

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

		#if UCodeLangDebug
		WasInit = true;
		#endif
	}
	void UnLoad()
	{
		#if UCodeLangDebug
		if (!InDestruct)
		{
			UCodeLangAssert(WasInit == true);
		}
	
		GotRetValue = false;
		CalledFuncBefor = false;
		#endif

		#if HasSupportforJit
		UFuncToCPPFunc.clear();
		#endif
		#if UCodeLang_KeepJitInterpreterFallback
		#if UCodeLangDebug
		_Interpreter.InDestruct = InDestruct;
		#endif
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
	
	//
	template<typename... Args> Return_t Call(const String& FunctionName, Args... parameters)
	{
		if (CheckIfFunctionExist(FunctionName)) 
		{
			return Call(Get_State()->FindAddress(FunctionName).value(),parameters...);
		}
		return Return_t(RetState::Error_Function_doesnt_exist);
	}
	template<typename... Args> Return_t Call(UAddress address, Args... parameters)
	{
		PushParameters(parameters...);
		return Call(address);
	}
	Return_t Call(const ClassMethod* Function)
	{
		return Call(Function->DecorationName);
	}


	template<typename T, typename... Args>
	T RCall(UAddress address, Args... parameters)
	{
		///if (CheckIfFunctionExist(FunctionName))
		{
			#if HasSupportforJit
			{//fast call
				if (UFuncToCPPFunc.HasValue(address))
				{
					using Func = T(*)(Args...);
					Func Ptr = (Func)UFuncToCPPFunc.GetValue(address).NativeFunc;
					return Ptr(parameters...);
				}
			}
			#endif

			auto V = Call(address, parameters...);
			if (V._Succeed == RetState::Success)
			{
				return Get_Return<T>();
			}
		}
		return {};
	}

	template<typename T, typename... Args>
	T RCall(const String& FunctionName, Args... parameters)
	{
		if (CheckIfFunctionExist(FunctionName))
		{
			return RCall<T>(Get_State()->FindAddress(FunctionName).value(), parameters...);
		}
		return {};
	}

	template<typename T, typename... Args>
	T RCall(const ClassMethod* Function, Args... parameters)
	{
		return RCall<T>(Function->DecorationName,parameters...);
	}
	

	template<typename... Args> Return_t ThisCall(const String& FunctionName, PtrType This, Args... parameters)
	{
		return Call(FunctionName,This,parameters...);
	}
	template<typename... Args>	Return_t ThisCall(UAddress address,PtrType This, Args... parameters)
	{
		return Call(address, This, parameters...);
	}
	template<typename... Args> Return_t ThisCall(const ClassMethod* Function,PtrType This, Args... parameters)
	{
		return ThisCall(Function->DecorationName, This, parameters...);
	}
	
	template<typename T, typename... Args>
	T RThisCall(const ClassMethod* Function,PtrType This, Args... parameters)
	{
		return RCall<T>(Function->DecorationName, This,parameters...);
	}
	template<typename T, typename... Args> T RThisCall(const String& Function,PtrType This, Args... parameters)
	{
		return RCall<T>(Function,This, parameters...);
	}
	//

	template<typename... Args> void PushParameters(Args... parameters)
	{
		([&]
		{
				PushParameter(parameters);
		} (), ...);
	}
	template<typename T> UCodeLangForceinline void PushParameter(const T& Value)
	{
		PushParameter((const void*)&Value, sizeof(T));
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


	inline void* GetThreadPtr()
	{
		return _Interpreter.GetThreadPtr();
	}
	inline const void* GetThreadPtr() const
	{
		return _Interpreter.GetThreadPtr();
	}
	bool AlwaysJit = false;
	String GetJitState();

	void TryBuildAllFuncs();

	//Makes UClib with the Generated native code and be used for fast reloading just link it in RunTimeState instead of the compiled byte-code.
	UClib GetStateAsLib();

	Optional<InterpretorError> XCall(UAddress address)
	{
		Call(address);
		return  CheckForIntperpreterError();
	}

	Optional<InterpretorError> XCall(const ClassMethod* Function)	
	{
		return XCall(Function->DecorationName);
	}

	Optional<InterpretorError> XCall(const String& FuncionName)
	{	
		UCodeLangAssert(CheckIfFunctionExist(FuncionName));
		return XCall(Get_State()->FindAddress(FuncionName).value());
	}
	template<typename T, typename... Args>
	Result<T,InterpretorError> RXCall(UAddress address, Args... parameters)
	{
		PushParameters(parameters...);
		auto r = XCall(address);
		if (r.has_value())
		{
			return r.value();
		}
		return Get_Return<T>();
	}

	template<typename T,typename... Args>
	Result<T,InterpretorError> RXCall(const ClassMethod* Funcion, Args... parameters)
	{
		return RXCall<T>(Funcion->DecorationName,parameters...);
	}

	template<typename T,typename... Args>
	Result<T,InterpretorError> RXCall(const String&  FuncionName, Args... parameters)
	{
   	UCodeLangAssert(CheckIfFunctionExist(FuncionName));
		return RXCall<T>(Get_State()->FindAddress(FuncionName,parameters...));
	}
	template<typename T,typename... Args>
	Result<T,InterpretorError> RXThisCall(const ClassMethod* Funcion,PtrType This, Args... parameters)
	{
		return RXCall<T>(Funcion->DecorationName,This,parameters...);
	}

	template<typename T,typename... Args>
	Result<T,InterpretorError> RXThisCall(const String&  FuncionName,PtrType This, Args... parameters)
	{
		return RXCall<T>(FuncionName,This,parameters...);
	}
	Optional<InterpretorError> CheckForIntperpreterError();
private:
	
	#if UCodeLangDebug
	bool WasInit = false;
	bool InDestruct = false;
	bool GotRetValue = false;
	bool CalledFuncBefor = false;
	#endif // DEBUG


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

	
	UnorderedMap<UAddress, JitFuncData> UFuncToCPPFunc;
	Vector<Byte> TepOutBuffer;
	#endif


	static void OnUAddressCall(UAddress addresstojit);
	void Call_CPPFunc(JitFunc ToCall);


	void BuildCheck(Jit_Interpreter::JitFuncData& Item, const UCodeLang::UAddress& address);
	void LogASM();
	Optional<String> GetNameForHex(const String& Hex);

	bool ShouldJit(UAddress address, const Vector<Instruction>& Insts);
};

UCodeLangEnd
