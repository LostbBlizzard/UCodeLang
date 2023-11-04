#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
#include "UCodeLang/Compilation/Helpers/ParseHelper.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnCharliteralNode(const CharliteralNode* num)
{
	auto& lookT = Type_Get_LookingForType();

	if (_PassType == PassType::BuidCode)
	{
		String V;
		bool ItWorked = false;
		Debug_Add_SetLineNumber(NeverNullptr(num->token), _IR_LookingAtIRBlock->Instructions.size());

		if (lookT._Type == TypesEnum::Uft8)
		{
			String8 V;
			ItWorked = !ParseHelper::ParseCharliteralToChar(num->token->Value._String, V);
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Utf8)V.front());
		}
		else if (lookT._Type == TypesEnum::Uft16)
		{
			String16 V;
			ItWorked = !ParseHelper::ParseCharliteralToChar(num->token->Value._String, V);
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Utf16)V.front());
		}
		else if (lookT._Type == TypesEnum::Uft32)
		{
			String32 V;
			ItWorked = !ParseHelper::ParseCharliteralToChar(num->token->Value._String, V);
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((Utf32)V.front());
		}
		else
		{
			String V;
			ItWorked = !ParseHelper::ParseCharliteralToChar(num->token->Value._String, V);
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((char)V.front());
		}


	}
	if (lookT._Type == TypesEnum::Uft8 || lookT._Type == TypesEnum::Uft16
		|| lookT._Type == TypesEnum::Uft32)
	{
		_LastExpressionType = TypeSymbol(lookT._Type);
	}
	else
	{
		_LastExpressionType = TypeSymbol(TypesEnum::Char);
	}
	_LastLookedAtToken = Nullableptr(num->token);
}

UCodeLangFrontEnd