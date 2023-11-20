#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnWhileNode(const WhileNode& node)
{
	TypeSymbol BoolType(TypesEnum::Bool);

	String ScopeName = std::to_string((size_t)&node);

	Push_NewStackFrame();
	_Table.AddScope(ScopeName);

	_LookingForTypes.push(BoolType);


	size_t BoolCode;
	size_t BoolJumps;
	if (_PassType == PassType::BuidCode)
	{
		BoolCode = _IR_LookingAtIRBlock->InsCount() ? _IR_LookingAtIRBlock->GetIndex() : 0;
		BoolJumps = IR_GetJumpsIndex();
	}
	OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);

	if (_PassType == PassType::FixedTypes)
	{
		if (!Type_CanBeImplicitConverted(_LastExpressionType, BoolType))
		{
			auto  Token = _LastLookedAtToken;
			LogError_CantCastImplicitTypes(Token.value(), _LastExpressionType, BoolType, true);
		}
	}


	IRBlock::NewConditionalFalseJump_t IfIndex;
	IRInstruction* BoolCode2;
	if (_PassType == PassType::BuidCode)
	{
		IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, BoolType);
		BoolCode2 = _IR_LastExpressionField;
		IfIndex = _IR_LookingAtIRBlock->NewConditionalFalseJump(BoolCode2);

	}




	_LookingForTypes.pop();

	for (const auto& node2 : node._Body._Nodes)
	{
		OnStatement(*node2);
	}


	if (_PassType == PassType::BuidCode)
	{
		_IR_LookingAtIRBlock->NewJump(BoolCode);

		size_t BreakCode = _IR_LookingAtIRBlock->GetIndex();

		_IR_LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, BreakCode);

		IR_Build_UpdateJumpsBreakContiunes(BoolJumps, BoolCode, BreakCode);
	}


	_Table.RemoveScope();

	Pop_StackFrame();
}

UCodeLangFrontEnd

#endif