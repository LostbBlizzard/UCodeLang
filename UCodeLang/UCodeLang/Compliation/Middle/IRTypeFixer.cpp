#include "IRTypeFixer.hpp"
UCodeLangStart


void IRTypeFixer::Reset()
{

	auto ErrorsOutput = _Errs;

	
	this->~IRTypeFixer();
	new (this)  IRTypeFixer;
	
	
	this->_Errs = ErrorsOutput;

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
				if (!Ins->ObjectType.IsType(IRTypes::Null)) { continue; }

				if (Ins->Type == IRInstructionType::Reassign)
				{
					OnOp(*Ins, Ins->Target());
				}
				if (Ins->Type == IRInstructionType::Reassign_dereference)
				{
					OnOp(*Ins, Ins->Input());
				}
				else if (IsLoadValue(Ins->Type))
				{
					OnOp(*Ins, Ins->Target());
				}
				else if (IsUnary(Ins->Type))
				{
					OnOp(*Ins, Ins->Target());
				}
				else if (IsBinary(Ins->Type))
				{
					OnOp(*Ins, Ins->Target());
				}
				else if (IsAction(Ins->Type))
				{
					Ins->ObjectType._Type = IRTypes::Void;
				}
				else if (Ins->Type == IRInstructionType::MallocCall)
				{
					Ins->ObjectType = IRType(IRTypes::pointer);
				}
				else if (Ins->Type == IRInstructionType::CallFuncPtr)
				{
					OnOp(*Ins, Ins->Target());
					auto Ob_T = Ins->ObjectType;
					if (Ob_T.IsType(IRTypes::IRsymbol))
					{
						auto V = Input->GetSymbol(Ob_T._symbol);
						if (V->SymType == IRSymbolType::FuncPtr)
						{
							auto FuncPtr = V->Get_ExAs<IRFuncPtr>();
							Ins->ObjectType = FuncPtr->Ret;
						}
					}
				}
				else if (Ins->Type == IRInstructionType::Call)
				{
					for (auto& Func2 : _Input->Funcs)
					{
						if (Func2->identifier == Ins->Target().identifer)
						{
							Ins->ObjectType = Func2->ReturnType;
							break;
						}
					}
				}
				else
				{
					throw std::exception("not added");
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
		Ins.ObjectType =Op.Parameter->type;
	}
}
UCodeLangEnd

