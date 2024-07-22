#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
#include "UCodeLang/Compilation/Helpers/ParseHelper.hpp"
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

#define Check_NumberliteralNodeU(x) \
			UInt##x V; \
			validliteral = ParseHelper::ParseStringToUInt##x(Str, V); \

#define Check_NumberliteralNodeS(x) \
			Int##x V; \
			validliteral = ParseHelper::ParseStringToInt##x(Str, V); \


	if (_PassType == PassType::FixedTypes)
	{
		auto& Str = num->token->Value._String;
		
		bool validliteral = false;
		switch (NewEx)
		{
		case TypesEnum::uInt8:
		{
			Check_NumberliteralNodeU(8);
		};
		break;
		case TypesEnum::uInt16:
		{
			Check_NumberliteralNodeU(16);
		};
		break;
		case TypesEnum::uInt32:
		{
			Check_NumberliteralNodeU(32);
		};
		break;
		case TypesEnum::uInt64:
		{
			Check_NumberliteralNodeU(64);
		};
		break;
		case TypesEnum::sInt8:
		{
			Check_NumberliteralNodeS(8);
		};
		break;
		case TypesEnum::sInt16:
		{
			Check_NumberliteralNodeS(16);
		};
		break;
		case TypesEnum::sInt32:
		{
			Check_NumberliteralNodeS(32);
		};
		break;
		case TypesEnum::sInt64:
		{
			Check_NumberliteralNodeS(64);
		};
		break;
		case TypesEnum::sIntPtr:
		{
			if (_Settings->PtrSize == IntSizes::Int64) 
			{
				Int64 V;
				validliteral = ParseHelper::ParseStringToInt64(Str, V);
			}
			else 
			{
				Int32 V;
				validliteral = ParseHelper::ParseStringToInt32(Str, V);
			}
			break;
		};
		case TypesEnum::uIntPtr:
		{
			UInt64 V;
			if (_Settings->PtrSize == IntSizes::Int64) 
			{
				UInt64 V;
				validliteral = ParseHelper::ParseStringToUInt64(Str, V);
			}
			else 
			{
				UInt32 V;
				validliteral = ParseHelper::ParseStringToUInt32(Str, V);
			}
		};
		break;
		case TypesEnum::float32:
		{
			Int32 V;
			validliteral = ParseHelper::ParseStringToInt32(Str, V);
			break;
		};
		case TypesEnum::float64:
		{
			Int64 V;
			validliteral = ParseHelper::ParseStringToInt64(Str, V);
			break;
		};
		default:
			UCodeLangUnreachable();
			break;
		};

		if (!validliteral) 
		{
			String msg ="Unable convert '";
			msg += num->token->Value._String;
			msg += "'";

			msg += " to ";
			msg += ToString(TypeSymbol(NewEx));

			LogError(ErrorCodes::InValidType,num->token->OnLine,num->token->OnPos, msg);
		}
	}
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

#endif
