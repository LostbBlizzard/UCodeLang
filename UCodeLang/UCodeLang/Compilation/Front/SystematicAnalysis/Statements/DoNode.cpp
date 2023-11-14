#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnDoNode(const DoNode& node)
{
	String ScopeName = std::to_string((size_t)&node);

	Push_NewStackFrame();
	_Table.AddScope(ScopeName);


	size_t StartIndex;
	size_t JumpIndex;
	if (_PassType == PassType::BuidCode)
	{
		StartIndex = _IR_LookingAtIRBlock->InsCount() ? _IR_LookingAtIRBlock->GetIndex() : 0;
		JumpIndex = IR_GetJumpsIndex();
	}


	for (const auto& node2 : node._Body._Nodes)
	{

		OnStatement(*node2);

	}

	_Table.RemoveScope();
	Pop_StackFrame();

	TypeSymbol BoolType(TypesEnum::Bool);
	_LookingForTypes.push(BoolType);

	size_t boolCode;
	if (_PassType == PassType::BuidCode)
	{
		boolCode = _IR_LookingAtIRBlock->InsCount() ? _IR_LookingAtIRBlock->GetIndex() : 0;
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


	if (_PassType == PassType::BuidCode)
	{
		IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, BoolType);

		size_t BreakCode = 0;

		_IR_LookingAtIRBlock->NewConditionalJump(_IR_LastExpressionField, StartIndex);


		IR_Build_UpdateJumpsBreakContiunes(JumpIndex, StartIndex, BreakCode);
	}




	_LookingForTypes.pop();
}

UCodeLangFrontEnd

#endif