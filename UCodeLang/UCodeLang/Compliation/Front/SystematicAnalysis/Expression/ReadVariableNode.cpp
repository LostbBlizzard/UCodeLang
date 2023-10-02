#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnReadVariable(const ReadVariableNode& nod)
{
	Symbol* Symbol = nullptr;
	if (_PassType != PassType::BuidCode)
	{
		GetMemberTypeSymbolFromVar_t V;
		if (!Symbol_MemberTypeSymbolFromVar(nod._VariableName, V))
		{
			_LastExpressionType = TypesEnum::Null;
			return;
		}
		_LastExpressionType = V.Type;
		Symbol = V._Symbol;

	}


	auto FToken = NeverNullptr(nod._VariableName._ScopedName.front()._token);


	auto Token = NeverNullptr(nod._VariableName._ScopedName.back()._token);
	auto Str = FToken->Value._String;


	bool DoStuff = false;
	if (FToken->Type == TokenType::KeyWord_This)
	{
		/*
		if (_ClassStack.size() == 0)
		{
			LogError_CantUseThisKeyWordHereError(FToken);
			return;
		}


		if (!Context_IsInThisFuncCall())
		{
			LogError_CantUseThisInStaticFunction(FToken);
			_LastExpressionType = TypeSymbol(TypesEnum::Null);
			return;
		}


		auto& Func = _FuncStack.back();
		auto ObjectType = Func.Pointer->GetObjectForCall();
		if (Type_IsUnMapType(*ObjectType))
		{
			_LastExpressionType = *ObjectType;
			return;
		}
		auto objecttypesyb = Symbol_GetSymbol(*ObjectType).value();
		ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();


		Symbol = Symbol_GetSymbol(*ObjectType).value_unchecked();

		DoStuff = true;
		*/
	}

	ReadVarErrorCheck_t Info;
	if (DoStuff == false)
	{
		/*
		Symbol = Symbol_GetSymbol(Str, SymbolType::Varable_t).value_unchecked();





		if (IsRead(_GetExpressionMode.top()))
		{
			Info = TryLogError_OnReadVar(Str, Token, Symbol);
		}
		if (IsWrite(_GetExpressionMode.top()))
		{
			Symbol->SetTovalid();

		}


		if (Info.CantFindVar)
		{
			return;
		}
		if (Symbol->Type == SymbolType::StackVarable)
		{
			auto Info = Symbol->Get_Info<DeclareVariableInfo>();


			if (Info->LineNumber > FToken->OnPos)
			{
				LogError_UseingVarableBeforDeclared(FToken);
			}
		}
		*/
	}

	if (!Info.VarIsInvalid)
	{
		if (_PassType == PassType::BuidCode)
		{
			auto& LookForT = Type_Get_LookingForType();



			GetMemberTypeSymbolFromVar_t V;
			if (!Symbol_MemberTypeSymbolFromVar(nod._VariableName, V))
			{
				return;
			}
			FileDependency_AddDependencyToCurrentFile(V._Symbol);

			bool LookIsAddress = LookForT.IsAddress() || LookForT.IsAddressArray();
			bool LookMove = LookForT.IsMovedType();

			bool AmIsAddress = V.Type.IsAddress();

			bool AsPointer = LookForT.IsAddress();

			//

			if (IsWrite(_GetExpressionMode.top()))
			{
				bool DoStore = true;
				if (V._Symbol->Type == SymbolType::ParameterVarable)
				{
					auto ParInfo = V._Symbol->Get_Info<ParameterInfo>();
					if (ParInfo->IsOutValue)
					{
						DoStore = false;
						_IR_LastStoreField = IR_Build_Member_DereferencStore(V);
					}
				}


				if (DoStore)
				{
					if (_GetExpressionMode.top() == GetValueMode::WritePointerReassment)
					{
						_IR_LastStoreField = IR_Build_Member_Store(V);
					}
					else
					{
						_IR_LastStoreField = IR_Build_Member_Store(V, V.Type);
					}
				}
			}

			if (IsRead(_GetExpressionMode.top()))
			{
				if (LookMove && HasMoveContructerHasIRFunc(V.Type))
				{
					_IR_LastExpressionField = IR_Build_Member_AsPointer(V);
				}
				else
					if (LookIsAddress == true && AmIsAddress == true)
					{
						_IR_LastExpressionField = IR_Build_Member_GetValue(V);
					}
					else if (LookIsAddress == false && AmIsAddress == false)
					{
						_IR_LastExpressionField = IR_Build_Member_AsValue(V);
					}
					else if (LookIsAddress == true && AmIsAddress == false)
					{
						_IR_LastExpressionField = IR_Build_Member_AsPointer(V);
					}
					else if (LookIsAddress == false && AmIsAddress == true)
					{
						_IR_LastExpressionField = IR_Build_Member_AsValue(V);
					}
					else
					{
						UCodeLangUnreachable();
					}

			}

			_LastExpressionType = V.Type;
		}
	}

	_LastLookedAtToken = Token.AsNullable();
}

