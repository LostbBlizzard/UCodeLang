#include "Parser.hpp"
#include "../Helpers/KeyWords.hpp"
UCodeLangStart

#define TokenNotNullCheck(Var) \
if (Var == nullptr)\
{\
auto& Error = _ErrorsOutput->AddError(ErrorCodes::ExpectingToken, Token::EndOfFile, Token::EndOfFile); \
Error._Msg = "was not expecting " + (String)StringHelper::ToString(TokenType::EndofFile); \
return GotNodeType::failed;\
}\

#define TokenTypeCheck(Var,_TokenType) \
TokenNotNullCheck(Var); \
if (Var->Type != _TokenType) \
{\
	auto& Error = _ErrorsOutput->AddError(ErrorCodes::ExpectingToken, Var->OnLine,Var->OnPos);\
\
if (Var->Type == TokenType::Name){	\
Error._Msg = "Expecting " + (String)StringHelper::ToString(_TokenType) + \
" Got " + (String)Var->Value._String.data(); \
}\
else\
{\
	Error._Msg = "Expecting " + (String)StringHelper::ToString(_TokenType) + \
		" Got " + (String)StringHelper::ToString(Var->Type); \
}\
 return GotNodeType::failed;\
}\


#define IsToken(Var,_TokenType) 

#define ThrowError(ErrorCode,Msg) \
auto& Error = _ErrorsOutput->AddError(ErrorCodes::ExpectingToken, Token::EndOfFile, Token::EndOfFile);\
Error._Msg = Msg \

#define ExpectingMsg(Var,_TokenType)  "Expecting " + (String)StringHelper::ToString(_TokenType) + \
		" Got " + (String)StringHelper::ToString(Var->Type)

#define ThrowErrorExpectingToken(Msg,Var) \
auto& Error = _ErrorsOutput->AddError(ErrorCodes::ExpectingToken,Line, Pos);\
Error._Msg = Msg \

#define ThrowErrorExpectingToken2(Var,_TokenType) \
auto& Error = _ErrorsOutput->AddError(ErrorCodes::ExpectingToken, Var->OnLine,Var->OnPos);\
Error._Msg = ExpectingMsg(Var,_TokenType);  \


#define ThrowErrorExpectingAToken(_TokenType) \
auto& Error = _ErrorsOutput->AddError(ErrorCodes::ExpectingToken, Token::EndOfFile,Token::EndOfFile); \
Error._Msg = "Expecting " + (String)StringHelper::ToString(_TokenType) + " Got " + (String)StringHelper::ToString(TokenType::Null)

#define ThrowErrorExpecting(Text) \
auto& Error = _ErrorsOutput->AddError(ErrorCodes::ExpectingToken, Token::EndOfFile, Token::EndOfFile); \
Error._Msg = "Expecting " + Text;

#define ThrowErrorExpectingOr(V2,_TokenType,_TokenType2) \
auto& Error =V2 ? _ErrorsOutput->AddError(ErrorCodes::ExpectingToken, V2->OnLine, V2->OnPos) \
	: _ErrorsOutput->AddError(ErrorCodes::ExpectingToken, Token::EndOfFile, Token::EndOfFile); \
Error._Msg = "Expecting " + (String)StringHelper::ToString(TokenType::Colon) + " or " \
+ (String)StringHelper::ToString(_TokenType) + " but got " + (String)StringHelper::ToString(_TokenType2) + "instead"; \
Data.GotNode = GotNodeType::failed; \
Data.Node = nullptr; \
return Data;

void Parser::Reset()
{
}

void Parser::Parse(const Vector<Token>&Tokens)
{
	for (const auto& Item : Tokens)
	{
		_Nodes.push_back(Item);
	}
	_TokenIndex = 0;
	GotNodeType GotNode = GotNodeType::Null;

	while (auto T = TryGetToken())
	{
		TryGetNode V;
		

		switch (T->Type)
		{
		case TokenType::Namespace:V = GetNamespaceNode(); break;
		case TokenType::Class:V = GetClassNode();break;
		case declareFunc:V = GetFuncNode(); break;
		case TokenType::KeyWorld_use:V = GetUseNode(); break;
		//case TokenType::Left_Bracket:V = GetAttribute();break;
		//case TokenType::KeyWorld_static:V = GetDeclareStaticVariable();break;
		default://This Need the same as GetDeclareStaticVariable();break;
			//auto newNode = DeclareStaticVariableNode::Gen();break;
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
			break;
		}

		if (V.Node)
		{
			_Tree._Nodes.push_back(V.Node);
		}
	}
	
}

