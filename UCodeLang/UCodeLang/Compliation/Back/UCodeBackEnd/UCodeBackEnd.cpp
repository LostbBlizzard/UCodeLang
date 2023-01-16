#include "UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
UCodeLangStart

#define GenIns(x) ResetIns(); x;
#define GenInsPush(x) ResetIns(); x;	ULib.Add_Instruction(_Ins);

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



#define BuildSybolIntSizeInsv(Ins,x,Pars) \
						case TypesEnum::uInt##x:\
						case TypesEnum::sInt##x:\
						GenInsPush(InstructionBuilder::##Ins##x##Pars);\
						break;\

#define BuildSybolIntSizeIns(VarType,Ins,Pars) \
if (VarType.IsAddress() || \
	VarType._Type == TypesEnum::uIntPtr ||\
	VarType._Type == TypesEnum::sIntPtr)\
{\
	GenInsPush(InstructionBuilder::##Ins##64##Pars);\
}\
else\
{\
	switch (VarType._Type)\
	{\
		BuildSybolIntSizeInsv(Ins,8,Pars);\
		BuildSybolIntSizeInsv(Ins,16,Pars);\
		BuildSybolIntSizeInsv(Ins,32,Pars);\
		BuildSybolIntSizeInsv(Ins,64,Pars);\
	default:\
		throw std::exception();\
		break;\
	}\
}\

void UCodeBackEndObject::BuildFunc()
{
	auto& Code = _BackInput->_Builder->Get_Code();
	auto& Func = Code[_Index];
	auto SybID = Func.Operand0.SymbolId;
	auto Sym = _BackInput->_Table->GetSymbol(SybID);

	auto& ULib = Getliboutput();
	
	UAddress FuncStart = ULib.Get_Instructions().size();
    StackSize = 0;

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
				OnReadVarOperand(R, IR, ULib);
			}break;
			case IRFieldInfoType::IRLocation:
			{
				R = GetOperandInAnyRegister(IR.Operand0);
			}break;
			case IRFieldInfoType::AsPointer:
			{
				OnAsPointer(R, IR);
			}break;
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
			Symbol& ParSymbol = _BackInput->_Table->GetSymbol(VarSymbolID);
			

			auto TypeSize = Analysis->GetTypeSize(ParSymbol.VarType);
			if (TypeSize <= RegisterSize && ParameterRegisterValue < RegisterID::EndParameterRegister)
			{
				Data.Type = BuildData_t::ParameterInRegister;
				Data.offset = (RegisterID_t)ParameterRegisterValue;
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
			auto TypeSize = 0;
			if (TypeSize <= RegisterSize && CallParameterRegisterValue < RegisterID::EndParameterRegister)
			{

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
			
			GenInsPush(InstructionBuilder::Call(NullAddress,_Ins));
			
			_InsCalls.push_back({ULib.GetLastInstruction(),VarSymbolID });
			CallParameterRegisterValue = RegisterID::StartParameterRegister;
		}
		break;
		case IROperator::Free:
		{
			auto R = GetOperandInAnyRegister(IR.Operand0);
			GenInsPush(InstructionBuilder::Free(_Ins,R));
			

		}
		break;
		case IROperator::Malloc:
		{
			auto Size = GetOperandInAnyRegister(IR.Operand0);
			_Registers.WeakLockRegister(Size);
			auto ROut = _Registers.GetFreeRegister();
			

			GenInsPush(InstructionBuilder::Malloc(_Ins, Size, ROut));
		

			_Registers.UnLockWeakRegister(Size);
			SetSybToRegister(Size,IR);
		}
		break;
		case IROperator::Ret_Value:
		{
			auto RetTypeSize = Analysis->GetTypeSize(Sym.VarType);
			if (RetTypeSize <= RegisterSize)
			{
				GetOperandInRegister(IR.Operand0,RegisterID::OuPutRegister);
				
			}
			else
			{
				throw std::exception();
			}
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


	DeclareCalls[SybID] = { FuncStart };
	ULib.Add_NameToInstruction(FuncStart, Sym.FullName);
}

void UCodeBackEndObject::OnAsPointer(UCodeLang::RegisterID& R, UCodeLang::IRCode& IR)
{
	auto& ULib = Getliboutput();
	R = _Registers.GetFreeRegisterAndWeakLock();

	auto VarSymbolID = IR.Operand0.SymbolId;
	Symbol& SybV = _BackInput->_Table->GetSymbol(VarSymbolID);
	BuildData& Data = SymbolToData[VarSymbolID];
	if (SybV.Type == SymbolType::StackVarable)
	{
		GenInsPush(InstructionBuilder::GetPointerOfStack(_Ins, R,Data.offset));
	}
	else
	{
		throw std::exception("not added");
	}
}

void UCodeBackEndObject::StoreResultIR(UCodeLang::IRCode& IR, UCodeLang::RegisterID R)
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
}

void UCodeBackEndObject::OnReadVarOperand(UCodeLang::RegisterID& R, UCodeLang::IRCode& IR, UCodeLang::UClib& ULib)
{
	R = _Registers.GetFreeRegisterAndWeakLock();

	auto VarSymbolID = IR.Operand0.SymbolId;
	Symbol& SybV = _BackInput->_Table->GetSymbol(VarSymbolID);
	BuildData& Data = SymbolToData[VarSymbolID];
	if (SybV.Type == SymbolType::StackVarable)
	{
		auto V = _Registers.GetInfo(IR.Result.IRLocation);

		if (V == RegisterID::NullRegister)
		{
			BuildSybolIntSizeIns(SybV.VarType, GetFromStack, (_Ins, Data.offset, R));
			SetSybToRegister(R, IR);
		}
		else
		{
			_Registers.UnLockWeakRegister(R);
			R = V;
		}
	}
	else if (SybV.Type == SymbolType::ParameterVarable)
	{
		if (Data.Type == BuildData_t::ParameterInRegister)
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
}

void UCodeBackEndObject::SetSybToRegister(RegisterID R, IRCode& IR)
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
	Symbol& Symbol = _BackInput->_Table->GetSymbol(VarSymbolID);
	if (SymbolToData.count(VarSymbolID))
	{
		Data = &SymbolToData[VarSymbolID];
	}
	else
	{
		if (Symbol.Type == SymbolType::StackVarable)
		{
			SymbolToData[VarSymbolID] = {};
			Data = &SymbolToData[VarSymbolID];

			Data->offset = StackSize;
			StackSize += Symbol.Size;
		}
		else
		{
			throw std::exception("not added");
		}
	}

	if (Data->Type == BuildData_t::ParameterInRegister)
	{
		auto R2 = (RegisterID)Data->offset;

		if (R != R2)
		{
			BuildSybolIntSizeIns(Symbol.VarType, StoreRegToReg, (_Ins, R, R2));
		}
	}
	else
	{
		BuildSybolIntSizeIns(Symbol.VarType, StoreRegOnStack, (_Ins, R, Data->offset));
	}
	
}

UCodeLangEnd

