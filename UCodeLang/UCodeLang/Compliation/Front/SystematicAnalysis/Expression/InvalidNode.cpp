#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnInvalidNode(const InvalidNode& node)
{
	if (_PassType == PassType::GetTypes) { return; }

	if (node._StringExpression._Value)
	{
		auto StrData = Eval_GetStrEVal(node._StringExpression._Value.get());

		if (StrData.IsConstantExpression)
		{
			auto Token = NeverNullptr(node._KeyWord);
			LogError_LogInvalidNodeError(Token, StrData.Msg);
		}
		else
		{

			TypeSymbol B = TypesEnum::Char;
			B.SetAsAddressArray();

			LogError_Eval_CantCastImplicitTypes(_LastLookedAtToken.value(), _LastExpressionType, B);
		}
	}
	else
	{
		auto Token = NeverNullptr(node._KeyWord);
		LogError_EmptyInvalidError(Token);
	}
}


UCodeLangFrontEnd