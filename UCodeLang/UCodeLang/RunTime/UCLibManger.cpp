#include "UCLibManger.hpp"

UCodeLangStart
UCLibManger::UCLibManger()
{
}

UCLibManger::~UCLibManger()
{
}

void UCLibManger::Link()
{
	for (const auto Item : Libs)
	{
		LinkLib(Item);
	}
	InitExCode();
}

void UCLibManger::LinkLib(UCodeLang::RunTimeLib* Item)
{
	auto& Ins_s = Item->Get_Instructions();
	size_t oldSize = _Instructions.size();
	_Instructions.reserve(oldSize + Ins_s.size());

	size_t oldSize2 = _Code.size();
	_Code.reserve(oldSize2 + _Code.size());

	for (const auto& Item2 : Ins_s)
	{
		Instruction Tep = Item2;

		if (Tep.OpCode == InstructionSet::LoadFuncPtr)
		{
			Tep.Value0.AsUIntNative += oldSize;
		}
		else if (Tep.OpCode == InstructionSet::Call)
		{
			Tep.Value0.AsUIntNative += oldSize;
		}
		else if (Tep.OpCode == InstructionSet::Call_Code)
		{
			Tep.Value0.AsUIntNative += oldSize;
		}

		_Instructions.push_back(Tep);
	}

	if (Item->Get_Lib())
	{
		for (const auto& Item2 : Item->Get_Lib()->Get_NameToPtr())
		{
			_NameToAddress[Item2._Key] = Item2._Value;
		}
		for (const auto& Item : Item->Get_Lib()->Get_StaticBytes())
		{
			StaticBytes.push_back(Item);
		}

		for (const auto& Item : Item->Get_Lib()->Get_Code())
		{
			_Code.push_back(Item);
		}

		auto& _Assembly = Item->Get_Lib()->Get_Assembly();
		ClassAssembly::PushCopyClasses(_Assembly, Assembly);
	}
	for (const auto& Item2 : Item->Get_CPPCalls())
	{
		_NameToCPP[Item2._Key] = Item2._Value;
	}
}

void UCLibManger::InitExCode()
{
	_ExCode.Alloc(_Code.data(), _Code.size());
	_ExCode.SetToExecuteMode();
}

UCodeLangEnd

