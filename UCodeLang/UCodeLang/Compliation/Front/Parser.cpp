#include "Parser.hpp"
#include "../Helpers/KeyWords.hpp"
#include "..\UAssembly\UAssembly_Parser.hpp"
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
" Got " + (String)Var->Value._String; \
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
	_TokenIndex = 0;
	_Nodes = nullptr;
	_Text = String_view();
	_Tree.Reset();
}



void Parser::Parse(const FileData& Data, const Vector<Token>&Tokens)
{
	Reset();
	_Text = Data.Text;
	_Tree.FilePath = Data.FilePath;
	_Nodes = &Tokens;
	
	

	while (auto T = TryGetToken())
	{
		TryGetNode V;


		switch (T->Type)
		{
		case TokenType::Namespace:V = GetNamespaceNode(); break;
		case TokenType::KeyWorld_Tag:V = GetTagNode(); break;
		case TokenType::KeyWorld_Enum:V = GetEnumNode(); break;
		case TokenType::Class:V = GetClassNode(); break;
		case Parser::declareFunc:V = GetFuncNode(); break;
		case TokenType::KeyWorld_use:V = GetUseNode(); break;
		case TokenType::Left_Bracket:V = GetAttribute(); break;
		case TokenType::KeyWorld_static:V = GetDeclareStaticVariable(); break;
		case TokenType::KeyWorld_Thread:V = GetDeclareThreadVariable(); break;
		default: GetDeclareVariableNoObject(V); break;
		}
		if (V.Node)
		{
			_Tree._Nodes.push_back(V.Node);
		}
		else { break; }
	}
	_ParseSuccess= !_ErrorsOutput->Has_Errors();
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
		case TokenType::KeyWorld_Tag:V = GetTagNode(); break;
		case TokenType::KeyWorld_Enum:V = GetEnumNode(); break;
		case TokenType::Class:V = GetClassNode();break;
		case Parser::declareFunc:V = GetFuncNode(); break;
		case TokenType::KeyWorld_use:V = GetUseNode(); break;
		case TokenType::Left_Bracket:V = GetAttribute();break;
		case TokenType::KeyWorld_static:V = GetDeclareStaticVariable(); break;
		case TokenType::KeyWorld_Thread:V = GetDeclareThreadVariable(); break;
		default: GetDeclareVariableNoObject(V); break;
		}

		if (V.Node)
		{
			out._Nodes.push_back(V.Node);
		}
		else { break; }
	}
	EndLoop:
	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();

	return GotNodeType::Success;
}
GotNodeType Parser::GetAlias(const Token* AliasName,GenericValuesNode& AliasGenerics, AliasNode& out)
{
	out.AliasName.Token = AliasName;
	out.Generic = AliasGenerics;//Move

	auto ClassToken = TryGetToken(); TokenTypeCheck(ClassToken, TokenType::equal);
	NextToken();

	auto r = GetType(out.Type);

	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();

	return r;
}
GotNodeType Parser::GetClassTypeNode(Node*& out)
{
	auto ClassToken = TryGetToken(); TokenTypeCheck(ClassToken, TokenType::Class);

	

	const String_view& ClassName = ClassToken->Value._String;
	
	
	NextToken();

	GenericValuesNode TepGenerics;

	TryGetGeneric(TepGenerics);

	auto ColonToken = TryGetToken(); TokenNotNullCheck(ColonToken);
	if (ColonToken->Type == TokenType::equal)
	{
		auto V = AliasNode::Gen();
		out = V->As();
		return GetAlias(ClassToken, TepGenerics, *V);//TepGenerics Move
	}
	else if (ColonToken->Type == TokenType::Semicolon)
	{
		NextToken();

		auto output = ClassNode::Gen(); out = output->As();
		output->ClassName.Token = ClassToken;
		output->Generic = TepGenerics;
		return GotNodeType::Success;
	}
	else
	{
		auto output = ClassNode::Gen();out = output->As();
		output->ClassName.Token = ClassToken;
		output->Generic = TepGenerics;
		
		TokenTypeCheck(ColonToken, TokenType::Colon);
		NextToken();

		auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);
		NextToken();

		while (auto T = TryGetToken())
		{
			TryGetNode V;

			switch (T->Type)
			{
			case TokenType::EndTab:goto EndLoop;
			case TokenType::KeyWorld_Tag:V = GetTagNode(); break;
			case TokenType::KeyWorld_Enum:V = GetEnumNode(); break;
			case TokenType::Class:V = GetClassNode(); break;
			case Parser::declareFunc:V = GetFuncNode(); break;
			case TokenType::KeyWorld_use:V = GetUseNode(); break;
			case TokenType::Left_Bracket:V = GetAttribute(); break;
			case TokenType::KeyWorld_static:V = GetDeclareStaticVariable(); break;
			case TokenType::KeyWorld_Thread:V = GetDeclareThreadVariable(); break;
			default:V = GetDeclareVariable();
			}

			if (V.Node)
			{
				output->_Nodes.push_back(V.Node);
			}
			else { break; }
		}

	EndLoop:
		auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
		NextToken();

		return GotNodeType::Success;
	}

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
	case TokenType::KeyWorld_asm:
	{
		auto r = GetAsmBlock();
		out = r.Node;
		return r.GotNode;
	}
	case TokenType::StartTab:
	{
		auto r = GetStatements();
		out = r.Node;
		return r.GotNode;
	};
	case TokenType::KeyWorld_Tag:
	{
		auto r = GetTagNode();
		out = r.Node;
		return r.GotNode;
	};
	case TokenType::KeyWorld_Enum:
	{
		auto r = GetEnumNode();
		out = r.Node;
		return r.GotNode;
	};
	case TokenType::Class:
	{
		auto r = GetClassNode();
		out = r.Node;
		return r.GotNode;
	};
	case TokenType::KeyWorld_use:
	{
		auto r = GetUseNode();
		out = r.Node;
		return r.GotNode;
	};
	case TokenType::KeyWorld_Ret:
	{
		auto r = GetRetStatement();
		out = r.Node;
		return r.GotNode;
	};
	case TokenType::KeyWorld_static:
	{
		auto r = GetDeclareStaticVariable();
		out = r.Node;
		return r.GotNode;
	}
	case TokenType::KeyWorld_Thread:
	{
		auto r = GetDeclareThreadVariable();
		out = r.Node;
		return r.GotNode;
	}
	break;
	case Parser::IfToken:
	{
		auto r = GetIfNode();
		out = r.Node;
		return r.GotNode;
	}
	break;
	case TokenType::KeyWorld_Drop:
	{
		auto r = GetDropStatementNode();
		out = r.Node;
		return r.GotNode;
	}
	break;
	default:
	{
		size_t OldIndex = _TokenIndex;
		NameNode Tep;
		auto Name = GetNameCheck(Tep);
		auto _Token = TryGetToken();

		TryGetNode r;
		size_t NewIndex= _TokenIndex;
		_TokenIndex = OldIndex;
		if (_Token) 
		{
			if (_Token->Type == TokenType::equal)
			{
				r = GetAssignVariable();
			}
			else if (_Token->Type == FuncCallStart)
			{
				r = GetFuncCallStatementNode();
			}
			else if (_Token->Type == TokenType::lessthan)
			{
				_TokenIndex = NewIndex;
				UseGenericsNode V;
				TryGetGeneric(V);
				auto _Token2 = TryGetToken(); TokenNotNullCheck(_Token2);

				_TokenIndex = OldIndex;
				if (_Token2->Type == FuncCallStart)
				{
					r = GetFuncCallStatementNode();
				}
				else
				{
					r = GetDeclareVariable();
				}
			}
			else if (IsPostfixOperator(_Token))
			{
				r =GetPostfixStatement();
			}
			else if (IsCompoundOperator(_Token))
			{
				r = GetCompoundStatement();
			}
			else
			{
				r = GetDeclareVariable();
			}
		}
		out = r.Node;
		return r.GotNode;
	}
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
		if (T->Type == TokenType::EndTab) { break; }

		Node* V = nullptr;
		auto R = GetStatement(V);
		if (V) { out._Nodes.push_back(V); }
		if (R != GotNodeType::Success){ break; }
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
		out.Body = {};
		break;
	case TokenType::Colon: 
	{
		NextToken();
		FuncBodyNode V;
		GetFuncBodyNode(V);
		out.Body = std::move(V);
	}break;
	case TokenType::RightAssignArrow:
	{
		NextToken();
		FuncBodyNode V;
		
		V.Statements._Nodes.push_back(nullptr);

		RetStatementNode* r = RetStatementNode::Gen();
		GetExpressionTypeNode(r->Expression);
		V.Statements._Nodes.back() = r;

		out.Body = std::move(V);

		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
		NextToken();
	}
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
	TokenTypeCheck(funcToken, Parser::declareFunc);
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
	TryGetGeneric(out.Generic);

	auto LPToken = TryGetToken();
	TokenTypeCheck(LPToken, declareFuncParsStart);
	NextToken();

	auto Parameters = GetNamedParametersNode(out.Parameters);

	auto RPToken = TryGetToken();
	TokenTypeCheck(RPToken, declareFuncParsEnd);
	NextToken();

	Node* Ret_Type;
	auto Arrow = TryGetToken(); TokenNotNullCheck(Arrow);
	if (Arrow->Type == TokenType::RightArrow)
	{
		NextToken();
		auto RetToken = TryGetToken(); TokenNotNullCheck(RetToken);
		if (RetToken->Type == AnonymousObjectStart)
		{
			NextToken();
			auto node = AnonymousTypeNode::Gen();
			out.ReturnType.node = node;
			GetNamedParametersNode(node->Fields);


			auto AnonymousObjectEndToken = TryGetToken();
			TokenTypeCheck(AnonymousObjectEndToken,AnonymousObjectEnd);
			NextToken();
		}
		else
		{
			GetTypeWithVoid(out.ReturnType);
		}
	}
	else  if (Arrow->Type == TokenType::RightAssignArrow)
	{
		TypeNode::Gen_Var(out.ReturnType, *Arrow);
	}
	else if (Arrow->Type == TokenType::Colon)
	{
		TypeNode::Gen_Var(out.ReturnType,*Arrow);
	}
	else if (Arrow->Type == TokenType::Semicolon)
	{
		TypeNode::Gen_Var(out.ReturnType, *Arrow);
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
GotNodeType Parser::GetExpressionNode(ValueExpressionNode& out)
{
	return GetExpressionNode(out.Value);
}
GotNodeType Parser::GetExpressionTypeNode(ExpressionNodeType& out)
{
	return GetExpressionTypeNode(out.Value);
}
GotNodeType Parser::GetExpressionNode(Node*& out)
{
	auto StatementTypeToken = TryGetToken();
	switch (StatementTypeToken->Type)
	{
	case TokenType::String_literal:
	{
		NextToken();
		auto r = StringliteralNode::Gen();
		r->Token = StatementTypeToken;
		out = r->As();
		return GotNodeType::Success;
	}
	case TokenType::Number_literal:
	{
		NextToken();
		auto r = NumberliteralNode::Gen();
		r->Token = StatementTypeToken;
		out = r->As();
		return GotNodeType::Success;
	}
	case TokenType::KeyWorld_True:
	{
		NextToken();
		auto r = BoolliteralNode::Gen();
		r->Value = true;
		out = r->As();
		return GotNodeType::Success;
	}
	case TokenType::KeyWorld_False:
	{
		NextToken();
		auto r = BoolliteralNode::Gen();
		r->Value = false;
		out = r->As();
		return GotNodeType::Success;
	}
	case AnonymousObjectStart:
	{
		auto r = GetAnonymousObjectConstructorNode();
		out = r.Node;
		return r.GotNode;
	}
	case TokenType::Name:
	{
		size_t OldIndex = _TokenIndex;
		NameNode Tep;
		auto Name = GetNameCheck(Tep);
		auto _Token = TryGetToken();

		_TokenIndex = OldIndex;
		if (_Token->Type == TokenType::lessthan ||
			_Token->Type == FuncCallStart)
		{
			auto r = GetFuncCallNode();
			out = r.Node;
			return r.GotNode;
		}
		else
		{
			auto r = ReadVariableNode::Gen();
			GetName(r->VariableName);
			out = r->As();
			return GotNodeType::Success;
		}
		
	}
	case TokenType::Left_Parentheses:
	{
		NextToken();
		auto V = ParenthesesExpresionNode::Gen();
		out = V;
		auto r = GetExpressionTypeNode(V->Expression);

		auto endtoken = TryGetToken();
		TokenTypeCheck(endtoken, TokenType::Right_Parentheses);
		NextToken();
		return r;
	}
	case TokenType::KeyWorld_Sizeof:
	{
		NextToken();
		auto V = SizeofExpresionNode::Gen();
		out = V;

		auto ParToken = TryGetToken();
		TokenTypeCheck(ParToken, FuncCallStart);
		NextToken();

		GetType(V->Type);

		auto Par2Token = TryGetToken();
		TokenTypeCheck(Par2Token, FuncCallEnd);
		NextToken();
		return GotNodeType::Success;
	}
	break;
	case TokenType::KeyWorld_new:
	{
		auto V = GetNewExpresionNode();
		out = V.Node;
		return V.GotNode;
	}
	break;
	default:
		#if CompliationTypeSafety
		throw std::exception("Cant UnWap BuildStatement");
		#endif
		return GotNodeType::failed;
		break;
	}
}
GotNodeType Parser::GetExpressionTypeNode(Node*& out)
{
	Node* ExNode = nullptr;
	auto Ex = GetExpressionNode(ExNode);
	auto Token = TryGetToken();
	GotNodeType r_t= GotNodeType::Null;
	Node* r_out =nullptr;
	if (Token && IsBinaryOperator(Token)) 
	{
		NextToken();
		Node* Other = nullptr;
		auto Ex2 = GetExpressionTypeNode(Other);

		auto r = BinaryExpressionNode::Gen();

		auto Ptr = ValueExpressionNode::Gen();
		Ptr->Value = ExNode;
		r->Value0.Value= Ptr->As();
		
		r->BinaryOp = Token;
		r->Value1.Value = Other;
		r_out = r->As();
		r_t = Merge(Ex, Ex2);
	}
	else
	{
		auto Ptr = ValueExpressionNode::Gen();
		Ptr->Value = ExNode;
		r_out = Ptr->As();
		r_t = Ex;
	}

	auto Token2 = TryGetToken();
	while (Token2 && Token2->Type == TokenType::RightArrow)
	{
		NextToken();
		auto cast = CastNode::Gen();
		


		auto Type = GetType(cast->ToType);
		cast->Expression.Value = r_out;
		r_t = Merge(Ex,Type);
		r_out = cast->As();
		Token2 = TryGetToken();
	}

	out = r_out;
	return r_t;
}
GotNodeType Parser::GetValueParameterNode(Node*& out)
{
	return GetExpressionTypeNode(out);//Just for consistency.
}
GotNodeType Parser::GetValueParametersNode(ValueParametersNode& out)
{
	auto _Token = TryGetToken();
	if (_Token && (_Token->Type == FuncCallEnd
		|| _Token->Type == TokenType::Right_Bracket))
	{
		return GotNodeType::Success;
	}

	while (true)
	{
		Node* node = nullptr;
		auto Ex = GetValueParameterNode(node);
		out._Nodes.push_back(node);
	
		auto Token = TryGetToken();

		if (Token == nullptr || Token->Type != TokenType::Comma)
		{
			break;
		}
		NextToken();
	}
	return GotNodeType::Success;
}
GotNodeType Parser::GetNamedParametersNode(NamedParametersNode& out)
{
	while (true)
	{
		NamedParameterNode Tep;
		auto Token = TryGetToken();
		if (!Token || Token->Type == TokenType::Right_Bracket) { break; }

		
		if (Token->Type == TokenType::KeyWorld_This)
		{
			NextToken();
			auto Token2 = TryGetToken();
			NextToken();
			auto Token3 = TryGetToken();
			if (Token2 && Token2->Type == TokenType::bitwise_and 
				&& Token3 && (Token3->Type == TokenType::Comma || Token3->Type == TokenType::Right_Bracket) )
			{
				if (out.Parameters.size() != 0)
				{
					_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos,"this& Must be the first parameter");
				}
				TypeNode::Gen_ThisMemberFunc(Tep.Type,*Token);
				goto End;
			}
			_TokenIndex-=2;//Move back
		}

		GetType(Tep.Type);
			
		GetName(Tep.Name);

		End:
		out.Parameters.push_back(std::move(Tep));

		auto CommaToken = TryGetToken();
		if (CommaToken == nullptr || CommaToken->Type != TokenType::Comma)
		{
			break;
		}
		NextToken();
	}
	return GotNodeType::Success;
}

