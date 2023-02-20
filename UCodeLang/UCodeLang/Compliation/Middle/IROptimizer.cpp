#include "IROptimizer.hpp"
#include <iostream>
UCodeLangStart

using OptimizationFunc = void(IROptimizer::*)();

struct OptimizationInfo
{
	OptimizationFlags AcitveIf;
	bool InverseAcitveIf;

	
	union 
	{
		OptimizationFunc FuncCall;
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

}
void IROptimizer::Optimized(IRBuilder& IRcode)
{
	Input = &IRcode;
	UpdateOptimizationList();
	
	
	do
	{
		auto S = Input->ToString();
		
		std::cout << "-----" << std::endl;
		std::cout << S;
		_UpdatedCode = false;

		return;
		UpdateCodePass();
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

	for (auto& Func : Input->Funcs)
	{
		for (auto& Block : Func->Blocks)
		{

			for (auto& Ins : Block->Instructions)
			{
				Get_IRData(Ins.get()).IsReferenced = false;
			}

			for (auto& Ins : Block->Instructions)
			{
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

					
					/*
					if (Ins->Target().Type == IROperatorType::IRInstruction) 
					{
						auto Target = Ins->Target().Pointer;
						if (Target->Type == IRInstructionType::LoadNone)
						{
							Target->Target(Ins->Input());
							Target->Type = IRInstructionType::Load;

							Ins->SetAsNone();
							UpdatedCode();
						}
					}
					*/

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
UCodeLangEnd