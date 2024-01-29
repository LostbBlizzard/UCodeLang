#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"


UCodeLangFrontStart

void SystematicAnalysis::OnForTypeNode(const ForTypeNode& node)
{
	const bool IsgenericInstantiation = _Generic_GenericSymbolStack.size() && _Generic_GenericSymbolStack.top().NodeTarget == &node;
	const bool Isgeneric = node._generic._Values.size();
	const bool Isgeneric_t = Isgeneric && IsgenericInstantiation == false;

	String ScopeName = ForTypeScope;
	if (IsgenericInstantiation)
	{
		ScopeName = _Generic_GenericSymbolStack.top()._IR_GenericFuncName;
	}

	_Table.AddScope(ScopeName);


	auto SymID = Symbol_GetSymbolID(node);

	_Table.RemoveScope();


	if (_PassType == PassType::GetTypes)
	{
		_Table.AddScope(ScopeName);

		auto& Sym = Symbol_AddSymbol(Isgeneric_t ? SymbolType::GenericForType : SymbolType::ForType, ForTypeScope, _Table._Scope.ThisScope, AccessModifierType::Default);
		_Table.AddSymbolID(Sym, SymID);
		Sym.NodePtr = &node;

		ForTypeInfo* info = new ForTypeInfo();
		Sym.Info.reset(info);

		info->Context = Save_SymbolContext();

		Sym.PassState = _PassType;

		Generic_InitGenericalias(node._generic, IsgenericInstantiation, info->_GenericData);

		_Table.RemoveScope();
	}
	else if (_PassType == PassType::FixedTypes)
	{
		if (node._generic._Values.size() == 0 || IsgenericInstantiation)
		{

			_Table.AddScope(ScopeName);

			auto type = Type_ConvertAndValidateType(node._typetoaddto, NodeSyb_t::Any);

			if (Type_IsPrimitive(type))
			{
				LogError(ErrorCodes::InValidType, "Cant add funcions to the primitive type '" + ToString(type) + "' ", NeverNullptr(node._typetoaddto._name._ScopedName.front()._token));
			}

			auto sym = Symbol_GetSymbol(SymID);
			sym->PassState = PassType::FixedTypes;
			sym->VarType = type;

			auto forinfo = sym->Get_Info<ForTypeInfo>();

			ClassStackInfo info;
			info.Syb = Symbol_GetSymbol(type).value_unchecked();

			_ClassStack.push(info);

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
	}
	else if (_PassType == PassType::BuidCode)
	{
		if (node._generic._Values.size() == 0 || IsgenericInstantiation)
		{

			_Table.AddScope(ScopeName);
			

			auto sym = Symbol_GetSymbol(SymID);
			sym->PassState = PassType::BuidCode;


			for (auto& Item : node._Nodes)
			{
				OnFuncNode(*Item);
			}


			_Table.RemoveScope();
		}
	}
}


UCodeLangFrontEnd
#endif
