#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::Symbol_SetOutExpression(const OutExpression* Ex, const TypeSymbol& TypeToSet)
{
	auto Str = Ex->_Name.token->Value._String;

	String FullName = _Table._Scope.ThisScope;
	ScopeHelper::GetApendedString(FullName, Str);

	auto Syb = &Symbol_AddSymbol(SymbolType::StackVarable, (String)Str, FullName, AccessModifierType::Public);

	_LookingForTypes.push(TypeToSet);
	Syb->VarType = Type_ConvertAndValidateType(Ex->_Type, NodeSyb_t::Variable);
	_LookingForTypes.pop();

	Type_DeclareVariableTypeCheck(Syb->VarType, TypeToSet, NeverNullptr(Ex->_Name.token));
	_Table.AddSymbolID(*Syb, Symbol_GetSymbolID(*Ex));
}
NeverNullPtr<Symbol> SystematicAnalysis::Symbol_GetSymbolFromExpression(const NeverNullPtr<OutExpression> Ex)
{
	return Symbol_GetSymbol(Symbol_GetSymbolID(*Ex));
}
void SystematicAnalysis::Eval_SetOutExpressionEval(const OutExpression* Ex, const EvaluatedEx& ObjectToSet)
{

	auto Str = Ex->_Name.token->Value._String;

	String FullName = _Table._Scope.ThisScope;
	ScopeHelper::GetApendedString(FullName, Str);

	auto Syb = &Symbol_AddSymbol(SymbolType::ConstantExpression, (String)Str, FullName, AccessModifierType::Public);

	_LookingForTypes.push(ObjectToSet.Type);
	Syb->VarType = Type_ConvertAndValidateType(Ex->_Type, NodeSyb_t::Variable);
	_LookingForTypes.pop();

	Type_DeclareVariableTypeCheck(Syb->VarType, ObjectToSet.Type, NeverNullptr(Ex->_Name.token));
	_Table.AddSymbolID(*Syb, Symbol_GetSymbolID(*Ex));

	ConstantExpressionInfo* Info = new ConstantExpressionInfo();
	Syb->Info.reset(Info);

	Info->Ex = ObjectToSet.EvaluatedObject;
}

UCodeLangFrontEnd

#endif