#include "UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
UCodeLangStart

#define GenIns(x) ResetIns(); x;
#define GenInsPush(x) ResetIns(); x;	ULib.Add_Instruction(_Ins);



UCodeBackEndObject::UCodeBackEndObject()
{
	
}

UCodeBackEndObject::~UCodeBackEndObject()
{
}


void UCodeBackEndObject::Reset()
{

}

void UCodeBackEndObject::Build(const IRBuilder* Input)
{
	_BackInput = Input;

	auto& Code = Input->Get_Code();
	
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
#define OperatorBitsFunc(Func,bit) \
	auto Value0 = GetOperandInAnyRegister(IR.Operand0);\
				_Registers.WeakLockRegister(Value0);\
				auto Value1 = GetOperandInAnyRegister(IR.Operand1);\
				_Registers.WeakLockRegister(Value1);\
				\
				GenInsPush(InstructionBuilder::##Func##bit(_Ins, Value0, Value1));\
				\
				_Registers.UnLockWeakRegister(Value0);\
				_Registers.UnLockWeakRegister(Value1);\
				\
				auto ROut = RegisterID::MathOuPutRegister;\
				SetSybToRegister(ROut,IR);\

#define OperatorBits(Func,bit) \
			case IROperator::##Func##bit:\
			{\
				OperatorBitsFunc(Func,bit);\
			}break; \

#define BitSet(bit) \
	OperatorBits(Add,bit)\
	OperatorBits(Sub,bit)\
	OperatorBits(MultS,bit)\
	OperatorBits(MultU,bit)\
	OperatorBits(DivU,bit)\
	OperatorBits(DivS,bit)\


#define BuildIRStore(bitsize,Value2)\
	auto Value = Value2; \
	R = GetOperandInAnyRegister(IR.Operand0); \
	GenIns(InstructionBuilder::Store##bitsize(_Ins, R, Value)); \
	ULib.Add_Instruction(_Ins); \
	SetSybToRegister(R, IR); \

#define IRFieldInt(bitsize) case IRFieldInfoType::Int##bitsize: \
{\
    BuildIRStore(bitsize,IR.Operand0.AnyValue.AsInt##bitsize);\
}\
break;\



#define BuildSybolIntSizeIns(VarType,Ins,Pars) \
	switch (VarType.InfoType.TypeSize)\
	{\
		case sizeof(UInt8):\
		GenInsPush(InstructionBuilder::##Ins##8##Pars); \
		break; \
		case sizeof(UInt16):\
		GenInsPush(InstructionBuilder::##Ins##16##Pars); \
		break; \
		case sizeof(UInt32):\
		GenInsPush(InstructionBuilder::##Ins##32##Pars); \
		break; \
		case sizeof(UInt64):\
		GenInsPush(InstructionBuilder::##Ins##64##Pars); \
		break; \
	default:\
		throw std::exception();\
		break;\
	}\



void UCodeBackEndObject::BuildFunc()
{
	auto& Code = _BackInput->Get_Code();
	auto& Func = Code[_Index];
	auto SybID = Func.Operand0.SymbolId;

	String_view FuncName = Func.InfoType.Name;

	auto& ULib = Getliboutput();
	
	UAddress FuncStart = ULib.Get_Instructions().size();
    StackSize = 0;

	IRCodeIndexToUAddressIndexs.push_back(FuncStart);
	
	for (_Index = _Index + 1; _Index < Code.size(); _Index++)
	{
		auto& IR = Code[_Index];
		IRCodeIndexToUAddressIndexs.push_back(ULib.Get_Instructions().size());

		switch (IR.Operator)
		{
			BitSet(8)
				BitSet(16)
				BitSet(32)
				BitSet(64)
		case IROperator::Assign_OperandOnPointer:
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
						OnReadVarOperand(R, IR, ULib);
					}
					break;
				case IRFieldInfoType::IRLocation:
				{
					R = GetOperandInAnyRegister(IR.Operand0);
				}
				break;
				case IRFieldInfoType::AsPointer:
				{
					OnAsPointer(R, IR);
				}
				break;
				case IRFieldInfoType::Nothing:
					break;
				default:
					throw std::exception("not added");
					break;
				}
				StoreResultIR(IR, R);
			}
			break;

		case IROperator::Func_Parameter:
		{
			auto VarSymbolID = IR.Operand0.SymbolId;
			BuildData& Data = SymbolToData[VarSymbolID];
		

			auto TypeSize = IR.InfoType.TypeSize;
			if (TypeSize <= RegisterSize && ParameterRegisterValue < RegisterID::EndParameterRegister)
			{
				Data.Type = BuildData_t::ParameterInRegister;
				Data.offset = (RegisterID_t)ParameterRegisterValue;

				_Registers.LockRegister(ParameterRegisterValue);

				SetIRToRegister(ParameterRegisterValue, IR.Result.IRLocation);
				(*(RegisterID_t*)&ParameterRegisterValue)++;
			}
			else
			{
				throw std::exception();
			}
		}
		break;
		case IROperator::PassParameter:
		{
			auto TypeSize = IR.InfoType.TypeSize;
			if (TypeSize <= RegisterSize && CallParameterRegisterValue < RegisterID::EndParameterRegister)
			{
				_Registers.LockRegister(CallParameterRegisterValue);

				GetOperandInRegister(IR.Operand0, CallParameterRegisterValue);
				(*(RegisterID_t*)&CallParameterRegisterValue)++;
			}
			else
			{
				throw std::exception();
			}
		}
		break;
		case IROperator::FuncCall:
		{
			auto VarSymbolID = IR.Operand0.SymbolId;

			GenInsPush(InstructionBuilder::Call(NullAddress, _Ins));

			_InsCalls.push_back({ ULib.GetLastInstruction(),VarSymbolID });
			for (RegisterID_t i = (RegisterID_t)RegisterID::StartParameterRegister; i < (RegisterID_t)CallParameterRegisterValue; i++)
			{
				_Registers.UnLockRegister((RegisterID)i);
			}

			CallParameterRegisterValue = RegisterID::StartParameterRegister;
		}
		break;
		case IROperator::Free:
		{
			auto R = GetOperandInAnyRegister(IR.Operand0);
			GenInsPush(InstructionBuilder::Free(_Ins, R));


		}
		break;
		case IROperator::Malloc:
		{
			auto Size = GetOperandInAnyRegister(IR.Operand0);
			_Registers.WeakLockRegister(Size);
			auto ROut = _Registers.GetFreeRegister();


			GenInsPush(InstructionBuilder::Malloc(_Ins, Size, ROut));


			_Registers.UnLockWeakRegister(Size);
			SetSybToRegister(Size, IR);
		}
		break;
		case IROperator::Ret_Value:
		{
			auto RetTypeSize = IR.InfoType.TypeSize;
			if (RetTypeSize <= RegisterSize)
			{
				GetOperandInRegister(IR.Operand0, RegisterID::OuPutRegister);

			}
			else
			{
				throw std::exception();
			}
		}
		break;
		case IROperator::DLLJump:
		{
			String_view FuncName = String_view(
				(const char*)IR.Operand0.AnyValue.AsPtr,
				IR.Operand1.AnyValue.AsUIntNative
			);
			auto offset = ULib.AddStaticBytes(FuncName);

			GenInsPush(InstructionBuilder::CPPCall(offset, _Ins));

			CallParameterRegisterValue = RegisterID::StartParameterRegister;
		}
		break;
		case IROperator::IfFalseJump:
		{
			auto R = GetOperandInAnyRegister(IR.Operand1);
			auto V = _Registers.GetFreeRegister();

			GenInsPush(InstructionBuilder::LogicalNot8(_Ins, R, V));
			GenInsPush(InstructionBuilder::Jumpif(NullAddress, V, _Ins));
			JumpCallsToUpdate.push_back({ ULib.GetLastInstruction(),IR.Operand0.IRLocation });
		}
		break;
		case IROperator::Jump:
		{
			GenInsPush(InstructionBuilder::Jump(NullAddress, _Ins));
			JumpCallsToUpdate.push_back({ ULib.GetLastInstruction(),IR.Operand0.IRLocation });
		}
		break;

		case IROperator::Ret:goto EndLoop;
		}

		
	}
	
	EndLoop:

	
	GenInsPush(InstructionBuilder::Return(ExitState::Success, _Ins));
	

	_Registers.Reset();
	ParameterRegisterValue = RegisterID::StartParameterRegister; 
	CallParameterRegisterValue = RegisterID::StartParameterRegister;

	if (DeclareCalls.count(SybID))
	{
		throw std::exception("item readded");
	}
	DeclareCalls[SybID] = { FuncStart };
	ULib.Add_NameToInstruction(FuncStart, (String)FuncName);
}

void UCodeBackEndObject::OnAsPointer(UCodeLang::RegisterID& R, const IRCode& IR)
{
	auto& ULib = Getliboutput();
	R = _Registers.GetFreeRegisterAndWeakLock();


	
	auto VarSymbolID = IR.Operand0.SymbolId;
	
	BuildData& Data = SymbolToData[VarSymbolID];
	if (Data.Type == BuildData_t::StackVarable)
	{
		GenInsPush(InstructionBuilder::GetPointerOfStack(_Ins, R,Data.offset));
	}
	else
	{
		throw std::exception("not added");
	}
	
}

void UCodeBackEndObject::StoreResultIR(const IRCode& IR, UCodeLang::RegisterID R)
{
	if (IR.Result.Type == IRFieldInfoType::IRLocation)
	{
		auto& RInfo = _Registers.GetInfo(R);
		RInfo.IRField = IR.Result.IRLocation;
	}
	else if (IR.Result.Type == IRFieldInfoType::Var)
	{
		StoreVar(IR, R);
	}
	else
	{
		throw std::exception();
	}
}

void UCodeBackEndObject::OnReadVarOperand(UCodeLang::RegisterID& R, const IRCode& IR, UCodeLang::UClib& ULib)
{
	R = _Registers.GetFreeRegisterAndWeakLock();


	auto VarSymbolID = IR.Operand0.SymbolId;
	BuildData& Data = SymbolToData[VarSymbolID];
	if (Data.Type == BuildData_t::StackVarable)
	{
		auto V = _Registers.GetInfo(IR.Result.IRLocation);

		if (V == RegisterID::NullRegister)
		{
			BuildSybolIntSizeIns(IR, GetFromStack, (_Ins, Data.offset + IR.Operand1.AnyValue.AsAddress, R));
			SetSybToRegister(R, IR);
		}
		else
		{
			_Registers.UnLockWeakRegister(R);
			R = V;
		}
	}
	else if (Data.Type == BuildData_t::ParameterInRegister)
	{

		_Registers.UnLockWeakRegister(R);
		R = (RegisterID)Data.offset;
		SetIRToRegister(R, IR.Result.IRLocation);
	}
	else
	{
		throw std::exception();
	}
	
}

void UCodeBackEndObject::Link()
{
	auto& ULib = Getliboutput();
	for (auto& Item : _InsCalls)
	{
		Instruction& Ins = ULib.Get_Instructions().operator[](Item.CallIns);
		Ins.Value0 = DeclareCalls.at(Item.ID).FuncAddress -1;
	}
	for (auto& Item : JumpCallsToUpdate)
	{
		Instruction& Ins = ULib.Get_Instructions().operator[](Item.InsAddress);
		Ins.Value0 = IRCodeIndexToUAddressIndexs[Item.IRField]-1;
	}

}

void UCodeBackEndObject::SetSybToRegister(RegisterID R, const IRCode& IR)
{
	SetIRToRegister(R,IR.Result.IRLocation);
}

void UCodeBackEndObject::SetIRToRegister(RegisterID R, IRField IR)
{
	auto& RInfo = _Registers.GetInfo(R);
	_Registers.WeakLockRegister(R);
	RInfo.IRField = IR;
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
		//throw std::exception("not added");
		return;
	}


	if (R != id)
	{
		GenInsPush(InstructionBuilder::StoreRegToReg64(_Ins, R, id));//Tem Fix for now
	}
}

void UCodeBackEndObject::StoreVar(const IRCode& IR, const RegisterID R)
{
	auto& ULib = Getliboutput();
	auto VarSymbolID = IR.Result.SymbolId;
	BuildData* Data;

	
	if (SymbolToData.count(VarSymbolID))
	{
		Data = &SymbolToData[VarSymbolID];
	}
	else
	{
		SymbolToData[VarSymbolID] = {};

		Data = &SymbolToData[VarSymbolID];
		Data->Type = BuildData_t::StackVarable;
		Data->offset = StackSize;
		Data->DataSize = IR.InfoType.TypeSize;
		StackSize += Data->DataSize;
	}

	if (IR.Operand0.Type == IRFieldInfoType::Nothing) { return; }
	

	if (Data->Type == BuildData_t::ParameterInRegister)
	{
		auto R2 = (RegisterID)Data->offset;

		if (R != R2)
		{
			BuildSybolIntSizeIns(IR, StoreRegToReg, (_Ins, R, R2));
		}
	}
	else if (Data->Type == BuildData_t::ThisObjectWithOffset)
	{
		throw std::exception("not added");
	}
	else
	{
		if (IR.Operator == IROperator::Assign_OperandOnPointer)
		{

			return;
		}


		BuildSybolIntSizeIns(IR, StoreRegOnStack, (_Ins, R, Data->offset + IR.Operand1.AnyValue.AsAddress));
	}
	
	
}

UCodeLangEnd

