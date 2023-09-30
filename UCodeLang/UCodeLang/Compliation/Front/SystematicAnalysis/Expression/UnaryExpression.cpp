#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnExpressionNode(const UnaryExpressionNode& node)
{
	if (_PassType == PassType::GetTypes)
	{
		OnExpressionTypeNode(node._Value0, GetValueMode::Read);
	}
	else if (_PassType == PassType::FixedTypes)
	{
		OnExpressionTypeNode(node._Value0, GetValueMode::Read);

		auto extype = _LastExpressionType;

		auto BinaryOp = NeverNullptr(node._UnaryOp);
		auto Info = Type_HasUrinaryOverLoadWith(extype, node._UnaryOp->Type);

		if (!Info.HasValue)
		{
			LogError_CantFindUnaryOpForTypes(BinaryOp, extype);
		}
		else
		{
			SymbolID SymID = Symbol_GetSymbolID(node);

			UnaryExpression_Data Data;
			Data.FuncToCall = nullptr;
			if (Info.Value.has_value())
			{
				Data.FuncToCall = Info.Value.value();
			}
			Data.Op0 = extype;

			_UnaryDatas.AddValue(SymID, std::move(Data));
		}

	}
	else if (_PassType == PassType::BuidCode)
	{
		SymbolID SymID = Symbol_GetSymbolID(node);
		auto& Data = _UnaryDatas.at(SymID);


		OnExpressionTypeNode(node._Value0, GetValueMode::Read);
		auto ex = _IR_LastExpressionField;

		switch (node._UnaryOp->Type)
		{
		case TokenType::Not:
		{
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewlogicalNot(ex);
		}
		case TokenType::bitwise_not:
		{
			UCodeLangUnreachable();
		}
		case TokenType::QuestionMark:
		{
			UCodeLangUnreachable();
		}
		case TokenType::plus:
		{
			UCodeLangUnreachable();
		}
		case TokenType::minus:
		{
			UCodeLangUnreachable();
		}
		default:
			UCodeLangUnreachable();
			break;
		}
	}
}


UCodeLangFrontEnd