void SystematicAnalysis::IR_Build_MemberDereferencStore(const GetMemberTypeSymbolFromVar_t& In, IRInstruction* Value)
{
	_IR_LookingAtIRBlock->NewDereferenc_Store(IR_Build_Member_DereferencStore(In), Value);
}
IRInstruction* SystematicAnalysis::IR_Build_Member_GetValue(const GetMemberTypeSymbolFromVar_t& In)
{
	switch (In._Symbol->Type)
	{
	case SymbolType::StaticVarable:
	case SymbolType::ThreadVarable:
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
	case  SymbolType::ParameterVarable:
	{
		IRInstruction* Output = nullptr;

		BuildMember_Access(In, Output);
		if (Output == nullptr)
		{
			Output = In._Symbol->IR_Ins;
		}
		bool UseOutput = In._Symbol->IR_Ins != Output;

		Debug_Add_SetLineNumber(NeverNullptr(In.Start->_token), _IR_LookingAtIRBlock->Instructions.size());

		if (In._Symbol->Type == SymbolType::StackVarable
			|| In._Symbol->Type == SymbolType::Class_Field)
		{
			return _IR_LookingAtIRBlock->NewLoad(Output);
		}
		else if (In._Symbol->Type == SymbolType::ParameterVarable)
		{
			return UseOutput ? _IR_LookingAtIRBlock->NewLoad(Output) : _IR_LookingAtIRBlock->NewLoad(In._Symbol->IR_Par);
		}
		else if (In._Symbol->Type == SymbolType::StaticVarable || In._Symbol->Type == SymbolType::ThreadVarable)
		{
			return UseOutput ? _IR_LookingAtIRBlock->NewLoad(Output) : _IR_LookingAtIRBlock->NewLoad_IRID(_IR_Builder.ToID(In._Symbol->FullName));
		}
		else
		{
			UCodeLangUnreachable();
		}


	}
	break;
	case SymbolType::Hard_Func_ptr:
	case SymbolType::Func_ptr:
	case SymbolType::Func:
	{
		FuncInfo* Finfo = In._Symbol->Get_Info<FuncInfo>();

		return _IR_LookingAtIRBlock->NewLoadFuncPtr(IR_GetIRID(Finfo));
	}

	break;
	case SymbolType::Enum_Field:
	{
		auto Einfo = In.Get_V2<EnumInfo>();
		auto FeldInfo = In.Get_V1<EnumFieldInfo>();

		if (Einfo->VariantData.has_value())
		{
			auto ID = _IR_Builder.ToID(Einfo->FullName);

			auto Key = LoadEvaluatedEx(FeldInfo->Ex, Einfo->Basetype);

			auto VariantClass = _IR_LookingAtIRBlock->NewLoad(IRType(ID));
			IRStruct* V = _IR_Builder.GetSymbol(ID)->Get_ExAs<IRStruct>();
			auto Member = _IR_LookingAtIRBlock->New_Member_Access(VariantClass, V, EnumVarantKeyIndex);
			_IR_LookingAtIRBlock->NewStore(Member, Key);
			return VariantClass;
		}
		else
		{
			return LoadEvaluatedEx(FeldInfo->Ex, Einfo->Basetype);
		}
	}
	case SymbolType::Type_class:
	{
		if (In.Start[0]._token->Type == TokenType::KeyWord_This)
		{
			IRInstruction* Output = nullptr;
			BuildMember_Access(In, Output);
			return _IR_LookingAtIRBlock->NewLoad(Output);
		}
		else
		{
			UCodeLangUnreachable();
		}
	}
	case  SymbolType::ConstantExpression:
	{
		auto Item = In._Symbol->Get_Info<ConstantExpressionInfo>();
		return LoadEvaluatedEx(Item->Ex, In._Symbol->VarType);
	}
	default:
		UCodeLangUnreachable();
		break;
	}
}
IRInstruction* SystematicAnalysis::IR_Build_Member_AsPointer(const GetMemberTypeSymbolFromVar_t& In)
{
	if (In.Type.IsAddress() || In.Type.IsAddressArray())
	{
		return IR_Build_Member_GetValue(In);
	}
	else
	{
		return IR_Build_Member_GetPointer(In);
	}
}
IRInstruction* SystematicAnalysis::IR_Build_Member_DereferenceValue(const GetMemberTypeSymbolFromVar_t& In)
{
	TypeSymbol T = In.Type;
	T._IsAddress = false;
	IRType IRT = IR_ConvertToIRType(T);

	IRInstruction* Output = nullptr;

	BuildMember_Access(In, Output);
	if (Output == nullptr)
	{
		Output = In._Symbol->IR_Ins;
	}
	bool UseOutput = In._Symbol->IR_Ins != Output;

	Debug_Add_SetLineNumber(NeverNullptr(In.Start->_token), _IR_LookingAtIRBlock->Instructions.size());

	switch (In._Symbol->Type)
	{
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
		return UseOutput ? _IR_LookingAtIRBlock->NewLoad_Dereferenc(Output, IRT) : _IR_LookingAtIRBlock->NewLoad_Dereferenc(In._Symbol->IR_Ins, IRT);
		break;
	case  SymbolType::ParameterVarable:
		return UseOutput ? _IR_LookingAtIRBlock->NewLoad_Dereferenc(Output, IRT) : _IR_LookingAtIRBlock->NewLoad_Dereferenc(In._Symbol->IR_Par, IRT);
	default:
		UCodeLangUnreachable();
		break;
	}
}
IRInstruction* SystematicAnalysis::IR_Build_Member_AsValue(const GetMemberTypeSymbolFromVar_t& In)
{
	if (In.Type.IsAddress() || In.Type.IsAddressArray())
	{
		return IR_Build_Member_DereferenceValue(In);
	}
	else
	{
		return IR_Build_Member_GetValue(In);
	}
}
void SystematicAnalysis::BuildMember_Reassignment(const GetMemberTypeSymbolFromVar_t& In, const TypeSymbol& Type, IRInstruction* Value)
{
	if (Type.IsAddress())
	{
		IR_Build_MemberDereferencStore(In, Value);
	}
	else
	{
		IR_Build_Member_Store(In, Value);
	}
}

