#include "Debuger.hpp"
#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
UCodeLangStart
void Debuger::Attach(Interpreter* interpreter)
{
	_Interpreter = interpreter;
}

void Debuger::DebugerOnHit(void* inter, void* Object)
{
	Interpreter* interpreter = (Interpreter*)inter;
	Debuger* This = (Debuger*)Object;

	UAddress LandOn = interpreter->_CPU.ProgramCounter;
}

void Debuger::AddRunTimeBreakPoint(UAddress Item, OnBreakPointCallBack OnHit)
{
	auto& Ins = _Interpreter->_State->GetInst(Item);
	BreakPointData _data;
	_data._CallBack = OnHit;
	_data._OldIns = Ins;
	_data._BreakPoint = Item;

	_CodeBreakPoints.AddValue(Item,std::move(_data));
	

	InstructionBuilder::DebugBreakPoint(Ins, DebugerOnHit, this);
}



void Debuger::RemoveRunTimeBreakPoint(UAddress Value)
{
	_CodeBreakPoints.erase(Value);
}

void Debuger::UpdateDebugData(DebugData& Out)
{
	bool ShouldUpdated = !Out._BreakPoint.has_value() || (Out._BreakPoint.value() != _CurrentBreakPoint);
	if (ShouldUpdated)
	{
		Out._BreakPoint = _CurrentBreakPoint;
	}
}

UCodeLangEnd