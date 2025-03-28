#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnAliasNode(const AliasNode& node)
{
	const bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &node;
	const bool Isgeneric = node.Generic._Values.size();
	const bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;


	const String ClassName = IsgenericInstantiation ? (String)ScopeHelper::GetNameFromFullName(_Generic_GenericSymbolStack.top()._IR_GenericFuncName) : (String)node._AliasName.token->Value._String;


	_Table.AddScope(ClassName);
	auto SybID = Symbol_GetSymbolID(node);//Must be pass AddScope thats how GetSymbolID works.

	if (_PassType == PassType::GetTypes)
	{
		Symbol_RedefinitionCheck((String_view)_Table._Scope.ThisScope, node._IsHardAlias ? SymbolType::Hard_Type_alias : SymbolType::Type_alias, NeverNullptr(node._AliasName.token));
	}

	SymbolType _Type;
	if (Isgeneric_t)
	{
		_Type = SymbolType::Generic_Alias;
	}
	else
	{
		_Type = node._IsHardAlias ? SymbolType::Hard_Type_alias : SymbolType::Type_alias;
	}
	auto& Syb = _PassType == PassType::GetTypes ?
		Symbol_AddSymbol(_Type, (String)ClassName, _Table._Scope.ThisScope, node._Access) :
		*Symbol_GetSymbol(SybID);

	//we may jump to this node non linearly
	if (Syb.PassState == _PassType)
	{
		_Table.RemoveScope();
		return;
	}

	if (_PassType == PassType::GetTypes)
	{
		_Table.AddSymbolID(Syb, SybID);


		Syb.NodePtr = node.As();

		if (!Isgeneric_t)
		{
			if (IsgenericInstantiation)
			{
				auto Ptr = new Generic_AliasInfo();
				Syb.Info.reset(Ptr);
				Generic_InitGenericalias(node.Generic, true, Ptr->_GenericData);
				Ptr->Context = Save_SymbolContextRemoveOneScopeName();
			}
			else if (node._AliasType == AliasType::Type)
			{
				auto V = new AliasInfo();
				Syb.Info.reset(V);

				Type_ConvertAndValidateType(node._Type, Syb.VarType, NodeSyb_t::Any);

				V->Context = Save_SymbolContextRemoveOneScopeName();
			}
			else if (node._AliasType == AliasType::Match)
			{
				const MatchExpression* ex = MatchExpression::As(node._Node.get());

				_LookingForTypes.push(TypesEnum::Var);
				_GetExpressionMode.push(GetValueMode::Read);
				
				OnMatchExpression(*ex);

				_GetExpressionMode.pop();
				_LookingForTypes.pop();
			}
			else
			{

				AliasNode_Func* node_ = (AliasNode_Func*)node._Node.get();
				auto V = new FuncPtrInfo();
				Syb.Info.reset(V);

				V->Pars.resize(node_->_Parameters._Parameters.size());

				for (size_t i = 0; i < V->Pars.size(); i++)
				{
					auto& NodePar = node_->_Parameters._Parameters[i];
					auto& Par = V->Pars[i];
					Par.IsOutPar = NodePar._IsOutVarable;
					Type_ConvertAndValidateType(NodePar._Type, Par.Type, NodeSyb_t::Parameter);
				}

				Type_ConvertAndValidateType(node_->_ReturnType, V->Ret, NodeSyb_t::Ret);

				Syb.VarType.SetType(SybID);

				Syb.Type = SymbolType::Func_ptr;

				V->Context = Save_SymbolContextRemoveOneScopeName();
			}
		}
		else
		{
			auto Ptr = new Generic_AliasInfo();
			Syb.Info.reset(Ptr);
			Ptr->Context = Save_SymbolContextRemoveOneScopeName();
			Generic_InitGenericalias(node.Generic, false, Ptr->_GenericData);
		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		if (!Isgeneric_t)
		{
			if (node._AliasType == AliasType::Type || node._AliasType == AliasType::Match)
			{
				Syb.PassState = PassType::FixedTypes;
			
				if (node._AliasType == AliasType::Type) 
				{
					Type_ConvertAndValidateType(node._Type, Syb.VarType, NodeSyb_t::Any);
				}
				else
				{
					const MatchExpression* ex = MatchExpression::As(node._Node.get());

					_LookingForTypes.push(TypesEnum::Var);
					_GetExpressionMode.push(GetValueMode::Read);
					_NodeTypeStack.push_back(NodeType::InturnalEvalTypeCheck);

					OnMatchExpression(*ex);

					_NodeTypeStack.pop_back();
					_GetExpressionMode.pop();
					_LookingForTypes.pop();

					auto outputtype = _LastExpressionType;
					if (!outputtype.IsTypeInfo() && !outputtype.IsNull())
					{
						LogError(ErrorCodes::InValidType, "Match must return 'typeinfo' type", NeverNullptr(ex->_Token));
					}

					if (outputtype.IsTypeInfo()) 
					{
						EvaluatedEx evalout = Eval_MakeEx(outputtype);
						bool itworked = Eval_Evaluate(evalout, *ex, false);

						if (itworked)
						{
							const TypeSymbol* EvalTypeSymbol = Eval_Get_ObjectAs<TypeSymbol>(evalout);

							TypeSymbol val = *EvalTypeSymbol;
							val._TypeInfo = TypeInfoPrimitive::Null;

							Syb.VarType = val;
						}
						else 
						{
							Syb.VarType = TypesEnum::Null;
						}
					}

				}

				bool ispublic = node._Access == AccessModifierType::Public;
				if (node.IsExport && ispublic)
				{
					if (!Type_IsTypeExported(Syb.VarType))
					{
						LogError_TypeIsNotExport(NeverNullptr(node._AliasName.token), Syb.VarType, NeverNullptr(&Syb));
					}
				}
			}
			else
			{
				const AliasNode_Func* node_ = (AliasNode_Func*)node._Node.get();
				FuncPtrInfo* nodeinfo_ = (FuncPtrInfo*)Syb.Info.get();

				auto& GenericList = node.Generic;
				Generic_GenericAliasFixTypes(GenericList, IsgenericInstantiation, nodeinfo_->_GenericData);

				bool ispublic = node._Access == AccessModifierType::Public;
				for (size_t i = 0; i < nodeinfo_->Pars.size(); i++)
				{
					auto& NodePar = node_->_Parameters._Parameters[i];
					auto& Par = nodeinfo_->Pars[i];
					Par.IsOutPar = NodePar._IsOutVarable;
					Type_ConvertAndValidateType(NodePar._Type, Par.Type, NodeSyb_t::Parameter);

					if (node.IsExport && ispublic)
					{
						if (!Type_IsTypeExported(Par.Type))
						{
							LogError_TypeIsNotExport(NeverNullptr(node._AliasName.token), Par.Type, NeverNullptr(&Syb));
						}
					}
				}

				Type_ConvertAndValidateType(node_->_ReturnType, nodeinfo_->Ret, NodeSyb_t::Ret);
				
				if (node.IsExport && ispublic)
				{
					if (!Type_IsTypeExported(nodeinfo_->Ret))
					{
						LogError_TypeIsNotExport(NeverNullptr(node._AliasName.token), nodeinfo_->Ret, NeverNullptr(&Syb));
					}
				}
			}
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		if (!Isgeneric_t)
		{
			if (node._AliasType == AliasType::Type || node._AliasType == AliasType::Match)
			{
				auto& V = _Lib.Get_Assembly().AddAlias((String)ClassName, RemoveSymboolFuncOverloadMangling(_Table._Scope.ThisScope));
				V.Type = Assembly_ConvertToType(Syb.VarType);

				if (node._IsHardAlias)
				{
					V.HardAliasTypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);
				}
				V.AccessModifier = Syb.Access;
				V.IsExported = node.IsExport ? ExportType::Exported : ExportType::NotExported;

				FileDependency_AddDependencyToCurrentFile(Syb.VarType);
			}
			else
			{
				auto& V = _Lib.Get_Assembly().AddFuncPtr((String)ClassName, RemoveSymboolFuncOverloadMangling(_Table._Scope.ThisScope));
				const FuncPtrInfo* nodeinfo_ = Syb.Get_Info<FuncPtrInfo>();
				V.ParsType.reserve(nodeinfo_->Pars.size());

				for (size_t i = 0; i < nodeinfo_->Pars.size(); i++)
				{
					auto& nodeinfopar = nodeinfo_->Pars[i];

					auto Symop = Symbol_GetSymbol(nodeinfopar.Type);
					if (Symop.has_value())
					{
						auto sym = Symop.value();

						if (sym->Type == SymbolType::Type_Pack)
						{
							auto parpack = sym->Get_Info<TypePackInfo>();

							for (auto& Iner : parpack->List)
							{
								ClassMethod::Par tep;
								tep.Type = Assembly_ConvertToType(Iner);
								tep.IsOutPar = false;
								V.ParsType.push_back(tep);
							}
							break;
						}
					}

					ClassMethod::Par tep;
					tep.Type = Assembly_ConvertToType(nodeinfopar.Type);
					tep.IsOutPar = nodeinfopar.IsOutPar;
					V.ParsType.push_back(tep);
				}
				V.RetType = Assembly_ConvertToType(nodeinfo_->Ret);
				V.AccessModifier = Syb.Access;

				bool indirectexport = nodeinfo_->_IsIndirectExport;
				V.IsExported = node.IsExport ? ExportType::Exported : ExportType::NotExported;

				if (V.IsExported == ExportType::NotExported && indirectexport) 
				{
					V.IsExported =  ExportType::IndrectExported;
				}

				V.TypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);
			}
		}

		if (Isgeneric_t && IsgenericInstantiation == false)
		{
			String_view Text = _LookingAtFile->FileText;

			String ClassStr = "$";
			ClassStr += node._AliasName.token->Value._String;

			String_view ClassBody =
				String_view(&Text[node._AliasName.token->OnPos],
					node.EndOfClass->OnPos - node._AliasName.token->OnPos + 1);

			auto newfullname = RemoveSymboolFuncOverloadMangling(Syb.FullName);
			GenericClass_Data& VClass = _Lib.Get_Assembly().AddGenericClass(
				(String)ScopeHelper::GetNameFromFullName(newfullname), newfullname);

			VClass.Base.Implementation = ClassStr + String(ClassBody);
			VClass.Base.Implementation += "\n\n";
			VClass.AccessModifier = Syb.Access;
			VClass.IsExported = node.IsExport ? ExportType::Exported : ExportType::NotExported;
			VClass.UseStatments = Generic_GetCurrentUseStatements();
		}
	}

	_Table.RemoveScope();

	Syb.PassState = _PassType;
}

UCodeLangFrontEnd

#endif
