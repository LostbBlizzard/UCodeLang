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

	Return_t ThisCall(UAddress This, const String& FunctionName, parameters Pars = NullParameters);
	Return_t ThisCall(UAddress This, UAddress address, parameters Pars = NullParameters);
	UCodeLangForceinline Return_t ThisCall(UAddress This, const ClassMethod& Function, parameters Pars = NullParameters)
	{
		return ThisCall(This, Function.FullName, Pars);
	}

	Return_t Call(const String& FunctionName, parameters Pars = NullParameters);
	Return_t Call(UAddress address, parameters Pars = NullParameters);
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

