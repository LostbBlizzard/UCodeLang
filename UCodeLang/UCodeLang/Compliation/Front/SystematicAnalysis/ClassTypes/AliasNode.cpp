#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
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
				Ptr->Conext = Save_SymbolContextRemoveOneScopeName();
			}
			else
				if (node._AliasType == AliasType::Type)
				{
					auto V = new AliasInfo();
					Syb.Info.reset(V);

					Type_ConvertAndValidateType(node._Type, Syb.VarType, NodeSyb_t::Any);

					V->Conext = Save_SymbolContextRemoveOneScopeName();
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

					V->Conext = Save_SymbolContextRemoveOneScopeName();
				}
		}
		else
		{
			auto Ptr = new Generic_AliasInfo();
			Syb.Info.reset(Ptr);
			Generic_InitGenericalias(node.Generic, false, Ptr->_GenericData);
		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		if (!Isgeneric_t)
		{
			if (node._AliasType == AliasType::Type)
			{
				Type_ConvertAndValidateType(node._Type, Syb.VarType, NodeSyb_t::Any);
			}
			else
			{
				AliasNode_Func* node_ = (AliasNode_Func*)node._Node.get();
				FuncPtrInfo* nodeinfo_ = (FuncPtrInfo*)Syb.Info.get();

				for (size_t i = 0; i < nodeinfo_->Pars.size(); i++)
				{
					auto& NodePar = node_->_Parameters._Parameters[i];
					auto& Par = nodeinfo_->Pars[i];
					Par.IsOutPar = NodePar._IsOutVarable;
					Type_ConvertAndValidateType(NodePar._Type, Par.Type, NodeSyb_t::Parameter);
				}

				Type_ConvertAndValidateType(node_->_ReturnType, nodeinfo_->Ret, NodeSyb_t::Ret);
			}
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		if (!Isgeneric_t)
		{
			if (node._AliasType == AliasType::Type)
			{
				auto& V = _Lib.Get_Assembly().AddAlias((String)ClassName, _Table._Scope.ThisScope);
				V.Type = Assembly_ConvertToType(Syb.VarType);

				if (node._IsHardAlias)
				{
					V.HardAliasTypeID = Type_GetTypeID(TypesEnum::CustomType, Syb.ID);
				}
				FileDependency_AddDependencyToCurrentFile(Syb.VarType);
			}
			else
			{
				auto& V = _Lib.Get_Assembly().AddFuncPtr((String)ClassName, _Table._Scope.ThisScope);
				const FuncPtrInfo* nodeinfo_ = Syb.Get_Info<FuncPtrInfo>();
				V.ParsType.resize(nodeinfo_->Pars.size());
				for (size_t i = 0; i < nodeinfo_->Pars.size(); i++)
				{
					//TODO should use a funcion like Assembly_ConvertToParType.
					V.ParsType[i].IsOutPar = nodeinfo_->Pars[i].IsOutPar;
					V.ParsType[i].Type = Assembly_ConvertToType(nodeinfo_->Pars[i].Type);
				}
				V.RetType = Assembly_ConvertToType(nodeinfo_->Ret);
			}
		}
	}

	_Table.RemoveScope();

	Syb.PassState = _PassType;
}

UCodeLangFrontEnd