GotNodeType Parser::TryGetGeneric(GenericValuesNode& out)
{
	auto token = TryGetToken();
	TokenNotNullCheck(token);
	if (token->Type == TokenType::lessthan)
	{
		NextToken();

		while (true)
		{
			GenericValueNode Item;
			auto NameToken = TryGetToken();
			if (NameToken && (NameToken->Type == TokenType::Name || TypeNode::IsPrimitive(NameToken->Type) ))
			{

			}
			else
			{
				TokenTypeCheck(NameToken, TokenType::Name);
			}
			Item.Token = NameToken;
			out.Values.push_back(Item);

			NextToken();
			auto Token = TryGetToken();
			if (Token == nullptr || Token->Type != TokenType::Comma)
			{
				break;
			}
			NextToken();
		}

		auto endtoken = TryGetToken();
		TokenTypeCheck(endtoken, TokenType::greaterthan);
		NextToken();
	}
	return GotNodeType::Success;
}

GotNodeType Parser::TryGetGeneric(UseGenericsNode& out)
{
	auto token = TryGetToken();
	TokenNotNullCheck(token);
	if (token->Type == TokenType::lessthan)
	{
		NextToken();

		while (true)
		{
			TypeNode Item;
			GetType(Item);
			out.Values.push_back(Item);


			auto Token = TryGetToken();
			if (Token == nullptr || Token->Type != TokenType::Comma)
			{
				break;
			}
			NextToken();
		}

		auto endtoken = TryGetToken();
		TokenTypeCheck(endtoken, TokenType::greaterthan);
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
		if (Token == nullptr || Token->Type != TokenType::ScopeResolution)
		{
			break;
		}
		NextToken();
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

GotNodeType Parser::GetNameCheck(NameNode& out)
{
	auto NameToken = TryGetToken();
	if (NameToken == nullptr || NameToken->Type != TokenType::Name)
	{
		NextToken();
		return  GotNodeType::failed;
	}

	out.Token = NameToken;
	NextToken();
	return GotNodeType::Success;
}

GotNodeType Parser::GetType(TypeNode& out, bool ignoreRighthandOFtype)
{
	GotNodeType r;
	auto Token = TryGetToken();
	if (Token->Type == TokenType::Name)
	{
		auto A = GetName(out.Name);
		auto B = TryGetGeneric(out.Generic);
		r = Merge(A,B);
	}
	else if (TypeNode::IsType(Token->Type))
	{
		NextToken();
		out.Name.Token = Token;
		r = GotNodeType::Success;
	}
	else
	{
		TokenTypeCheck(Token, TokenType::Name);
	}
	

	auto Token2 = TryGetToken();
	if (!ignoreRighthandOFtype && Token2)
	{
		if (Token2->Type == TokenType::bitwise_and)
		{
			NextToken();
			out.PushAsAddess();
		}
		
		Token2 = TryGetToken();
		if (Token2 && Token2->Type == TokenType::Left_Bracket)
		{
			NextToken();
			Token2 = TryGetToken(); TokenNotNullCheck(Token2);
			if (Token2->Type == TokenType::bitwise_and)
			{
				NextToken();

				Token2 = TryGetToken(); TokenNotNullCheck(Token2);
				if (Token2->Type == TokenType::Right_Bracket)
				{
					out.PushAsArrayAddess();
					
				}
				else
				{
					throw std::exception("not added");
				}
			}
			else if (Token2->Type == TokenType::Right_Bracket) 
			{
				throw std::exception("not added");
				goto Done;
			}
			else
			{
				throw std::exception("not added");
			}
		
		
		Done:
			Token2 = TryGetToken();
			TokenTypeCheck(Token2, TokenType::Right_Bracket);
			NextToken();
		}
	}
	return r;
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
GotNodeType Parser::GetAttribute(AttributeNode& out)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::Left_Bracket);
	NextToken();

	auto ScopedName = GetName(out.ScopedName);

	auto ParToken = TryGetToken(); TokenNotNullCheck(ParToken);

	if (ParToken->Type == FuncCallStart)
	{
		NextToken();
		auto Pars = GetValueParametersNode(out.Parameters);

		auto Par2Token = TryGetToken();
		TokenTypeCheck(Par2Token, FuncCallEnd);
		NextToken();
	}


	auto BracketToken = TryGetToken();
	TokenTypeCheck(BracketToken, TokenType::Right_Bracket);
	NextToken();
	
	return GotNodeType::Success;
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
		size_t StringStart = StartToken->OnPos;
		while (auto T = TryGetToken())
		{
			if (T->Type == TokenType::EndTab) { break; }
			NextToken();
		}
		size_t StringEnd = TryGetToken()->OnPos;

		size_t StringSize = StringEnd - StringStart;
		out.AsmText = _Text.substr(StringStart, StringSize);

		auto EndToken = TryGetToken();
		TokenTypeCheck(EndToken, TokenType::EndTab);
		NextToken();
	}
	else
	{

	}
	


	return GotNodeType::Success;
}

