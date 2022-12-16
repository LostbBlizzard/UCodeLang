#include "Assembler.hpp"
UCodeLangStart

#define GenIns(X) ReSetIns(); X
void Assembler::Assemble(UClib* Output,UClib* Data)
{
	_OutPut = Output;
	_OutPut->ClearState();
	

	_Input = Data;
	Index = 0;
	ResetRegistersData();
	Symbols.Symbols.clear();

	BuildBuffer();
	
	Scope.ThisScope = ScopeHelper::_globalScope;
	
	StaticStackSize = 0;
	Debugoffset = 0;

	UpdateAllTypes();

	while (Intermediate_Instruction* Inter = Get_Ins())
	{
		switch (Inter->OpCode)
		{
		case Intermediate_Set::DeclareStaticVar:
			DeclareStaticVar(*Inter);
			break;
		}

		NextIns();
	}
	
	Index = 0;

	while (Intermediate_Instruction* Inter = Get_Ins())
	{
		switch (Inter->OpCode)
		{
		case Intermediate_Set::DeclareFunc:
			BuildDeclareFunc(*Inter);
			break;
		case Intermediate_Set::DeclareStaticVar:
			break;
		case Intermediate_Set::FileEnd:
			Debugoffset = Inter->Value0_AsUInt64; NextIns();
			break;
		default:
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
			break;
		}
	}

	auto& In_s = _OutPut->Get_Instructions();
	for (size_t i = 0; i < In_s.size(); i++)
	{
		auto& Ins = In_s[i];
		if (Ins.OpCode_AsInt == (InstructionSet_t)Intermediate_Set::TepFuncCall)
		{
			String_view FuncName = Get_StringFromDebug(Ins.Value0_AsAddress);
			UAddress funcpos = _OutPut->Get_NameToInstruction(FuncName.data());
			InstructionBuilder::Call(funcpos, Ins);
		}
	}

	ClassAssembly::Move(Data->Get_Assembly(), Output->Get_Assembly());
}
void Assembler::BuildBuffer()
{
	InstructionBuilder::Exit(ExitState::Failure, _Ins);
	_OutPut->Add_Instruction(_Ins);
}
void Assembler::GetSymbolInRegister(symbol* Symbol, RegisterID id)
{
}
void Assembler::ForceSymbolInRegister(symbol* Symbol, RegisterID id)
{
}
RegisterID Assembler::DeclareExpression()
{
	NextIns();
	if (Intermediate_Instruction* Inter = Get_Ins())
	{
		if (Inter->OpCode == Intermediate_Set::DeclareExpression)
		{
			NextIns();
			 Inter = Get_Ins();
		}
		switch (Inter->OpCode)
		{
		case Intermediate_Set::DeclareBinaryExpression:
			return DeclareBinaryExpression(*Inter);
			break;
		case Intermediate_Set::UnaryExpression:
			return UnaryExpression(*Inter);
			break;
		case Intermediate_Set::StoreNumber:
		{
			NextIns();
			auto _String = Get_StringFromDebug(Inter->Value0_AsUInt64);
			auto V = std::stoi(_String.data());
			auto Register = GetFreeRegister();
			GenIns(InstructionBuilder::Store8(_Ins, Register, (UInt8)V)); PushIns();
			return Register;
		}
			break;
		case Intermediate_Set::GetVar:
		{
			return GetVar(*Inter);
		}
		break;
		default:
#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
#endif
			break;
		}

	}
	return RegisterID::NullRegister;
}
RegisterID Assembler::DeclareBinaryExpression(Intermediate_Instruction& Ins)
{
	Intermediate_Set Op = (Intermediate_Set)Ins.Value0_AsUInt64;
	

	RegisterID r;
	auto V = DeclareExpression();
	RegisterLock(V);
	auto V1 = DeclareExpression();
	RegisterLock(V1);


	switch (Op)
	{
	case UCodeLang::Intermediate_Set::Binary_plus:
		GenIns(InstructionBuilder::Add8U(_Ins,V,V1)); PushIns();
		r = RegisterID::MathOuPutRegister;
		break;
	case UCodeLang::Intermediate_Set::Binary_minus:
		GenIns(InstructionBuilder::Sub8U(_Ins, V, V1)); PushIns();
		r = RegisterID::MathOuPutRegister;
		break;
	case UCodeLang::Intermediate_Set::Binary_Mult:
		GenIns(InstructionBuilder::Mult8U(_Ins, V, V1)); PushIns();
		r = RegisterID::MathOuPutRegister;
		break;
	case UCodeLang::Intermediate_Set::Binary_Div:
		GenIns(InstructionBuilder::Div8U(_Ins, V, V1)); PushIns();
		r = RegisterID::MathOuPutRegister;
		break;
	case UCodeLang::Intermediate_Set::Binary_logical_and:
		GenIns(InstructionBuilder::LogicalAnd8(_Ins, V, V1)); PushIns();
		r = RegisterID::BoolRegister;
		break;
	case UCodeLang::Intermediate_Set::Binary_logical_or:
		GenIns(InstructionBuilder::LogicalOr8(_Ins, V, V1)); PushIns();
		r = RegisterID::BoolRegister;
		break;
	case UCodeLang::Intermediate_Set::Binary_equal_Comparison:
		GenIns(InstructionBuilder::equalto8(_Ins, V, V1)); PushIns();
		r = RegisterID::BoolRegister;
		break;
	case UCodeLang::Intermediate_Set::Binary_Notequal_Comparison:
		GenIns(InstructionBuilder::notequalto8(_Ins, V, V1)); PushIns();
		r = RegisterID::BoolRegister;
		break;
	case UCodeLang::Intermediate_Set::Bianry_greaterthan:
		GenIns(InstructionBuilder::greaterthan8(_Ins, V, V1)); PushIns();
		r = RegisterID::BoolRegister;
		break;
	case UCodeLang::Intermediate_Set::Bianry_Lessthan:
		GenIns(InstructionBuilder::lessthan8(_Ins, V, V1)); PushIns();
		r = RegisterID::BoolRegister;
		break;
	case UCodeLang::Intermediate_Set::Bianry_less_than_or_equalto:
		GenIns(InstructionBuilder::equal_lessthan8(_Ins, V, V1)); PushIns();
		r = RegisterID::BoolRegister;
		break;
	case UCodeLang::Intermediate_Set::Bianry_greater_than_or_equalto:
		GenIns(InstructionBuilder::equal_greaterthan8(_Ins, V, V1)); PushIns();
		r = RegisterID::BoolRegister;
		break;

	case UCodeLang::Intermediate_Set::Bianry_bitwise_and:
		GenIns(InstructionBuilder::bitwise_and8(_Ins, V, V1)); PushIns();
		r = RegisterID::BitwiseRegister;
		break;
	case UCodeLang::Intermediate_Set::Bianry_bitwise_or:
		GenIns(InstructionBuilder::bitwise_or8(_Ins, V, V1)); PushIns();
		r = RegisterID::BitwiseRegister;
		break;
	case UCodeLang::Intermediate_Set::Bianry_bitwise_LeftShift:
		GenIns(InstructionBuilder::bitwise_LeftShift8(_Ins, V, V1)); PushIns();
		r = RegisterID::BitwiseRegister;
		break;
	case UCodeLang::Intermediate_Set::Bianry_bitwise_RightShift:
		GenIns(InstructionBuilder::bitwise_RightShift8(_Ins, V, V1)); PushIns();
		r = RegisterID::BitwiseRegister;
		break;
	case UCodeLang::Intermediate_Set::Bianry_bitwise_Xor:
		GenIns(InstructionBuilder::bitwise_Xor8(_Ins, V, V1)); PushIns();
		r = RegisterID::BitwiseRegister;
		break;
	default:
#if CompliationTypeSafety
		throw std::exception("Cant UnWap BuildStatement");
#endif
		break;
	}

	RegisterUnLock(V);
	RegisterUnLock(V1);
	return r;
}
RegisterID Assembler::UnaryExpression(Intermediate_Instruction& Ins)
{
	NextIns();
	RegisterID r = GetFreeRegister();
	Intermediate_Set Op = (Intermediate_Set)Ins.Value0_AsUInt64;

	RegisterLock(r);

	auto V = DeclareExpression();
	RegisterLock(V);
	switch (Op)
	{
	case UCodeLang::Intermediate_Set::bitwise_not:
		GenIns(InstructionBuilder::bitwise_Not8(_Ins, V, r)); PushIns();
		break;
	default:
#if CompliationTypeSafety
		throw std::exception("Cant UnWap BuildStatement");
#endif
		break;
	}
	
	RegisterUnLock(V);
	RegisterUnLock(r);
	return r;
}
void Assembler::BuildStore8RegToReg(RegisterID id, RegisterID out)
{
	if (id == out) { return; }

	GenIns(InstructionBuilder::Push8(_Ins, id)); PushIns();
	GenIns(InstructionBuilder::Pop8(_Ins, out)); PushIns();
}
void Assembler::BuildStore64RegToReg(RegisterID id, RegisterID out)
{
}
void Assembler::DeclareVar(Intermediate_Instruction& Ins)
{
	NextIns();
	auto _VarType = Get_StringFromDebug(Ins.Value0_AsUInt64);
	auto _VarName = Get_StringFromDebug(Ins.Value1_AsUInt64);

	auto& syb = AddVarSymbol(_VarName,0,0);
	syb.VarType = _VarType;

	size_t TypeSize;
	if (_VarType == Uint8TypeName)
	{
		TypeSize = sizeof(UInt8);
	}
	else
	{
		auto Type = GetType(_VarType);
		if (Type)
		{
			if (Type->Type == ClassType::Enum)
			{
				TypeSize = sizeof(UInt8);
			}
			else if (Type->Type == ClassType::Class)
			{
				TypeSize =Type->_Class.Size;
			}
			else
			{
				#if CompliationTypeSafety
				throw std::exception("Cant UnWap BuildStatement");
				#endif
			}

		}
		else
		{
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
		}
	}
	syb.Size = TypeSize;
	syb.Address = funcStackSize;
	funcStackSize += TypeSize;
}
void Assembler::DeclareThisVar(Intermediate_Instruction& Ins)
{
	NextIns();
	auto _VarType = Get_StringFromDebug(Ins.Value0_AsUInt64);
	auto _VarName = Get_StringFromDebug(Ins.Value1_AsUInt64);

	auto& syb = AddVarSymbol(_VarName, 0, 0);
	syb.VarType = _VarType;
	syb.Type = symbolType::ThisVar;
	if (_VarType == Uint8TypeName)
	{
		constexpr size_t TypeSize = sizeof(UInt8);
		syb.Size = TypeSize;
		syb.Address = funcStackSize;
		funcStackSize += TypeSize;
	}
	else
	{
		auto Type = GetType(_VarType);

		if (Type)
		{
			if (Type->Type == ClassType::Enum)
			{
				constexpr size_t TypeSize = sizeof(UInt8);
				syb.Size = TypeSize;
				syb.Address = funcStackSize;
				funcStackSize += TypeSize;
			}

		}
		else
		{
#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
#endif
		}
	}
}
void Assembler::DeclareThis(Intermediate_Instruction& Ins)
{
	NextIns();
	auto _VarType = Get_StringFromDebug(Ins.Value0_AsUInt64);
	auto _VarName = Get_StringFromDebug(Ins.Value1_AsUInt64);

	auto& syb = AddVarSymbol(_VarName, 0, 0);
	syb.VarType = _VarType;
	syb.Type = symbolType::This;
}

