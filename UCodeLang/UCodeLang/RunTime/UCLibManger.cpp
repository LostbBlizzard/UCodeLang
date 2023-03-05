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
}

void UCLibManger::LinkLib(UCodeLang::RunTimeLib* Item)
{
	auto& Ins_s = Item->Get_Instructions();
	size_t oldSize = _Instructions.size();
	_Instructions.reserve(oldSize + Ins_s.size());
	for (const auto& Item2 : Ins_s)
	{
		Instruction Tep = Item2;

		if (Tep.OpCode == InstructionSet::LoadFuncPtr)
		{
			Tep.Value0.AsUIntNative += oldSize;
		}
		else
		if (Tep.OpCode == InstructionSet::Call)
		{
			Tep.Value0.AsUIntNative += oldSize;
		}

		_Instructions.push_back(Tep);
	}

	if (Item->Get_Lib())
	{
		for (const auto& Item2 : Item->Get_Lib()->Get_NameToPtr())
		{
			_NameToAddress[Item2.first] = Item2.second;
		}
		for (const auto& Item : Item->Get_Lib()->Get_StaticBytes())
		{
			StaticBytes.push_back(Item);
		}

		auto& _Assembly = Item->Get_Lib()->Get_Assembly();
		ClassAssembly::PushCopyClasses(_Assembly, Assembly);
	}
	for (const auto& Item2 : Item->Get_CPPCalls())
	{
		_NameToCPP[Item2.first] = Item2.second;
	}
}

UCodeLangEnd

