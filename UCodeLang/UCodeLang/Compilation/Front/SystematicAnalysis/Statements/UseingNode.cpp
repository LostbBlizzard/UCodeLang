#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnUseingNode(const UsingNode& node)
{
	auto& T = NeverNullptr(node._ScopedName._ScopedName.back()._token);
	const auto UseingString = Str_GetScopedNameAsString(node._ScopedName);
	_Table.AddUseing(UseingString);

	if (_PassType == PassType::FixedTypes)
	{
		if (!Symbol_GetSymbol(UseingString, SymbolType::Namespace))
		{
			//LogError_CantFindNamespace(T, UseingString);
			//TODO Readd this
		}
	}
}

UCodeLangFrontEnd

#endif