#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnForNode(const ForNode& node)
{
	auto& StrVarName = node._Name->Value._String;



	Symbol* syb;

	String ScopeName = std::to_string((size_t)&node);

	Push_NewStackFrame();
	_Table.AddScope(ScopeName);
	auto FullName = _Table._Scope.GetApendedString(StrVarName);

	SymbolID sybId = Symbol_GetSymbolID(node);

	if (_PassType == PassType::GetTypes)
	{
		Symbol_RedefinitionCheck(FullName, SymbolType::StackVarable, NeverNullptr(node._Name));


		syb = &Symbol_AddSymbol(SymbolType::StackVarable, (String)StrVarName, FullName, AccessModifierType::Public);
		_Table.AddSymbolID(*syb, sybId);


		{
			DeclareVariableInfo* info = new DeclareVariableInfo();

			syb->Info.reset(info);

			info->LineNumber = node._Name->OnLine;
		}


		if (node._Type == ForNode::ForType::Traditional)
		{
			TypeSymbol BoolType(TypesEnum::Bool);

			_LookingForTypes.push(BoolType);
			OnExpressionTypeNode(node._BoolExpression._Value.get(), GetValueMode::Read);
			_LookingForTypes.pop();

			OnPostfixVariableNode(node._OnNextStatement);
		}
		else
		{
			OnExpressionTypeNode(node._Modern_List._Value.get(), GetValueMode::Read);
		}

		for (const auto& node2 : node._Body._Nodes)
		{
			OnStatement(*node2);
		}
	}
	else
	{
		syb = Symbol_GetSymbol(sybId).value();
	}

	if (_PassType == PassType::FixedTypes)
	{
		if (node._Type == ForNode::ForType::Traditional)
		{
			{
				auto& VarType = syb->VarType;
				Type_Convert(node._typeNode, VarType);
				VarType.SetAsLocation();


				auto Ex = node._Traditional_Assignment_Expression._Value.get();
				Type_DeclareVarableCheck(VarType, Ex, NeverNullptr(node._typeNode._name._ScopedName.back()._token));

				if (node._Traditional_Assignment_Expression._Value)
				{
					_LookingForTypes.push(VarType);

					OnExpressionTypeNode(node._Traditional_Assignment_Expression._Value.get(), GetValueMode::Read);

					_LookingForTypes.pop();

					syb->SetTovalid();

					auto& VarType = syb->VarType;


					auto& Ex = _LastExpressionType;
					auto token = node._typeNode._name._ScopedName.back()._token;
					Type_DeclareVariableTypeCheck(VarType, Ex, NeverNullptr(token));
				
				}
			}

			{
				TypeSymbol BoolType(TypesEnum::Bool);

				_LookingForTypes.push(BoolType);
				OnExpressionTypeNode(node._BoolExpression._Value.get(), GetValueMode::Read);
				_LookingForTypes.pop();

				if (_PassType == PassType::FixedTypes)
				{
					if (!Type_CanBeImplicitConverted(_LastExpressionType, BoolType))
					{
						auto  Token = _LastLookedAtToken.value();
						LogError_CantCastImplicitTypes(Token, _LastExpressionType, BoolType, true);
					}
				}
			}

			{
				OnPostfixVariableNode(node._OnNextStatement);
			}

			for (const auto& node2 : node._Body._Nodes)
			{
				OnStatement(*node2);
			}
		}
		else
		{
			{
				auto& VarType = syb->VarType;
				Type_Convert(node._typeNode, VarType);
				VarType.SetAsLocation();
			}

			auto Ex = node._Modern_List._Value.get();

			{
				_LookingForTypes.push(TypeSymbol(TypesEnum::Any));
				OnExpressionTypeNode(Ex, GetValueMode::Read);
				_LookingForTypes.pop();
			}
			auto& ExType = _LastExpressionType;

			auto HasInfo = Type_HasForOverLoadWith(ExType);
			if (!HasInfo.HasValue)
			{
				auto  Token = _LastLookedAtToken.value();
				LogError_DoesNotHaveForOverload(Token, ExType);

			}
			else
			{
				if (HasInfo.Value.value())
				{
					FuncInfo* FuncSym = HasInfo.Value.value()->Get_Info< FuncInfo>();
					const auto& TypeForType = FuncSym->Ret;

					Optional<Symbol*> GetFunc = {};

					if (syb->VarType.IsAn(TypesEnum::Var))
					{
						auto tep = Symbol_GetAnExplicitlyConvertedFunc(TypeForType);
						if (tep.has_value())
						{
							GetFunc = Symbol_GetSymbol(tep.value()).value();
						}
					}
					else
					{
						auto Tep = Type_CanBeExplicitlyConverted(TypeForType, syb->VarType);
						if (Tep.HasValue && Tep.Value.has_value())
						{
							GetFunc = Tep.Value.value();
						}
					}

					Optional<Symbol*>CheckFunc = {};
					auto V = Type_HasUrinaryOverLoadWith(TypeForType, TokenType::QuestionMark);

					if (V.HasValue && V.Value.has_value())
					{
						auto BoolType = TypeSymbol(TypesEnum::Bool);
						auto retType = V.Value.value()->VarType;
						if (Type_CanBeImplicitConverted(retType, BoolType, false))
						{
							CheckFunc = V.Value.value();
						}
						else
						{
							auto  Token = _LastLookedAtToken;
							LogError_CantCastImplicitTypes(Token.value(), BoolType, retType, false);
						}
					}

					if (!GetFunc.has_value())
					{
						const NeverNullPtr<Token> token = NeverNullptr(node._typeNode._name._ScopedName.front()._token);

						if (syb->VarType.IsAn(TypesEnum::Var)) {
							LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos,
								"The Type '" + ToString(TypeForType) + "' has no cast(->) overload.it is needed to access the object for the 'for' loop.");
						}
						else
						{
							LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos,
								"The Type '" + ToString(TypeForType) + "' has no explicit cast(->) overload for the type '" + ToString(syb->VarType) + "'.it is needed to access the object for  the 'for' loop.");
						}

						syb->VarType.SetType(TypesEnum::Null);
					}
					else if (!CheckFunc.has_value())
					{
						const NeverNullPtr<Token> token = NeverNullptr(node._typeNode._name._ScopedName.back()._token);

						LogError(ErrorCodes::InValidType, token->OnLine, token->OnPos,
							"The Type '" + ToString(TypeForType) + "' has no exist(?) overload.it is needed to check when to end the loop.");

						syb->VarType.SetType(TypesEnum::Null);
					}
					else
					{
						ForExpresion_Data g;
						g.FuncGetLoopAble = HasInfo.Value.value();
						g.FuncToGet = GetFunc.value();
						g.FuncToCheck = CheckFunc.value();
						_For_Datas.AddValue(Symbol_GetSymbolID(node), g);



						auto token = NeverNullptr(node._typeNode._name._ScopedName.back()._token);
						Type_DeclareVariableTypeCheck(syb->VarType, g.FuncToGet->Get_Info<FuncInfo>()->Ret, token);
					}
				}
				else
				{
					UCodeLangUnreachable();
				}
			}


			for (const auto& node2 : node._Body._Nodes)
			{
				OnStatement(*node2);
			}
		}

	}
	else if (_PassType == PassType::BuidCode)
	{
		Debug_Add_SetLineNumber(NeverNullptr(node._Name), _IR_LookingAtIRBlock->InsCount() ? _IR_LookingAtIRBlock->GetIndex() : 0);
		if (node._Type == ForNode::ForType::Traditional)
		{
			IRInstruction* OnVarable{};
			bool IsStructObjectPassRef = false;
			if (node._Traditional_Assignment_Expression._Value)
			{


				OnVarable = _IR_LookingAtIRBlock->NewLoad(IR_ConvertToIRType(syb->VarType));
				syb->IR_Ins = OnVarable;

				Debug_Add_SetLineNumber(NeverNullptr(node._Name), _IR_LookingAtIRBlock->GetIndex());
				Debug_Add_SetVarableInfo(*syb, _IR_LookingAtIRBlock->GetIndex());

				IsStructObjectPassRef = Type_IsStructPassByRef(syb);

				if (IsStructObjectPassRef)
				{
					_IR_IRlocations.push({ OnVarable ,false });
				}


				OnExpressionTypeNode(node._Traditional_Assignment_Expression._Value.get(), GetValueMode::Read);

				IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, syb->VarType);

				IR_Build_OnStoreVarable(IsStructObjectPassRef, OnVarable, syb, sybId);

				FileDependency_AddDependencyToCurrentFile(syb->VarType);
			}


			{
				TypeSymbol BoolType(TypesEnum::Bool);
				_LookingForTypes.push(BoolType);

				size_t BoolCode;
				size_t BoolJumps;
				if (_PassType == PassType::BuidCode)
				{
					BoolCode = _IR_LookingAtIRBlock->GetIndex();
					BoolJumps = IR_GetJumpsIndex();
				}
				OnExpressionTypeNode(node._BoolExpression._Value.get(), GetValueMode::Read);

				IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, BoolType);

				IRInstruction* BoolCode2 = _IR_LastExpressionField;
				IRBlock::NewConditionalFalseJump_t IfIndex = _IR_LookingAtIRBlock->NewConditionalFalseJump(BoolCode2);


				_LookingForTypes.pop();


				for (const auto& node2 : node._Body._Nodes)
				{
					OnStatement(*node2);
				}

				OnPostfixVariableNode(node._OnNextStatement);

				_IR_LookingAtIRBlock->NewJump(BoolCode);

				size_t BreakCode = _IR_LookingAtIRBlock->GetIndex();

				_IR_LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, BreakCode);


				IR_Build_UpdateJumpsBreakContiunes(BoolJumps, BoolCode, BreakCode);
			}

		}
		else
		{

			const ForExpresion_Data& Data = _For_Datas.GetValue(Symbol_GetSymbolID(node));
			{
				FileDependency_AddDependencyToCurrentFile(Data.FuncGetLoopAble);
				FileDependency_AddDependencyToCurrentFile(Data.FuncToCheck);
				FileDependency_AddDependencyToCurrentFile(Data.FuncToGet);
			}

			{
				auto finfo = Data.FuncGetLoopAble->Get_Info<FuncInfo>();
				auto Ex = node._Modern_List._Value.get();
				_LookingForTypes.push(finfo->Pars[0].Type);
				OnExpressionTypeNode(Ex, GetValueMode::Read);
				_LookingForTypes.pop();

				IR_Build_AddDestructorToStack(_LastExpressionType, _IR_LastExpressionField);
			}

			{
				Get_FuncInfo f;
				f.Func = Data.FuncGetLoopAble->Get_Info<FuncInfo>();
				f.SymFunc = Data.FuncGetLoopAble;
				f.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;
				IR_Build_FuncCall(f, {}, {});
			}
			IRInstruction* Loopobject = _IR_LastExpressionField;
			auto LoopObjectType = _LastExpressionType;
			{
				TypeSymbol BoolType(TypesEnum::Bool);
				_LookingForTypes.push(BoolType);
				auto BoolCode = _IR_LookingAtIRBlock->GetIndex();
				auto BoolJumps = IR_GetJumpsIndex();
				{//get if check
					Get_FuncInfo f;
					f.Func = Data.FuncToCheck->Get_Info<FuncInfo>();
					f.SymFunc = Data.FuncToCheck;
					f.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;


					if (f.Func->Pars[0].Type.IsAddress())
					{
						if (!LoopObjectType.IsAddress())
						{
							_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadPtr(Loopobject);
						}
					}

					IR_Build_FuncCall(f, {}, {});

					IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, TypesEnum::Bool);
				}
				_LookingForTypes.pop();

				IRInstruction* BoolCode2 = _IR_LastExpressionField;
				IRBlock::NewConditionalFalseJump_t IfIndex = _IR_LookingAtIRBlock->NewConditionalFalseJump(BoolCode2);

				{//get item
					_IR_LastExpressionField = Loopobject;
					Get_FuncInfo f;
					f.Func = Data.FuncToGet->Get_Info<FuncInfo>();
					f.SymFunc = Data.FuncToGet;
					f.ThisPar = Get_FuncInfo::ThisPar_t::PushFromLast;

					if (f.Func->Pars[0].Type.IsAddress())
					{
						if (!LoopObjectType.IsAddress())
						{
							_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoadPtr(Loopobject);
						}
					}

					IR_Build_FuncCall(f, {}, {});

					IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, syb->VarType);
					auto FuncRet = _IR_LastExpressionField;
					syb->IR_Ins = FuncRet;

					Debug_Add_SetLineNumber(NeverNullptr(node._Name), _IR_LookingAtIRBlock->GetIndex());
					Debug_Add_SetVarableInfo(*syb, _IR_LookingAtIRBlock->GetIndex());

					IR_Build_AddDestructorToStack(syb, syb->ID, syb->IR_Ins);
				}

				for (const auto& node2 : node._Body._Nodes)
				{
					OnStatement(*node2);
				}


				_IR_LookingAtIRBlock->NewJump(BoolCode);

				size_t BreakCode = _IR_LookingAtIRBlock->GetIndex();

				_IR_LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, BreakCode);





				IR_Build_UpdateJumpsBreakContiunes(BoolJumps, BoolCode, BreakCode);
			}


		}
	}

	_Table.RemoveScope();

	Pop_StackFrame();
}


UCodeLangFrontEnd