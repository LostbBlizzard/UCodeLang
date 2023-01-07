#include "UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
UCodeLangStart

#define GenIns(x) ResetIns(); x;
void UCodeBackEnd::Bind(BackEndInterface& Obj)
{
	Obj
	.Set_GenBackEnd(Gen)
	.Set_DeleteBackEnd(Delete)
	.Set_ErrorsOutput(Set_ErrorsOutput)
	.Set_Analysis(Set_Analysis)
	.Set_BuildInput(Build)
		;
}
const BackEndInterface* UCodeBackEnd::Get()
{
	static bool IsMade = false;
	static BackEndInterface BackEnd;
	if (!IsMade)
	{
		Bind(BackEnd);
		IsMade = true;
	}

	return &BackEnd;
}

UCodeBackEndObject::UCodeBackEndObject()
{
	
}

UCodeBackEndObject::~UCodeBackEndObject()
{
}


void UCodeBackEndObject::Build(BackEndInput& Input)
{
	_BackInput = &Input;

	auto& Code = Input._Builder->Get_Code();
	
	for (_Index = 0; _Index < Code.size(); _Index++)
	{
		auto& IR = Code[_Index];
		if (IR.Operator == IROperator::Func)
		{
			BuildFunc();
		}
	}
}

void UCodeBackEndObject::BuildFunc()
{
	auto& Code = _BackInput->_Builder->Get_Code();
	auto& Func = Code[_Index];
	auto SybID = Func.Operand0.SymbolId;
	auto Sym = _BackInput->_Table->GetSymbol(SybID);

	auto& ULib = Getliboutput();
	
	UAddress FuncStart = _Index;
	UAddress StackSize = 0;

	for (_Index = _Index + 1; _Index < Code.size(); _Index++)
	{
		auto& IR = Code[_Index];

		switch (IR.Operator)
		{
		case IROperator::Add8:
		{
			auto Value0 = GetOperandInAnyRegister(IR.Operand0);
			_Registers.WeakLockRegister(Value0);
			auto Value1 = GetOperandInAnyRegister(IR.Operand1);
			_Registers.WeakLockRegister(Value1);

			GenIns(InstructionBuilder::Add8(_Ins, Value0, Value1));
			ULib.Add_Instruction(_Ins);

			_Registers.UnLockWeakRegister(Value0);
			_Registers.UnLockWeakRegister(Value1);

			auto ROut = RegisterID::MathOuPutRegister;
			auto& RInfo = _Registers.GetInfo(ROut);
			_Registers.WeakLockRegister(ROut);
			RInfo.IRField = IR.Result.IRLocation;
		}
		break;
		case IROperator::Assign_Operand0:
		{
			RegisterID R = RegisterID::NullRegister;
		
			
			switch (IR.Operand0.Type)
			{
			case IRFieldInfoType::Int8:
			{
				R = _Registers.GetFreeRegisterAndWeakLock();
				GenIns(InstructionBuilder::Store8(_Ins, R, IR.Operand0.AnyValue.AsInt8));
				ULib.Add_Instruction(_Ins);
			}
			break;
			case IRFieldInfoType::IRLocation:
			{
				R = GetOperandInAnyRegister(IR.Operand0);
			}
			break;
			default:
				break;
			}
			if (IR.Result.Type == IRFieldInfoType::IRLocation)
			{
				auto& RInfo = _Registers.GetInfo(R);
				RInfo.IRField = IR.Result.IRLocation;
			}
			else if (IR.Result.Type == IRFieldInfoType::Var)
			{ 
				GenIns(InstructionBuilder::StoreRegOnStack8(_Ins,R,StackSize));
				ULib.Add_Instruction(_Ins);
				
				StackSize += sizeof(UInt8);
			}
		}
		break;
		
		case IROperator::Ret:
				goto EndLoop;
		break;
		}
	}
EndLoop:

	GenIns(InstructionBuilder::Return(ExitState::Success, _Ins));
	ULib.Add_Instruction(_Ins);


	ULib.Add_NameToInstruction(FuncStart, Sym.FullName);
}

RegisterID UCodeBackEndObject::GetOperandInAnyRegister(const IROperand& operand)
{
	auto IsInR = _Registers.GetInfo(operand.IRLocation);
	if (IsInR == RegisterID::NullRegister)
	{
		auto id = _Registers.GetFreeRegister();
		GetOperandInRegister(operand, id);
		return id;
	}
	return IsInR;
}

void UCodeBackEndObject::GetOperandInRegister(const IROperand& operand, RegisterID id)
{
	auto& ULib = Getliboutput();
	auto R = _Registers.GetInfo(operand.IRLocation);
	if (R == RegisterID::NullRegister)
	{
		R = id;
		int a = 0;
		return;
	}


	if (R != id)
	{
		GenIns(InstructionBuilder::StoreRegToReg8(_Ins, R, id));
		ULib.Add_Instruction(_Ins);
	}
}

UCodeLangEnd