GotNodeType Parser::GetRetStatement(RetStatementNode& out)
{
	auto RetToken = TryGetToken();
	TokenTypeCheck(RetToken, TokenType::KeyWorld_Ret);
	NextToken();

	auto Token = TryGetToken(); TokenNotNullCheck(Token);
	if (Token->Type == TokenType::Semicolon)
	{
		NextToken();
		out.Expression.Value = nullptr;
		return GotNodeType::Success;
	}
	else
	{
		GetExpressionTypeNode(out.Expression); 
		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
		NextToken();
	}

	return GotNodeType::Success;
}

GotNodeType Parser::GetDeclareStaticVariable(DeclareStaticVariableNode& out)
{
	auto RetToken = TryGetToken();
	TokenTypeCheck(RetToken, TokenType::KeyWorld_static);
	NextToken();

	return GetDeclareVariable(out.Variable);
}

GotNodeType Parser::GetDeclareThreadVariable(DeclareThreadVariableNode& out)
{
	auto RetToken = TryGetToken();
	TokenTypeCheck(RetToken, TokenType::KeyWorld_Thread);
	NextToken();

	return GetDeclareVariable(out.Variable);
}

void Parser::GetDeclareVariableNoObject(TryGetNode& out)
{
	DeclareVariableNode node;
	out.GotNode = GetDeclareVariable(node);
	if (out.GotNode == GotNodeType::Success)
	{
		auto ptr = DeclareThreadVariableNode::Gen();
		_ErrorsOutput->AddError(ErrorCodes::InternalCompilerError, 0, 0, "Cant move varble");
		//ptr->Variable = std::move(node);
		out.Node = ptr->As();
	}
}

