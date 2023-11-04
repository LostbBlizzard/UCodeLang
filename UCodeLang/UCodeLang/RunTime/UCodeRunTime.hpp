#pragma once
#include "Jit/Jit_Interpreter.hpp"
UCodeLangStart


//Use This if you know The CPU Core count at compile time.
//#define UCodeLangCPUCoreKnow
//#define UCodeLangCPUCoreCount 4
 
//Use This if You can't jit on platfrom
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

	UCodeRunTime(const UCodeRunTime& Other) = delete;
	UCodeRunTime& operator=(const UCodeRunTime& Other) = default;

	void Init(RunTimeLangState* State, Optional<int> SubInterpreterCount = {});
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
	
	Return_t ThisCall(const String& FunctionName, PtrType This)
	{
		return Get_MyInterpreter().ThisCall(FunctionName, This);
	}
	Return_t ThisCall(UAddress address, PtrType This)
	{
		return Get_MyInterpreter().ThisCall(address, This);
	}
	UCodeLangForceinline Return_t ThisCall(const ClassMethod& Function, PtrType This)
	{
		return ThisCall(Function.DecorationName, This);
	}
	//
	

	template<typename... Args> Return_t Call(const String& FunctionName, Args... parameters)
	{
		return Get_MyInterpreter().Call(FunctionName, parameters...);
	}
	template<typename... Args> Return_t Call(UAddress address, Args... parameters)
	{
		return Get_MyInterpreter().Call(address, parameters...);
	}
	
	template<typename... Args> Return_t ThisCall(const String& FunctionName,PtrType This, Args... parameters)
	{
		return Get_MyInterpreter().ThisCall(FunctionName, This, parameters...);
	}
	template<typename... Args> Return_t ThisCall(UAddress address, PtrType This, Args... parameters)
	{
		return Get_MyInterpreter().ThisCall(address, This, parameters...);
	}

	template<typename... Args> Return_t ThisCall(const ClassMethod& Function,PtrType This, Args... parameters)
	{
		return ThisCall(Function.DecorationName, This);
	}


	//
	template<typename T, typename... Args>
	T RCall(const String& FunctionName, Args... parameters)
	{
		return  Get_MyInterpreter().RCall<T>(FunctionName, parameters...);
	}
	template<typename T, typename... Args>
	T RThisCall(const ClassMethod& Function, PtrType This, Args... parameters)
	{
		return RThisCall<T>(Function.DecorationName, This, parameters...);
	}
	template<typename T, typename... Args> T RThisCall(const String& Function, PtrType This, Args... parameters)
	{
		return Get_MyInterpreter().RThisCall(Function, This, parameters...);
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

	MyInterpreter& Get_MainInterpreter()
	{
		return _MainInterpreter;
	}
	auto& GetInterpretersList()
	{
		return  _Interpreters;
	}
private:
	MyInterpreter _MainInterpreter;

	#ifdef UCodeLangCPUCoreKnow
	static constexpr size_t CPUCoresCount = UCodeLangCPUCoreCount;
	Array<Jit_Interpreter,CPUCoresCount> _Interpreters;
	#else
	Vector<Unique_ptr<MyInterpreter>> _Interpreters;
	#endif // DEBUG
	Jit_State _State;
};
UCodeLangEnd

