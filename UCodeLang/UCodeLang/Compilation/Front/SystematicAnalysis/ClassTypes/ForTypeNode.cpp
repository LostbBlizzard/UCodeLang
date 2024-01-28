#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"


UCodeLangFrontStart

void SystematicAnalysis::OnForTypeNode(const ForTypeNode& node)
{
	auto SymID = Symbol_GetSymbolID(node);
	if (_PassType == PassType::GetTypes)
	{
		_Table.AddScope(ForTypeScope);

		auto& Sym = Symbol_AddSymbol(SymbolType::ForType, ForTypeScope, _Table._Scope.ThisScope, AccessModifierType::Default);
		_Table.AddSymbolID(Sym, SymID);
		Sym.NodePtr = &node;

		ForTypeInfo* info = new ForTypeInfo();
		Sym.Info.reset(info);

		info->Context = Save_SymbolContext();

		Sym.PassState = _PassType;

		_Table.RemoveScope();
	}
	else if (_PassType == PassType::FixedTypes)
	{	
	
		auto type = Type_ConvertAndValidateType(node._typetoaddto, NodeSyb_t::Any);
	
		if (Type_IsPrimitive(type))
		{
			LogError(ErrorCodes::InValidType, "Cant add funcions to the primitive type '" + ToString(type) + "' ",NeverNullptr(node._typetoaddto._name._ScopedName.front()._token));
		}

		auto sym = Symbol_GetSymbol(SymID);
		sym->PassState = PassType::FixedTypes;
		sym->VarType = type;

		auto forinfo = sym->Get_Info<ForTypeInfo>();

		ClassStackInfo info;
		info.Syb = Symbol_GetSymbol(type).value_unchecked();

		_ClassStack.push(info);

		_Table.AddScope(ForTypeScope);

		_PassType = PassType::GetTypes;
		for (auto& Item : node._Nodes)
		{
			size_t index = _Table.Symbols.size();

			OnFuncNode(*Item);

			auto& newSym = _Table.Symbols[index];
	
			forinfo->Funcs.push_back(newSym.get());

		}
				
		_PassType = PassType::FixedTypes;
		for (auto& Item : node._Nodes)
		{
			OnFuncNode(*Item);
		}

		_Table.RemoveScope();
		_ClassStack.pop();
	}
	else if (_PassType ==PassType::BuidCode)
	{

		auto sym = Symbol_GetSymbol(SymID);
		sym->PassState = PassType::BuidCode;


		_Table.AddScope(ForTypeScope);

		for (auto& Item : node._Nodes)
		{
			OnFuncNode(*Item);
		}


		_Table.RemoveScope();
	}
}


UCodeLangFrontEnd
#endif
