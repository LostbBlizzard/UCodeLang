#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnDeferStatement(const DeferStatementNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		for (auto& Item : node._Base._Nodes)
		{
			if (Item->Get_Type() == NodeType::RetStatementNode)
			{
				RetStatementNode* node = RetStatementNode::As(Item.get());
				NeverNullPtr<Token> token = _LastLookedAtToken.value();

				this->LogError(ErrorCodes::ExpectingToken, "Cant use return statement in Defer Statement.", token);
			}
			else
			{
				OnStatement(*Item);
			}
		}
	}
	else if (_PassType == PassType::FixedTypes)
	{
		for (auto& Item : node._Base._Nodes)
		{
			OnStatement(*Item);
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		for (auto& Item : node._Base._Nodes)
		{
			OnStatement(*Item);
		}//only for tests shold be updated
	}
}

UCodeLangFrontEnd