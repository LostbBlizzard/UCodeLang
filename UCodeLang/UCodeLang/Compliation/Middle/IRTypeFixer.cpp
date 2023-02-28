#include "IRTypeFixer.hpp"
UCodeLangStart


void IRTypeFixer::Reset()
{

}

void IRTypeFixer::FixTypes(IRBuilder* Input)
{
	_Input = Input;
	for (auto& Func : _Input->Funcs)
	{
		for (auto& Block : Func->Blocks)
		{
			for (auto& Ins : Block->Instructions)
			{
				if (Ins->Type == IRInstructionType::Reassign)
				{
					OnOp(*Ins, Ins->Target());
				}

				if (IsLoadValue(Ins->Type))
				{
					OnOp(*Ins, Ins->Target());
				}
				if (IsUnary(Ins->Type))
				{
					OnOp(*Ins, Ins->Target());
				}
				if (IsBinary(Ins->Type))
				{
					OnOp(*Ins, Ins->Target());
				}
				if (Ins->Type == IRInstructionType::MallocCall)
				{
					Ins->ObjectType= IRType(IRTypes::pointer);
				}

					
			}
		}

	}
}
void IRTypeFixer::OnOp(IRInstruction& Ins, IROperator& Op)
{
	//Ins->Type =

	if (Op.Type == IROperatorType::IRInstruction)
	{
		Ins.ObjectType = Op.Pointer->ObjectType;
	}
	else if (Op.Type == IROperatorType::Get_PointerOf_IRInstruction)
	{
		Ins.ObjectType = IRType(IRTypes::pointer);
	}
	else if (Op.Type == IROperatorType::IRParameter)
	{
		Ins.ObjectType = IRType(Op.Parameter->type);
	}
}
UCodeLangEnd