GotNodeType Parser::GetNamespaceNode(NamespaceNode& out)
{
	auto NamespaceToken = TryGetToken(); TokenTypeCheck(NamespaceToken, TokenType::Namespace);
	NextToken();

	auto ScopeResolutionToken = TryGetToken(); TokenNotNullCheck(ScopeResolutionToken);
	out.NamespaceName.ScopedName.push_back(NamespaceToken);
	if (ScopeResolutionToken->Type == TokenType::ScopeResolution)
	{
		NextToken();
		GetName(out.NamespaceName);
	}

	auto ColonToken = TryGetToken(); TokenTypeCheck(ColonToken, TokenType::Colon);
	NextToken();

	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);
	NextToken();


	while (auto T = TryGetToken())
	{
		TryGetNode V;

		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		case TokenType::Namespace:V = GetNamespaceNode(); break;
		case TokenType::Class:V = GetClassNode();break;
		case declareFunc:V = GetFuncNode(); break;
		case TokenType::KeyWorld_use:V = GetUseNode(); break;
			//case TokenType::Left_Bracket:V = GetAttribute();break;
			//case TokenType::KeyWorld_static:V = GetDeclareStaticVariable();break;
		default://This Need the same as GetDeclareStaticVariable();break;
			//auto newNode = DeclareStaticVariableNode::Gen();break;
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
			break;
		}

		if (V.Node)
		{
			out._Nodes.push_back(V.Node);
		}
	}
	EndLoop:
	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();

	return GotNodeType::Success;
}

GotNodeType Parser::GetClassNode(ClassNode& out)
{
	auto ClassToken = TryGetToken(); TokenTypeCheck(ClassToken, TokenType::Class);
	out.ClassName.Token = ClassToken;
	NextToken();

	auto ColonToken = TryGetToken(); TokenTypeCheck(ColonToken, TokenType::Colon);
	NextToken();

	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);
	NextToken();

	while (auto T = TryGetToken())
	{
		TryGetNode V;

		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		case TokenType::Class:V = GetClassNode(); break;
		case declareFunc:V = GetFuncNode(); break;
		case TokenType::KeyWorld_use:V = GetUseNode(); break;
			//case TokenType::Left_Bracket:V = GetAttribute();break;
			//case TokenType::KeyWorld_static:V = GetDeclareStaticVariable();break;
		default://This Need the same as GetDeclareStaticVariable();break;
			//auto newNode = DeclareStaticVariableNode::Gen();break;
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
			break;
		}

		if (V.Node)
		{
			out._Nodes.push_back(V.Node);
		}
	}

EndLoop:
	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();

	return GotNodeType::Success;
}

GotNodeType Parser::GetStatementsorStatementNode(StatementsNode& out)
{
	auto TabToken = TryGetToken(); TokenNotNullCheck(TabToken);

	if (TabToken->Type == TokenType::StartTab)
	{
		return GetStatements(out);
	}
	else
	{
		out._Nodes.push_back(nullptr);
		auto& node = out._Nodes.back();
		return GetStatement(node);
	}
}

GotNodeType Parser::GetStatement(Node*& out)
{
	auto StatementTypeToken = TryGetToken();
	switch (StatementTypeToken->Type)
	{
	case UCodeLang::TokenType::KeyWorld_asm:
	{
		auto r = GetAsmBlock();
		out = r.Node;
		return r.GotNode;
	}
	case UCodeLang::TokenType::StartTab:
	{
		auto r = GetStatements();
		out = r.Node;
		return r.GotNode;
	};
	case UCodeLang::TokenType::Class:
	{
		auto r = GetClassNode();
		out = r.Node;
		return r.GotNode;
	};
	case UCodeLang::TokenType::KeyWorld_use:
	{
		auto r = GetUseNode();
		out = r.Node;
		return r.GotNode;
	};
	default:
		#if CompliationTypeSafety
		throw std::exception("Cant UnWap BuildStatement");
		#endif
		return GotNodeType::failed;
		break;
	}
}

GotNodeType Parser::GetStatements(StatementsNode& out)
{
	auto TabToken = TryGetToken();
	TokenTypeCheck(TabToken, TokenType::StartTab);
	NextToken();

	while (auto T = TryGetToken())
	{
		if (IsStartofaStatement(T->Type))
		{
			Node* V = nullptr;
			GetStatement(V);
			if (V){out._Nodes.push_back(V);}
		}
		else if (T->Type == TokenType::EndTab) { break; }
		else
		{
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
		}
	}


	auto EndToken = TryGetToken();
	TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();
	return GotNodeType::Success;
}

