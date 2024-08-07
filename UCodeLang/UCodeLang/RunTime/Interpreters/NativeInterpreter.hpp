#pragma once

#include "../RunTimeLangState.hpp"
#include "..//../LangCore.hpp"
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "Interpreter.hpp"
UCodeLangStart
class NativeInterpreter
{
public:
	using CPPInput = InterpreterCPPinterface&;
	using JitFunc = RunTimeLib::CPPCallBack;
	using Return_t = Interpreter::Return_t;
	using RetState = Interpreter::RetState;
	NativeInterpreter()
	{
	
	}
	~NativeInterpreter()
	{
		UnLoad();
	}
	void Init(RunTimeLangState* State)
	{
		_State = State;
	}
	UCodeLangForceinline auto Get_State() { return _State; }
	bool CheckIfFunctionExist(const String& FunctionName);

	Return_t Call(const String& FunctionName);
	Return_t Call(UAddress address);
	Return_t Call(const ClassMethod* Function)
	{
		return Call(Function->DecorationName);
	}

	template<typename... Args> Return_t ThisCall(UAddress address, PtrType This, Args... parameters)
	{
		PushParameter(This);
		PushParameters(parameters...);
		return Call(address);
	}
	template<typename... Args> Return_t ThisCall(const String& FunctionName, PtrType This, Args... parameters)
	{
		if (CheckIfFunctionExist(FunctionName))
		{
			return ThisCall(Get_State()->FindAddress(FunctionName).value(), This, parameters...);
		}
		return Return_t(RetState::Error_Function_doesnt_exist);
	}
	template<typename... Args> Return_t ThisCall(const ClassMethod* Function, PtrType This, Args... parameters)
	{
		return ThisCall(Function->DecorationName, This, parameters...);
	}



	template<typename T, typename... Args>
	T RCall(const String& FunctionName, Args... parameters)
	{
		if (CheckIfFunctionExist(FunctionName))
		{
			PushParameters(parameters...);

			auto V = Call(FunctionName);
			if (V._Succeed == RetState::Success)
			{
				return Get_Return<T>();
			}
		}
		return {};
	}
	template<typename T, typename... Args>
	T RCall(const ClassMethod* Function, Args... parameters)
	{
		return RCall<T>(Function->DecorationName, parameters...);
	}
	template<typename T, typename... Args>
	T RThisCall(const ClassMethod* Function, PtrType This, Args... parameters)
	{
		return RThisCall<T>(Function->DecorationName, This, parameters...);
	}
	template<typename T, typename... Args> T RThisCall(const String& Function, PtrType This, Args... parameters)
	{
		if (CheckIfFunctionExist(Function))
		{
			auto V = ThisCall(Function, This, parameters...);
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
		(

			PushParameter(parameters)
			, ...);
	}

	template<typename T> UCodeLangForceinline void PushParameter(const T& Value)
	{
		PushParameter((const void*)&Value, sizeof(Value));
	}
	void PushParameter(const void* Value, size_t ValueSize)
	{
		
	}

	template<typename T> T Get_Return()
	{
		T r;
		Get_Return(&r, sizeof(T));
		return r;
	}
	void Get_Return(void* Output, size_t OutputSize);

	void UnLoad()
	{

	}
	inline void* GetThreadPtr()
	{
		return nullptr;
	}
	inline const void* GetThreadPtr() const
	{
		return nullptr;
	}
	//Error Calls
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
		return XCall(_State->FindAddress(FuncionName).value());
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
		return RXCall<T>(_State->FindAddress(FuncionName,parameters...));
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
	RunTimeLangState* _State = nullptr;
	ParameterPassingHelper _Parameters;
};
UCodeLangEnd
