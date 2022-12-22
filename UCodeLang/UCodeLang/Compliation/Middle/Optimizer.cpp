#include "Optimizer.hpp"
#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
UCodeLangStart
#define LibUpdate() UpdatedLib = true;
void Optimizer::optimiz(UClib& Lib)
{
	Reset();
	_Lib = &Lib;
	
	if (_Settings->_Flags == OptimizationFlags::NoOptimization) { return; }

	
	do
	{
		UpdatedLib = false;
		for (auto Item : Lib.Get_NameToPtr())
		{
			optimiz_FuncForSize(Item.second);
		}
	} while (UpdatedLib);
}
void Optimizer::Reset()
{
	_Lib = nullptr;
	Index = 0;
}
UAddress Optimizer::GetFuncEnd(UAddress Index)
{
	UAddress FuncEnd = 0;
	for (size_t i = Index; i < _Lib->Get_Instructions().size(); i++)
	{
		auto& Item = _Lib->Get_Instructions()[i];
		if (Item.OpCode == InstructionSet::Return)
		{
			FuncEnd = i;
			break;
		}
	}
	return FuncEnd;
}
void Optimizer::optimiz_FuncForSize(UAddress FuncIndex)
{
	UAddress FuncEnd = GetFuncEnd(Index);
	for (Index = FuncEnd - 1; Index >= FuncIndex; Index--)
	{
		auto& Item = _Lib->Get_Instructions()[Index];
		
		switch (Item.OpCode)
		{
		case InstructionSet::Pop8:
		{
			auto NextIns = GetLastIns();
			if (NextIns == nullptr) { continue; }

			if (NextIns->OpCode == InstructionSet::Push8)
			{
				LibUpdate();
				InstructionBuilder::Store8RegToPtr(*NextIns, Item.Value0.AsRegister, NextIns->Value0.AsRegister);
				RemoveIns(Index);
				
				Index--;//
			}
		}break;
		case InstructionSet::Store8RegToReg:
		{
			RemoveIns(Index);
		}break;

		default:
			break;
		}
	}
}
UCodeLangEnd