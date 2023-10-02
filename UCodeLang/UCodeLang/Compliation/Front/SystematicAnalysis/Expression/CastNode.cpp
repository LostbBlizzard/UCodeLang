#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnExpressionNode(const CastNode& node)
{
	TypeSymbol ToTypeAs;
	Type_ConvertAndValidateType(node._ToType, ToTypeAs, NodeSyb_t::Any);
	_LookingForTypes.push(ToTypeAs);


	if (_PassType == PassType::BuidCode)
	{
		auto& Item = _CastDatas.GetValue(Symbol_GetSymbolID(node));
		if (Item.FuncToCall)
		{
			_LookingForTypes.push(Item.FuncToCall->Get_Info<FuncInfo>()->Pars[0].Type);

			OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
			auto Ex0 = _IR_LastExpressionField;
			auto Ex0Type = _LastExpressionType;

			_LookingForTypes.pop();
		}
		else
		{
			OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
			auto Ex0 = _IR_LastExpressionField;
			auto Ex0Type = _LastExpressionType;
		}
	}
	else
	{
		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);
		auto Ex0 = _IR_LastExpressionField;
		auto Ex0Type = _LastExpressionType;
	}


	if (_PassType == PassType::FixedTypes)
	{
		if (!ToTypeAs.IsNull())
		{
			CastExpressionNode_Data data;


			TypeSymbol ToTypeAs;
			Type_ConvertAndValidateType(node._ToType, ToTypeAs, NodeSyb_t::Any);

			auto Ex0Type = _LastExpressionType;
			auto HasInfo = Type_CanBeExplicitlyConverted(Ex0Type, ToTypeAs);
			if (!HasInfo.HasValue)
			{
				auto  Token = NeverNullptr(node._ToType._name._ScopedName.back()._token);

				LogError_CantCastExplicityTypes(Token, Ex0Type, ToTypeAs);
				_LastExpressionType = TypesEnum::Null;
			}
			else
			{
				if (HasInfo.Value.has_value())
				{
					data.FuncToCall = HasInfo.Value.value();
					_LastExpressionType = HasInfo.Value.value()->Get_Info<FuncInfo>()->Ret;
				}
				else
				{
					_LastExpressionType = ToTypeAs;
				}

			}


			_CastDatas.AddValue(Symbol_GetSymbolID(node), data);
		}
	}

	if (_PassType == PassType::BuidCode)
	{
		IR_Build_ExplicitConversion(_IR_LastExpressionField, _LastExpressionType, ToTypeAs, _CastDatas.GetValue(Symbol_GetSymbolID(node)));

		_LastExpressionType = ToTypeAs;
	}
	_LookingForTypes.pop();
}


UCodeLangFrontEnd