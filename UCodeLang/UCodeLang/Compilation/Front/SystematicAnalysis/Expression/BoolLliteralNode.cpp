#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
#include "UCodeLang/Compilation/Helpers/ParseHelper.hpp"
UCodeLangFrontStart
void SystematicAnalysis::OnBoolliteralNode(const BoolliteralNode* num)
{
	if (_PassType == PassType::BuidCode)
	{
		Debug_Add_SetLineNumber(NeverNullptr(num->token), _IR_LookingAtIRBlock->Instructions.size());
		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(num->Get_Value());
	}
	_LastExpressionType.SetType(TypesEnum::Bool);
	_LastLookedAtToken = Nullableptr(num->token);
}
UCodeLangFrontEnd

#endif