GotNodeType Parser::GetDeclareVariable(DeclareVariableNode& out)
{
	auto Type = GetType(out.Type);
	auto Name = GetName(out.Name);

	auto Token = TryGetToken();
	if (Token && Token->Type == TokenType::equal)
	{
		NextToken();
		GetExpressionTypeNode(out.Expression);
	}

	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();

	return Merge(Type, Name);
}
GotNodeType Parser::GetAssignVariable(AssignVariableNode& out)
{
	auto Name = GetName(out.Name);

	auto Token = TryGetToken(); TokenTypeCheck(Token, TokenType::equal);

	NextToken();
	auto Ex =GetExpressionTypeNode(out.Expression);


	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();

	return Merge(Name, Ex);
}
GotNodeType Parser::GetIfNode(IfNode& out)
{
	auto RetToken = TryGetToken();
	TokenTypeCheck(RetToken, Parser::IfToken);
	NextToken();

	auto Token = TryGetToken();
	TokenTypeCheck(Token, Parser::declareFuncParsStart);
	NextToken();

	auto GetEx = GetExpressionTypeNode(out.Expression);
	
	auto Token2 = TryGetToken();
	TokenTypeCheck(Token2, Parser::declareFuncParsEnd);
	NextToken();

	auto Token3 = TryGetToken();
	TokenTypeCheck(Token3, TokenType::Colon);
	NextToken();

	auto Statements = GetStatementsorStatementNode(out.Body);

	while (auto T = TryGetToken())
	{
		if (T->Type != Parser::ElseToken){break;}
		TokenTypeCheck(RetToken, Parser::ElseToken);
		NextToken();

		auto T2 = TryGetToken();
		if (T2->Type == Parser::declareFuncParsStart)
		{

		}
		TokenTypeCheck(Token, Parser::declareFuncParsStart);
		NextToken();

	}

	return Statements;
}
GotNodeType Parser::GetEnumNode(EnumNode& out)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWorld_Enum);
	NextToken();
	GetName(out.EnumName);


	auto LeftBracket = TryGetToken();
	if (LeftBracket && LeftBracket->Type == TokenType::Left_Bracket)
	{
		NextToken();
		GetType(out.BaseType);

		auto RightToken = TryGetToken(); TokenTypeCheck(RightToken, TokenType::Right_Bracket);
		NextToken();
	}
	else
	{
		if (LeftBracket) {
			TypeNode::Gen_Byte(out.BaseType, *LeftBracket);
		}
	}



	auto ColonToken = TryGetToken(); TokenNotNullCheck(ColonToken);
	if (ColonToken->Type == TokenType::Semicolon) { NextToken(); return GotNodeType::Success; }

	TokenTypeCheck(ColonToken, TokenType::Colon); NextToken();

	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);
	NextToken();

	while (auto T = TryGetToken())
	{
		if (T == nullptr || T->Type == TokenType::EndTab) { break; }
		
		out.Values.push_back({});
		EnumValueNode& EnumValue =out.Values.back();
		GetEnumValueNode(EnumValue);


		auto ColonToken = TryGetToken();
		if (ColonToken == nullptr || ColonToken->Type != TokenType::Comma){ break;}
		NextToken();
	}

	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();

	return GotNodeType::Success;
}
GotNodeType Parser::GetEnumValueNode(EnumValueNode& out)
{
	GetName(out.Name);

	auto EqualToken = TryGetToken();
	if (EqualToken && EqualToken->Type == TokenType::equal)
	{
		NextToken();
		return GetExpressionTypeNode(out.Expression);
	}
	return GotNodeType::Success;
}
GotNodeType Parser::GetTagNode(TagTypeNode& out)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWorld_Tag);
	NextToken();
	GetName(out.AttributeName);

	auto ColonToken = TryGetToken(); TokenNotNullCheck(ColonToken);
	if (ColonToken->Type == TokenType::Semicolon) { NextToken(); return GotNodeType::Success;}

	TokenTypeCheck(ColonToken, TokenType::Colon); NextToken();
	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);NextToken();

	while (auto T = TryGetToken())
	{
		TryGetNode V;

		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		default:V = GetDeclareVariable();
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

GotNodeType Parser::GetFuncCallStatementNode(FuncCallStatementNode& out)
{
	GetFuncCallNode(out.Base);

	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();
	return GotNodeType::Success;
}
GotNodeType Parser::GetFuncCallNode(FuncCallNode& out)
{
	GetName(out.FuncName);

	auto ParToken = TryGetToken();
	if (ParToken->Type == TokenType::lessthan)
	{
		TryGetGeneric(out.Generics);
		ParToken = TryGetToken();
	}


	TokenTypeCheck(ParToken, FuncCallStart);
	NextToken();
	
	GetValueParametersNode(out.Parameters);

	auto Par2Token = TryGetToken();
	TokenTypeCheck(Par2Token, FuncCallEnd);
	NextToken();
	
	return GotNodeType::Success;
}

GotNodeType Parser::GetPostfixStatement(PostfixVariableNode& out)
{
	auto Name = GetName(out.Name);
	auto Token = TryGetToken(); TokenNotNullCheck(Token)
	out.PostfixOp = Token;
	NextToken();

	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();
	return GotNodeType::Success;
}
GotNodeType Parser::GetCompoundStatement(CompoundStatementNode& out)
{
	auto Name = GetName(out.VariableName);
	auto Token = TryGetToken(); TokenNotNullCheck(Token)
	out.CompoundOp= Token;
	NextToken();

	GetExpressionTypeNode(out.Expession);

	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();
	return GotNodeType::Success;
}

GotNodeType Parser::GetAnonymousObjectConstructorNode(AnonymousObjectConstructorNode& out)
{
	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, AnonymousObjectStart);
	NextToken();

	GetValueParametersNode(out.Fields);

	auto SemicolonToken2 = TryGetToken(); TokenTypeCheck(SemicolonToken2, AnonymousObjectEnd);
	NextToken();
	return GotNodeType::Success;
}