void  SystematicAnalysis::BuildMember_Access(const GetMemberTypeSymbolFromVar_t& In, IRInstruction*& Output)
{
	TypeSymbol Last_Type = In._Symbol->VarType;

	//

	if (In._Symbol->Type == SymbolType::Class_Field && _FuncStack.size() && _ClassStack.top().Info)
	{
		auto& Func = _FuncStack.back().Pointer;
	
		auto ThisParSym = Symbol_GetSymbol(Func->Pars.front().Type).value();
		TypeSymbol ObjectType;
		Variant<IRPar*, IRInstruction*> PointerIr;
		if (IsSymbolLambdaObjectClass(ThisParSym))
		{
			//auto parsym = Symbol_GetSymbol(ScopeHelper::ApendedStrings(Symbol_GetSymbol(Func)->FullName, ThisSymbolName), SymbolType::ParameterVarable).value();
			PointerIr = &_IR_LookingAtIRFunc->Pars.front();// LoadSymbolWhenInLambdaObjectInvoke(parsym);
			ObjectType = *Func->GetObjectForCall();
		}
		else
		{
			PointerIr = &_IR_LookingAtIRFunc->Pars.front();
			ObjectType = *Func->GetObjectForCall();
		}

		ObjectType._IsAddress = false;

		auto objecttypesyb = Symbol_GetSymbol(ObjectType).value();

		auto IRStructV = IR_Build_ConvertToIRClassIR(*objecttypesyb);
		auto F = _IR_Builder.GetSymbol(IRStructV)->Get_ExAs<IRStruct>();

		auto Token = In.Start[In.End - 1]._token;
		auto& Str = Token->Value._String;
		ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();
		size_t MemberIndex = V->GetFieldIndex(Str).value();


		if (auto ir = PointerIr.Get_If<IRPar*>()) 
		{
			Output = _IR_LookingAtIRBlock->New_Member_Dereference(*ir, IRType(IRSymbol(IRStructV)), MemberIndex);
		}
		else if (auto ir = PointerIr.Get_If<IRInstruction*>())
		{
			Output = _IR_LookingAtIRBlock->New_Member_Dereference(*ir, IRType(IRSymbol(IRStructV)), MemberIndex);
		}
		else
		{
			UCodeLangUnreachable();
		}

		return;
	}
	if (In.Start[0]._token->Type == TokenType::KeyWord_This)
	{
		auto& PointerIr = _IR_LookingAtIRFunc->Pars.front();
		Output = _IR_LookingAtIRBlock->NewLoad(&PointerIr);

		auto& Func = _FuncStack.back();
		Last_Type = *Func.Pointer->GetObjectForCall();
	}
	//


	for (size_t i = 1; i < In.End; i++)
	{
		Symbol* Sym = Symbol_GetSymbol(Last_Type).value_unchecked();

		auto& ITem = In.Start[i];
		ScopedName::Operator_t OpType = i == 0 ? ScopedName::Operator_t::Null : In.Start[i - 1]._operator;

		StepBuildMember_Access(ITem, Last_Type, OpType, In, Output);
	}

}

