#ifndef UCodeLangNoCompiler
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
			bool isvarableok = false;
				
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
						GetFunc = HasInfo.Value.value();
					}
					else
					{
						auto Tep = Type_CanBeExplicitlyConverted(TypeForType, syb->VarType);
						if (Tep.HasValue && Tep.Value.has_value())
						{
							GetFunc = Tep.Value.value();
						}
					}
					NullablePtr<Symbol> NextFunc = {};
					String NextFuncName;
					bool HasAnyNextFunc = false;
					{
						NextFuncName += ToString(TypeForType);
						ScopeHelper::GetApendedString(NextFuncName, "Next");

						NextFunc = Symbol_GetSymbol(NextFuncName, SymbolType::Func);
						if (NextFunc.has_value())
						{
							if (NextFunc.value()->Type != SymbolType::Func)
							{
								NextFunc = {};
							}
						}
					}


					const NeverNullPtr<Token> token = NeverNullptr(node._typeNode._name._ScopedName.front()._token);
					if (!NextFunc.has_value())
					{
						ParInfo ThisPar;
						ThisPar.IsOutPar = false;
						ThisPar.Type = TypeForType;
						ThisPar.Type._IsAddress = true;

						LogError_CantFindFuncError(token, NextFuncName, {}, { ThisPar }, TypesEnum::Any);
					}
					else
					{
						auto nfunc = NextFunc.value().value();
						auto finfo = nfunc->Get_Info<FuncInfo>();

						bool ok = true;
						Optional<OptionalTypeInfo> typeinfo;
						if (!finfo->IsObjectCall())
						{
							ok = false;
							LogError(ErrorCodes::InValidName, "The Funcion '" + nfunc->FullName + "' must be a Object Call",token);
						}
						else
						{
							auto& Ret = finfo->Ret;

							auto optinfo = IsOptionalType(Ret);
							typeinfo = std::move(optinfo);
							if (!optinfo.has_value())
							{
								ok = false;
								LogError(ErrorCodes::InValidName, "The Funcion '" + nfunc->FullName + "' must return an OptionalType", token);
							}
						}

						if (ok)
						{	
							auto type = typeinfo.value().SomeType;

							ForExpresion_Data g;
							g.FuncGetLoopAble = HasInfo.Value.value();
							g.FuncNext = nfunc;
							_For_Datas.AddValue(Symbol_GetSymbolID(node), g);



							auto token = NeverNullptr(node._typeNode._name._ScopedName.back()._token);
							Type_DeclareVariableTypeCheck(syb->VarType,type, token);
							isvarableok = true;
						}
					}
				}
				else
				{
					UCodeLangUnreachable();
				}
			}

			if (!isvarableok)
			{
				syb->VarType = TypeSymbol(TypesEnum::Null);
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

				_LookingForTypes.push(syb->VarType);

				OnExpressionTypeNode(node._Traditional_Assignment_Expression._Value.get(), GetValueMode::Read);

				_LookingForTypes.pop();

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
				FileDependency_AddDependencyToCurrentFile(Data.FuncNext);
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
				auto BoolCode = _IR_LookingAtIRBlock->GetIndex();
				auto BoolJumps = IR_GetJumpsIndex();

				IRInstruction* opt = nullptr;
				IRInstruction* boolval = nullptr;
				TypeSymbol OptType;
				{//get opt 	
					Get_FuncInfo f;
					f.Func = Data.FuncNext->Get_Info<FuncInfo>();
					f.SymFunc = Data.FuncNext;
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

					OptType = _LastExpressionType;
					opt = _IR_LastExpressionField;

					boolval = IR_OptionalIsSomeType(OptType, opt);
				}

				IRInstruction* BoolCode2 = boolval;
				IRBlock::NewConditionalFalseJump_t IfIndex = _IR_LookingAtIRBlock->NewConditionalFalseJump(BoolCode2);

				{//get item
					_IR_LastExpressionField = Loopobject;
					
					syb->IR_Ins = IR_OptionalGetSomeType(OptType,opt,OptionalGetValueMode::Move);

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

#endif