GotNodeType Parser::GetDropStatementNode(DropStatementNode& out)
{
	auto DropToken = TryGetToken(); TokenTypeCheck(DropToken, TokenType::KeyWorld_Drop);
	NextToken();

	auto ParToken = TryGetToken();
	TokenTypeCheck(ParToken, FuncCallStart);
	NextToken();

	GetExpressionTypeNode(out.expression.Value);

	auto Par2Token = TryGetToken();
	TokenTypeCheck(Par2Token, FuncCallEnd);
	NextToken();

	auto SemicolonToken2 = TryGetToken(); TokenTypeCheck(SemicolonToken2, TokenType::Semicolon);
	NextToken();
	return GotNodeType::Success;
}
GotNodeType Parser::GetNewExpresionNode(NewExpresionNode& out)
{
	auto NewToken = TryGetToken(); TokenTypeCheck(NewToken, TokenType::KeyWorld_new);
	NextToken();
	GetType(out.Type,true);

	auto ParToken = TryGetToken(); TokenNotNullCheck(ParToken);
	if (ParToken->Type == FuncCallStart) {

		NextToken();

		GetValueParametersNode(out.Parameters);

		auto Par2Token = TryGetToken();
		TokenTypeCheck(Par2Token, FuncCallEnd);
		NextToken();
	}
	else if (ParToken->Type == TokenType::Left_Bracket)
	{
		NextToken();
		
		GetExpressionTypeNode(out.Arrayexpression);

		auto Par2Token = TryGetToken();
		TokenTypeCheck(Par2Token, TokenType::Right_Bracket);
		NextToken();
	}
	else
	{
		TokenTypeCheck(ParToken, FuncCallStart);
	}
	return GotNodeType::Success;
}
UCodeLangEnd