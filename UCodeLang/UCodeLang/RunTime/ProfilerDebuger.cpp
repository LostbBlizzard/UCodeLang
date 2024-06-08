#include "ProfilerDebuger.hpp"
UCodeLangStart

AnyInterpreterPtr Cast(DebugContext::InterpreterInfo Info)
{
	if (Info.type == DebugContext::Type::Interpreter)
	{
		return AnyInterpreterPtr((Interpreter*)Info.ThisInterpreter);
	}
	else
	{
		UCodeLangThrowException("not added");
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

UCodeLangAPIExport void ProfilerDebuger::UpdateDebugData(DebugData& Out,Cach& cach)
{
	auto stackoffset = StepedInterpreter->_CPU.Stack.StackOffSet;
	auto ins= GetCurrentInstruction();
	auto func = GetStartofFunc(ins);

	auto statckframesize = 0;

	Out._StackFrames.clear();


	auto& DebugInfo = StepedInterpreter->Get_State()->Get_Libs().Get_DebugInfo();

	DebugStackFrame F;
	F._Function = func;

	if (!cach.cach.has_value())
	{
		cach.cach = DebugInfo.MakeCach();
	}

	const ClassMethod* FuncString = nullptr;
	for (size_t i = func; i < ins; i++)
	{
		auto infoop = DebugInfo.GetForIns(i-1,cach.cach.value());
		if (!infoop.has_value())
		{
			continue;
		}
		auto& info = *infoop.value();

		for (auto& Item : info)
		{
			if (auto loc = Item->Debug.Get_If<UDebugSetVarableLoc>()) 
			{
				if (FuncString == nullptr)
				{
					FuncString = StepedInterpreter->Get_State()->GetMethod(func);
				}

				DebugVarable d;
				d.VarableName = ScopeHelper::GetNameFromFullName(loc->VarableFullName);

				const VarableInfo& Var = DebugInfo.VarablesInfo.GetValue(loc->VarableFullName);

				if (auto v = loc->Type.Get_If<RegisterID>())
				{
					d.Object = &StepedInterpreter->Get_Register(*v).Value;
				}
				else
				{
					UCodeLangUnreachable();
				}

				d.VarableType = DebugVarable::VarType::Stack;
				d.Type = Var.ReflectionType;


				bool wasadded = false;
				for (auto& Item : F._Varables)
				{
					if (Item.VarableName == d.VarableName)
					{
						Item = std::move(d);
						wasadded = true;
						break;
					}
				}
				if (wasadded == false) 
				{
					F._Varables.push_back(d);
				}
			}
		}
	}


	Out._StackFrames.push_back(std::move(F));


    return;
}
size_t ProfilerDebuger::GetLineNumber(UAddress Ins,Cach& cach)
{
	auto func = GetStartofFunc(Ins);

	auto& DebugInfo = StepedInterpreter->Get_State()->Get_Libs().Get_DebugInfo();

	size_t liner = 0;

	//this would faster if we go reverse
	for (size_t i = func; i < Ins; i++)
	{
		auto infoop = DebugInfo.GetForIns(i - 1, cach.cach.value());
		if (!infoop.has_value())
		{
			continue;
		}
		auto& info = *infoop.value();

		for (auto& Item : info)
		{
			if (auto loc = Item->Debug.Get_If<UDebugSetLineNumber>())
			{
				liner = loc->LineNumber;
			}
		}
	}

	return liner;
}
Path ProfilerDebuger::GetFile(UAddress Ins,Cach& cach)
{

	return {};
}

void ProfilerDebuger::StepInto(Interpreter* Interpreter, UAddress Address)
{
	auto OldStackPrePars = Interpreter->_CPU.Stack.StackOffSet;
	Interpreter->FlushParametersIntoCPU();

	auto OldStackoffset = Interpreter->_CPU.Stack.StackOffSet;
	Interpreter->_CPU.Stack.PushStack(Interpreter->_CPU.ProgramCounter);
	Interpreter->_CPU.ProgramCounter = Address;

	_OldStackPrePars = OldStackPrePars;
	_OldStackoffset = OldStackoffset;

	StepedInterpreter = Interpreter;
	IsInFunc = true;
}

bool ProfilerDebuger::HasExitedFunc()
{
	if (!IsInFunc)
	{
		return false;
	}
	return StepedInterpreter->_CPU.Stack.StackOffSet == _OldStackoffset;
}

void ProfilerDebuger::StepOutof()
{
	StepedInterpreter->_CPU.Stack.StackOffSet = _OldStackPrePars;
	IsInFunc = false;
}
void ProfilerDebuger::Continue()
{

}
void ProfilerDebuger::StepIn()
{
	bool Go = false;
	do
	{
		VM_StepIn();

		auto& Ins = StepedInterpreter->Get_State()->GetInst(GetCurrentInstruction());
		Go =
			Ins.OpCode == InstructionSet::Debug_LineEnter
		 || Ins.OpCode == InstructionSet::Return;
	} while (!Go);
}
void ProfilerDebuger::StepOver()
{
	bool Go = false;
	do
	{
		VM_StepOver();

		auto& Ins = StepedInterpreter->Get_State()->GetInst(GetCurrentInstruction());
		Go =
			Ins.OpCode == InstructionSet::Debug_LineEnter
			|| Ins.OpCode == InstructionSet::Return;
	} while (!Go);
}
void ProfilerDebuger::StepOut()
{
	VM_StepOut();
}
UAddress ProfilerDebuger::GetStartofFunc(UAddress Ins)
{
	UAddress v =Ins;
	while (v != 0)
	{
		if (StepedInterpreter->Get_State()->GetName(v) != "")
		{
			return v;
		}
		v--;
	}
	return 0;
}
void ProfilerDebuger::VM_StepIn()
{
	auto& v = StepedInterpreter->Get_State()->GetInst(GetCurrentInstruction());
	
	bool Is32bit = sizeof(void*) == 4;

	size_t Inslength = 1;
	if (v.OpCode == InstructionSet::Store32v1)
	{
		Inslength = 2;
	}
	else if (v.OpCode == InstructionSet::Store64v1)
	{
		Inslength = 4;
	}
	else if (v.OpCode == InstructionSet::Callv1
		|| v.OpCode == InstructionSet::LoadFuncPtrV1
		|| v.OpCode == InstructionSet::Jumpif
		|| v.OpCode == InstructionSet::Jumpv1)
	{
		Inslength = Is32bit ? 2 : 4;
	}


	for (size_t i = 0; i < Inslength; i++)
	{
		StepedInterpreter->Extecute(StepedInterpreter->Get_State()->GetInst(StepedInterpreter->_CPU.ProgramCounter));
		StepedInterpreter->_CPU.ProgramCounter++;
	}
	
}
void ProfilerDebuger::VM_StepOver()
{
	auto& v = StepedInterpreter->Get_State()->GetInst(GetCurrentInstruction());
	if (v.OpCode == InstructionSet::Callv1
		|| v.OpCode == InstructionSet::CallReg)
	{
		size_t vl = StepedInterpreter->GetStackOffset();
		do
		{
			VM_StepIn();
		} while (vl != StepedInterpreter->GetStackOffset());

	}
	else
	{
		VM_StepIn();
	}

}
void ProfilerDebuger::VM_StepOut()
{
	auto ThisStackFrame = StepedInterpreter->GetStackOffset();
	while (StepedInterpreter->Get_State()->GetInst(GetCurrentInstruction()).OpCode != InstructionSet::Return
		|| ThisStackFrame != StepedInterpreter->GetStackOffset())
	{
		VM_StepIn();
	}
	VM_StepIn();
}

void ProfilerDebuger::OnFuncStart(AnyInterpreterPtr Ptr)
{

	auto now = ProfilerClock::now();
	if (Ptr.Get_InterpreterType() == InterpreterTypes::Interpreter)
	{
		auto Interpreter = Ptr.GetAs_Interpreter();

		DebugStackFrame frame;
		frame._Function = Interpreter->_CPU.ProgramCounter;
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
		


		LastOnLineTime = ProfilerClock::now();

	}
}
UCodeLangEnd