#include "ProfilerDebuger.hpp"
UCodeLangStart

AnyInterpreterPtr Cast(DebugContext::InterpreterInfo Info)
{
	if (Info.Type == DebugContext::Type::Interpreter)
	{
		return AnyInterpreterPtr((Interpreter*)Info.ThisInterpreter);
	}
	else
	{
		throw std::exception("not added");
	}
}

UCodeLangAPIExport void ProfilerDebuger::Attach(RunTimeLangState* state)
{
	_state = state;
	auto& Context = _state->Get_DebugContext();
	Context._Ptr = this;
	Context._FuncStart = [](RunTimeLangState& This, void* _Ptr, DebugContext::InterpreterInfo Info)
	{
		((ProfilerDebuger*)_Ptr)->OnFuncStart(Cast(Info));
	};
	Context._FuncEnd = [](RunTimeLangState& This, void* _Ptr, DebugContext::InterpreterInfo Info)
	{
		((ProfilerDebuger*)_Ptr)->OnFuncEnd(Cast(Info));
	};
	Context._FuncOnLine= [](RunTimeLangState& This, void* _Ptr, DebugContext::InterpreterInfo Info)
	{
		((ProfilerDebuger*)_Ptr)->OnFuncOnline(Cast(Info));
	};
}
UCodeLangAPIExport void ProfilerDebuger::AddRunTimeBreakPoint(UAddress Item, OnBreakPointCallBack OnHit)
{
    return;
}

UCodeLangAPIExport void ProfilerDebuger::RemoveRunTimeBreakPoint(UAddress Item)
{
    return;
}

UCodeLangAPIExport void ProfilerDebuger::UpdateDebugData(DebugData& Out)
{
    return;
}

void ProfilerDebuger::OnFuncStart(AnyInterpreterPtr Ptr)
{

	auto now = ProfilerClock::now();
	if (Ptr.Get_InterpreterType() == InterpreterTypes::Interpreter)
	{
		auto Interpreter = Ptr.GetAs_Interpreter();

		DebugStackFrame frame;
		frame._Funcion = Interpreter->_CPU.ProgramCounter;
		frame._Method = _state->GetMethod(frame._Funcion);
		frame.FuncStart = { now };
		

		_Debug._StackFrames.push_back(std::move(frame));
	}
}
void ProfilerDebuger::OnFuncEnd(AnyInterpreterPtr Ptr)
{
	auto now = ProfilerClock::now();
	if (Ptr.Get_InterpreterType() == InterpreterTypes::Interpreter)
	{
		auto Interpreter = Ptr.GetAs_Interpreter();

		_Debug._StackFrames.pop_back();
	}
}
void ProfilerDebuger::OnFuncOnline(AnyInterpreterPtr Ptr)
{
	auto now = ProfilerClock::now();
	if (Ptr.Get_InterpreterType() == InterpreterTypes::Interpreter)
	{
		auto Interpreter = Ptr.GetAs_Interpreter();
		std::cout << "OnFuncion On Line" << std::endl;

		auto diff = now - LastOnLineTime;
		InFunc.push_back(diff);


		std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (diff).count() << "[ms]" << std::endl;
		std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << "[microsecond]" << std::endl;
		std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (diff).count() << "[ns]" << std::endl;

		LastOnLineTime = ProfilerClock::now();

	}
}
UCodeLangEnd