void SystematicAnalysis::StepBuildMember_Access(const ScopedName& ITem, TypeSymbol& Last_Type, ScopedName::Operator_t OpType, const GetMemberTypeSymbolFromVar_t& In, IRInstruction*& Output)
{
	Symbol* Sym = Symbol_GetSymbol(Last_Type).value_unchecked();

	Debug_Add_SetLineNumber(NeverNullptr(ITem._token), _IR_LookingAtIRBlock->Instructions.size());

	if (!(OpType == ScopedName::Operator_t::Null
		|| OpType == ScopedName::Operator_t::Dot
		|| OpType == ScopedName::Operator_t::ScopeResolution))
	{

		auto Datav = Systematic_MemberOverloadData::GetOverloadData(OpType);
		if (Datav.has_value())
		{
			auto& Data = *Datav.value();
			String Scope = ToString(Last_Type);
			ScopeHelper::GetApendedString(Scope, Data.CompilerName);

			auto ConstructorSymbols = GetSymbolsWithName(Scope, SymbolType::Any);

			Symbol* funcToCallSys = nullptr;

			for (auto& Item2 : ConstructorSymbols)
			{
				if (Item2->Type == SymbolType::Func)
				{
					auto Info = Item2->Get_Info<FuncInfo>();

					if (Info->Pars.size() == 1) {
						funcToCallSys = Item2;
					}
					break;
				}
			}
			if (!funcToCallSys)
			{
				UCodeLangUnreachable();
			}

			auto Funcf = funcToCallSys->Get_Info< FuncInfo>();
			Get_FuncInfo V;
			V.Func = Funcf;
			V.SymFunc = funcToCallSys;
			V.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;

			if (Output == nullptr)
			{
				_IR_LastExpressionField = In._Symbol->IR_Ins;
			}
			else
			{
				_IR_LastExpressionField = Output;
			}


			ValueParametersNode h;
			IR_Build_FuncCall(Last_Type, V, h);

			Last_Type = Funcf->Ret;
			Sym = Symbol_GetSymbol(Last_Type).value().value();
			Output = _IR_LastExpressionField;
		}
	}

	String MemberName;
	if (ITem._token->Type == TokenType::Class)
	{
		MemberName = _VarableMemberDatas.GetValue(Symbol_GetSymbolID(&ITem)).MemberString;
	}
	else
	{
		MemberName = ITem._token->Value._String;
	}


	switch (Sym->Type)
	{
	case  SymbolType::Type_class:
	{
		auto* Classinfo = Sym->Get_Info<ClassInfo>();
		size_t MemberIndex = Classinfo->GetFieldIndex(MemberName).value();
		FieldInfo* FInfo = &Classinfo->Fields[MemberIndex];
		IRStruct* IRstruct = _IR_Builder.GetSymbol(_Symbol_SybToIRMap.GetValue(Sym->ID))->Get_ExAs<IRStruct>();
		if (Output == nullptr)
		{
			switch (In._Symbol->Type)
			{
			case  SymbolType::StackVarable:
			{
				TypeSymbol& TypeSys = Last_Type;
				if (TypeSys.IsAddress())
				{
					Output = _IR_LookingAtIRBlock->New_Member_Dereference(In._Symbol->IR_Ins, IR_ConvertToIRType(Sym->VarType), MemberIndex);
				}
				else
				{
					Output = _IR_LookingAtIRBlock->New_Member_Access(In._Symbol->IR_Ins, IRstruct, MemberIndex);
				}
			}
			break;
			case  SymbolType::ParameterVarable:
			{
				TypeSymbol& TypeSys = Last_Type;
				if (TypeSys.IsAddress())
				{
					Output = _IR_LookingAtIRBlock->New_Member_Dereference(In._Symbol->IR_Par, IR_ConvertToIRType(Sym->VarType), MemberIndex);
				}
				else
				{
					Output = _IR_LookingAtIRBlock->New_Member_Access(In._Symbol->IR_Par, IRstruct, MemberIndex);
				}

			}
			break;
			case SymbolType::ThreadVarable:
			case SymbolType::StaticVarable:
			{
				TypeSymbol& TypeSys = Last_Type;
				auto id = _IR_Builder.ToID(In._Symbol->FullName);
				if (TypeSys.IsAddress())
				{
					Output = _IR_LookingAtIRBlock->New_Member_Dereference(id, IR_ConvertToIRType(Sym->VarType), MemberIndex);
				}
				else
				{
					Output = _IR_LookingAtIRBlock->New_Member_Access(id, IRstruct, MemberIndex);
				}

			}
			break;
			default:
				UCodeLangUnreachable();
				break;
			}
		}
		else
		{
			TypeSymbol& TypeSys = Last_Type;
			if (TypeSys.IsAddress())
			{
				Output = _IR_LookingAtIRBlock->New_Member_Dereference(Output, IR_ConvertToIRType(Sym->VarType), MemberIndex);
			}
			else
			{
				Output = _IR_LookingAtIRBlock->New_Member_Access(Output, IRstruct, MemberIndex);
			}
		}
		Last_Type = FInfo->Type;
	}
	break;

	default:
		UCodeLangUnreachable();
		break;
	}
}

UCodeLangFrontEnd