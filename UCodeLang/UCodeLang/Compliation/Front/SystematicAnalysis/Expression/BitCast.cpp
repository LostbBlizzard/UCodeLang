#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnBitCast(const BitCastExpression& node)
{
	if (_PassType == PassType::FixedTypes)
	{
		if (!IsInUnSafeBlock())
		{
			auto Token = node._KeywordToken;
			LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant do bitcast in safe mode.");
			return;
		}

		auto Token = node._KeywordToken;
		TypeSymbol ToType = Type_ConvertAndValidateType(node._Type, NodeSyb_t::Any);

		if (!ToType.IsBadType() && !Type_IsUnMapType(ToType))
		{
			if (!ToType.IsAddress() && !ToType.IsAddressArray() && (ToType._Type != TypesEnum::uIntPtr))
			{
				LogError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "bitcast<T>. T must be an address or an uintptr and not '" + ToString(ToType) + "'");
			}
		}
		bool IsIntPtr = ToType._Type == TypesEnum::uIntPtr && ToType.IsAddress() == false;

		if (IsIntPtr) {
			_LookingForTypes.push(TypesEnum::uIntPtr);
		}
		else
		{
			TypeSymbol V = TypesEnum::Any;
			V.SetAsAddress();
			_LookingForTypes.push(V);
		}

		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);

		_LookingForTypes.pop();

		auto ExType = _LastExpressionType;


		_LastExpressionType = ToType;
	}
	else if (_PassType == PassType::BuidCode)
	{
		TypeSymbol ToType = Type_ConvertAndValidateType(node._Type, NodeSyb_t::Any);

		bool IsIntPtr = ToType._Type == TypesEnum::uIntPtr && ToType.IsAddress() == false;

		//!to stop *(_Expression) and do (_Expression)
		if (IsIntPtr) {
			_LookingForTypes.push(TypesEnum::uIntPtr);
		}
		else
		{
			TypeSymbol V = TypesEnum::Any;
			V.SetAsAddress();
			_LookingForTypes.push(V);
		}

		OnExpressionTypeNode(node._Expression._Value.get(), GetValueMode::Read);

		_LookingForTypes.pop();

		auto ExType = _LastExpressionType;

		_LastExpressionType = ToType;
	}
}

UCodeLangFrontEnd