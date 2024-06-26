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
			if (node._AliasType == AliasType::Type)
			{
				Syb.PassState = PassType::FixedTypes;
				Type_ConvertAndValidateType(node._Type, Syb.VarType, NodeSyb_t::Any);

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
				AliasNode_Func* node_ = (AliasNode_Func*)node._Node.get();
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
			if (node._AliasType == AliasType::Type)
			{
				auto& V = _Lib.Get_Assembly().AddAlias((String)ClassName, RemoveSymboolFuncOverloadMangling(_Table._Scope.ThisScope));
				V.Type = Assembly_ConvertToType(Syb.VarType);

				if (node._IsHardAlias)
				{
					V.HardAliasTypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);
				}
				V.AccessModifier = Syb.Access;
				V.IsExported = node.IsExport;

				FileDependency_AddDependencyToCurrentFile(Syb.VarType);
			}
			else
			{
				auto& V = _Lib.Get_Assembly().AddFuncPtr((String)ClassName, RemoveSymboolFuncOverloadMangling(_Table._Scope.ThisScope));
				const FuncPtrInfo* nodeinfo_ = Syb.Get_Info<FuncPtrInfo>();
				V.ParsType.resize(nodeinfo_->Pars.size());
				for (size_t i = 0; i < nodeinfo_->Pars.size(); i++)
				{
					V.ParsType[i].IsOutPar = nodeinfo_->Pars[i].IsOutPar;
					V.ParsType[i].Type = Assembly_ConvertToType(nodeinfo_->Pars[i].Type);
				}
				V.RetType = Assembly_ConvertToType(nodeinfo_->Ret);
				V.AccessModifier = Syb.Access;
				V.IsExported = node.IsExport;
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
			VClass.IsExported = node.IsExport;
			VClass.UseStatments = Generic_GetCurrentUseStatements();
		}
	}

	_Table.RemoveScope();

	Syb.PassState = _PassType;
}

UCodeLangFrontEnd

#endif