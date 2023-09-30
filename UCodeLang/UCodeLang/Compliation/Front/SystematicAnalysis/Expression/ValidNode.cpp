#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnvalidNode(const ValidNode& node)
{
	if (_PassType == PassType::GetTypes)
	{

		if (node._IsExpression)
		{
			OnExpressionTypeNode(node._ExpressionToCheck._Value.get(), GetValueMode::Read);
		}
		else
		{
			for (auto& Item : node._StatementToCheck._Nodes)
			{
				OnStatement(*Item);
			}
		}
		_LastExpressionType = TypesEnum::Bool;
	}
	if (_PassType == PassType::FixedTypes)
	{
		bool IsValid = true;
		auto ErrCount = _ErrorsOutput->Get_Errors().size();

		if (node._IsExpression)
		{
			OnExpressionTypeNode(node._ExpressionToCheck._Value.get(), GetValueMode::Read);
		}
		else
		{
			for (auto& Item : node._StatementToCheck._Nodes)
			{
				OnStatement(*Item);
			}
		}

		if (ErrCount < _ErrorsOutput->Get_Errors().size())
		{
			size_t CountToPop = _ErrorsOutput->Get_Errors().size() - ErrCount;
			for (size_t i = 0; i < CountToPop; i++)
			{
				_ErrorsOutput->Get_Errors().pop_back();
			}
			IsValid = false;
		}


		_LastExpressionType = TypesEnum::Bool;

		_ValidNodes.AddValue(Symbol_GetSymbolID(node), IsValid);
	}
	if (_PassType == PassType::BuidCode)
	{
		bool IsValid = _ValidNodes.at(Symbol_GetSymbolID(node));


		_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(IsValid);
		_LastExpressionType = TypesEnum::Bool;
	}
}

UCodeLangFrontEnd