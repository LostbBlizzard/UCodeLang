#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
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
		bool lookforIntPtr =!(ToType._Type == TypesEnum::uIntPtr && ToType.IsAddress() == false);

		if (lookforIntPtr) {
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

		if (lookforIntPtr)
		{
			auto lookingfortype = _LookingForTypes.top();
			if (ToType.IsAddress() && !lookingfortype.IsAddress())
			{
				_LastExpressionType._IsAddress = false;
			}
		}
	}
	else if (_PassType == PassType::BuidCode)
	{
		TypeSymbol ToType = Type_ConvertAndValidateType(node._Type, NodeSyb_t::Any);

		bool lookforIntPtr = !(ToType._Type == TypesEnum::uIntPtr && ToType.IsAddress() == false);

		//!to stop *(_Expression) and do (_Expression)
		if (lookforIntPtr) {
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

		if (lookforIntPtr) 
		{
			auto lookingfortype = _LookingForTypes.top();
			if (ToType.IsAddress() && !lookingfortype.IsAddress())
			{
				auto ex = _IR_LastExpressionField;

				auto tep = ToType;
				tep._IsAddress = false;
				IRType v = IR_ConvertToIRType(tep);
				
				_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad_Dereferenc(ex, v);
			}
		}
	}
}

UCodeLangFrontEnd

#endif