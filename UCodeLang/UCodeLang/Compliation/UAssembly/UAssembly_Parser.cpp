#include "UAssembly_Parser.hpp"
#include "UCodeLang/Compliation/Helpers/ParseHelper.hpp"
UAssemblyStart
void Parser::Reset()
{
	_TokenIndex = 0;
}
void Parser::Parse(const Vector<Token>& Tokens, UClib* Output)
{
	Reset();
	_Output = Output;
	_Nodes = &Tokens;
	

	while (auto T = TryGetToken())
	{
		if (T->Type == TokenType::Name)
		{
			ParseIns();
		}
		else
		{
			_ErrorsOutput->AddError(ErrorCodes::CantParseTree, T->OnLine, T->OnPos, "bad Token");
		}
	}
}
void Parser::ParseIns()
{
	auto T = TryGetToken();
	if (T == nullptr)
	{
		_ErrorsOutput->AddError(ErrorCodes::CantParseTree,-1,-1, "bad Token");
		return;
	}
	const String_view& InsName = T->Value._String;
	if (StringToInsMap.count(InsName))
	{
		NextToken();
		auto& Item = StringToInsMap.at(InsName);
		_TepIns.OpCode = Item.OpCode;

		if (Item.Op_0 != OpCodeType::NoOpCode)
		{
			ParseOp(_TepIns.Value0, Item.Op_0);
		}

		auto CommaToken = TryGetToken();
		if (CommaToken && CommaToken->Type == TokenType::Comma && Item.Op_1 != OpCodeType::NoOpCode)
		{
			NextToken();	
			ParseOp(_TepIns.Value1, Item.Op_1);
		}
	
		_Output->Add_Instruction(_TepIns);
		_TepIns = Instruction();
	}
	else
	{
		NextToken();
		_ErrorsOutput->AddError(ErrorCodes::CantParseTree, T->OnLine, T->OnPos,(String)InsName + "Is Not a valid instrucion");
	}

}
void Parser::ParseOp(AnyInt64& Out, OpCodeType Type)
{

	switch (Type)
	{
	case UCodeLang::UAssembly::OpCodeType::NoOpCode:
		break;
	case UCodeLang::UAssembly::OpCodeType::AnyInt8:ParseAny8(Out);break;
	case UCodeLang::UAssembly::OpCodeType::AnyInt16:
		break;
	case UCodeLang::UAssembly::OpCodeType::AnyInt32:
		break;
	case UCodeLang::UAssembly::OpCodeType::UIntPtr :
	case UCodeLang::UAssembly::OpCodeType::AnyInt64:
		ParseAny8(Out);
		break;
	case UCodeLang::UAssembly::OpCodeType::Register:ParseOpRegister(Out); break;
	default:
		_ErrorsOutput->AddError(ErrorCodes::CantParseTree);
		break;
	}
}
void Parser::ParseOpRegister(AnyInt64& Out)
{
	auto T = TryGetToken();
	if (T == nullptr)
	{
		_ErrorsOutput->AddError(ErrorCodes::CantParseTree, 0, 0, "bad Token(no np)");
		return;
	}
	if (T->Type != TokenType::Left_Bracket)
	{
		_ErrorsOutput->AddError(ErrorCodes::CantParseTree, T->OnLine, T->OnPos, "bad Token");
		return;
	}
	NextToken();

	T = TryGetToken();
	if (T == nullptr)
	{
		_ErrorsOutput->AddError(ErrorCodes::CantParseTree,-1,-1, "bad Token");
		return;
	}
	if (T->Type != TokenType::Name)
	{
		_ErrorsOutput->AddError(ErrorCodes::CantParseTree, T->OnLine, T->OnPos, "bad Token");
		return;
	}
	const String_view& Name = T->Value._String;
	if (Name == "A"){Out = RegisterID::A;}
	else if (Name == "B") { Out = RegisterID::B; }
	else if (Name == "C") { Out = RegisterID::C; }
	else if (Name == "D") { Out = RegisterID::D; }
	else if (Name == "E") { Out = RegisterID::E; }
	else if (Name == "F") { Out = RegisterID::F; }
	else if (Name == "Input") { Out = RegisterID::InPutRegister; }
	else if (Name == "In") { Out = RegisterID::InPutRegister; }
	else if (Name == "Output") { Out = RegisterID::OuPutRegister; }
	else if (Name == "Out") { Out = RegisterID::OuPutRegister; }
	else if (Name == "This") { Out = RegisterID::ThisRegister; }
	else if (Name == "Mathout") { Out = RegisterID::MathOuPutRegister; }
	else if (Name == "CPUbool") { Out = RegisterID::BoolRegister; }
	else if (Name == "Bitwis") { Out = RegisterID::BitwiseRegister; }
	else
	{
		_ErrorsOutput->AddError(ErrorCodes::CantParseTree, T->OnLine, T->OnPos, "bad Token");
		return;
	}

	NextToken();
	T = TryGetToken();
	if (T == nullptr)
	{
		_ErrorsOutput->AddError(ErrorCodes::CantParseTree, -1,-1, "bad Token");
		return;
	}
	if (T->Type != TokenType::Right_Bracket)
	{
		_ErrorsOutput->AddError(ErrorCodes::CantParseTree, T->OnLine, T->OnPos, "bad Token");
		return;
	}
	NextToken();
}
void Parser::ParseAny8(AnyInt64& Out)
{
	auto T = TryGetToken();
	ParseHelper::ParseStringToInt8(T->Value._String, Out.AsInt8);
	NextToken();
}
UAssemblyEnd