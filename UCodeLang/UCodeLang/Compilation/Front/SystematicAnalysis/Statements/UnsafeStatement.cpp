#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnUnsafeStatement(const UnsafeStatementsNode& node)
{
	const String ScopeName = GetScopeLabelName(&node);
	_Table.AddScope(ScopeName);

	auto& block = _FuncStack.front().BlockContexts.front();
	auto oldblock = block.IsUnSafeBlock;

	block.IsUnSafeBlock = true;
	for (auto& Item : node._Base._Nodes)
	{
		OnStatement(*Item);
	}

	block.IsUnSafeBlock = oldblock;

	_Table.RemoveScope();
}
void SystematicAnalysis::OnUnsafeExpression(const UnsafeExpression& node)
{
	auto& block = _FuncStack.front().BlockContexts.front();
	auto oldblock = block.IsUnSafeBlock;
	block.IsUnSafeBlock = true;
	OnExpressionTypeNode(node._Base, _GetExpressionMode.top());
	block.IsUnSafeBlock = oldblock;

}

UCodeLangFrontEnd

#endif