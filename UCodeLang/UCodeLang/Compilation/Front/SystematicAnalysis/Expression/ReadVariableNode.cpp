#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
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


			{
				auto symop = Symbol_GetSymbol(V.Type);

				if (symop.has_value())
				{
					auto& sym = symop.value();
	
					bool unpacktype = false;

					if (unpacktype)
					{
						//... make struct
					}
					else
					{
						if (sym->Type == SymbolType::Type_Pack)
						{
							_LastExpressionType = V.Type;
							return;
						}
					}
				}
			}
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
					V.Type.SetAsAddress();
				}
				else if (LookIsAddress == true && AmIsAddress == true)
				{
					if (_GetExpressionMode.top() == GetValueMode::ReadAndWrite) 
					{
						_IR_LastExpressionField = IR_Build_Member_AsValue(V);
					}
					else
					{
						_IR_LastExpressionField = IR_Build_Member_GetValue(V);
					}
				}
				else if (LookIsAddress == false && AmIsAddress == false)
				{
					_IR_LastExpressionField = IR_Build_Member_AsValue(V);
				}
				else if (LookIsAddress == true && AmIsAddress == false)
				{
					_IR_LastExpressionField = IR_Build_Member_AsPointer(V);
					V.Type.SetAsAddress();
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

IROperator  SystematicAnalysis::IR_Build_Member_Store(const GetMemberTypeSymbolFromVar_t& In)
{

	IRInstruction* Output = nullptr;

	BuildMember_Access(In, Output);
	if (Output == nullptr)
	{
		Output = In._Symbol->IR_Ins;
	}
	bool UseOutput = In._Symbol->IR_Ins != Output;

	//


	switch (In._Symbol->Type)
	{
	case  SymbolType::Type_class://this
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
		return Output;
	case  SymbolType::ParameterVarable:
		return UseOutput ? IROperator(Output) : IROperator(In._Symbol->IR_Par);
		break;
	case SymbolType::StaticVarable:
	case SymbolType::ThreadVarable:
		return UseOutput ? IROperator(Output) : IROperator(_IR_Builder.ToID(In._Symbol->FullName));
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}
IROperator  SystematicAnalysis::IR_Build_Member_DereferencStore(const GetMemberTypeSymbolFromVar_t& In)
{
	IRInstruction* Output = nullptr;

	BuildMember_Access(In, Output);
	if (Output == nullptr)
	{
		Output = In._Symbol->IR_Ins;
	}
	bool UseOutput = In._Symbol->IR_Ins != Output;

	switch (In._Symbol->Type)
	{
	case  SymbolType::Type_class://this
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
		return  Output;
	case  SymbolType::ParameterVarable:
		return UseOutput ? IROperator(IROperatorType::DereferenceOf_IRInstruction, Output) : IROperator(IROperatorType::DereferenceOf_IRParameter, In._Symbol->IR_Par);
		break;
	case  SymbolType::StaticVarable:
	case  SymbolType::ThreadVarable:
		return UseOutput ? IROperator(IROperatorType::DereferenceOf_IRInstruction, Output) : IROperator(IROperatorType::DereferenceOf_IRParameter, _IR_Builder.ToID(In._Symbol->FullName));
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}
IROperator SystematicAnalysis::IR_Build_Member_Store(const GetMemberTypeSymbolFromVar_t& In, const TypeSymbol& Type)
{
	if (Type.IsAddress())
	{
		return IR_Build_Member_DereferencStore(In);
	}
	else
	{
		return IR_Build_Member_Store(In);
	}
}

IRInstruction* SystematicAnalysis::IR_Build_Member_GetPointer(const GetMemberTypeSymbolFromVar_t& In)
{
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
	case  SymbolType::Type_class://this
	case  SymbolType::Class_Field:
	case  SymbolType::StackVarable:
		return _IR_LookingAtIRBlock->NewLoadPtr(Output);
		break;
	case  SymbolType::ParameterVarable:
		return UseOutput ? _IR_LookingAtIRBlock->NewLoadPtr(Output) : _IR_LookingAtIRBlock->NewLoadPtr(In._Symbol->IR_Par);
		break;
	case  SymbolType::StaticVarable:
	case  SymbolType::ThreadVarable:
		return UseOutput ? _IR_LookingAtIRBlock->NewLoadPtr(Output) : _IR_LookingAtIRBlock->NewLoadPtr(_IR_Builder.ToID(In._Symbol->FullName));
	default:
		UCodeLangUnreachable();
		break;
	}
}


bool SystematicAnalysis::Symbol_MemberTypeSymbolFromVar(size_t Start, size_t End, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out)
{

	if (_PassType == PassType::GetTypes) { return false; }

	auto& Mod = _GetExpressionMode.top();


	size_t ScopedCount = 0;
	if (Out._Symbol == nullptr && Out.Type.IsBadType())
	{


		if (node._ScopedName[Start]._token->Type != TokenType::KeyWord_This)
		{
			auto ScopeName = node._ScopedName[Start];
			auto Token = NeverNullptr(ScopeName._token);
			auto& Str = Token->Value._String;
			auto SymbolVarOp = Symbol_GetSymbol(Str, SymbolType::Varable_t);
			_LastLookedAtToken = Token.AsNullable();


			if (!SymbolVarOp.has_value())
			{


				if (node._ScopedName.size() == 1)
				{
					auto look = Type_Get_LookingForType();
					if (auto val = Symbol_GetSymbol(look).value_unchecked())
					{
						if (val->Type == SymbolType::Enum)
						{
							EnumInfo* Einfo = val->Get_Info<EnumInfo>();
							auto& NameString = Str;

							auto FeldInfo = Einfo->GetFieldIndex(NameString);
							if (FeldInfo.has_value())
							{
								if (Einfo->VariantData)
								{
									auto& Item = Einfo->VariantData.value().Variants[FeldInfo.value()];

									bool IsOk = Item.Types.size() == 0;

									if (!IsOk)
									{

										LogError_MustMakeEnumLikeafunction(Einfo, FeldInfo.value(), Token);


										Out._Symbol = nullptr;
										Out.Type = TypesEnum::Null;
										return false;
									}
								}

								Out.Type.SetType(val->ID);//set enum type

								{
									String FieldSymFullName = val->FullName;
									ScopeHelper::GetApendedString(FieldSymFullName, NameString);

									Symbol* FieldSym = Symbol_GetSymbol(FieldSymFullName, SymbolType::Enum_Field).value().value();

									Out._Symbol = FieldSym;//set symbol as enum field


									Out.Set_V1(&Einfo->Fields[*FeldInfo]);
									Out.Set_V2(Einfo);

								}
								return true;
							}
						}
					}

					if (Type_IsUnMapType(look))
					{
						Out._Symbol = nullptr;
						Out.Type = look;

						return true;
					}
				}

				LogError_CantFindVarError(Token, Str);
				Out._Symbol = nullptr;
				Out.Type = TypeSymbol();

				return false;
			}
			auto SymbolVar = SymbolVarOp.value();

			if (SymbolVar->PassState == PassType::GetTypes)
			{
				Symbol_Update_Sym_ToFixedTypes(SymbolVar);
			}


			if (IsWrite(Mod))
			{
				TryLogError_OnWritingVar(SymbolVar, Token, String_view(Str));
			}
			if (IsRead(Mod))
			{
				TryLogError_OnReadVar(String_view(Str), Token, SymbolVar.AsNullable());
			}


			Out.Type = SymbolVar->VarType;
			Out._Symbol = SymbolVar.value();

			if (SymbolVar->Type == SymbolType::Func)
			{
				auto funcinfo = SymbolVar->Get_Info<FuncInfo>();
				auto StrFunc = GetTepFuncPtrName(funcinfo);
				Out.Type = TypeSymbol(GetTepFuncPtrSyb(StrFunc, funcinfo).value()->ID);
			}

			if (SymbolVar->Type == SymbolType::Class_Field)
			{
				if (!Context_IsInThisFuncCall())
				{
					LogError_CantUseThisInStaticFunction(Token);
					Out._Symbol = nullptr;
					Out.Type = TypeSymbol();
					return false;
				}
			}

			if (ScopeName._generic.get() && ScopeName._generic->_Values.size())
			{
				TypeNode Tep;
				ScopedName V;
				V._token = ScopeName._token;
				Tep._name._ScopedName.push_back(std::move(V));
				auto& Other = *ScopeName._generic;
				auto& _generic = *Tep._name._ScopedName.back()._generic;

				{//can't copy TypeNode but we need anyway.
					_generic._Values.resize(Other._Values.size());
					memcpy(_generic._Values.data(), Other._Values.data(), sizeof(TypeNode) * Other._Values.size());

				}

				TypeSymbol Type;
				Type_Convert(Tep, Type);
				if (Type._Type != TypesEnum::Null)
				{
					Out.Type = Type;
					Out._Symbol = Symbol_GetSymbol(Type).value_unchecked();
				}

				{// TypeNode has Unique_ptr we do this to not free it.
					new (_generic._Values.data()) TypeNode[Other._Values.size()];
				}
			}
			//
			Start++;
			End--;
			ScopedCount++;
		}
		else
		{
			if (_ClassStack.size() == 0)
			{
				LogError_CantUseThisKeyWordHereError(NeverNullptr(node._ScopedName[Start]._token));
				return false;
			}


			if (!Context_IsInThisFuncCall())
			{
				LogError_CantUseThisInStaticFunction(NeverNullptr(node._ScopedName[Start]._token));
				return false;
			}

			bool readcopythisptr = true;
			if (_LookingForTypes.top().IsAddress())
			{
				readcopythisptr = false;
			}

			auto& Func = _FuncStack.back().Pointer;
			auto ObjectType = Func->GetObjectForCall();
			if (Type_IsUnMapType(*ObjectType))
			{
				Out.Type = *Func->GetObjectForCall();
				Out._Symbol = Symbol_GetSymbol(ScopeHelper::ApendedStrings(Func->FullName, ThisSymbolName), SymbolType::ParameterVarable)
					.value().value();



				if (readcopythisptr)
				{
					auto old = Out.Type;


					Out.Type = TypeSymbol();

					Out.Type._Type = old._Type;
					Out.Type._CustomTypeSymbol = old._CustomTypeSymbol;
				}

				return true;
			}


			auto ThisParSym = Symbol_GetSymbol(Func->Pars.front().Type).value();
			if (IsSymbolLambdaObjectClass(ThisParSym))
			{
				//If The ThisPar an Lambda Object
				auto parsym = Symbol_GetSymbol(ScopeHelper::ApendedStrings(ThisParSym->FullName, ThisSymbolName), SymbolType::ParameterVarable).value();

				Out.Type = parsym->VarType;
				Out._Symbol = parsym.value();
			}
			else
			{
				auto objecttypesyb = Symbol_GetSymbol(*ObjectType).value();
				ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();

				Out.Type = *Func->GetObjectForCall();
				Out._Symbol =
					Symbol_GetSymbol(ScopeHelper::ApendedStrings(Func->FullName, ThisSymbolName), SymbolType::ParameterVarable)
					.value().value();


				if (readcopythisptr)
				{
					auto old = Out.Type;


					Out.Type = TypeSymbol();

					Out.Type._Type = old._Type;
					Out.Type._CustomTypeSymbol = old._CustomTypeSymbol;
				}
			}//
			Start++;
			End--;
			ScopedCount++;
		}
	}


	bool BadValue = false;
	for (size_t i = Start; i < node._ScopedName.size(); i++)
	{
		if (i > End) { break; }
		ScopedCount++;

		ScopedName::Operator_t OpType = i == 0 ? ScopedName::Operator_t::Null : node._ScopedName[i - 1]._operator;
		if (Symbol_StepGetMemberTypeSymbolFromVar(node, i, OpType, Out) == false)
		{
			BadValue = true;
			continue;
		}
	}
	if (BadValue == false)
	{
		Out.Start = &node._ScopedName[Start - 1];
		Out.End = ScopedCount;
		if (End == -1)
		{
			Out.End++;//i dont know why this needed its most likely its us using two diff funcs for the same thing to get the start off syb.
		}

		{
			auto OldTepSyb = Out._Symbol;
			auto ConstExCheck = Out._Symbol;
			while (ConstExCheck &&
				(ConstExCheck->Type == SymbolType::Type_alias || ConstExCheck->Type == SymbolType::Hard_Type_alias))
			{
				ConstExCheck = Symbol_GetSymbol(ConstExCheck->VarType).value_unchecked();
			}
			if (ConstExCheck == nullptr)
			{
				Out._Symbol = OldTepSyb;

			}
			else if (ConstExCheck->Type == SymbolType::ConstantExpression)
			{
				ConstantExpressionInfo* ConstInfo = ConstExCheck->Get_Info<ConstantExpressionInfo>();

				Out.Type = ConstExCheck->VarType;
				Out._Symbol = ConstExCheck;
			}
		}

		if (!(Out._Symbol->Type == SymbolType::Class_Field
			|| Out._Symbol->Type == SymbolType::Enum_Field
			|| Out._Symbol->Type == SymbolType::Func
			|| Out._Symbol->Type == SymbolType::ConstantExpression
			|| Symbol_IsVarableType(Out._Symbol->Type)))
		{

			if (_PassType == PassType::FixedTypes)
			{
				auto& Item = NeverNullptr(node._ScopedName.back()._token);
				LogError_LogWantedAVariable(Item, Out._Symbol);
			}
			Out.Type.SetType(TypesEnum::Null);
			Out._Symbol = nullptr;

			return false;
		}

		/*
		if (IsWrite(Mod) && !(Out._Symbol->Type == SymbolType::Class_Field || Symbol_IsVarableType(Out._Symbol->Type)))
		{
			auto& Item = NeverNullptr(node._ScopedName.back()._token);
			LogError(ErrorCodes::InValidType, Item->OnLine, Item->OnPos, "You Cant Write to a " + ToString(Out._Symbol->Type));
		}
		*/



		if (_Varable.size())
		{
			auto& Data = _Varable.top();

			if (Out._Symbol->Type == SymbolType::ParameterVarable || Out._Symbol->Type == SymbolType::StackVarable) 
			{
				Data._UsedSymbols.push_back(Out._Symbol);
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
bool SystematicAnalysis::Symbol_StepGetMemberTypeSymbolFromVar(const ScopedNameNode& node, size_t Index, ScopedName::Operator_t OpType, GetMemberTypeSymbolFromVar_t& Out)
{
	auto& Item = node._ScopedName[Index];


	auto ItemToken = NeverNullptr(Item._token);
	_LastLookedAtToken = ItemToken.AsNullable();


	String_view ItemTokenString;

	if (ItemToken->Type == TokenType::Class)
	{
		if (_PassType == PassType::FixedTypes)
		{
			VarableMemberData Data;
			ExpressionNodeType TepEx;
			ValueExpressionNode TepExValue;
			ReadVariableNode TepExName;
			ScopedName ScopeN;
			Token TepToken = *ItemToken;
			TepToken.Type = TokenType::Name;


			ScopeN._token = &TepToken;
			TepExName._VariableName._ScopedName.push_back(ScopeN);

			TepExValue._Value.reset(&TepExName);
			TepEx._Value.reset(&TepExValue);


			OnExpressionTypeNode(TepEx, GetValueMode::Read);

			auto Type = _LastExpressionType;

			TypeSymbol StackCharArr;
			StackCharArr.SetType(TypesEnum::Char);



			bool IsCharArr = Type_IsStaticCharArr(Type);


			if (IsCharArr)
			{
				auto V = Eval_Evaluate(Type, TepEx);

				{//stop double free 
					TepEx._Value.release();
					TepExValue._Value.release();
				}

				if (V.has_value())
				{
					auto BuffData = V.value();
					char* Buffer = (char*)Eval_Get_Object(BuffData);


					Data.MemberString.resize(BuffData.EvaluatedObject.ObjectSize);
					memcpy(Data.MemberString.data(), BuffData.EvaluatedObject.Object_AsPointer.get(), BuffData.EvaluatedObject.ObjectSize);


					_VarableMemberDatas.AddValue(Symbol_GetSymbolID(&Item), std::move(Data));
					ItemTokenString = _VarableMemberDatas.GetValue(Symbol_GetSymbolID(&Item)).MemberString;
				}
				else
				{
					return false;
				}
			}
			else
			{

				{//stop double free 
					TepEx._Value.release();
					TepExValue._Value.release();
				}

				auto Token = Item._token;
				LogError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Type must be a char[/] and not a '" + ToString(Type) + "' to be used as a identfier.");
				return false;
			}
		}
		else if (_PassType == PassType::BuidCode)
		{
			auto& Data = _VarableMemberDatas.GetValue(Symbol_GetSymbolID(&Item));
			ItemTokenString = Data.MemberString;
		}

	}
	else
	{
		ItemTokenString = ItemToken->Value._String;
	}

	if (Out.Type._Type != TypesEnum::CustomType
		|| Out._Symbol == nullptr)
	{
		if (_PassType == PassType::FixedTypes)
		{
			LogError_CantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);
		}
		return false;
	}

	if (Out._Symbol->PassState == PassType::GetTypes)
	{
		Symbol_Update_Sym_ToFixedTypes(Out._Symbol);
	}

	bool IsOverloadMember = !(OpType == ScopedName::Operator_t::Null
		|| OpType == ScopedName::Operator_t::ScopeResolution
		|| OpType == ScopedName::Operator_t::Dot);


	if (IsOverloadMember)
	{
		auto Datav = Systematic_MemberOverloadData::GetOverloadData(OpType);
		if (Datav.has_value())
		{
			auto& Data = *Datav.value();
			String Scope = ToString(Out.Type);
			ScopeHelper::GetApendedString(Scope, Data.CompilerName);

			auto OverloadFunctionSymbols = GetSymbolsWithName(Scope, SymbolType::Any);

			Symbol* funcToCallSys = nullptr;

			for (auto& Item2 : OverloadFunctionSymbols)
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
				Out.Type.SetType(TypesEnum::Null);
				Out._Symbol = nullptr;

				auto& Item = ItemToken;

				LogError_CantFindMemberOverloadForType(Item, Data.token, Out.Type);
				return false;
			}
			else
			{
				Out.Type = funcToCallSys->Get_Info<FuncInfo>()->Ret;
				Out._Symbol = Symbol_GetSymbol(Out.Type).value_unchecked();

				OpType = ScopedName::Operator_t::Dot;
			}
		}
	}


	if (OpType == ScopedName::Operator_t::Null
		|| OpType == ScopedName::Operator_t::ScopeResolution
		|| OpType == ScopedName::Operator_t::Dot)
	{

		if (Out._Symbol->Type == SymbolType::Type_class
			|| Out._Symbol->Type == SymbolType::Generic_class)
		{
			ClassInfo* CInfo = Out._Symbol->Get_Info<ClassInfo>();

			if (OpType != ScopedName::Operator_t::ScopeResolution)
			{

				auto FeldInfo = CInfo->GetField(ItemTokenString);
				if (!FeldInfo.has_value())
				{
					if (_PassType == PassType::FixedTypes)
					{
						auto SymFullName = Out._Symbol->FullName;
						ScopeHelper::GetApendedString(SymFullName, ItemTokenString);
						auto SybOp = Symbol_GetSymbol(SymFullName, SymbolType::Type);
						if (SybOp)
						{
							auto Sym = SybOp.value();
							LogError_UseScopeResolutionAndNotDot(Sym, ItemToken);
						}
						else
						{
							LogError_CantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);
						}
					}
					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}

				auto fieldFullName = Out._Symbol->FullName;
				ScopeHelper::GetApendedString(fieldFullName, ItemTokenString);
				auto fieldSymbol = Symbol_GetSymbol(fieldFullName, SymbolType::Class_Field).value();
				{
					Symbol_AccessCheck(fieldSymbol, ItemToken);
				}


				auto& FieldType2 = (*FeldInfo)->Type;
				if (FieldType2._Type == TypesEnum::CustomType)
				{
					Out._Symbol = Symbol_GetSymbol(FieldType2._CustomTypeSymbol).value();
					Out.Type = FieldType2;
				}
				else
				{
					Out._Symbol = fieldSymbol.value();
					Out.Type = FieldType2;

					if (Index + 1 < node._ScopedName.size())
					{
						const NeverNullPtr<Token> Token = NeverNullptr(node._ScopedName.begin()->_token);

						auto Token2 = NeverNullptr(node._ScopedName[Index + 1]._token);
						auto& Str2 = Token->Value._String;
						if (_PassType == PassType::FixedTypes)
						{
							LogError_CantFindVarMemberError(Token2, Str2, Out.Type);
						}

						Out._Symbol = nullptr;
						Out.Type = TypesEnum::Null;
						return false;
					}
				}
				if (_PassType == PassType::BuidCode)
				{
					FileDependency_AddDependencyToCurrentFile(Out._Symbol);
				}
			}
			else
			{
				auto SymFullName = Out._Symbol->FullName;
				ScopeHelper::GetApendedString(SymFullName, ItemTokenString);
				auto FeldSybOp = Symbol_GetSymbol(SymFullName, SymbolType::Type);
				if (FeldSybOp)
				{
					auto FeldSyb = FeldSybOp.value();

					if (FeldSyb->Type == SymbolType::Class_Field)
					{
						LogError_UseDotAndNotScopeResolution(FeldSyb, ItemToken);
						Out._Symbol = nullptr;
						Out.Type = TypesEnum::Null;
						return false;
					}
					else
					{
						Out._Symbol = FeldSyb.value();
						Out.Type = FeldSyb->VarType;
						if (FeldSyb->Type == SymbolType::Func)
						{
							auto funcinfo = FeldSyb->Get_Info<FuncInfo>();
							auto StrFunc = GetTepFuncPtrName(funcinfo);
							Out.Type = TypeSymbol(GetTepFuncPtrSyb(StrFunc, funcinfo).value()->ID);
						}
					}
				}
				else
				{
					LogError_CantFindSymbolError(ItemToken, SymFullName);
					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}
			}
		}
		else if (Out._Symbol->Type == SymbolType::Enum)
		{
			if (OpType == ScopedName::Operator_t::ScopeResolution)
			{
				{
					const NeverNullPtr<Token> Token = NeverNullptr(node._ScopedName.begin()->_token);
					Symbol_AccessCheck(Out._Symbol, Token);
				}

				EnumInfo* Einfo = Out._Symbol->Get_Info<EnumInfo>();
				auto& NameString = ItemTokenString;

				auto FeldInfo = Einfo->GetFieldIndex(NameString);
				if (!FeldInfo.has_value())
				{
					if (_PassType == PassType::FixedTypes)
					{
						LogError_CantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);
					}

					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}
				else if (Einfo->VariantData)
				{
					auto& Item = Einfo->VariantData.value().Variants[FeldInfo.value()];

					bool IsOk = Item.Types.size() == 0;

					if (!IsOk)
					{

						LogError_MustMakeEnumLikeafunction(Einfo, FeldInfo.value(), ItemToken);


						Out._Symbol = nullptr;
						Out.Type = TypesEnum::Null;
						return false;
					}
				}

				if (Index + 1 < node._ScopedName.size())
				{
					const NeverNullPtr<Token> Token = NeverNullptr(node._ScopedName.begin()->_token);

					auto Token2 = NeverNullptr(node._ScopedName[Index + 1]._token);
					auto& Str2 = Token->Value._String;
					if (_PassType == PassType::FixedTypes)
					{
						LogError_CantFindVarMemberError(Token2, Str2, Out.Type);
					}

					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}


				Out.Type.SetType(Out._Symbol->ID);//set enum type

				{
					String FieldSymFullName = Out._Symbol->FullName;
					ScopeHelper::GetApendedString(FieldSymFullName, NameString);

					Symbol* FieldSym = Symbol_GetSymbol(FieldSymFullName, SymbolType::Enum_Field).value().value();

					Out._Symbol = FieldSym;//set symbol as enum field


					Out.Set_V1(&Einfo->Fields[*FeldInfo]);
					Out.Set_V2(Einfo);

				}

				if (_PassType == PassType::BuidCode)
				{
					FileDependency_AddDependencyToCurrentFile(Out._Symbol);
				}
			}
			else
			{
				auto SymFullName = Out._Symbol->FullName;
				ScopeHelper::GetApendedString(SymFullName, ItemTokenString);
				auto FeldSybOp = Symbol_GetSymbol(SymFullName, SymbolType::Type);
				if (FeldSybOp)
				{
					auto FeldSyb = FeldSybOp.value();


					LogError_UseDotAndNotScopeResolution(FeldSyb, ItemToken);
				}
				else
				{
					LogError_CantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);

				}
				Out._Symbol = nullptr;
				Out.Type = TypesEnum::Null;
				return false;
			}
		}
		else if (Out._Symbol->Type == SymbolType::Func)
		{
			if (Index + 1 < node._ScopedName.size())
			{
				const NeverNullPtr<Token> Token = NeverNullptr(node._ScopedName.begin()->_token);

				auto Token2 = NeverNullptr(node._ScopedName[Index + 1]._token);
				auto& Str2 = Token->Value._String;
				if (_PassType == PassType::FixedTypes)
				{
					LogError_CantFindVarMemberError(Token2, Str2, Out.Type);
				}

				Out._Symbol = nullptr;
				Out.Type = TypesEnum::Null;
				return false;
			}


			Symbol* Func = Out._Symbol;

			FuncInfo* Finfo = Out._Symbol->Get_Info<FuncInfo>();
			String TepFuncPtr = GetTepFuncPtrName(Finfo);

			NeverNullPtr<Symbol> V = GetTepFuncPtrSyb(TepFuncPtr, Finfo).value();
			Out._Symbol = V.value();
			Out.Type.SetType(V->ID);


			if (_PassType == PassType::BuidCode)
			{
				FileDependency_AddDependencyToCurrentFile(Func);
			}

		}
		else if (Symbol_IsVarableType(Out._Symbol->Type))
		{
			TypeSymbol VarableType = Out._Symbol->VarType;
			NullablePtr<Symbol> TypeAsSymbolOp = Symbol_GetSymbol(VarableType);
			if (TypeAsSymbolOp)
			{
				auto TypeAsSymbol = TypeAsSymbolOp.value();
				if (Out._Symbol->Type == SymbolType::Class_Field)
				{
					const NeverNullPtr<Token> token = NeverNullptr(node._ScopedName.begin()->_token);


					Symbol_AccessCheck(Out._Symbol, token);
				}

				if (!(TypeAsSymbol->Type == SymbolType::Type_class || TypeAsSymbol->Type == SymbolType::Tag_class))
				{
					LogError_CantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);

					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}
				ClassInfo* CInfo = TypeAsSymbol->Type == SymbolType::Type_class  ? TypeAsSymbol->Get_Info<ClassInfo>() : TypeAsSymbol->Get_Info<TagInfo>();


				auto FeldInfo = CInfo->GetField(ItemTokenString);
				if (!FeldInfo.has_value())
				{
					if (_PassType == PassType::FixedTypes)
					{
						LogError_CantFindVarMemberError(ItemToken, ItemTokenString, Out.Type);
					}

					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}

				NeverNullPtr<Symbol> FeldSyb;
				{
					auto FeldFullName = TypeAsSymbol->FullName;
					ScopeHelper::GetApendedString(FeldFullName, ItemTokenString);
					FeldSyb = Symbol_GetSymbol(FeldFullName, SymbolType::Class_Field).value();
					{
						Symbol_AccessCheck(FeldSyb, ItemToken);
					}
				}

				auto& FieldType2 = (*FeldInfo)->Type;
				if (FieldType2._Type == TypesEnum::CustomType)
				{
					Out._Symbol = FeldSyb.value();
					Out.Type = FieldType2;
				}
				else
				{
					Out.Type = FieldType2;
					//Out._Symbol = FeldSyb.value();

					if (Index + 1 < node._ScopedName.size())
					{
						const NeverNullPtr<Token> Token = NeverNullptr(node._ScopedName.begin()->_token);

						auto Token2 = NeverNullptr(node._ScopedName[Index + 1]._token);
						auto& Str2 = Token->Value._String;
						if (_PassType == PassType::FixedTypes)
						{
							LogError_CantFindVarMemberError(Token2, Str2, Out.Type);
						}

						Out._Symbol = nullptr;
						Out.Type = TypesEnum::Null;
						return false;
					}
				}
				if (_PassType == PassType::BuidCode)
				{
					FileDependency_AddDependencyToCurrentFile(Out._Symbol);
					//UCodeLangThrowException("not added");
				}
			}
			else
			{
				if (Index + 1 < node._ScopedName.size())
				{
					const NeverNullPtr<Token> Token = NeverNullptr(node._ScopedName.begin()->_token);

					auto Token2 = node._ScopedName[Index + 1]._token;
					auto& Str2 = Token->Value._String;
					if (_PassType == PassType::FixedTypes)
					{
						LogError_CantFindVarMemberError(NeverNullptr(Token2), Str2, VarableType);
					}

					Out._Symbol = nullptr;
					Out.Type = TypesEnum::Null;
					return false;
				}
			}

		}
		else if (Out._Symbol->Type == SymbolType::Class_Field)
		{
			int a = 0;
			String ClassSym = ScopeHelper::GetReMoveScope(Out._Symbol->FullName);
			auto LamdbSym = GetSymbolsWithName(ClassSym).front();
			UCodeLangAssert(IsSymbolLambdaObjectClass(NeverNullptr(LamdbSym)));
			UCodeLangAssert(_PassType == PassType::BuidCode);
			
			UCodeLangAssert(OpType == ScopedName::Operator_t::Dot);//TODO remove this Assert and add if for it

			ClassInfo* CInfo = LamdbSym->Get_Info<ClassInfo>();
			auto ClassSym2 = Symbol_GetSymbol(CInfo->GetField(ThisSymbolName).value()->Type).value();
			auto CInfo2 = ClassSym2->Get_Info<ClassInfo>();
			auto field = CInfo2->GetField(ItemTokenString);
			auto FeldInfo = field.value();

			auto SymFullName = ClassSym2->FullName;
			ScopeHelper::GetApendedString(SymFullName, ItemTokenString);
			auto FeldSybOp = Symbol_GetSymbol(SymFullName, SymbolType::Type);

			Out._Symbol = FeldSybOp.value().value();
			Out.Type = FeldInfo->Type;
		}
		else
		{
			UCodeLangUnreachable();//Bad Object
		}

	}

	return true;
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

		if (Einfo->IsOptionalAddress().has_value())
		{
			return IR_Load_UIntptr(0);
		}
		else if (Einfo->VariantData.has_value())
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
	Symbol* LastVarSym = nullptr;
	//

	if (In._Symbol->Type == SymbolType::Class_Field 
		&& _FuncStack.size() 
		&& _ClassStack.size()
		&& _ClassStack.top().Info)
	{
		auto& Func = _FuncStack.back().Pointer;
	
		auto ThisParSym = Symbol_GetSymbol(Func->Pars.front().Type).value();
		TypeSymbol ObjectType;
		Variant<IRPar*, IRInstruction*> PointerIr;

		auto Token = In.Start[In.End - 1]._token;

		bool test = false;
		if (IsSymbolLambdaObjectClass(ThisParSym) && Token->Type != TokenType::KeyWord_This
			&& IsLambdaClassSymFromThisPtr(ThisParSym, In._Symbol))
		{
			ClassInfo* f = ThisParSym->Get_Info<ClassInfo>();
			auto parsym = Symbol_GetSymbol(ScopeHelper::ApendedStrings(ThisParSym->FullName, ThisSymbolName), SymbolType::ParameterVarable).value();
			
			PointerIr = _IR_LookingAtIRBlock->New_Member_Dereference(
				&_IR_LookingAtIRFunc->Pars.front(),
				_IR_LookingAtIRFunc->Pars.front().type,
				f->GetFieldIndex(ThisSymbolName).value());

			ObjectType = parsym.value()->VarType;

			test = true;
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

		
		const String_view& Str = Token->Type == TokenType::KeyWord_This ? ThisSymbolName : Token->Value._String;
		ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();

		if (IsSymbolLambdaObjectClass(objecttypesyb) && Token->Type != TokenType::KeyWord_This
			&& IsLambdaClassSymFromThisPtr(objecttypesyb, In._Symbol))
		{
			auto ClassSym2 = Symbol_GetSymbol(V->GetField(ThisSymbolName).value()->Type).value();
			auto CInfo2 = ClassSym2->Get_Info<ClassInfo>();

			V = CInfo2;
		}

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
	else if (In.Start[0]._token->Type == TokenType::KeyWord_This)
	{
		auto& PointerIr = _IR_LookingAtIRFunc->Pars.front();
		Output = _IR_LookingAtIRBlock->NewLoad(&PointerIr);

		auto& Func = _FuncStack.back();
		Last_Type = *Func.Pointer->GetObjectForCall();
	
		auto& lookingfortype = _LookingForTypes.top();
		if (1 == In.End && !lookingfortype.IsAddress())
		{
			auto newtype = Last_Type;
			newtype._IsAddress = false;

			Output = _IR_LookingAtIRBlock->NewLoad_Dereferenc(Output, IR_ConvertToIRType(newtype));
			Last_Type = newtype;
		}
		LastVarSym =Symbol_GetSymbol(ScopeHelper::ApendedStrings(_FuncStack.front().Pointer->FullName, ThisSymbolName),SymbolType::ParameterVarable).value().value();
	}
	else if (In._Symbol->Type == SymbolType::Class_Field)
	{
		
		auto token = In.Start[0]._token;

		auto sym  = Symbol_GetSymbol(token->Value._String,SymbolType::Any).value();

		Last_Type = sym->VarType;

		if (sym->Type == SymbolType::StackVarable) {
			Output = sym->IR_Ins;
		}
		LastVarSym = sym.value();
	}
	//


	for (size_t i = 1; i < In.End; i++)
	{
		Symbol* Sym = Symbol_GetSymbol(Last_Type).value_unchecked();

		auto& Item = In.Start[i];
		ScopedName::Operator_t OpType = i == 0 ? ScopedName::Operator_t::Null : In.Start[i - 1]._operator;

		StepBuildMember_Access(Item, Last_Type, OpType, In, Output, LastVarSym);
	}

}

void SystematicAnalysis::StepBuildMember_Access(const ScopedName& Item, TypeSymbol& Last_Type, ScopedName::Operator_t OpType, const GetMemberTypeSymbolFromVar_t& In, IRInstruction*& Output, Symbol*& LastVarSym)
{
	Symbol* Sym = Symbol_GetSymbol(Last_Type).value_unchecked();

	Debug_Add_SetLineNumber(NeverNullptr(Item._token), _IR_LookingAtIRBlock->Instructions.size());

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

			auto MemberOverloadSymbols = GetSymbolsWithName(Scope, SymbolType::Any);

			Symbol* funcToCallSys = nullptr;

			for (auto& Item2 : MemberOverloadSymbols)
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
			UCodeLangAssert(funcToCallSys);

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

			auto par = Funcf->Pars[0].Type;
			if (par.IsAddress() && !Last_Type.IsAddress())
			{
				_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadPtr(_IR_LastExpressionField);
			}

			ValueParametersNode h;
			IR_Build_FuncCall(Last_Type, V, h);

			Last_Type = Funcf->Ret;
			Sym = Symbol_GetSymbol(Last_Type).value().value();
			Output = _IR_LastExpressionField;
		}
	}

	String MemberName;
	if (Item._token->Type == TokenType::Class)
	{
		MemberName = _VarableMemberDatas.GetValue(Symbol_GetSymbolID(&Item)).MemberString;
	}
	else
	{
		MemberName = Item._token->Value._String;
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
			if (LastVarSym == nullptr)
			{
				LastVarSym = In._Symbol;
			}
			switch (LastVarSym->Type)
			{
			case  SymbolType::StackVarable:
			{
				TypeSymbol& TypeSys = Last_Type;
				if (TypeSys.IsAddress())
				{
					Output = _IR_LookingAtIRBlock->New_Member_Dereference(LastVarSym->IR_Ins, IR_ConvertToIRType(Sym->VarType), MemberIndex);
				}
				else
				{
					Output = _IR_LookingAtIRBlock->New_Member_Access(LastVarSym->IR_Ins, IRstruct, MemberIndex);
				}
			}
			break;
			case  SymbolType::ParameterVarable:
			{
				TypeSymbol& TypeSys = Last_Type;
				if (TypeSys.IsAddress())
				{
					Output = _IR_LookingAtIRBlock->New_Member_Dereference(LastVarSym->IR_Par, IR_ConvertToIRType(Sym->VarType), MemberIndex);
				}
				else
				{
					Output = _IR_LookingAtIRBlock->New_Member_Access(LastVarSym->IR_Par, IRstruct, MemberIndex);
				}

			}
			break;
			case SymbolType::ThreadVarable:
			case SymbolType::StaticVarable:
			{
				TypeSymbol& TypeSys = Last_Type;
				auto id = _IR_Builder.ToID(LastVarSym->FullName);
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

#endif