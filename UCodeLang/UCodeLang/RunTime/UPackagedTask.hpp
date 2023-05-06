#pragma once

#include "../LangCore.hpp"
#include "Interpreters/Interpreter.hpp"
UCodeLangStart

//template<typename Interpreter_t>
class UPackagedTask_t
{
public:
	using Interpreter_t = Interpreter;
	UPackagedTask_t() { Reset(); }
	~UPackagedTask_t(){}

	void Reset()
	{
		_Parameters.Clear();
		Func = NullAddress;
	}
	

	void Invoke(Interpreter_t& This)
	{
		auto State = _Parameters.StartLoop();
		while (_Parameters.Next(State))
		{
			size_t Size = _Parameters.GetSize(State);
			const void* Pointer = _Parameters.GetPointer(State);
			This.PushParameter(Pointer, Size);
		}
		This.Call(Func);
	}

	template<typename T> T RInvoke(Interpreter_t& This)
	{
		T CompilerRet;
		RInvoke(This, &CompilerRet, sizeof(T));
		return CompilerRet;
	}

	void RInvoke(Interpreter_t& This, void* OutObject, size_t ReturnObjectSize)
	{
		auto State = _Parameters.StartLoop();
		while (_Parameters.Next(State))
		{
			size_t Size = _Parameters.GetSize(State);
			const void* Pointer = _Parameters.GetPointer(State);
			This.PushParameter(Pointer, Size);
		}
		//
		This.Call(Func);
		This.Get_Return(OutObject, ReturnObjectSize);
	}


	template<typename T> UCodeLangForceinline void PushParameter(const T& Value)
	{
		PushParameter((const void*)&Value, sizeof(Value));
	}
	void PushParameter(const void* Value, size_t ValueSize)
	{
		_Parameters.Push(Value, ValueSize);
	}
	void Set_Func(UAddress func)
	{
		Func = func;
	}

	void GetParameters(Interpreter_t& This)
	{
		
	}

	void GetParameters(Interpreter_t& This,const Vector<size_t> Varables)
	{

	}
private:
	ParameterPassingHelper _Parameters;
	UAddress Func=NullAddress;
};

//using UPackagedTask = UPackagedTask_t<Interpreter>;
using UPackagedTask = UPackagedTask_t;

UCodeLangEnd
