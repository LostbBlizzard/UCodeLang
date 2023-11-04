#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnEnum(const EnumNode& node)
{
	const bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &node;
	const bool Isgeneric = node._generic._Values.size();
	const bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;



	const String ClassName = IsgenericInstantiation ? (String)ScopeHelper::GetNameFromFullName(_Generic_GenericSymbolStack.top()._IR_GenericFuncName) : (String)node._EnumName.token->Value._String;
	_Table.AddScope(ClassName);
	SymbolID SybID = Symbol_GetSymbolID(node);//Must be pass AddScope thats how GetSymbolID works.

	if (_PassType == PassType::GetTypes)
	{
		Symbol_RedefinitionCheck((String_view)_Table._Scope.ThisScope, SymbolType::Type_class, NeverNullptr(node._EnumName.token));
	}

	auto& Syb = _PassType == PassType::GetTypes ?
		Symbol_AddSymbol(Isgeneric_t ? SymbolType::Generic_Enum : SymbolType::Enum
			, (String)ClassName, _Table._Scope.ThisScope, node._Access) :
		*Symbol_GetSymbol(SybID);

	//we may jump to this node non linearly
	if (Syb.PassState == _PassType)
	{
		_Table.RemoveScope();
		return;
	}

	EnumInfo* ClassInf;
	if (_PassType == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);
		Syb.NodePtr = node.As();


		ClassInf = new EnumInfo();
		ClassInf->Context = Save_SymbolContextRemoveOneScopeName();
		ClassInf->FullName = Syb.FullName;
		Syb.Info.reset(ClassInf);
		Syb.VarType.SetType(Syb.ID);

		Generic_InitGenericalias(node._generic, IsgenericInstantiation, ClassInf->_GenericData);

	}
	else
	{
		ClassInf = (EnumInfo*)Syb.Get_Info<EnumInfo>();
	}
	//add  dependency cycle.
	EvaluatedEx ex;
	if (_PassType == PassType::FixedTypes)
	{
		Type_ConvertAndValidateType(node._BaseType, ClassInf->Basetype, NodeSyb_t::Any);
		if (ClassInf->Basetype.IsBadType() || Type_IsUnMapType(ClassInf->Basetype)) { _Table.RemoveScope(); return; }
		if (!Eavl_ConstantExpressionAbleType(ClassInf->Basetype))
		{
			LogError_TypeMustBeAnConstantExpressionAble(NeverNullptr(node._BaseType._name._ScopedName.back()._token), ClassInf->Basetype);
		}
		ex = std::move(Eval_MakeEx(ClassInf->Basetype));
	}


	bool HasCheckedForincrementOp = false;
	_LookingForTypes.push(ClassInf->Basetype);

	for (size_t i = 0; i < node._Values.size(); i++)
	{
		auto& Item = node._Values[i];

		String_view ItemName = Item._Name.token->Value._String;

		if (_PassType == PassType::GetTypes)
		{
			ClassInf->AddField(ItemName);

			if (Item._Expression._Value)
			{
				OnExpressionTypeNode(Item._Expression._Value.get(), GetValueMode::Read);
			}


			_Table._Scope.AddScope(ItemName);
			{
				auto& EnumFeild = Symbol_AddSymbol(SymbolType::Enum_Field, (String)ItemName, _Table._Scope.ThisScope, AccessModifierType::Public);
				EnumFeild.NodePtr = &Item;
			}
			_Table._Scope.ReMoveScope();


			if (Item._VariantType)
			{
				if (!ClassInf->VariantData.has_value())
				{
					ClassInf->VariantData = EnumVariantData();
				}
			}
		}
		else if (_PassType == PassType::FixedTypes)
		{
			if (ClassInf->VariantData)
			{

				EnumVariantData& EnumVa = ClassInf->VariantData.value();


				if (Item._VariantType)
				{
					auto& VariantType_ = Item._VariantType.value();
					if (VariantType_._node && VariantType_._node->Get_Type() == NodeType::AnonymousTypeNode)
					{
						EnumVariantFeild V;

						{
							//
							_Table._Scope.AddScope(ItemName);
							auto NewName = Str_GetFuncAnonymousObjectFullName(_Table._Scope.ThisScope);
							_Table._Scope.ReMoveScope();
							//

							SymbolID AnonymousSybID = Symbol_GetSymbolID(VariantType_._node.get());
							auto& AnonymousSyb = Symbol_AddSymbol(SymbolType::Type_class, (String)NewName, NewName, AccessModifierType::Public);
							AnonymousSyb.OutputIR = Syb.Type == SymbolType::Enum;//Dont Output IR type if Generic
							AnonymousSyb.PassState = PassType::FixedTypes;
							_Table.AddSymbolID(AnonymousSyb, AnonymousSybID);


							auto ClassInf = new ClassInfo();
							ClassInf->FullName = NewName;
							AnonymousSyb.Info.reset(ClassInf);
							AnonymousSyb.VarType.SetType(AnonymousSyb.ID);

							AnonymousTypeNode* Typenode = AnonymousTypeNode::As(VariantType_._node.get());

							size_t NewSize = 0;
							for (auto& Item3 : Typenode->_Fields._Parameters)
							{
								auto Fieldtype = Type_ConvertAndValidateType(Item3._Type, NodeSyb_t::Parameter);
								V.Types.push_back(Fieldtype);
								ClassInf->AddField(Item3._Name.AsString(), Fieldtype);

								NewSize += Type_GetSize(Fieldtype).value_or(0);
							}
							V.ClassSymbol = AnonymousSybID;

							ClassInf->SizeInitialized = true;
							ClassInf->Size = NewSize;
						}

						EnumVa.Variants.push_back(std::move(V));
					}
					else
					{
						EnumVariantFeild V;
						V.Types.push_back(Type_ConvertAndValidateType(VariantType_, NodeSyb_t::Parameter));

						EnumVa.Variants.push_back(std::move(V));
					}
				}
				else
				{
					EnumVariantFeild V;
					EnumVa.Variants.push_back(std::move(V));
				}

			}

			auto FieldOpt = ClassInf->GetField(ItemName);
			auto& Field = *FieldOpt.value();
			if (Item._Expression._Value)
			{
				auto& Type = ClassInf->Basetype;

				auto V = Eval_Evaluate(ex, Type, Item._Expression);

				Field.Ex = ex.EvaluatedObject;
			}
			else
			{
				if (i != 0)
				{
					auto& Type = ClassInf->Basetype;
					if (HasCheckedForincrementOp == false)
					{
						const NeverNullPtr<Token> LineDataToken = NeverNullptr(Item._Name.token);

						auto HasInfo = Type_HasPostfixOverLoadWith(Type, TokenType::increment);

						if (!HasInfo.HasValue)
						{

							Token temp;
							temp.OnLine = LineDataToken->OnLine;
							temp.OnPos = LineDataToken->OnPos;
							temp.Type = TokenType::increment;

							LogError_CantFindPostfixOpForTypes(&temp, Type);

							_Table.RemoveScope();
							return;
						}
						if (!Eval_HasConstantPostfixOperator(Type, TokenType::increment))
						{
							Token temp;
							temp.OnLine = LineDataToken->OnLine;
							temp.OnPos = LineDataToken->OnPos;
							temp.Type = TokenType::increment;


							LogError_Eval_CantDoPostfixOpForTypes(&temp, Type);

							_Table.RemoveScope();
							return;
						}
						HasCheckedForincrementOp = true;
					}

					Eval_EvaluatePostfixOperator(ex, TokenType::increment);
				}
				else
				{
					Eval_EvaluateDefaultConstructor(ex);
				}


				Field.Ex = ex.EvaluatedObject;
			}
		}

	}

	_LookingForTypes.pop();
	if (_PassType == PassType::BuidCode)
	{
		FileDependency_AddDependencyToCurrentFile(ClassInf->Basetype);
		if (Syb.Type == SymbolType::Enum) //Dont output type if Generic
		{
			Assembly_AddEnum(NeverNullptr(&Syb));
		}

		if (Isgeneric_t && IsgenericInstantiation == false)
		{
			String_view Text = _LookingAtFile->FileText;

			String ClassStr = "$";
			ClassStr += node._EnumName.token->Value._String;

			String_view ClassBody =
				String_view(&Text[node._EnumName.token->OnPos],
					node.EndOfClass->OnPos - node._EnumName.token->OnPos);

			GenericClass_Data& VClass = _Lib.Get_Assembly().AddGenericClass((String)ClassInf->Get_Name(), ClassInf->FullName);

			VClass.Base.Implementation = ClassStr + String(ClassBody);
			VClass.Base.Implementation += "\n\n";
			
		}
	}

	_Table.RemoveScope();
	Syb.PassState = _PassType;
}


UCodeLangFrontEnd