#pragma once
#include "Jit/Jit_Interpreter.hpp"
UCodeLangStart


//Use This if you know The CPU Core count at compile time.
//#define UCodeLangCPUCoreKnow
//#define UCodeLangCPUCoreCount 4
 
//This is Best used as obect static
class UCodeRunTime
{
public:
	using Return_t = Jit_Interpreter::Return_t;

	UCodeRunTime();
	~UCodeRunTime();
	void Init(RunTimeLangState* State);
	void UnLoad();
	UCodeLangForceinline auto Get_State()
	{
		return _MainInterpreter.Get_State();
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
		return _MainInterpreter.Call(FunctionName, parameters...);
	}
	template<typename... Args> Return_t Call(UAddress address, Args&&... parameters)
	{
		return _MainInterpreter.Call(address, parameters...);
	}
	
	template<typename... Args> Return_t ThisCall(UAddress This, const String& FunctionName, Args&&... parameters)
	{
		return _MainInterpreter.ThisCall(This, FunctionName, parameters...);
	}
	template<typename... Args> Return_t ThisCall(UAddress This, UAddress address, Args&&... parameters)
	{
		return _MainInterpreter.ThisCall(This, address, parameters...);
	}

	template<typename... Args> UCodeLangForceinline Return_t ThisCall(UAddress This, const ClassMethod& Function, Args&&... parameters)
	{
		return ThisCall(This, Function.FullName);
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
	void PushParameter(const void* Value, size_t ValueSize) { return _MainInterpreter.PushParameter(Value, ValueSize); }
	bool CheckIfFunctionExist(const String& FunctionName) { return _MainInterpreter.CheckIfFunctionExist(FunctionName); }
private:
	Jit_Interpreter _MainInterpreter;

	#ifdef UCodeLangCPUCoreKnow
	static constexpr size_t CPUCoresCount = UCodeLangCPUCoreCount;
	Array<Jit_Interpreter,CPUCoresCount> _Interpreters;
	#else
	Vector<Jit_Interpreter> _Interpreters;
	#endif // DEBUG
};
UCodeLangEnd

