#include "IROptimizer.hpp"
#include <iostream>
UCodeLangStart

using OptimizationFunc = void(IROptimizer::*)();

struct OptimizationInfo
{
	OptimizationFlags AcitveIf= OptimizationFlags::O_None;
	bool InverseAcitveIf=false;

	
	union 
	{
		OptimizationFunc FuncCall=nullptr;
	};
	OptimizationInfo()
	{

	};
	OptimizationInfo(OptimizationFlags f, bool Inverse, OptimizationFunc Call)
		:AcitveIf(f), InverseAcitveIf(Inverse), FuncCall(Call)
	{

	};
};
static const Array<OptimizationInfo, IROptimizer::OptimizationCount> OptimizationList =
{

};

void IROptimizer::Reset() 
{
	this->~IROptimizer();
	new (this) IROptimizer;
}
void IROptimizer::Optimized(IRBuilder& IRcode)
{
	Input = &IRcode;
	UpdateOptimizationList();
	
	

	do
	{
		_TypeFixer.Set_ErrorsOutput(_ErrorsOutput);
		_TypeFixer.FixTypes(Input);
		if (_ErrorsOutput->Has_Errors())
		{
			return;
		}





		{//for debuging
			auto S = Input->ToString();

			std::cout << "-----" << std::endl;
			std::cout << S;
		}
		_UpdatedCode = false;

		for (auto& Func : Input->Funcs)
		{
			if (Func->Linkage == IRFuncLink::StaticExternalLink
				|| Func->Linkage == IRFuncLink::DynamicExternalLink)
			{
				if (Func->StaticPure.Read == PureState::Null)
				{
					Func->StaticPure.Read = PureState::ImPure;
				}
				if (Func->StaticPure.Write == PureState::Null)
				{
					Func->StaticPure.Write = PureState::ImPure;
				}
				if (Func->ThreadPure.Read == PureState::Null)
				{
					Func->ThreadPure.Read = PureState::ImPure;
				}
				if (Func->ThreadPure.Write == PureState::Null)
				{
					Func->ThreadPure.Write = PureState::ImPure;
				}
			}
		}
		for (auto& Func : Input->Funcs)
		{
			if (Func->Linkage == IRFuncLink::StaticExternalLink
				|| Func->Linkage == IRFuncLink::DynamicExternalLink)
			{
				continue;
			}
			IROptimizationFuncData& FuncData = Funcs[Func.get()];
			size_t InsCount = 0;

			for (auto& Block : Func->Blocks)
			{
				InsCount += Block->Instructions.size();
			}

			if (InsCount < 10)
			{
				FuncData.Inlinestate = InlineState::AutoInline;
			}
			else if (InsCount < 30)
			{
				FuncData.Inlinestate = InlineState::MayBeInlined;
			}
			else
			{
				FuncData.Inlinestate = InlineState::FuncionCantBeInline;
			}

		}
		//return;
		UpdateCodePass();

		//
		

		

	} while (_UpdatedCode);
	

}
void IROptimizer::UpdateOptimizationList()
{
	auto& Stettings = *_Settings;

	bool ForDebuging = (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationFlags::Debug;
	bool ForSize =  (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSize;
	bool ForSpeed = (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationFlags::ForSpeed;

	for (size_t i = 0; i < OptimizationCount; i++)
	{
		auto& OptimizationInfo = OptimizationList[i];
		auto& OptimizationState = OptimizationListState[i];
	

		bool IsAcitve = (OptimizationFlags_t)Stettings._Flags & (OptimizationFlags_t)OptimizationInfo.AcitveIf;
		if (OptimizationInfo.InverseAcitveIf){IsAcitve = !IsAcitve;}

		OptimizationState.IsActive = IsAcitve;
	}
}
void IROptimizer::UpdateCodePass()
{
	/*
	for (auto& Func : Input->Funcs)
	{
		UpdateCodePassFunc(Func.get());
	}
	*/

	for (auto& Func : Input->Funcs)
	{
		DoInlines(Func.get());
	}
}
void IROptimizer::UpdateCodePassFunc(const IRFunc* Func)
{
	SSAState _State;
	ToSSA(Func, _State);
	for (auto& Block : Func->Blocks)
	{
		for (auto& Ins : Block->Instructions)
		{
			Get_IRData(Ins.get()).IsReferenced = false;
		}

		for (size_t i = 0; i < Block->Instructions.size(); i++)
		{
			auto& Ins = Block->Instructions[i];
			if (Ins->Type == IRInstructionType::PushParameter)
			{

				ConstantFoldOperator(*Ins, Ins->Input());
				if (Ins->Target().Type == IROperatorType::IRInstruction)
				{
					Get_IRData(Ins->Target().Pointer).IsReferenced = true;
				}
			}
			else if (Ins->Type == IRInstructionType::Logical_Not)
			{
				ConstantFoldOperator(*Ins, Ins->Target());
				if (Ins->Target().Type == IROperatorType::IRInstruction)
				{
					Get_IRData(Ins->Input().Pointer).IsReferenced = true;
				}
			}
			if (Ins->Type == IRInstructionType::Reassign)
			{
				ConstantFoldOperator(*Ins, Ins->Input());
			}
			else if (Ins->Type == IRInstructionType::Load
				|| Ins->Type == IRInstructionType::LoadReturn)
			{
				ConstantFoldOperator(*Ins, Ins->Target());
			}
			if (IsBinary(Ins->Type))
			{
				ConstantFoldOperator(*Ins, Ins->A);
				ConstantFoldOperator(*Ins, Ins->B);
#define	ConstantBinaryFold(bits) \
					switch (Ins->Type) \
					{\
					case IRInstructionType::Add:Ins->Target().Value.AsInt##bits = Ins->A.Value.AsInt##bits  + Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::Sub:Ins->Target().Value.AsInt##bits = Ins->A.Value.AsInt##bits  - Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::UMult:Ins->Target().Value.AsUInt##bits = Ins->A.Value.AsUInt##bits  * Ins->B.Value.AsUInt##bits ;break;\
					case IRInstructionType::SMult:Ins->Target().Value.AsInt##bits  = Ins->A.Value.AsInt##bits  * Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::UDiv:Ins->Target().Value.AsUInt##bits  = Ins->A.Value.AsUInt##bits  / Ins->B.Value.AsUInt##bits ;break;\
					case IRInstructionType::SDiv:Ins->Target().Value.AsInt##bits  = Ins->A.Value.AsInt##bits  / Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::EqualTo:Ins->Target().Value.AsInt##bits  = Ins->A.Value.AsInt##bits  == Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::NotEqualTo:Ins->Target().Value.AsInt##bits  = Ins->A.Value.AsInt##bits  != Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::SGreaterThan:Ins->Target().Value.AsInt##bits = Ins->A.Value.AsInt##bits > Ins->B.Value.AsInt##bits; break; \
					case IRInstructionType::SLessThan:Ins->Target().Value.AsInt##bits = Ins->A.Value.AsInt##bits < Ins->B.Value.AsInt##bits; break; \
					case IRInstructionType::SGreaterThanOrEqual:Ins->Target().Value.AsInt##bits = Ins->A.Value.AsInt##bits >= Ins->B.Value.AsInt##bits; break; \
					case IRInstructionType::SLessThanOrEqual:Ins->Target().Value.AsInt##bits = Ins->A.Value.AsInt##bits <= Ins->B.Value.AsInt##bits; break; \
					case IRInstructionType::UGreaterThan:Ins->Target().Value.AsUInt##bits = Ins->A.Value.AsUInt##bits > Ins->B.Value.AsUInt##bits; break; \
					case IRInstructionType::ULessThan:Ins->Target().Value.AsUInt##bits = Ins->A.Value.AsUInt##bits < Ins->B.Value.AsUInt##bits; break; \
					case IRInstructionType::UGreaterThanOrEqual:Ins->Target().Value.AsUInt##bits = Ins->A.Value.AsUInt##bits >= Ins->B.Value.AsUInt##bits; break; \
					case IRInstructionType::ULessThanOrEqual:Ins->Target().Value.AsUInt##bits = Ins->A.Value.AsUInt##bits <= Ins->B.Value.AsUInt##bits; break; \
					case IRInstructionType::Logical_And:Ins->Target().Value.AsInt##bits  = Ins->A.Value.AsInt##bits && Ins->B.Value.AsInt##bits ;break;\
					case IRInstructionType::Logical_Or:Ins->Target().Value.AsInt##bits  = Ins->A.Value.AsInt##bits || Ins->B.Value.AsInt##bits ;break;\
					default:\
						throw std::exception("not added");\
						break;\
					}\

				if (Ins->A.Type == IROperatorType::Value && Ins->B.Type == IROperatorType::Value)
				{
					if (Ins->ObjectType.IsType(IRTypes::i8))
					{
						ConstantBinaryFold(8);
						Ins->Type = IRInstructionType::Load;
						UpdatedCode();
					}
					else if (Ins->ObjectType.IsType(IRTypes::i16))
					{
						ConstantBinaryFold(16);
						Ins->Type = IRInstructionType::Load;
						UpdatedCode();
					}
					else if (Ins->ObjectType.IsType(IRTypes::i32))
					{
						ConstantBinaryFold(32);
						Ins->Type = IRInstructionType::Load;
						UpdatedCode();
					}
					else if (Ins->ObjectType.IsType(IRTypes::i64))
					{
						ConstantBinaryFold(64);
						Ins->Type = IRInstructionType::Load;
						UpdatedCode();
					}
				}
			}



		}
		bool RemoveUnused = true;
		if (RemoveUnused) 
		{
			for (auto& Ins : Block->Instructions)
			{
				if (Ins->Type == IRInstructionType::Load) //removeing dead Instructions
				{
					auto& Data = Get_IRData(Ins.get());
					if (!Data.IsReferenced)
					{
						Ins->SetAsNone(); UpdatedCode();
					}
				}

				if (Ins->Type == IRInstructionType::LoadNone)//removeing dead Instructions
				{
					auto& Data = Get_IRData(Ins.get());
					if (!Data.IsReferenced)
					{
						Ins->SetAsNone(); UpdatedCode();
					}
				}

				if (Ins->Type == IRInstructionType::Reassign)//removeing dead Instructions
				{
					if (Ins->Target().Pointer->Type == IRInstructionType::None)
					{
						Ins->SetAsNone();
						UpdatedCode();
					}
				}
			}
		}
	}
	UndoSSA(Func, _State);
}
void IROptimizer::DoInlines(IRFunc* Func)
{
	for (auto& Item : Func->Blocks)
	{
		DoInlines(Item.get());
	}
}
void IROptimizer::DoInlines(IRBlock* Block)
{
	for (size_t i = 0; i < Block->Instructions.size(); i++)
	{
		auto& Ins = Block->Instructions[i];

		if (Ins->Type == IRInstructionType::Call)
		{
			auto* FuncToCall = Input->GetFunc(Ins->Target().identifer);
			if (FuncToCall)
			{
				IROptimizationFuncData& FuncData = Funcs[FuncToCall];

				if (FuncData.Inlinestate == InlineState::AutoInline)
				{
					InLineData Data;
					Data.Block = Block;
					Data.CallIndex = i;
					Data.Func = FuncToCall;
					InLineFunc(Data);
				}
			}
		}
	}
}
void IROptimizer::ConstantFoldOperator(IRInstruction& I, IROperator& Value)
{
	if (Value.Type == IROperatorType::IRInstruction)
	{
		auto Ptr = Value.Pointer;
		if (Ptr->Type == IRInstructionType::Load)
		{
			if (Ptr->Target().Type == IROperatorType::Value)
			{
				Value = Ptr->Target();
				I.ObjectType = Ptr->ObjectType;
				UpdatedCode();
			}
		}
		Get_IRData(Ptr).IsReferenced = true;
	}
}
void IROptimizer::ToSSA(const IRFunc* Func, SSAState& state)
{
	for (auto& Block : Func->Blocks)
	{
		for (size_t i = 0; i < Block->Instructions.size(); i++)
		{
			auto& Ins = Block->Instructions[i];

			if (Ins->Type == IRInstructionType::Reassign)
			{
				IROperator ToUpdate = Ins->Target();

				Ins->Type = IRInstructionType::Load;
				Ins->Target() =Ins->Input();
				Ins->Input() = IROperator();

				state.Map[ToUpdate] = Ins.get();
			}
			else
			{
				if (IsOperatorValueInTarget(Ins->Type))
				{
					if (state.Map.HasValue(Ins->Target()))
					{
						state.Updated.AddValue(&Ins->Target(), Ins->Target());
						Ins->Target() = IROperator(state.Map.at(Ins->Target()));
					}
				}	
				
				if (IsOperatorValueInInput(Ins->Type))
				{
					if (state.Map.HasValue(Ins->Input()))
					{
						state.Updated.AddValue(&Ins->Input(), Ins->Input());
						Ins->Target() = IROperator(state.Map.at(Ins->Input()));
					}
				}
			}
		}
	}
}
void  IROptimizer::UndoSSA(const IRFunc* Func, const SSAState& state)
{
	for (auto& Item : state.Updated)
	{
		*Item._Key = Item._Value;
	}
}
void IROptimizer::InLineFunc(InLineData& Data)
{
	IRInstruction* Call = Data.Block->Instructions[Data.CallIndex].get();

	//IRFunc* CalleFunc = Data.Func;
	const IRFunc* FuncToCall = Input->GetFunc(Call->Target().identifer);


	auto FuncParsCount = FuncToCall->Pars.size();
	Vector<IRInstruction*> PushIns;//its Parameters are backwards

	for (int i = Data.CallIndex - 1; i >= 0; i--)
	{
		IRInstruction* Ins = Data.Block->Instructions[i].get();
		if (Ins->Type != IRInstructionType::PushParameter)
		{
			continue;
		}

		PushIns.push_back(Ins);
		if (PushIns.size() == FuncParsCount) { break; }
	}

	for (size_t i = 0; i < PushIns.size() / 2; i++)
	{
		auto& Item = PushIns[i];
		auto& Item2 = PushIns[PushIns.size() - i];
		std::swap(Item, Item2);
	}


	Vector< IRInstruction*> AddedLoadRets;
	IRInstruction* CalledRetVar = nullptr;
	Vector<IRInstruction*> _MovedIns;
	Data.AddIns = &_MovedIns;



	{//move func at call site
		size_t Offset = 0;
		for (auto& Item : PushIns)
		{
			Item->Type = IRInstructionType::Load;
			Data.InlinedPushedPars.push_back(Item);
		}

		if (FuncToCall->ReturnType._Type != IRTypes::Void)
		{

			auto NewIns = Unique_ptr<IRInstruction>(new IRInstruction());
			NewIns->Type = IRInstructionType::LoadNone;
			NewIns->ObjectType = FuncToCall->ReturnType;
			CalledRetVar = NewIns.get();
			Data.Block->Instructions.insert(Data.Block->Instructions.begin() + Data.CallIndex + Offset, std::move(NewIns));


			Data.CallIndex++;
		}

		for (const auto& Block : FuncToCall->Blocks)
		{
			for (const auto& Item : Block->Instructions)
			{
				if (Item->Type == IRInstructionType::Return)
				{
					continue;
				}
				if (Item->Type == IRInstructionType::LoadReturn)
				{

					auto NewIns = Unique_ptr<IRInstruction>(new IRInstruction(*Item));

					{
						NewIns->Type = IRInstructionType::Reassign;
						NewIns->Input() = NewIns->Target();
						InLineSubOperator(Data, NewIns->Input(), Offset);
						NewIns->Target() = IROperator(CalledRetVar);
					}
					AddedLoadRets.push_back(NewIns.get());
					Data.Block->Instructions.insert(Data.Block->Instructions.begin() + Data.CallIndex + Offset, std::move(NewIns));
					Offset++;

				}
				else
				{
					auto NewIns = Unique_ptr<IRInstruction>(new IRInstruction(*Item));

					_MovedIns.push_back(NewIns.get());

					if (IsOperatorValueInTarget(NewIns->Type))
					{
						InLineSubOperator(Data, NewIns->Target(), Offset);
					}

					if (IsOperatorValueInInput(NewIns->Type))
					{
						InLineSubOperator(Data, NewIns->Input(), Offset);
					}

					Data.Block->Instructions.insert(Data.Block->Instructions.begin() + Data.CallIndex + Offset, std::move(NewIns));
					Offset++;
				}
			}
		}

	}


	if (FuncToCall->ReturnType._Type == IRTypes::Void)
	{
		Call->Type = IRInstructionType::LoadNone;
	}
	else
	{
		Call->Type = IRInstructionType::Load;
		Call->Target() = IROperator(CalledRetVar);
	}

	UpdatedCode();
	//
	
	{
		SSAState S;
		ToSSA(Data.Func, S);
	}

	auto S = Input->ToString();

	std::cout << "-----" << std::endl;
	std::cout << S;
}
void IROptimizer::InLineSubOperator(InLineData& Data, IROperator& Op,size_t Offset)
{
	IRInstruction* Call = Data.Block->Instructions[Data.CallIndex+Offset].get();
	const IRFunc* CallFunc = Input->GetFunc(Call->Target().identifer);
	
	#ifdef DEBUG
	if (CallFunc == nullptr)
	{
		throw std::exception("bad ptr");
	}
	#endif // DEBUG

	
	if (Op.Type == IROperatorType::IRParameter)
	{
		size_t IndexPar = Op.Parameter - CallFunc->Pars.data();

		Op = IROperator(IROperatorType::IRInstruction,Data.InlinedPushedPars[IndexPar]);
	}
	else if (
		Op.Type == IROperatorType::DereferenceOf_IRParameter)
	{
		size_t IndexPar = Op.Parameter - CallFunc->Pars.data();

		Op = IROperator(IROperatorType::DereferenceOf_IRInstruction, Data.InlinedPushedPars[IndexPar]);
	}
	else if (
		Op.Type == IROperatorType::Get_PointerOf_IRParameter)
	{
		size_t IndexPar = Op.Parameter - CallFunc->Pars.data();

		Op = IROperator(IROperatorType::Get_PointerOf_IRInstruction, Data.InlinedPushedPars[IndexPar]);
	}
	
	else if (Op.Type == IROperatorType::IRInstruction
		|| Op.Type == IROperatorType::Get_PointerOf_IRInstruction
		|| Op.Type == IROperatorType::DereferenceOf_IRInstruction)
	{
		size_t Index = 0;
		for (size_t i = 0; i < CallFunc->Blocks.size(); i++)
		{
			auto& Block = CallFunc->Blocks[i];
			for (auto& BlockIns : Block->Instructions)
			{
				if (BlockIns.get() == Op.Pointer) {
					Index++;
				}
			}
		}

		Op = IROperator(Op.Type,(*Data.AddIns)[Index]);
	}
	
}
UCodeLangEnd