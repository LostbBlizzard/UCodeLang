#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
#include "UCodeLang/Compliation/Helpers/ParseHelper.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnNumberliteralNode(const NumberliteralNode* num)
{
	auto& lookT = Type_Get_LookingForType();
	TypesEnum NewEx;
	if (lookT._Type == TypesEnum::Var)
	{
		NewEx = TypesEnum::sInt32;
	}
	else
	{
		NewEx = (Type_IsfloatType(lookT) || Type_IsIntType(lookT)) ? lookT._Type : TypesEnum::sInt32;
	}
#define Set_NumberliteralNodeU(x) \
			UInt##x V; \
			ParseHelper::ParseStringToUInt##x(Str, V); \
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(V);\

#define Set_NumberliteralNodeS(x) \
			Int##x V; \
			ParseHelper::ParseStringToInt##x(Str, V); \
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad(V);\

	if (_PassType == PassType::BuidCode)
	{
		auto& Str = num->token->Value._String;

		Debug_Add_SetLineNumber(NeverNullptr(num->token), _IR_LookingAtIRBlock->Instructions.size());

		switch (NewEx)
		{
		case TypesEnum::uInt8:
		{
			Set_NumberliteralNodeU(8);
		};
		break;
		case TypesEnum::uInt16:
		{
			Set_NumberliteralNodeU(16);
		};
		break;
		case TypesEnum::uInt32:
		{
			Set_NumberliteralNodeU(32);
		};
		break;
		case TypesEnum::uInt64:
		{
			Set_NumberliteralNodeU(64);
		};
		break;
		case TypesEnum::uIntPtr:
		{
			UInt64 V;
			ParseHelper::ParseStringToUInt64(Str, V);
			_IR_LastExpressionField = IR_Load_UIntptr(V);
		};
		break;

		case TypesEnum::sInt8:
		{
			Set_NumberliteralNodeS(8);
		};
		break;
		case TypesEnum::sInt16:
		{
			Set_NumberliteralNodeS(16);
		};
		break;
		case TypesEnum::sInt32:
		{
			Set_NumberliteralNodeS(32);
		};
		break;
		case TypesEnum::sInt64:
		{
			Set_NumberliteralNodeS(64);
		};
		break;
		case TypesEnum::sIntPtr:
		{
			Int64 V;
			ParseHelper::ParseStringToInt64(Str, V);
			_IR_LastExpressionField = IR_Load_SIntptr(V);
			break;
		};


		case TypesEnum::float32:
		{
			Int32 V;
			ParseHelper::ParseStringToInt32(Str, V);
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((float32)V);
			break;
		};
		case TypesEnum::float64:
		{
			Int64 V;
			ParseHelper::ParseStringToInt64(Str, V);
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((float64)V);
			break;
		};
		default:
			UCodeLangUnreachable();
			break;
		}
	}


	_LastExpressionType.SetType(NewEx);
	_LastLookedAtToken = Nullableptr(num->token);
}

UCodeLangFrontEnd