void Assembler::DeclareStaticVar(Intermediate_Instruction& Ins)
{
	NextIns();
	auto _VarType = Get_StringFromDebug(Ins.Value0_AsUInt64);
	auto _VarName = Get_StringFromDebug(Ins.Value1_AsUInt64);

	auto& syb = AddVarSymbol(_VarName, 0, 0);
	syb.Type = symbolType::StaticVar;
	syb.VarType = _VarType;
	if (_VarType == Uint8TypeName)
	{
		constexpr size_t TypeSize = sizeof(UInt8);
		syb.Size = TypeSize;
		syb.Address = StaticStackSize;
		StaticStackSize += TypeSize;
		_OutPut->AddStaticBytes((unsigned char)0,TypeSize);
	}
	else
	{
#if CompliationTypeSafety
		throw std::exception("Cant UnWap BuildStatement");
#endif
	}
}
void Assembler::StoreVar(Intermediate_Instruction& Ins)
{
	NextIns();
	auto _VarName = Get_StringFromDebug(Ins.Value0_AsUInt64);
	auto syb = FindVarSymbol(_VarName);
	if (syb)
	{
		switch (syb->Type)
		{
		case symbolType::ThisVar:
		{
			auto Register = GetFreeRegister();	RegisterLock(Register);

			GenIns(InstructionBuilder::StoreNativeU(_Ins, Register, syb->Address)); PushIns();

			GenIns(InstructionBuilder::AddNativeU(_Ins, RegisterID::ThisRegister, Register)); PushIns();

			GenIns(InstructionBuilder::Store8RegToPtr(_Ins,OutValue, RegisterID::MathOuPutRegister)); PushIns();

			RegisterUnLock(Register);
			RegisterUnLock(OutValue);
		}	break;
		case symbolType::Var:
			GenIns(InstructionBuilder::Store8RegOnStack(_Ins, OutValue, syb->Address)); PushIns();

			RegisterUnLock(OutValue);
			break;
		case symbolType::StaticVar:
		{
			auto Register = GetFreeRegister();
			GenIns(InstructionBuilder::GetPointerOfStaticMem(_Ins, Register)); PushIns();
			RegisterLock(Register);
			auto V2 = GetFreeRegister();
			RegisterLock(V2);

			GenIns(InstructionBuilder::StoreNativeU(_Ins, V2, syb->Address)); PushIns();

			GenIns(InstructionBuilder::AddNativeU(_Ins, Register, V2)); PushIns();

			GenIns(InstructionBuilder::Store8RegToPtr(_Ins, OutValue, RegisterID::MathOuPutRegister)); PushIns();

			RegisterUnLock(Register);
			RegisterUnLock(V2);
			RegisterUnLock(OutValue);
		}
		break;
		default:
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
			break;
		}
	}
	else
	{
		#if CompliationTypeSafety
		throw std::exception("Cant UnWap BuildStatement");
		#endif
	}
}
RegisterID Assembler::GetVar(Intermediate_Instruction& Ins)
{
	NextIns();
	auto _VarName = Get_StringFromDebug(Ins.Value0_AsUInt64);
	auto syb = FindVarSymbol(_VarName);
	if (syb)
	{
		auto Register = GetFreeRegister();
		switch (syb->Type)
		{
		case symbolType::Var:
			GenIns(InstructionBuilder::Get8FromStack(_Ins, syb->Address, Register)); PushIns();
			break;
		case symbolType::StaticVar:
		{
			GenIns(InstructionBuilder::GetPointerOfStaticMem(_Ins, Register)); PushIns();
			RegisterLock(Register);
			auto V2 = GetFreeRegister();
			RegisterLock(V2);

			GenIns(InstructionBuilder::StoreNativeU(_Ins, V2, syb->Address)); PushIns();

			GenIns(InstructionBuilder::AddNativeU(_Ins, Register, V2)); PushIns();

			GenIns(InstructionBuilder::Store8FromPtrToReg(_Ins, RegisterID::MathOuPutRegister, Register)); PushIns();

			RegisterUnLock(Register);
			RegisterUnLock(V2);
		}
		break;
		case symbolType::ThisVar:
		{auto RRegister = GetFreeRegister();
		Register = RegisterID::ThisRegister;

		auto V2 = GetFreeRegister();
		RegisterLock(V2);



		GenIns(InstructionBuilder::StoreNativeU(_Ins, V2, syb->Address)); PushIns();

		GenIns(InstructionBuilder::AddNativeU(_Ins, Register, V2)); PushIns();

		GenIns(InstructionBuilder::Store8FromPtrToReg(_Ins, RegisterID::MathOuPutRegister, RRegister)); PushIns();

		RegisterUnLock(V2);
		}
		break;
		case symbolType::Parameter:
		{
			auto ParOfset = (ParameterSize - syb->Address);
			auto RetAddressSize = sizeof(UAddress);
			auto Offset = RetAddressSize + ParOfset;
			GenIns(InstructionBuilder::Get8FromStackSub(_Ins, Offset, Register)); PushIns();
			break;
		}
		case symbolType::Parameter_Input:
		{
			return RegisterID::InPutRegister;
		}
		break;
		default:
#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
#endif
			break;
		}
		return Register;
	}
	else
	{
#if CompliationTypeSafety
		throw std::exception("Cant UnWap BuildStatement");
#endif
	}
}
void Assembler::DeclareParameter(Intermediate_Instruction& Ins)
{
	NextIns();
	auto _VarType = Get_StringFromDebug(Ins.Value1_AsUInt64);
	auto _VarName = Get_StringFromDebug(Ins.Value0_AsUInt64);

	auto& syb = AddVarSymbol(_VarName, 0, 0);
	syb.VarType = _VarType;
	syb.Type = symbolType::Parameter;

	size_t TypeSize;
	if (_VarType == Uint8TypeName){TypeSize = sizeof(UInt8);}
	else if (_VarType == UintPtrTypeName) { TypeSize = sizeof(uintptr_t); }
	else
	{
#if CompliationTypeSafety
		throw std::exception("Cant UnWap BuildStatement");
#endif

	}
	syb.Size = TypeSize;
	syb.Address = ParameterSize;
	ParameterSize += TypeSize;
}

