#ifndef UCodeLangNoCompiler

#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnExtenedFuncStatmentNode(const ExtendedFuncStatement& node)
{
	_GetExpressionMode.push(GetValueMode::Read);
	_LookingForTypes.push(TypesEnum::Any);

	OnExpressionNode(node._Base);

	_LookingForTypes.pop();
	_GetExpressionMode.pop();

	if (_PassType == PassType::BuidCode)
	{
		//TODO Deal with destructors here
	}
}
UCodeLangFrontEnd

#endif

