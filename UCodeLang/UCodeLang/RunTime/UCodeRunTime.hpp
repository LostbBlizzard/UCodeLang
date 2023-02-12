#pragma once
#include "Jit/Jit_Interpreter.hpp"
UCodeLangStart


//Use This if you know The CPU Core count at compile time.
//#define UCodeLangCPUCoreKnow
//#define UCodeLangCPUCoreCount 4
 
//Use This if You cant jit on platfrom
//#define RunTimeCantBeJit 1

//This is Best used as obect static
class UCodeRunTime
{
public:

	#if RunTimeCantBeJit
	using MyInterpreter = Interpreter;
	#else
	using MyInterpreter = Jit_Interpreter;
	#endif // RunTimeCantBeJit

	
	using Return_t = MyInterpreter::Return_t;
	using RetState = MyInterpreter::RetState;

	UCodeRunTime();
	~UCodeRunTime();
	void Init(RunTimeLangState* State);
	void UnLoad();
	MyInterpreter& Get_MyInterpreter()
	{
		return _MainInterpreter;
	}
	UCodeLangForceinline auto Get_State()
	{
		return Get_MyInterpreter().Get_State();
	}
	

	Return_t Call(const String& FunctionName);
	Return_t Call(UAddress address);
	
	Return_t ThisCall(UAddress This, const String& FunctionName);
	Return_t ThisCall(UAddress This, UAddress address);
	UCodeLangForceinline Return_t ThisCall(UAddress This, const ClassMethod& Function)
	{
		return ThisCall(This, Function.FullName);
	}
	//
	

	template<typename... Args> Return_t Call(const String& FunctionName, Args&&... parameters)
	{
		return Get_MyInterpreter().Call(FunctionName, parameters...);
	}
	template<typename... Args> Return_t Call(UAddress address, Args&&... parameters)
	{
		return Get_MyInterpreter().Call(address, parameters...);
	}
	
	template<typename... Args> Return_t ThisCall(UAddress This, const String& FunctionName, Args&&... parameters)
	{
		return Get_MyInterpreter().ThisCall(This, FunctionName, parameters...);
	}
	template<typename... Args> Return_t ThisCall(UAddress This, UAddress address, Args&&... parameters)
	{
		return Get_MyInterpreter().ThisCall(This, address, parameters...);
	}

	template<typename... Args> UCodeLangForceinline Return_t ThisCall(UAddress This, const ClassMethod& Function, Args&&... parameters)
	{
		return ThisCall(This, Function.FullName);
	}


	//
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
	void PushParameter(const void* Value, size_t ValueSize) { return Get_MyInterpreter().PushParameter(Value, ValueSize); }
	bool CheckIfFunctionExist(const String& FunctionName) { return Get_MyInterpreter().CheckIfFunctionExist(FunctionName); }

	template<typename T> T Get_Return()
	{
		T r;
		Get_Return(&r, sizeof(T));
		return r;
	}
	void Get_Return(void* Output, size_t OutputSize);
private:
	MyInterpreter _MainInterpreter;

	#ifdef UCodeLangCPUCoreKnow
	static constexpr size_t CPUCoresCount = UCodeLangCPUCoreCount;
	Array<Jit_Interpreter,CPUCoresCount> _Interpreters;
	#else
	Vector<MyInterpreter> _Interpreters;
	#endif // DEBUG
};
UCodeLangEnd

