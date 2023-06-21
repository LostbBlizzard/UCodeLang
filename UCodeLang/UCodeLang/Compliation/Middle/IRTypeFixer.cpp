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

	//FixSize
	for (auto& Sys : _Input->_Symbols)
	{
		switch (Sys->SymType)
		{
		case IRSymbolType::Struct:
		{
			Input->Fix_Size(Sys->Get_ExAs<IRStruct>());
		}
			break;
		default:
			break;
		}
	}


	for (auto& Sys : _Input->_Symbols)
	{
		switch (Sys->SymType)
		{
		case IRSymbolType::ThreadLocalVarable:
		case IRSymbolType::StaticVarable:
		{
			IRBufferData* b = Sys->Get_ExAs<IRBufferData>();
			if (b->Bytes.size() == 0) 
			{
				b->Bytes.resize(_Input->GetSize(Sys->Type));
			}
		}
		break;
		default:
			break;
		}
	}

	OnFunc(&_Input->_StaticInit);

	OnFunc(&_Input->_StaticdeInit);

	OnFunc(&_Input->_threadInit);

	OnFunc(&_Input->_threadInit);

	for (auto& Func : _Input->Funcs)
	{
		OnFunc(Func.get());
	}
}
void IRTypeFixer::OnFunc(IRFunc* Func)
{
	_Func = Func;
	for (auto& Block : Func->Blocks)
	{
		for (auto& Ins : Block->Instructions)
		{
			if (Ins->Type == IRInstructionType::Member_Access)
			{
				if (Ins->Target().Type == IROperatorType::IRInstruction)
				{
					auto ClassType = Ins->Target().Pointer->ObjectType;
					GetMemberAccessTypeForIns(ClassType, _Input, Ins);
				}
				else if (Ins->Target().Type == IROperatorType::IRParameter)
				{
					auto ClassType = Ins->Target().Parameter->type;
					GetMemberAccessTypeForIns(ClassType, _Input, Ins);

					if (!InList(Ins->Target().Parameter, _Func->Pars))
					{
						LogErrorCantFindPar(Ins->Target());
					}
				}
				else
				{

					throw std::exception("not added");
				}
				continue;
			}
			else if (Ins->Type == IRInstructionType::Member_Access_Dereference)
			{
				if (Ins->Target().Type == IROperatorType::IRInstruction)
				{
					auto ClassType = Ins->Target().Pointer->ObjectType;
					GetMemberAccessTypeForIns(ClassType, _Input, Ins);
				}
				else if (Ins->Target().Type == IROperatorType::IRParameter)
				{
					auto ClassType = Ins->Target().Parameter->type;
					GetMemberAccessTypeForIns(ClassType, _Input, Ins);

					if (!InList(Ins->Target().Parameter, _Func->Pars))
					{
						LogErrorCantFindPar(Ins->Target());
					}
				}
				else
				{


					throw std::exception("not added");
				}
				continue;
			}
			else
				if (!Ins->ObjectType.IsType(IRTypes::Null)) { continue; }
			if (Ins->Type == IRInstructionType::None) { continue; }
			if (Ins->Type == IRInstructionType::Jump) { continue; }

			if (Ins->Type == IRInstructionType::Reassign)
			{
				OnOp(*Ins, Ins->Target());
			}
			else if (Ins->Type == IRInstructionType::Reassign_dereference)
			{
				OnOp(*Ins, Ins->Input());
			}
			else if (IsLoadValueOnInput(Ins->Type))
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
					auto V = _Input->GetSymbol(Ob_T._symbol);
					if (V->SymType == IRSymbolType::FuncPtr)
					{
						auto FuncPtr = V->Get_ExAs<IRFuncPtr>();
						Ins->ObjectType = FuncPtr->Ret;
					}
				}
			}
			else if (Ins->Type == IRInstructionType::Call)
			{
				auto Item = _Input->GetFunc(Ins->Target().identifer);
				if (Item)
				{
					Ins->ObjectType = Item->ReturnType;
				}
			}
			else
			{
				throw std::exception("not added");
			}

		}
	}
}
void IRTypeFixer::GetMemberAccessTypeForIns(UCodeLang::IRType& ClassType, UCodeLang::IRBuilder* Input, UCodeLang::Unique_ptr<UCodeLang::IRInstruction>& Ins)
{
	if (ClassType.IsType(IRTypes::IRsymbol))
	{
		auto Syb = Input->GetSymbol(ClassType._symbol);
		auto StructType = Syb->Get_ExAs<IRStruct>();


		Ins->ObjectType = StructType->Fields[Ins->Input().Value.AsUIntNative].Type;
	}
}
void IRTypeFixer::OnOp(IRInstruction& Ins, IROperator& Op)
{
	//Ins->Type =

	if (Op.Type == IROperatorType::IRInstruction)
	{
		if (Op.Pointer->Type == IRInstructionType::Member_Access_Dereference)
		{
			auto& StructType = Op.Pointer->ObjectType;
			auto Type = _Input->GetSymbol(StructType._symbol)->Get_ExAs<IRStruct>();
			auto feildType = Type->Fields[Op.Pointer->Input().Value.AsUIntNative].Type;
			Ins.ObjectType= feildType;
		}
		else {
			Ins.ObjectType = Op.Pointer->ObjectType;
		}
	}
	else if (Op.Type == IROperatorType::Get_PointerOf_IRInstruction
		|| Op.Type == IROperatorType::Get_PointerOf_IRParameter
		|| Op.Type == IROperatorType::Get_PointerOf_IRidentifier)
	{
		Ins.ObjectType = IRType(IRTypes::pointer);

		if (Op.Type == IROperatorType::Get_PointerOf_IRParameter)
		{
			if (!InList(Op.Parameter, _Func->Pars))
			{
				LogErrorCantFindPar(Op);
			}
		}
	}
	else if (Op.Type == IROperatorType::IRParameter)
	{
		Ins.ObjectType =Op.Parameter->type;


		if (!InList(Op.Parameter,_Func->Pars))
		{
			LogErrorCantFindPar(Op);
		}
	}
	else if (Op.Type == IROperatorType::Get_Func_Pointer)
	{
		auto FunSym = _Input->GetFunc(Op.identifer);

		if (FunSym)
		{
			for (auto& FuncSyb : _Input->_Symbols)
			{
				if (FuncSyb->SymType == IRSymbolType::FuncPtr)
				{
					IRFuncPtr* V = FuncSyb->Get_ExAs<IRFuncPtr>();
					bool Same = _Input->IsTheSame(V, FunSym);
					if (Same)
					{
						Ins.ObjectType.SetType(FuncSyb->identifier);
						return;
					}

				}
			}

		}


		Ins.ObjectType.SetType(IRTypes::pointer);
	}
	else if (Op.Type == IROperatorType::IRidentifier)
	{
		auto V = _Input->GetSymbol(Op.identifer);
		Ins.ObjectType =V->Type;

	}
	else
	{
		throw std::exception("bad");
	}
}
void IRTypeFixer::LogErrorCantFindPar(UCodeLang::IROperator& Op)
{
	IRPar* Par = Op.Parameter;
	auto ParName= _Input->FromID(Par->identifier);
	String MSG = "InternalCompilerError: the IR Funcion does not have the name Parameter Named '" + ParName + "'";

	MSG += "the pointer is pointing to ";

	IRFunc* Func =nullptr;
	for (auto& Item : _Input->Funcs)
	{
		for (auto& ItemPar : Item->Pars)
		{
			if (&ItemPar == Par)
			{
				Func = Item.get();
				break;
			}
		}

		if (Func) { break; }
	}

	if (Func == nullptr)
	{
		MSG += "[Cant Find Funcion]";
	}
	else
	{
		MSG += "'" + _Input->FromID(Func->identifier) + "''s '" + ParName + '\'';
	}

	MSG += "and not '" + _Input->FromID(_Func->identifier) + "'";

	_Errs->AddError(ErrorCodes::InternalCompilerError, 0, 0,MSG);
}
UCodeLangEnd