void Assembler::BuildDeclareFunc(Intermediate_Instruction& Ins)
{
	ResetRegistersData();
	NextIns();
	auto _FuncName = Get_StringFromDebug(Ins.Value0_AsUInt64);
	auto& syb = AddFuncSymbol(_FuncName, 0, 0);

	Scope.AddScope(_FuncName.data());

	GenIns(InstructionBuilder::DoNothing(_Ins));//StackSize;
	auto FuncPos =syb.Address = PushIns();
	if (FuncPos == NullAddress)
	{
		GenIns(InstructionBuilder::DoNothing(_Ins));
		FuncPos = syb.Address = PushIns();
	}

	auto FuncIndex = Index;
	funcStackSize = 0;
	ParameterSize = 0;
	while (Intermediate_Instruction* Inter = Get_Ins())
	{
		switch (Inter->OpCode)
		{
		case Intermediate_Set::DeclareParameter:
			DeclareParameter(*Inter);
			break;
		case Intermediate_Set::DeclareThis:
			DeclareThis(*Inter);
			break;
		case Intermediate_Set::DeclareThisVar:
			DeclareThisVar(*Inter);
			break;
		case Intermediate_Set::DeclareVar:
			DeclareVar(*Inter);
			break;
		case Intermediate_Set::FuncEnd:
			goto EndGetVarsLoop;
		default:
			NextIns();
			break;
		}
	
	}
EndGetVarsLoop:
	Index = FuncIndex;
	bool IsCallingFunc = false;

	while (Intermediate_Instruction* Inter = Get_Ins())
	{
		switch (Inter->OpCode)
		{
		case Intermediate_Set::DeclareExpression:
			OutValue =DeclareExpression();
			RegisterLock(OutValue);
			break;
		case Intermediate_Set::DeclareBinaryExpression:
			OutValue = DeclareBinaryExpression(*Inter);
			RegisterLock(OutValue);
			break;
		case Intermediate_Set::PushParameter:
			if (!IsCallingFunc)
			{
				IsCallingFunc = true;
				auto V = GetFreeRegister();
				InstructionBuilder::StoreNativeU(_Ins, V, (UInt64)funcStackSize); PushIns();
				InstructionBuilder::IncrementStackPointer(_Ins, V); PushIns();
			}
			GenIns(InstructionBuilder::Push8(_Ins,OutValue)); PushIns();
			RegisterLock(OutValue);
			NextIns();
			break;
		case Intermediate_Set::PopParameter:
		{
			auto PopAndTrashRegister = GetFreeRegister();

			GenIns(InstructionBuilder::Pop8(_Ins, PopAndTrashRegister)); PushIns();
			NextIns();
		}	break;
		case Intermediate_Set::StoreVar:
			StoreVar(*Inter);
			break;
		case Intermediate_Set::Ret:
			NextIns();
			if (OutValue != RegisterID::NullRegister) {
				BuildStore8RegToReg(OutValue, RegisterID::OuPutRegister);
			}
			break;
		case Intermediate_Set::FuncEnd:
			GenIns(InstructionBuilder::Return(ExitState::Success,_Ins)); PushIns();
			NextIns();
			goto EndLoop;
		case Intermediate_Set::CallFunc:
		{
			
			auto V =GetFreeRegister();
			/*
			InstructionBuilder::StoreNativeU(_Ins, V, (UInt64)funcStackSize); PushIns();
			InstructionBuilder::IncrementStackPointer(_Ins, V); PushIns();
		    */

			GenIns(
				_Ins.OpCode_AsInt = (InstructionSet_t)Intermediate_Set::TepFuncCall;
			_Ins.Value0_AsAddress = Inter->Value0_AsAddress;
			);
			PushIns();

			ResetRegistersData();//who know what will happen to the Registers

			V = GetFreeRegister();
			InstructionBuilder::StoreNativeU(_Ins, V, (UInt64)funcStackSize); PushIns();
			InstructionBuilder::DecrementStackPointer(_Ins, V); PushIns();

			OutValue = RegisterID::OuPutRegister;

			NextIns();
			IsCallingFunc = false;
		}	break;
		case Intermediate_Set::DeclareStaticVar:
		case Intermediate_Set::DeclareParameter:
		case Intermediate_Set::DeclareThis:
		case Intermediate_Set::DeclareThisVar:
		case Intermediate_Set::DeclareVar:
			NextIns();
			break;
		default:
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
			break;
		}
	}
	EndLoop:
	funcStackSize = 0;
	ParameterSize = 0;
	_OutPut->Add_NameToInstruction(FuncPos, _FuncName.data());
	Scope.ReMoveScope();
}
UCodeLangEnd


