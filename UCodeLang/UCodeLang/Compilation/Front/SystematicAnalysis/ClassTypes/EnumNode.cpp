#ifndef UCodeLangNoCompiler
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

	EnumInfo* enumInf;
	if (_PassType == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);
		Syb.NodePtr = node.As();


		enumInf = new EnumInfo();
		enumInf->Context = Save_SymbolContextRemoveOneScopeName();
		enumInf->FullName = Syb.FullName;
		Syb.Info.reset(enumInf);
		Syb.VarType.SetType(Syb.ID);

		Generic_InitGenericalias(node._generic, IsgenericInstantiation, enumInf->_GenericData);

	}
	else
	{
		enumInf = (EnumInfo*)Syb.Get_Info<EnumInfo>();
	}
	//add  dependency cycle.
	EvaluatedEx ex;
	if (_PassType == PassType::FixedTypes)
	{
		if (Isgeneric_t) 
		{
			auto& GenericList = node._generic;
			Generic_GenericAliasFixTypes(GenericList, IsgenericInstantiation, enumInf->_GenericData);
		}

		Type_ConvertAndValidateType(node._BaseType, enumInf->Basetype, NodeSyb_t::Any);
		if (enumInf->Basetype.IsBadType() || Type_IsUnMapType(enumInf->Basetype)) { _Table.RemoveScope(); return; }
		if (!Eavl_ConstantExpressionAbleType(enumInf->Basetype))
		{
			LogError_TypeMustBeAnConstantExpressionAble(NeverNullptr(node._BaseType._name._ScopedName.back()._token), enumInf->Basetype);
		}
		ex = Eval_MakeEx(enumInf->Basetype);
	}


	bool HasCheckedForincrementOp = false;
	_LookingForTypes.push(enumInf->Basetype);

	Syb.PassState = _PassType;
	for (size_t i = 0; i < node._Values.size(); i++)
	{
		auto& Item = node._Values[i];

		String_view ItemName = Item._Name.token->Value._String;

		if (_PassType == PassType::GetTypes)
		{
			enumInf->AddField(ItemName);

			if (Item._Expression._Value)
			{
				OnExpressionTypeNode(Item._Expression._Value.get(), GetValueMode::Read);
			}


			_Table._Scope.AddScope(ItemName);
			{
				auto& EnumField = Symbol_AddSymbol(SymbolType::Enum_Field, (String)ItemName, _Table._Scope.ThisScope, AccessModifierType::Public);
				EnumField.NodePtr = &Item;
			}
			_Table._Scope.ReMoveScope();


			if (Item._VariantType)
			{
				if (!enumInf->VariantData.has_value())
				{
					enumInf->VariantData = EnumVariantData();
				}
			}
		}
		else if (_PassType == PassType::FixedTypes)
		{
			if (enumInf->VariantData)
			{

				EnumVariantData& EnumVa = enumInf->VariantData.value();


				if (Item._VariantType)
				{
					auto& VariantType_ = Item._VariantType.value();
					if (VariantType_._node && VariantType_._node->Get_Type() == NodeType::AnonymousTypeNode)
					{
						EnumVariantField V;

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

								if (Symbol_HasDestructor(Fieldtype)) {
									enumInf->HasDestructer = true;
								}
							}
							V.ClassSymbol = AnonymousSybID;

							ClassInf->SizeInitialized = true;
							ClassInf->Size = NewSize;


						}

						EnumVa.Variants.push_back(std::move(V));
					}
					else
					{
						EnumVariantField V;
						V.Types.push_back(Type_ConvertAndValidateType(VariantType_, NodeSyb_t::Parameter));

						if (Symbol_HasDestructor(V.Types.front())) {
							enumInf->HasDestructer = true;
						}

						EnumVa.Variants.push_back(std::move(V));
					}
				}
				else
				{
					EnumVariantField V;
					EnumVa.Variants.push_back(std::move(V));
				}

			}

			auto FieldOpt = enumInf->GetField(ItemName);
			auto& Field = *FieldOpt.value();
			if (Item._Expression._Value)
			{
				auto& Type = enumInf->Basetype;

				auto V = Eval_Evaluate(ex, Type, Item._Expression);

				Field.Ex = ex.EvaluatedObject;
			}
			else
			{
				if (i != 0)
				{
					auto& Type = enumInf->Basetype;
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


			bool ispublic = node._Access == AccessModifierType::Public;
			if (node._IsExport && ispublic)
			{
				if (!Type_IsTypeExported(enumInf->Basetype))
				{
					LogError_TypeIsNotExport(NeverNullptr(node._EnumName.token), enumInf->Basetype, NeverNullptr(&Syb));
				}

				if (enumInf->VariantData.has_value())
				{
					for (size_t i = 0; i < enumInf->VariantData.value().Variants.size(); i++)
					{
						auto& Item = enumInf->VariantData.value().Variants[i];
				

						for (auto& Item2 : Item.Types)
						{
							if (!Type_IsTypeExported(Item2))
							{
								LogError_TypeIsNotExport(NeverNullptr(node._Values[i]._Name.token), Item2,NeverNullptr(&Syb));
							}
						}
					}
				}
			}

			if (!ispublic && node._IsExport)
			{
				LogError_ExportIsPrivate(NeverNullptr(node._EnumName.token),NeverNullptr(&Syb));
			}
		}
	

		if (enumInf->HasDestructer)
		{

			auto DropFunc = &Symbol_AddSymbol(SymbolType::Func, ClassDestructorFunc, _Table._Scope.GetApendedString((String)ClassDestructorFunc), AccessModifierType::Public);
			FuncInfo* V = new FuncInfo();
			DropFunc->Info.reset(V);

			V->FullName = DropFunc->FullName;
			V->_FuncType = FuncInfo::FuncType::Drop;
			V->Ret = TypesEnum::Void;
			V->FrontParIsUnNamed = true;

			auto ThisParType =TypeSymbol(Syb.ID);
			ThisParType._IsAddress = true;

			ParInfo parinfo;
			parinfo.IsOutPar = false;
			parinfo.Type = ThisParType;
			V->Pars.push_back(parinfo);

			DropFunc->PassState = PassType::BuidCode;

			enumInf->FuncDestructer = V;
		}
	}

	_LookingForTypes.pop();
	if (_PassType == PassType::BuidCode)
	{
		FileDependency_AddDependencyToCurrentFile(enumInf->Basetype);
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

			GenericClass_Data& VClass = _Lib.Get_Assembly().AddGenericClass((String)enumInf->Get_Name(), enumInf->FullName);

			VClass.Base.Implementation = ClassStr + String(ClassBody);
			VClass.Base.Implementation += "\n\n";
			VClass.IsExported = node._IsExport;
			VClass.AccessModifier = node._Access;
			VClass.UseStatments = Generic_GetCurrentUseStatements();
		}

		bool hasdestructer = false;


		if (enumInf->HasDestructer)
		{
			String destructerfuncname = enumInf->FullName;
			ScopeHelper::GetApendedString(destructerfuncname, ClassDestructorFunc);

			auto oldfunc = _IR_LookingAtIRFunc;
			auto oldblock = _IR_LookingAtIRBlock;

			auto DropFunc = Symbol_GetSymbol(ClassDestructorFunc, SymbolType::Func).value();
			_IR_LookingAtIRFunc = _IR_Builder.NewFunc(IR_GetIRID(DropFunc->Get_Info<FuncInfo>()),IRTypes::Void);
			
			{
				IRPar V;
				V.identifier = _IR_Builder.ToID(ThisSymbolName);
				V.type = IR_ConvertToIRType(Syb.VarType);
				V.type._Type = IRTypes::pointer;

				_IR_LookingAtIRFunc->Pars.push_back(V);
			}
			IRPar* ThisPar = &_IR_LookingAtIRFunc->Pars.front();
			_IR_LookingAtIRBlock = _IR_LookingAtIRFunc->NewBlock(".");

			if (enumInf->VariantData.has_value()) 
			{

				auto UnionName = Str_GetEnumVariantUnionName(Syb.FullName);
				auto UnionIRSym = _IR_Builder.GetSymbol(_IR_Builder.ToID(UnionName));
				auto UnionIRSymStruct = UnionIRSym->Get_ExAs<IRStruct>();

				size_t UnionMemberIndex = 0;
				for (size_t i = 0; i < enumInf->Fields.size(); i++)
				{
					auto& Item = enumInf->Fields[i];
					auto& V = enumInf->VariantData.value().Variants[i];
					
					if (V.Types.size() == 0) { continue; }

					bool isstruct = V.ClassSymbol.has_value();

					if (isstruct)
					{
						bool HasDestructInlist = false;
						for (auto& type : V.Types)
						{
							if (Symbol_HasDestructor(type))
							{
								HasDestructInlist = true;
								break;
							}
						}

						if (HasDestructInlist)
						{
							auto key = _IR_LookingAtIRBlock->New_Member_Dereference(ThisPar, ThisPar->type, EnumVarantKeyIndex);
							auto unionir = _IR_LookingAtIRBlock->New_Member_Dereference(ThisPar, ThisPar->type, EnumVarantUnionIndex);

							auto check = _IR_LookingAtIRBlock->NewC_NotEqualto(key, LoadEvaluatedEx(Item.Ex, enumInf->Basetype));
							auto jump = _IR_LookingAtIRBlock->NewConditionalJump(check);

							auto Parstruct =_IR_LookingAtIRBlock->New_Member_Access(unionir, UnionIRSymStruct, UnionMemberIndex);

							for (size_t i = 0; i < V.Types.size(); i++)
							{
								auto& type = V.Types[i];

								if (Symbol_HasDestructor(type))
								{
									auto Par = _IR_LookingAtIRBlock->NewLoadPtr(_IR_LookingAtIRBlock->New_Member_Access(Parstruct, UnionIRSymStruct, i));

									ObjectToDrop tep;
									tep.DropType = ObjectToDropType::IRInstructionNoMod;
									tep.Type = type;
									tep._Object = Par;

									IR_Build_DestructorCall(tep);


								}
							}
							_IR_LookingAtIRBlock->UpdateConditionaJump(jump, check, _IR_LookingAtIRBlock->GetIndex());

							_IR_LookingAtIRBlock->NewRet();

						}
					}
					else
					{
						auto& type = V.Types.front();
						if (Symbol_HasDestructor(type))
						{

							auto key = _IR_LookingAtIRBlock->New_Member_Dereference(ThisPar, ThisPar->type, EnumVarantKeyIndex);
							auto unionir = _IR_LookingAtIRBlock->New_Member_Dereference(ThisPar, ThisPar->type, EnumVarantUnionIndex);

							auto check = _IR_LookingAtIRBlock->NewC_NotEqualto(key, LoadEvaluatedEx(Item.Ex, enumInf->Basetype));

							auto jump = _IR_LookingAtIRBlock->NewConditionalJump(check);


							auto Par = _IR_LookingAtIRBlock->NewLoadPtr(_IR_LookingAtIRBlock->New_Member_Access(unionir, UnionIRSymStruct, UnionMemberIndex));
							
							ObjectToDrop tep;
							tep.DropType = ObjectToDropType::IRInstructionNoMod;
							tep.Type = type;
							tep._Object = Par;

							IR_Build_DestructorCall(tep);

							_IR_LookingAtIRBlock->UpdateConditionaJump(jump, check, _IR_LookingAtIRBlock->GetIndex());

							_IR_LookingAtIRBlock->NewRet();
						}
					}
					UnionMemberIndex++;
				}
			}


			_IR_LookingAtIRFunc = oldfunc;
			_IR_LookingAtIRBlock = oldblock;
		}
	}

	_Table.RemoveScope();
}


UCodeLangFrontEnd

#endif