GotNodeType Parser::GetFuncNode(FuncNode& out)
{
	auto V = GetFuncSignatureNode(out.Signature);

	auto ColonToken = TryGetToken(); TokenNotNullCheck(ColonToken);

	switch (ColonToken->Type)
	{
	case TokenType::Semicolon:
		NextToken();
		out.Body.HasValue = false;
		break;
	case TokenType::Colon:
		NextToken();
	    GetFuncBodyNode(out.Body.Item);
		out.Body.HasValue = true;
		break;
	default:
		TokenTypeCheck(ColonToken, TokenType::Colon);
		break;
	}

	return GotNodeType::Success;
}
GotNodeType Parser::GetFuncSignatureNode(FuncSignatureNode& out)
{
	auto funcToken = TryGetToken();
	TokenTypeCheck(funcToken, declareFunc);
	NextToken();

	auto NameToken = TryGetToken();

	if (NameToken->Type == TokenType::KeyWorld_This
		|| NameToken->Type == TokenType::KeyWorld_Drop
		|| IsOverLoadableOperator(NameToken))
	{
		NextToken();
	}
	else
	{
		TokenTypeCheck(NameToken, TokenType::Name);
		NextToken();
	}
	out.Name.Token = NameToken;

	auto LPToken = TryGetToken();
	TokenTypeCheck(LPToken, declareFuncParsStart);
	NextToken();

	auto Parameters = GetNamedParametersNode(out.Parameters);

	auto RPToken = TryGetToken();
	TokenTypeCheck(RPToken, declareFuncParsEnd);
	NextToken();

	Node* Ret_Type;
	auto Arrow = TryGetToken();
	if (Arrow->Type == TokenType::RightArrow)
	{
		NextToken();
		GetTypeWithVoid(out.ReturnType);
	}
	else if (Arrow->Type == TokenType::Colon)
	{
		TypeNode::Gen_void(out.ReturnType,*Arrow);
	}
	else
	{
		Ret_Type = nullptr;
		TokenTypeCheck(Arrow, TokenType::Colon);
	}

	return GotNodeType::Success;
}
GotNodeType Parser::GetFuncBodyNode(FuncBodyNode& out)
{
	return GetStatementsorStatementNode(out.Statements);
}
GotNodeType Parser::GetNamedParametersNode(NamedParametersNode& out)
{
	TryGetNode Data;
	Data.GotNode = GotNodeType::failed;
	Data.Node = nullptr;

	auto Parameters = NamedParametersNode::Gen();
	do
	{
		NamedParameterNode Tep;
		auto Token = TryGetToken();
		if (!Token || Token->Type == TokenType::Right_Bracket) { break; }

		GetType(Tep.Type);
		GetName(Tep.Name);

		out.Statements.push_back(Tep);

		auto CommaToken = TryGetToken();
		if (CommaToken && CommaToken->Type == TokenType::Comma)
		{
			NextToken();
			continue;
		}
		else
		{
			break;
		}



	} while (true);
	return GotNodeType::Success;
}

GotNodeType Parser::TryGetGeneric(GenericValuesNode& out)
{
	auto token = TryGetToken();
	TokenNotNullCheck(token);
	if (token->Type == TokenType::greaterthan)
	{


		auto endtoken = TryGetToken();
		TokenTypeCheck(endtoken, TokenType::lessthan);
		NextToken();
	}
	return GotNodeType::Success;
}

GotNodeType Parser::GetName(ScopedNameNode& out)
{
	while (true)
	{
		auto NameToken = TryGetToken();

		TokenTypeCheck(NameToken, TokenType::Name);

		out.ScopedName.push_back(NameToken);
		NextToken();


		auto Token = TryGetToken();
		if (Token && Token->Type == TokenType::ScopeResolution)
		{
			NextToken();
			continue;
		}
		else
		{
			break;
		}
	}
	return GotNodeType::Success;
}

GotNodeType Parser::GetName(NameNode& out)
{
	auto NameToken = TryGetToken();
	TokenTypeCheck(NameToken, TokenType::Name);

	out.Token = NameToken;
	NextToken();
	return GotNodeType::Success;
}

GotNodeType Parser::GetType(TypeNode& out)
{
	auto Token = TryGetToken();
	if (Token->Type == TokenType::Name) 
	{
		GetName(out.Name);
	}
	else 
	{
		TryGetGeneric(out.Generic);
		NextToken();
	}
	return GotNodeType::Success;
}

GotNodeType Parser::GetTypeWithVoid(TypeNode& out)
{
	auto token = TryGetToken();
	TokenNotNullCheck(token);
	
	if (token->Type == TokenType::Void)
	{
		NextToken();
		out.Name.Token = token;
		return GotNodeType::Success;
	}
	else
	{
		return GetType(out);
	}
}

GotNodeType Parser::GetNumericType(TypeNode& out)
{
	return GetType(out);
}

GotNodeType Parser::GetUseNode(UsingNode& out)
{
	auto UseToken = TryGetToken(); TokenTypeCheck(UseToken, TokenType::KeyWorld_use);
	NextToken();

	GetName(out.ScopedName);


	auto SemicolonToken = TryGetToken();TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();

	return GotNodeType::Success;
}

GotNodeType Parser::GetAsmBlock(AsmBlockNode& out)
{
	auto AsmToken = TryGetToken();
	TokenTypeCheck(AsmToken, TokenType::KeyWorld_asm);
	NextToken();

	auto ColonToken = TryGetToken();
	TokenTypeCheck(ColonToken, TokenType::Colon);
	NextToken();

	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);
	if (StartToken->Type == TokenType::StartTab)
	{
		NextToken();
		while (auto T = TryGetToken())
		{
			if (T->Type == TokenType::EndTab) { break; }
			else 
			{
			

				Token::PushString(out.AsmText, *T); NextToken();	
				
				switch (T->Type)
				{
				case TokenType::Number_literal:
				case TokenType::String_literal:
				case TokenType::Name:
					out.AsmText += " ";
					break;
				default:break;
				}
			}
		}

		auto EndToken = TryGetToken();
		TokenTypeCheck(EndToken, TokenType::EndTab);
		NextToken();
	}
	else
	{

	}
	


	return GotNodeType::Success;
}





UCodeLangEnd