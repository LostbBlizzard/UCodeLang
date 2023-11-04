#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnUnsafeStatement(const UnsafeStatementsNode& node)
{
	size_t ScopeCounter = 0;
	const String ScopeName = std::to_string((uintptr_t)&node);

	_Table.AddScope(ScopeName + std::to_string(ScopeCounter));

	auto& block = _FuncStack.front().BlockConexts.front();
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
	auto& block = _FuncStack.front().BlockConexts.front();
	auto oldblock = block.IsUnSafeBlock;
	block.IsUnSafeBlock = true;
	OnExpressionTypeNode(node._Base, _GetExpressionMode.top());
	block.IsUnSafeBlock = oldblock;

}

UCodeLangFrontEnd