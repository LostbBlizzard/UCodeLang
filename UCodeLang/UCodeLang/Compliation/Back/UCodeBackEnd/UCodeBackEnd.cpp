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
	Link();
}

#define OperatorBits(Func,bit) \
			case IROperator::##Func##bit:\
			{\
				auto Value0 = GetOperandInAnyRegister(IR.Operand0);\
				_Registers.WeakLockRegister(Value0);\
				auto Value1 = GetOperandInAnyRegister(IR.Operand1);\
				_Registers.WeakLockRegister(Value1);\
				\
				GenIns(InstructionBuilder::##Func##bit(_Ins, Value0, Value1));\
				ULib.Add_Instruction(_Ins);\
				\
				_Registers.UnLockWeakRegister(Value0);\
				_Registers.UnLockWeakRegister(Value1);\
				\
				auto ROut = RegisterID::MathOuPutRegister;\
				SetSybToRegister(ROut,IR);\
			}break; \

#define BitSet(bit) \
	OperatorBits(Add,bit)\
	OperatorBits(Sub,bit)\


#define IRFieldInt(bitsize) case IRFieldInfoType::Int##bitsize: \
{\
    auto Value =IR.Operand0.AnyValue.AsInt##bitsize;\
	R = GetOperandInAnyRegister(IR.Operand0);\
	GenIns(InstructionBuilder::Store##bitsize(_Ins, R, Value)); \
	ULib.Add_Instruction(_Ins); \
	SetSybToRegister(R,IR);\
}\
break;\

void UCodeBackEndObject::BuildFunc()
{
	auto& Code = _BackInput->_Builder->Get_Code();
	auto& Func = Code[_Index];
	auto SybID = Func.Operand0.SymbolId;
	auto Sym = _BackInput->_Table->GetSymbol(SybID);

	auto& ULib = Getliboutput();
	
	UAddress FuncStart = ULib.Get_Instructions().size();
	UAddress StackSize = 0;

	for (_Index = _Index + 1; _Index < Code.size(); _Index++)
	{
		auto& IR = Code[_Index];

		switch (IR.Operator)
		{
		BitSet(8)
		BitSet(16)
		BitSet(32)
		BitSet(64)
		case IROperator::Assign_Operand0:
		{
			RegisterID R = RegisterID::NullRegister;
		
			
			switch (IR.Operand0.Type)
			{
			IRFieldInt(8)
			IRFieldInt(16)
			IRFieldInt(32)
			IRFieldInt(64)
			case IRFieldInfoType::ReadVar:
			{
				R = _Registers.GetFreeRegisterAndWeakLock();

				auto VarSymbolID = IR.Operand0.SymbolId;
				Symbol& V = _BackInput->_Table->GetSymbol(VarSymbolID);
				BuildData& Data = SymbolToData[VarSymbolID];
				if (V.Type == SymbolType::StackVarable) 
				{
					auto V = _Registers.GetInfo(IR.Result.IRLocation);

					if (V == RegisterID::NullRegister) 
					{
						GenIns(InstructionBuilder::GetFromStack8(_Ins, Data.offset, R));
						ULib.Add_Instruction(_Ins);

						SetSybToRegister(R, IR);
					}
					else
					{
						_Registers.UnLockWeakRegister(R);
						R = V;
					}
				}
				else
				{
					throw std::exception();
				}
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
				auto VarSymbolID = IR.Result.SymbolId;
				BuildData* Data;
				Symbol& Symbol = _BackInput->_Table->GetSymbol(VarSymbolID);
				if (SymbolToData.count(VarSymbolID))
				{
					Data = &SymbolToData[VarSymbolID];
				}
				else
				{
					SymbolToData[VarSymbolID] = {};
					Data = &SymbolToData[VarSymbolID];

					Data->offset = StackSize;
					StackSize += Symbol.Size;
				}


				GenIns(InstructionBuilder::StoreRegOnStack8(_Ins,R, Data->offset));
				ULib.Add_Instruction(_Ins);

			}
		}
		break;
		
		case IROperator::FuncCall:
		{
			auto VarSymbolID = IR.Operand0.SymbolId;
			
			GenIns(InstructionBuilder::Call(NullAddress,_Ins));
			ULib.Add_Instruction(_Ins);
			
			_InsCalls.push_back({ULib.GetLastInstruction(),VarSymbolID });
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
	_Registers.Reset();


	DeclareCalls[SybID] = { FuncStart };
	ULib.Add_NameToInstruction(FuncStart, Sym.FullName);
}

void UCodeBackEndObject::Link()
{
	auto& ULib = Getliboutput();
	for (auto& Item : _InsCalls)
	{
		Instruction& Ins = ULib.Get_Instructions().operator[](Item.CallIns);
		Ins.Value0 = DeclareCalls.at(Item.ID).FuncAddress -1;
	}
}

void UCodeBackEndObject::SetSybToRegister(UCodeLang::RegisterID R, UCodeLang::IRThreeAddressCode& IR)
{
	auto& RInfo = _Registers.GetInfo(R); \
		_Registers.WeakLockRegister(R); \
		RInfo.IRField = IR.Result.IRLocation; \
}

void UCodeBackEndObject::SetIRToRegister(UCodeLang::RegisterID R, IRField IR)
{
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
		
		return;
	}


	if (R != id)
	{
		GenIns(InstructionBuilder::StoreRegToReg8(_Ins, R, id));
		ULib.Add_Instruction(_Ins);
	}
}

UCodeLangEnd

