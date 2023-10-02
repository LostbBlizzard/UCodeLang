#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
#include "UCodeLang/Compliation/Helpers/ParseHelper.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnIfNode(const IfNode& node)
{

	TypeSymbol BoolType(TypesEnum::Bool);

	String ScopeName = std::to_string((size_t)&node);

	Push_NewStackFrame();
	_Table.AddScope(ScopeName);

	_LookingForTypes.push(BoolType);




	OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);


	if (_PassType == PassType::FixedTypes)
	{
		if (!Type_CanBeImplicitConverted(_LastExpressionType, BoolType))
		{
			auto  Token = _LastLookedAtToken;
			LogError_CantCastImplicitTypes(Token.value(), _LastExpressionType, BoolType, true);
		}
	}


	IRBlock::NewConditionalFalseJump_t IfIndex{};
	IRInstruction* BoolCode{};
	if (_PassType == PassType::BuidCode)
	{
		IR_Build_ImplicitConversion(_IR_LastExpressionField, _LastExpressionType, BoolType);
		BoolCode = _IR_LastExpressionField;
		IfIndex = _IR_LookingAtIRBlock->NewConditionalFalseJump(BoolCode);
		BoolCode = IfIndex.logicalNot;
	}




	_LookingForTypes.pop();

	for (const auto& node2 : node._Body._Nodes)
	{
		OnStatement(*node2);
	}

	Pop_StackFrame();
	_Table.RemoveScope();

	if (node._Else)
	{
		IRInstruction* ElseIndex{};
		size_t ElseI{};
		if (_PassType == PassType::BuidCode)
		{
			ElseIndex = _IR_LookingAtIRBlock->NewJump();
			ElseI = _IR_LookingAtIRBlock->GetIndex();
		}


		if (node._Else->Get_Type() != NodeType::ElseNode)
		{
			OnStatement(*node._Else);
		}
		else
		{
			ElseNode* Elsenode = ElseNode::As(node._Else.get());

			for (const auto& node3 : Elsenode->_Body._Nodes)
			{
				OnStatement(*node3);
			}
		}


		if (_PassType == PassType::BuidCode)
		{
			auto JumpIndex = _IR_LookingAtIRBlock->GetIndex() - 1;
			_IR_LookingAtIRBlock->UpdateJump(ElseIndex, JumpIndex);
			_IR_LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, IfIndex.logicalNot, ElseI);
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		_IR_LookingAtIRBlock->UpdateConditionaJump(IfIndex.ConditionalJump, BoolCode, _IR_LookingAtIRBlock->GetIndex());
	}


}

UCodeLangFrontEnd