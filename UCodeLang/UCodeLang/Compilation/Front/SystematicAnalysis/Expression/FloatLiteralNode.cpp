#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
#include "UCodeLang/Compilation/Helpers/ParseHelper.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnFloatLiteralNode(const FloatliteralNode* num)
{
	auto& lookT = Type_Get_LookingForType();
	if (_PassType == PassType::BuidCode)
	{

		switch (lookT._Type)
		{
		case TypesEnum::float32:
		{
			float32 V;
			bool ItWorked = ParseHelper::ParseStringTofloat32(num->token->Value._String, V);

			Debug_Add_SetLineNumber(NeverNullptr(num->token), _IR_LookingAtIRBlock->Instructions.size());
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(V);
			break;
		}
		case TypesEnum::float64:
		{
			float64 V;
			bool ItWorked = ParseHelper::ParseStringTofloat64(num->token->Value._String, V);

			Debug_Add_SetLineNumber(NeverNullptr(num->token), _IR_LookingAtIRBlock->Instructions.size());
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(V);
			break;
		}
		default:
			UCodeLangUnreachable();
			break;
		}

	}

	TypesEnum NewEx;
	if (lookT._Type == TypesEnum::Var)
	{
		NewEx = TypesEnum::float32;
	}
	else
	{
		NewEx = (Type_IsfloatType(lookT)) ? lookT._Type : TypesEnum::float32;
	}


	_LastExpressionType.SetType(NewEx);
	_LastLookedAtToken = Nullableptr(num->token);
}

UCodeLangFrontEnd

#endif