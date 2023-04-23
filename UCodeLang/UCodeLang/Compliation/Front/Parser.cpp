#include "Parser.hpp"
#include "../Helpers/KeyWords.hpp"
#include "..\UAssembly\UAssembly_Parser.hpp"
UCodeLangFrontStart


void Parser::Reset()
{
	auto ErrorsOutput = _ErrorsOutput;
	auto Settings = _Settings;

	this->~Parser();
	new (this) Parser;

	this->_ErrorsOutput = ErrorsOutput;
	this->_Settings = Settings;
}



void Parser::Parse(const Vector<Token>&Tokens)
{
	Reset();
	_Nodes = &Tokens;
	
	

	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();
		TryGetNode V;


		switch (T->Type)
		{
		case TokenType::Namespace:V = GetNamespaceNode(); break;
		case TokenType::KeyWord_Tag:V = GetTagNode(); break;
		case TokenType::KeyWord_Enum:V = GetEnumNode(); break;
		case TokenType::Class:V = GetClassNode(); break;
		case Parser::declareFunc:V = GetFuncNode(); break;
		case TokenType::KeyWord_use:V = GetUseNode(); break;
		case TokenType::Left_Bracket:V = GetAttribute(); break;
		case TokenType::KeyWord_static:V = GetDeclareStaticVariable(); break;
		case TokenType::KeyWord_Thread:V = GetDeclareThreadVariable(); break;
		case TokenType::KeyWord_umut:V = GetumutVariableDeclare(); break;
		case TokenType::KeyWord_trait:V = GetTraitNode(); break;
		default: GetDeclareVariableNoObject(V); break;
		}
		if (V.Node)
		{
			_Tree._Nodes.push_back(Unique_ptr<Node>(V.Node));
		}
		else { break; }
		if (V.GotNode != GotNodeType::Success) { break; }
	}
	_ParseSuccess= !_ErrorsOutput->Has_Errors();
}


void Parser::TokenTypeCheck(const Token* Value, TokenType Type)
{
	if (Value->Type != Type)
	{
		if (!_HasTripped) 
		{
			auto& Error = _ErrorsOutput->AddError(ErrorCodes::ExpectingToken, Value->OnLine, Value->OnPos);

			if (Value->Type == TokenType::Name) {

				Error._Msg = "Expecting " + (String)StringHelper::ToString(Type) +
					" Got " + (String)Value->Value._String;
			}
			else
			{
				Error._Msg = "Expecting " + (String)StringHelper::ToString(Type) +
					" Got " + (String)StringHelper::ToString(Value->Type);
			}
			Tripped();
		}
	}
}

GotNodeType Parser::GetNamespaceNode(NamespaceNode& out)
{
	auto NamespaceToken = TryGetToken(); TokenTypeCheck(NamespaceToken, TokenType::Namespace);
	NextToken();

	auto ScopeResolutionToken = TryGetToken(); 
	ScopedName V;
	V.token = NamespaceToken;
	
	out.NamespaceName.ScopedName.push_back(std::move(V));

	NamespaceNode* tepout = &out;

	while (ScopeResolutionToken->Type == TokenType::ScopeResolution)
	{
		NextToken();

		Unique_ptr<NamespaceNode> TepNamespace = Unique_ptr<NamespaceNode>(NamespaceNode::Gen());
		auto Ptr = TepNamespace.get();
		tepout->_Nodes.push_back(std::move(TepNamespace));

		
		tepout = Ptr;
		
		out.NamespaceName.ScopedName.back().Operator =ScopedName::Operator_t::ScopeResolution;
		GetName(out.NamespaceName);
		ScopeResolutionToken = TryGetToken();
	}



	auto ColonToken = TryGetToken(); TokenTypeCheck(ColonToken, TokenType::Colon);
	NextToken();

	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);
	NextToken();


	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();
		TryGetNode V;

		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		case TokenType::Namespace:V = GetNamespaceNode(); break;
		case TokenType::KeyWord_Tag:V = GetTagNode(); break;
		case TokenType::KeyWord_Enum:V = GetEnumNode(); break;
		case TokenType::Class:V = GetClassNode();break;
		case Parser::declareFunc:V = GetFuncNode(); break;
		case TokenType::KeyWord_use:V = GetUseNode(); break;
		case TokenType::Left_Bracket:V = GetAttribute();break;
		case TokenType::KeyWord_static:V = GetDeclareStaticVariable(); break;
		case TokenType::KeyWord_Thread:V = GetDeclareThreadVariable(); break;
		case TokenType::KeyWord_umut:V = GetumutVariableDeclare(); break;
		case TokenType::KeyWord_trait:V = GetTraitNode(); break;
		default: GetDeclareVariableNoObject(V); break;
		}

		if (V.Node)
		{
			tepout->_Nodes.push_back(Unique_ptr<Node>(V.Node));
		}
		else { break; }
		if (V.GotNode != GotNodeType::Success) { break; }
	}
	EndLoop:
	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();

	return GotNodeType::Success;
}
GotNodeType Parser::GetAlias(const Token* AliasName,GenericValuesNode&& AliasGenerics, AliasNode& out)
{
	out.AliasName.Token = AliasName;
	out.Generic = AliasGenerics;

	auto ClassToken = TryGetToken(); TokenTypeCheck(ClassToken, TokenType::equal);
	NextToken();

	if (TryGetToken()->Type == TokenType::Not)
	{
		out.IsHardAlias = true;
		NextToken();
	}

	GotNodeType r;
	auto FirtToken = TryGetToken();
	if (FirtToken->Type == declareFunc)
	{
		NextToken();

		out._Type = AliasType::Func;
		auto Func = new AliasNode_Func();
		out._Node.reset(Func);

		auto LPToken = TryGetToken();
		TokenTypeCheck(LPToken, declareFuncParsStart);
		NextToken();

		auto Parameters = GetNamedParametersNode(Func->Parameters);

		auto RPToken = TryGetToken();
		TokenTypeCheck(RPToken, declareFuncParsEnd);
		NextToken();

		auto Arrow = TryGetToken();
		TokenTypeCheck(Arrow, TokenType::RightArrow);
		NextToken();

		r = GetTypeWithVoid(Func->ReturnType);
	}
	else 
	{
		r = GetType(out.Type);
	}
	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();

	return r;
}
GotNodeType Parser::GetClassTypeNode(Node*& out)
{
	auto ClassToken = TryGetToken(); TokenTypeCheck(ClassToken, TokenType::Class);

	SetNotTopScope();

	const String_view& ClassName = ClassToken->Value._String;
	
	
	NextToken();

	GenericValuesNode TepGenerics;

	TryGetGeneric(TepGenerics);



	auto LeftBracket = TryGetToken();
	if (LeftBracket && LeftBracket->Type == TokenType::Left_Bracket)
	{

		auto output = ClassNode::Gen(); out = output->As();
		output->ClassName.Token = ClassToken;
		output->Generic = std::move(TepGenerics);
		NextToken();

		do
		{
			NameNode V;
			GetName(V);
			output->Inherited.Values.push_back(std::move(V));


			auto CToken = TryGetToken();
			if (CToken->Type == TokenType::Comma)
			{
				NextToken();
				continue;
			}
			else
			{
				break;
			}

		} while (true);

		auto RightToken = TryGetToken(); TokenTypeCheck(RightToken, TokenType::Right_Bracket);
		NextToken();

		auto ColonToken = TryGetToken();

		if (ColonToken->Type == TokenType::Semicolon)
		{
			NextToken();
			return GotNodeType::Success;
		}
		else 
		{
			return DoClassType(output, ClassToken, TepGenerics, ColonToken);
		}
	}
	
	
		


	auto ColonToken = TryGetToken();
	if (ColonToken->Type == TokenType::equal)
	{
		auto V = AliasNode::Gen();
		out = V->As();
		return GetAlias(ClassToken,std::move(TepGenerics), *V);
	}
	else if (ColonToken->Type == TokenType::Semicolon)
	{
		NextToken();

		auto output = ClassNode::Gen(); out = output->As();
		output->ClassName.Token = ClassToken;
		output->Generic = std::move(TepGenerics);
		return GotNodeType::Success;
	}
	else
	{
		auto output = ClassNode::Gen();out = output->As();
		return DoClassType(output, ClassToken, TepGenerics, ColonToken);
	}


	UnSetTopScope();
}

GotNodeType Parser::DoClassType(ClassNode* output, const Token* ClassToken, GenericValuesNode& TepGenerics, const Token* ColonToken)
{
	output->ClassName.Token = ClassToken;
	output->Generic = std::move(TepGenerics);

	TokenTypeCheck(ColonToken, TokenType::Colon);
	NextToken();

	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);
	NextToken();

	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();
		TryGetNode V;

		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		case TokenType::KeyWord_Tag:V = GetTagNode(); break;
		case TokenType::KeyWord_Enum:V = GetEnumNode(); break;
		case TokenType::Class:V = GetClassNode(); break;
		case Parser::declareFunc:V = GetFuncNode(); break;
		case TokenType::KeyWord_use:V = GetUseNode(); break;
		case TokenType::Left_Bracket:V = GetAttribute(); break;
		case TokenType::KeyWord_static:V = GetDeclareStaticVariable(); break;
		case TokenType::KeyWord_Thread:V = GetDeclareThreadVariable(); break;
		case TokenType::KeyWord_umut:V = GetumutVariableDeclare(); break;
		case TokenType::KeyWord_trait:V = GetTraitNode(); break;
		default:V = GetDeclareVariable();
		}

		if (V.Node)
		{
			output->_Nodes.push_back(Unique_ptr<Node>(V.Node));
		}
		else { break; }
		if (V.GotNode != GotNodeType::Success) { break; }
	}

EndLoop:
	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();

	return GotNodeType::Success;
}

GotNodeType Parser::GetStatementsorStatementNode(StatementsNode& out)
{
	auto TabToken = TryGetToken();

	if (TabToken->Type == TokenType::StartTab)
	{
		return GetStatements(out);
	}
	else
	{
		Node* nodeptr = nullptr;

		auto r= GetStatement(nodeptr);
		if (nodeptr) 
		{
			out._Nodes.push_back(Unique_ptr<Node>(nodeptr));
		}
		TrippedCheck(r);
		return r;
	}
}

GotNodeType Parser::GetStatement(Node*& out)
{
	auto StatementTypeToken = TryGetToken();
	switch (StatementTypeToken->Type)
	{
	case TokenType::EndTab:return GotNodeType::EndLoop;
	case Parser::declareFuncParsStart:
	{
		NextToken();
		auto r = GetStatements();

		TokenTypeCheck(TryGetToken(), Parser::declareFuncParsStart);
		NextToken();

		out = r.Node;
		return r.GotNode;
	};
	case TokenType::KeyWord_Tag:
	{
		auto r = GetTagNode();
		out = r.Node;
		return r.GotNode;
	};
	case TokenType::KeyWord_Enum:
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
	case TokenType::KeyWord_use:
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
	case TokenType::KeyWord_static:
	{
		auto r = GetDeclareStaticVariable();
		out = r.Node;
		return r.GotNode;
	}
	case TokenType::KeyWord_Thread:
	{
		auto r = GetDeclareThreadVariable();
		out = r.Node;
		return r.GotNode;
	}
	break;
	case  TokenType::KeyWord_while:
	{
		auto r =GetWhileNode();
		out = r.Node;
		return r.GotNode;
	}
	break;
	case  TokenType::KeyWord_do:
	{
		auto r = GetDoNode();
		out = r.Node;
		return r.GotNode;
	}
	break;
	case  TokenType::KeyWord_If:
	{
		auto r = GetIfNode();
		out = r.Node;
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_continue:
	{
		auto r = GetContinueNode();
		out = r.Node;
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_break:
	{
		auto r = GetBreakNode();
		out = r.Node;
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_for:
	{
		auto r = GetForNode();
		out = r.Node;
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_Drop:
	{
		auto r = GetDropStatementNode();
		out = r.Node;
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_umut:
	{
		auto r = GetumutVariableDeclare();
		out = r.Node;
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_trait:
	{
		auto r = GetTraitNode();
		out = r.Node;
		return r.GotNode;
	};
	break;
	case TokenType::KeyWord_panic:
	{
		auto r = GetPanicNode();
		out = r.Node;
		return r.GotNode;
	}
	break;
	default:
	{
		size_t OldIndex = _TokenIndex;
		ScopedNameNode Tep;
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
				auto _Token2 = TryGetToken();

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
			else if (ExpressionNodeType::IsPostfixOperator(_Token))
			{
				r =GetPostfixStatement();
			}
			else if (ExpressionNodeType::IsCompoundOperator(_Token))
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

	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();
		if (T->Type == TokenType::EndTab) { break; }

		Node* V = nullptr;
		auto R = GetStatement(V);
		if (V) { out._Nodes.push_back(Unique_ptr<Node>(V)); }

		TrippedCheck(R);
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

	auto ColonToken = TryGetToken(); 

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
		V.Statements._Nodes.back() = Unique_ptr<Node>(r);

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

	if (NameToken->Type == TokenType::KeyWord_new
		|| NameToken->Type == TokenType::KeyWord_Drop
		|| ExpressionNodeType::IsOverLoadableOperator(NameToken))
	{
		NextToken();
	}
	else if (NameToken->Type == TokenType::Left_Bracket)
	{
		NextToken();
		TokenTypeCheck(TryGetToken(), TokenType::Right_Bracket);
		NextToken();
	}
	else if (NameToken->Type == TokenType::Left_Parentheses)
	{
		NextToken();
		TokenTypeCheck(TryGetToken(), TokenType::Right_Parentheses);
		NextToken();
	}
	else if (NameToken->Type == TokenType::KeyWord_for)
	{
		NextToken();
	}
	else if (NameToken->Type == TokenType::RightArrow)
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
	auto Arrow = TryGetToken(); 
	if (Arrow->Type == TokenType::RightArrow)
	{
		NextToken();
		auto RetToken = TryGetToken(); 
		if (RetToken->Type == AnonymousObjectStart)
		{
			NextToken();
			auto node = AnonymousTypeNode::Gen();
			out.ReturnType.node = Unique_ptr<Node>(node);
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
	Node* nodeptr = nullptr;
	auto r= GetExpressionNode(nodeptr);
	out.Value = Unique_ptr<Node>(nodeptr);
	return r;
}
GotNodeType Parser::GetExpressionTypeNode(ExpressionNodeType& out)
{
	Node* nodeptr = nullptr;
	auto r = GetExpressionTypeNode(nodeptr);

	out.Value = Unique_ptr<Node>(nodeptr);
	return r;
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
	case TokenType::Float_literal:
	{
		NextToken();
		auto r = FloatliteralNode::Gen();
		r->Token = StatementTypeToken;
		out = r->As();
		return GotNodeType::Success;
	}
	case TokenType::Char_literal:
	{
		NextToken();
		auto r = CharliteralNode::Gen();
		r->Token = StatementTypeToken;
		out = r->As();
		return GotNodeType::Success;
	}
	case TokenType::KeyWorld_True:
	case TokenType::KeyWorld_False:
	{
		NextToken();
		auto r = BoolliteralNode::Gen();
		r->Token = StatementTypeToken;
		out = r->As();
		return GotNodeType::Success;
	}
	case AnonymousObjectStart:
	{
		
		size_t ThisIndex = _TokenIndex;
		NextToken();
		
		bool ParenthesesExpresion = true;
		while (TryGetToken()->Type == TokenType::Name)
		{
			NextToken();

			if (TryGetToken()->Type == TokenType::Comma)
			{
				NextToken();
			}
			else
			{
				break;
			}
		}

		if (TryGetToken()->Type == TokenType::Right_Bracket)
		{
			NextToken();
			auto Other = TryGetToken()->Type;
			ParenthesesExpresion = 
				!(Other == TokenType::RightAssignArrow || Other == TokenType::Colon);
		}

		_TokenIndex = ThisIndex;

		if (ParenthesesExpresion)
		{
			auto r = GetAnonymousObjectConstructorNode();
			out = r.Node;
			return r.GotNode;
		}
		else
		{
			NextToken();
			auto r = GetShortLambdaNode();
			out = r.Node;
			return r.GotNode;
		}
	}
	TokenType_Name:
	case TokenType::Name:
	{
		size_t OldIndex = _TokenIndex;
		ScopedNameNode Tep;
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
	break;
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
	case TokenType::KeyWord_new:
	{
		auto V = GetNewExpresionNode();
		out = V.Node;
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_move:
	{
		NextToken();
		auto V = MoveNode::Gen();	out = V;
		GetExpressionTypeNode(V->expression);
	
		return  GotNodeType::Success;
	}
	break;
	case Parser::declareFunc:
	{
		auto V = GetLambdaNode();
		out = V.Node;
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_bitcast:
	{
		auto V = GetBitCastExpression();
		out = V.Node;
		return V.GotNode;
	}
	break;
	default:

		if (TypeNode::IsType(StatementTypeToken->Type))
		{
			goto TokenType_Name;
		}
		else 
		{
			TokenTypeCheck(StatementTypeToken, TokenType::Name);
		}

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
	if (Token && ExpressionNodeType::IsBinaryOperator(Token))
	{
		NextToken();
		Node* Other = nullptr;
		auto Ex2 = GetExpressionTypeNode(Other);

		auto r = BinaryExpressionNode::Gen();

		auto Ptr = ValueExpressionNode::Gen();
		Ptr->Value = Unique_ptr<Node>(ExNode);
		r->Value0.Value= Unique_ptr<Node>(Ptr->As());
		
		r->BinaryOp = Token;
		r->Value1.Value = Unique_ptr<Node>(Other);
		r_out = r->As();
		r_t = Merge(Ex, Ex2);
	}
	else
	{
		auto Ptr = ValueExpressionNode::Gen();
		Ptr->Value = Unique_ptr<Node>(ExNode);
		r_out = Ptr->As();
		r_t = Ex;
	}

	auto Token2 = TryGetToken();
	while (Token2->Type == TokenType::RightArrow
		|| Token2->Type == TokenType::Left_Bracket)
	{
		NextToken();

		if (Token2->Type == TokenType::RightArrow) 
		{
			auto cast = CastNode::Gen();


			auto Type = GetType(cast->ToType);
			cast->Expression.Value = Unique_ptr<Node>(r_out);
			r_t = Merge(Ex, Type);
			r_out = cast->As();
			Token2 = TryGetToken();
		}
		else if (Token2->Type == TokenType::Left_Bracket)
		{
			auto IndexExNode = IndexedExpresionNode::Gen();
			auto V = GetExpressionTypeNode(IndexExNode->IndexExpression);
			IndexExNode->SourceExpression.Value = Unique_ptr<Node>(r_out);

			r_out = IndexExNode->As();

			Token2 = TryGetToken();
			TokenTypeCheck(Token2, TokenType::Right_Bracket);
			NextToken();

			Token2 = TryGetToken();
		}
		else
		{
			throw std::exception("not added");
		}
	}

	out = r_out;
	return r_t;
}
GotNodeType Parser::GetValueParameterNode(Node*& out)
{
	if (TryGetToken()->Type == TokenType::KeyWord_out)
	{
		NextToken();

		size_t oldindex = this->_TokenIndex;
		NextToken();

		bool HasType = false;


		auto ParToken = TryGetToken();
		HasType =!(ParToken->Type == TokenType::Comma || ParToken->Type == TokenType::Right_Parentheses);


		this->_TokenIndex = oldindex;


		auto B = OutExpression::Gen();
		out = B->As();

		if (HasType)
		{
			GetType(B->_Type);
		}
		else
		{
			TypeNode::Gen_Var(B->_Type, *TryGetToken());
		}


		return GetName(B->_Name);
	}
	else
	{
		return GetExpressionTypeNode(out);//Just for consistency.
	}
}
GotNodeType Parser::GetValueParametersNode(ValueParametersNode& out)
{
	auto _Token = TryGetToken();
	if (_Token && (_Token->Type == FuncCallEnd
		|| _Token->Type == TokenType::Right_Bracket))
	{
		return GotNodeType::Success;
	}

	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		

		Node* node = nullptr;
		auto Ex = GetValueParameterNode(node);
		out._Nodes.push_back(Unique_ptr<Node>(node));
	
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

	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		NamedParameterNode Tep;
		auto Token = TryGetToken();
		if (!Token || Token->Type == TokenType::Right_Bracket) { break; }


		GetType(Tep.Type,false,false);
		
		auto Token2 = TryGetToken();
		if (Token2->Type == TokenType::Comma || Token2->Type == TokenType::Right_Bracket)
		{
			if (out.Parameters.size() != 0)
			{
				_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "this& Must be the first parameter");
			}
			
		}
		else 
		{
			GetName(Tep.Name);
		}
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
	if (token->Type == TokenType::lessthan)
	{
		NextToken();


		while (TryGetToken()->Type != TokenType::EndofFile)
		{
			GenericValueNode Item;
			auto NameToken = TryGetToken();
			
			if (NameToken->Type == TokenType::Left_Parentheses)
			{
				NextToken();
				NameToken = TryGetToken();
				TokenTypeCheck(NameToken, TokenType::Name);

				NextToken();
				auto RPar = TryGetToken();
				TokenTypeCheck(RPar, TokenType::Right_Parentheses);

				Item.Token = NameToken;
				Item.IsConstantExpression = true;
				out.Values.push_back(std::move(Item));
			}
			else
			{
				TokenTypeCheck(NameToken, TokenType::Name);

				Item.Token = NameToken;
				out.Values.push_back(std::move(Item));
			}

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
	
	if (token->Type == TokenType::lessthan)
	{
		NextToken();


		while (TryGetToken()->Type != TokenType::EndofFile)
		{
			if (TryGetToken()->Type == TokenType::Left_Parentheses) 
			{
				NextToken();
				TypeNode Item;
				TypeNode::Gen_Expression(Item, *TryGetToken());

				ExpressionNodeType* node = new ExpressionNodeType();
				GetExpressionTypeNode(*node);
				
				Item.node.reset(node);

				auto Rtoken = TryGetToken();
				TokenTypeCheck(Rtoken, TokenType::Right_Parentheses);
				NextToken();
				out.Values.push_back(std::move(Item));
			}
			else 
			{
				TypeNode Item;
				GetType(Item);
				out.Values.push_back(std::move(Item));
			}
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

	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		ScopedName V;
		auto NameToken = TryGetToken();

		if (!TypeNode::IsType(NameToken->Type)) 
		{
			TokenTypeCheck(NameToken, TokenType::Name);
		}

		V.token = NameToken;

		
		NextToken();


		auto Token = TryGetToken();
		if (Token == nullptr || !ScopedName::Get_IsScoped(Token->Type))
		{
			out.ScopedName.push_back(std::move(V));
			break;
		}

		V.Operator = ScopedName::Get_Scoped(Token->Type);
		out.ScopedName.push_back(std::move(V));
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

Parser::GetNameCheck_ret Parser::GetNameCheck(ScopedNameNode& out)
{
	NameCheck_t LookingAtT = NameCheck_t::Name;


	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		ScopedName V;
		UseGenericsNode Generic;
		auto NameToken = TryGetToken();

		if (NameToken == nullptr 
			|| 
			(
				NameToken->Type != TokenType::Name && !(NameToken->Type == TokenType::KeyWord_This && out.ScopedName.size() == 0)
			)
			)
		{
			NextToken();
			return { LookingAtT,GotNodeType::failed };
		}

		V.token = NameToken;

		NextToken();
		TryGetGeneric(Generic);




		auto Token = TryGetToken();


		if (Token) 
		{
			V.Operator = ScopedName::Get_Scoped(Token->Type);
		}
		V.Generic = std::move(std::make_unique<UseGenericsNode>(std::move(Generic)));

		bool MemAccess = V.Operator == ScopedName::Operator_t::Dot
			|| V.Operator == ScopedName::Operator_t::IndirectMember
			|| V.Operator == ScopedName::Operator_t::OptionalChain;

		if (out.ScopedName.size())
		{
			auto& Back = out.ScopedName.back();

			if (LookingAtT == NameCheck_t::MemberAccess && V.Generic.get()->Values.size())
			{
				auto Token = out.ScopedName[0].token;
				_ErrorsOutput->AddError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos,
					"generic cant be with a Memberaccess operator '" + (String)StringHelper::ToString(Token->Type) + "'");
			}
		}
		if (MemAccess)
		{
			LookingAtT = NameCheck_t::MemberAccess;
		}
		
		out.ScopedName.push_back(std::move(V));
		if (Token == nullptr || !ScopedName::Get_IsScoped(Token->Type))
		{
			break;
		}
		




		NextToken();
	}
	return{ LookingAtT,GotNodeType::Success };
}

GotNodeType Parser::GetType(TypeNode*& out, bool ignoreRighthandOFtype, bool ignoreleftHandType)
{
	GotNodeType r = GotNodeType::Success;
	auto Token = TryGetToken();
	if (!ignoreleftHandType)
	{
		if (Token->Type == TokenType::KeyWord_umut)
		{
			out->SetAsimmutable();
			NextToken();
			Token = TryGetToken();
		}
		else
		if (Token->Type == TokenType::KeyWord_moved)
		{
				out->SetMovedType();
				NextToken();
				Token = TryGetToken();
		}
		
		
		
		if (Token->Type == TokenType::KeyWord_dynamic)
		{
			out->SetDynamic();
			NextToken();
			Token = TryGetToken();
		   
			TokenTypeCheck(Token, TokenType::lessthan);
			NextToken();

			Token = TryGetToken();
		}
	}

	if (Token->Type == TokenType::Name)
	{
		auto A = GetName(out->Name);
		auto B = TryGetGeneric(out->Generic);
		r = Merge(A,B);
	}
	else if (TypeNode::IsType(Token->Type))
	{
		NextToken();
		out->Name.Token = Token;
		r = GotNodeType::Success;
	}
	else
	{
		TokenTypeCheck(Token, TokenType::Name);
	}
	

	auto Token2 = TryGetToken();
	while (!ignoreRighthandOFtype && Token2)
	{
		if (Token2->Type == TokenType::bitwise_and)//int&
		{
			NextToken();
			out->SetAsAddess();
			break;
		}
		else if (Token2 && Token2->Type == TokenType::Left_Bracket)
		{
			bool Break = false;
			NextToken();
			Token2 = TryGetToken();
			if (Token2->Type == TokenType::bitwise_and)//int[&]
			{
				NextToken();

				Token2 = TryGetToken();
				out->SetAsArrayAddess();
				Break = true;
			}
			else if (Token2->Type == TokenType::forwardslash)//int[/10] or int[/]  
			{
				NextToken();
				auto _NextToken = TryGetToken();
				if (_NextToken->Type != TokenType::Right_Bracket)//Array expression
				{
					auto ExNode = new ExpressionNodeType();
					out->node.reset(ExNode);
					auto Ex = GetExpressionTypeNode(*ExNode);
				}
				out->SetAsStaticArray();
				Break = true;
			}

			else if (Token2->Type == TokenType::Colon)//int[:] => View<int>
			{
				NextToken();
				TypeNode* New = new TypeNode();

				New->Isimmutable = out->Isimmutable;
				out->Isimmutable = false;
				New->Generic.Values.push_back(std::move(*out));

				auto NameToken = new UCodeLang::Token();
				NameToken->OnLine = Token2->OnLine;
				NameToken->OnPos = Token2->OnPos;
				NameToken->Type = TokenType::Name;
				NameToken->Value._String = UCode_ViewType;

				_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

				New->Name.Token = NameToken;
				out = New;
			}
			else if (Token2->Type == TokenType::Right_Bracket)//int[] => Vector<int>
			{
				TypeNode* New = new TypeNode();

				New->Isimmutable = out->Isimmutable;
				out->Isimmutable = false;
				New->Generic.Values.push_back(std::move(*out));

				auto NameToken = new UCodeLang::Token();
				NameToken->OnLine = Token2->OnLine;
				NameToken->OnPos = Token2->OnPos;
				NameToken->Type = TokenType::Name;
				NameToken->Value._String = UCode_VectorType;

				_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

				New->Name.Token = NameToken;
				out = New;
			}
			else//int[0] => Array<int,(0)>
			{
				//NextToken();
				

				TypeNode* New = new TypeNode();
				{
					New->Isimmutable = out->Isimmutable;
					out->Isimmutable = false;
					New->Generic.Values.push_back(std::move(*out));

					auto NameToken = new UCodeLang::Token();
					NameToken->OnLine = Token2->OnLine;
					NameToken->OnPos = Token2->OnPos;
					NameToken->Type = TokenType::Name;
					NameToken->Value._String = UCode_ArrayType;

					_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

					New->Name.Token = NameToken;
				}

				{//Array expression
					auto ExTypeNode = TypeNode();
					TypeNode::Gen_Expression(ExTypeNode, *TryGetToken());


					auto ExNode = new ExpressionNodeType();
					ExTypeNode.node.reset(ExNode);
					auto Ex = GetExpressionTypeNode(*ExNode);
					
					New->Generic.Values.push_back(std::move(ExTypeNode));
				}


				out = New;
			}



			Token2 = TryGetToken();
			TokenTypeCheck(Token2, TokenType::Right_Bracket);
			NextToken();
			if (Break)
			{
				break;
			}
			Token2 = TryGetToken();
		}
		else if (Token2->Type == TokenType::QuestionMark)//int? => Optional<int>
		{
			NextToken();
			TypeNode* New = new TypeNode();

			New->Isimmutable = out->Isimmutable;
			out->Isimmutable = false;
			New->Generic.Values.push_back(std::move(*out));

			auto NameToken = new UCodeLang::Token();
			NameToken->OnLine = Token2->OnLine;
			NameToken->OnPos = Token2->OnPos;
			NameToken->Type = TokenType::Name;
			NameToken->Value._String = UCode_OptionalType;

			_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

			New->Name.Token = NameToken;
			out = New;

			Token2 = TryGetToken();
		}
		else
		{
			break;
		}

	}
	
	if (out->IsDynamic)
	{
		TokenTypeCheck(TryGetToken(), TokenType::greaterthan);
		NextToken();
	}

	return r;
}

GotNodeType Parser::GetType(TypeNode& out, bool ignoreRighthandOFtype, bool ignoreleftHandType)
{
	TypeNode tep;
	TypeNode* tepptr = &tep;
	auto r = GetType(tepptr,ignoreRighthandOFtype, ignoreleftHandType);

	out = std::move(*tepptr);

	return r;
}
GotNodeType Parser::GetTypeWithVoid(TypeNode& out)
{
	auto token = TryGetToken();
	
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

	auto ParToken = TryGetToken();

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
	auto UseToken = TryGetToken(); TokenTypeCheck(UseToken, TokenType::KeyWord_use);
	NextToken();

	GetName(out.ScopedName);


	auto SemicolonToken = TryGetToken();TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();

	return GotNodeType::Success;
}


GotNodeType Parser::GetRetStatement(RetStatementNode& out)
{
	auto RetToken = TryGetToken();
	TokenTypeCheck(RetToken, TokenType::KeyWorld_Ret);
	NextToken();

	auto Token = TryGetToken(); 
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

GotNodeType Parser::GetDeclareStaticVariable(DeclareStaticVariableNode& out, bool ignoreleftHandType)
{
	auto RetToken = TryGetToken();
	TokenTypeCheck(RetToken, TokenType::KeyWord_static);
	NextToken();

	size_t OldIndex = _TokenIndex;

	NameNode NameValue;
	GetNameCheck(NameValue);
	auto Token3 = TryGetToken();
	if (Token3->Type == TokenType::equal)
	{
		NextToken();

		TypeNode::Gen_Var(out.Variable.Type, *NameValue.Token);
		out.Variable.Name = std::move(NameValue);
		GetExpressionTypeNode(out.Variable.Expression);

		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
		NextToken();
		return GotNodeType::Success;
	}
	else
	{
		_TokenIndex = OldIndex;
		return GetDeclareVariable(out.Variable, ignoreleftHandType);
	}
}

GotNodeType Parser::GetDeclareThreadVariable(DeclareThreadVariableNode& out, bool ignoreleftHandType)
{
	auto RetToken = TryGetToken();
	TokenTypeCheck(RetToken, TokenType::KeyWord_Thread);
	NextToken();


	size_t OldIndex = _TokenIndex;

	NameNode NameValue;
	GetNameCheck(NameValue);
	auto Token3 = TryGetToken();
	if (Token3->Type == TokenType::equal) 
	{
		NextToken();

		TypeNode::Gen_Var(out.Variable.Type, *NameValue.Token);
		out.Variable.Name = std::move(NameValue);
		GetExpressionTypeNode(out.Variable.Expression);

		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
		NextToken();
		return GotNodeType::Success;
	}
	else
	{
		_TokenIndex = OldIndex;
		return GetDeclareVariable(out.Variable, ignoreleftHandType);
	}
}

void Parser::GetDeclareVariableNoObject(TryGetNode& out)
{
	DeclareVariableNode node;
	out.GotNode = GetDeclareVariable(node);
	if (out.GotNode == GotNodeType::Success)
	{
		auto ptr = DeclareThreadVariableNode::Gen();
		//ptr->Variable = std::move(node);
		ptr->Variable.Type = std::move(node.Type);
		ptr->Variable.Name = std::move(node.Name);
		ptr->Variable.Expression.Value = std::move(node.Expression.Value);
		
		out.Node = ptr->As();
	}
}

GotNodeType Parser::GetDeclareVariable(DeclareVariableNode& out, bool ignoreleftHandType)
{
	auto Type = GetType(out.Type,false, ignoreleftHandType);
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
	TokenTypeCheck(RetToken, TokenType::KeyWord_If);
	NextToken();

	auto GetEx = GetExpressionTypeNode(out.Expression);

	auto Token3 = TryGetToken();
	TokenTypeCheck(Token3, TokenType::Colon);
	NextToken();

	auto Statements = GetStatementsorStatementNode(out.Body);


	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();
	
		if (T->Type != TokenType::KeyWord_Else){break;}
		
		NextToken();
		auto T2 = TryGetToken();
		
		if (T2->Type != TokenType::Colon)
		{//else if or Item();
			auto elseNode = std::make_unique<ElseNode>();

			//auto RetToken2 = TryGetToken();
			//TokenTypeCheck(RetToken2, TokenType::KeyWorld_If);
			
			Node* V;
			auto Statements = GetStatement(V);
			if (V)
			{
				elseNode->Body._Nodes.push_back(Unique_ptr<Node>(V));
			}

			out.Else = std::move(elseNode);
			break;
		}
		else
		{//else
			auto elseNode = std::make_unique<ElseNode>();

			auto ColonToken = TryGetToken();
			TokenTypeCheck(ColonToken, TokenType::Colon);
			NextToken();

			auto Statements = GetStatementsorStatementNode(elseNode->Body);

			out.Else = std::move(elseNode);

			break;
		}

	}

	return Statements;
}
GotNodeType Parser::GetWhileNode(WhileNode& out)
{
	auto RetToken = TryGetToken();
	TokenTypeCheck(RetToken, TokenType::KeyWord_while);
	NextToken();

	auto GetEx = GetExpressionTypeNode(out.Expression);

	auto Token3 = TryGetToken();
	if (Token3->Type != TokenType::Semicolon)
	{
		TokenTypeCheck(Token3, TokenType::Colon);
		NextToken();

		auto Statements = GetStatementsorStatementNode(out.Body);
	}
	else
	{
		NextToken();
	}

	return GotNodeType::Success;
}
GotNodeType Parser::GetDoNode(DoNode& out)
{
	auto RetToken = TryGetToken();
	TokenTypeCheck(RetToken, TokenType::KeyWord_do);
	NextToken();

	auto Token4 = TryGetToken();
	TokenTypeCheck(Token4, TokenType::Colon);
	NextToken();
	

	auto TabToken = TryGetToken();

	if (TabToken->Type != TokenType::KeyWord_while)
	{
		auto Statements = GetStatementsorStatementNode(out.Body);
	}

	auto Token3 = TryGetToken();
	TokenTypeCheck(Token3, TokenType::KeyWord_while);
	NextToken();

	auto GetEx = GetExpressionTypeNode(out.Expression);

	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();
	return GotNodeType::Success;
}
GotNodeType Parser::GetEnumNode(EnumNode& out)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWord_Enum);
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



	auto ColonToken = TryGetToken();
	if (ColonToken->Type == TokenType::Semicolon) { NextToken(); return GotNodeType::Success; }

	TokenTypeCheck(ColonToken, TokenType::Colon); NextToken();

	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);
	NextToken();


	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();
	
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


	auto BToken = TryGetToken();
	AnonymousTypeNode* ClassType = nullptr;
	if (BToken->Type == TokenType::Left_Bracket)
	{
		NextToken();


		while (true)
		{
			TypeNode V;
			GetType(V);
			

			auto CToken = TryGetToken();
			if (ClassType || CToken->Type == TokenType::Name)
			{
				NamedParameterNode pars;
				if (CToken->Type == TokenType::Name)
				{
					pars.Name.Token = CToken;
					NextToken();
				}
				else
				{
					pars.Name.Token = out.Name.Token;
				}

				if (ClassType == nullptr) {
					ClassType = AnonymousTypeNode::Gen();
				}

				
				
				pars.Type = std::move(V);
				ClassType->Fields.Parameters.push_back(std::move(pars));


				auto ColonToken = TryGetToken();
				if (ColonToken == nullptr || ColonToken->Type != TokenType::Comma) { break; }
				NextToken();
			}
			else
			{
				if (CToken->Type == TokenType::Comma)
				{
					if (ClassType == nullptr) {
						ClassType = AnonymousTypeNode::Gen();
					}

					NamedParameterNode pars;
					pars.Name.Token = out.Name.Token;
					pars.Type = std::move(V);
					ClassType->Fields.Parameters.push_back(std::move(pars));
					NextToken();
					continue;
				}

				out.VariantType = std::move(V);
				break;
			}

			

		}
		if (ClassType)
		{
			TypeNode V;
			V.node.reset(ClassType);
			out.VariantType = std::move(V);
		}

		auto RToken = TryGetToken();
		TokenTypeCheck(RToken, TokenType::Right_Bracket);
		NextToken();
	}

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
	TokenTypeCheck(Token, TokenType::KeyWord_Tag);
	NextToken();
	GetName(out.AttributeName);

	auto ColonToken = TryGetToken();
	if (ColonToken->Type == TokenType::Semicolon) { NextToken(); return GotNodeType::Success;}

	TokenTypeCheck(ColonToken, TokenType::Colon); NextToken();
	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);NextToken();


	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();
	
		TryGetNode V;

		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		case Parser::declareFunc:V = GetFuncNode(); break;
		default:V = GetDeclareVariable();
		}

		if (V.Node)
		{
			out._Nodes.push_back(Unique_ptr<Node>(V.Node));
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

GotNodeType Parser::GetPostfixStatement(PostfixVariableNode& out,bool DoSemicolon)
{
	auto Name = GetName(out.Name);
	auto Token = TryGetToken(); 
	out.PostfixOp = Token;
	NextToken();

	if (DoSemicolon) {
		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
		NextToken();
	}
	return GotNodeType::Success;
}
GotNodeType Parser::GetCompoundStatement(CompoundStatementNode& out)
{
	auto Name = GetName(out.VariableName);
	auto Token = TryGetToken();
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
	auto DropToken = TryGetToken(); TokenTypeCheck(DropToken, TokenType::KeyWord_Drop);
	NextToken();

	auto ParToken = TryGetToken();
	TokenTypeCheck(ParToken, FuncCallStart);
	NextToken();

	Node* Ptr = nullptr;
	GetExpressionTypeNode(Ptr);
	out.expression.Value = Unique_ptr<Node>(Ptr);

	auto Par2Token = TryGetToken();
	TokenTypeCheck(Par2Token, FuncCallEnd);
	NextToken();

	auto SemicolonToken2 = TryGetToken(); TokenTypeCheck(SemicolonToken2, TokenType::Semicolon);
	NextToken();
	return GotNodeType::Success;
}
GotNodeType Parser::GetNewExpresionNode(NewExpresionNode& out)
{
	auto NewToken = TryGetToken(); TokenTypeCheck(NewToken, TokenType::KeyWord_new);
	NextToken();
	GetType(out.Type,true);

	auto ParToken = TryGetToken();
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
GotNodeType Parser::GetumutVariableDeclare(Node*& out)
{
	auto NewToken = TryGetToken(); TokenTypeCheck(NewToken, TokenType::KeyWord_umut);
	NextToken();

	auto Token2 = TryGetToken(); 

	size_t OldIndex = _TokenIndex;
	
	NameNode NameValue;
	GetNameCheck(NameValue);

	
	TypeNode* Tnode = nullptr;
	auto Token3 = TryGetToken();

	GotNodeType r;

	if (Token3->Type == TokenType::equal)
	{
		NextToken();

		if (IsTopScope())
		{
			DeclareThreadVariableNode* V = DeclareThreadVariableNode::Gen();
			out = V;
			r = GetDeclareThreadVariable(*V, true);
		}
		else
		{
			DeclareVariableNode* V = DeclareVariableNode::Gen();//this need to be checked if this dose not happen at top scope

			out = V;
			TypeNode::Gen_Var(V->Type, *NameValue.Token);
			V->Name = std::move(NameValue);
			GetExpressionTypeNode(V->Expression);

			auto SemicolonToken2 = TryGetToken(); TokenTypeCheck(SemicolonToken2, TokenType::Semicolon);
			NextToken();

			Tnode = &V->Type;
			r = GotNodeType::Success;
		}
	}
	else if (Token3->Type == TokenType::Semicolon)
	{

		auto Token = NameValue.Token;
		_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
			, "cant guess type theres no '=' [expression]");
		NextToken();
		return GotNodeType::Success;
	}
	else
	{
		_TokenIndex = OldIndex;
		switch (Token2->Type)
		{
		case TokenType::KeyWord_Thread:
		{
			DeclareThreadVariableNode* V = DeclareThreadVariableNode::Gen();
			out = V;
			r = GetDeclareThreadVariable(*V,true);

			Tnode = &V->Variable.Type;
		}
		break;
		case TokenType::KeyWord_static:
		{
			DeclareStaticVariableNode* V = DeclareStaticVariableNode::Gen();
			out = V;
			r =GetDeclareStaticVariable(*V, true);

			Tnode = &V->Variable.Type;
		}break;
		default:
		{
			DeclareVariableNode* V = DeclareVariableNode::Gen();
			out = V;
			r = GetDeclareVariable(*V,true);

			Tnode = &V->Type;
		}break;
		}
	}
	Tnode->SetAsimmutable();
	return r;
}
GotNodeType Parser::GetForNode(ForNode& out)
{
	auto ForToken = TryGetToken(); TokenTypeCheck(ForToken, TokenType::KeyWord_for);
	NextToken();

	auto LeftBracket = TryGetToken(); TokenTypeCheck(LeftBracket, TokenType::Left_Bracket);
	NextToken();
	
	TypeNode V;
	auto TypeV = GetType(V, false, false);

	auto NameToken = TryGetToken(); TokenTypeCheck(NameToken, TokenType::Name);
	NextToken();

	auto Assemnt = TryGetToken();
	if (Assemnt->Type == TokenType::equal)
	{

		out.TypeNode = std::move(V);
		out.Name = NameToken;
		out.Type = ForNode::ForType::Traditional;

		NextToken();
		GetExpressionTypeNode(out.Traditional_Assignment_Expression);

		TokenTypeCheck(TryGetToken(), TokenType::Semicolon); NextToken();

		GetExpressionTypeNode(out.BoolExpression);

		TokenTypeCheck(TryGetToken(), TokenType::Semicolon); NextToken();

		GetPostfixStatement(out.OnNextStatement,false);
	}
	else if (Assemnt->Type == TokenType::Colon)
	{
		NextToken();
		out.TypeNode = std::move(V);
		out.Name = NameToken;
		out.Type = ForNode::ForType::modern;

		GetExpressionTypeNode(out.Modern_List);
	}
	else
	{
		TokenTypeCheck(NameToken, TokenType::equal);
	}
	
	auto RightBracket = TryGetToken(); TokenTypeCheck(RightBracket, TokenType::Right_Bracket); NextToken();

	auto Token3 = TryGetToken();
	if (Token3->Type != TokenType::Semicolon)
	{
		TokenTypeCheck(Token3, TokenType::Colon);
		NextToken();

		auto Statements = GetStatementsorStatementNode(out.Body);
	}
	else
	{
		NextToken();
	}

	return GotNodeType::Success;
}
GotNodeType Parser::GetContinueNode(ContinueNode& out)
{
	auto KeyToken = TryGetToken(); TokenTypeCheck(KeyToken, TokenType::KeyWord_continue);
	NextToken();

	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::Semicolon);
	NextToken();

	out.token = KeyToken;
	return  GotNodeType::Success;
}
GotNodeType Parser::GetBreakNode(BreakNode& out)
{
	auto KeyToken = TryGetToken(); TokenTypeCheck(KeyToken, TokenType::KeyWord_break);
	NextToken();

	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::Semicolon);
	NextToken();

	out.token = KeyToken;
	return  GotNodeType::Success;
}
GotNodeType Parser::GetLambdaNode(LambdaNode& out)
{
	auto KeyToken = TryGetToken(); TokenTypeCheck(KeyToken, Parser::declareFunc);
	NextToken();


	auto LeftBracket = TryGetToken(); TokenTypeCheck(LeftBracket, TokenType::Left_Bracket);
	NextToken();

	out.LambdaStart = KeyToken;
	//
	{  
		const Token* LoopToken = TryGetToken();
		while (LoopToken->Type != TokenType::Right_Bracket || LoopToken->Type != TokenType::EndofFile)
		{
			bool HasType = true;
			if (LoopToken->Type == TokenType::Name)
			{
				auto NextToken = TryPeekNextToken(1);
				if (NextToken->Type == TokenType::Comma || NextToken->Type == TokenType::Right_Bracket)
				{
					HasType = false;
				}
			}

			if (HasType)
			{
				NamedParameterNode par;
				GetType(par.Type, false, false);
				GetName(par.Name);
				out.Pars.Parameters.push_back(std::move(par));
			}
			else
			{
				NamedParameterNode par;
				GetName(par.Name);

				TypeNode::Gen_Var(par.Type,*par.Name.Token);

				out.Pars.Parameters.push_back(std::move(par));
			}

			auto CommaToken = TryGetToken();
			if (CommaToken->Type == TokenType::Comma)
			{
				NextToken();
			}
			else
			{
				break;
			}

		}
	}
	auto RightBracket = TryGetToken(); TokenTypeCheck(RightBracket, TokenType::Right_Bracket); NextToken();
	//
	auto ParToken = TryGetToken();
	if (ParToken->Type == TokenType::Left_Parentheses)
	{
		NextToken();
		LambdaCapturesData Captures;

		out._Capture = std::move(Captures);
		auto OtherPar = TryGetToken(); TokenTypeCheck(OtherPar, TokenType::Right_Parentheses); NextToken();
	}


	auto AssmentToken = TryGetToken();
	if (AssmentToken->Type == TokenType::Colon)
	{
		NextToken();
		StatementsNode Statements;
		GetStatementsorStatementNode(Statements);

		out._Statements = std::move(Statements);
	}
	else if (AssmentToken->Type == TokenType::RightAssignArrow)
	{
		NextToken();
		StatementsNode Statements;

		RetStatementNode* r = RetStatementNode::Gen();
		GetExpressionTypeNode(r->Expression);
		Statements._Nodes.push_back(Unique_ptr<Node>(r));


		out._Statements = std::move(Statements);
		
	}
	else if (AssmentToken->Type == TokenType::Semicolon)
	{
		NextToken();
	}
	else
	{
		TokenTypeCheck(AssmentToken, TokenType::Colon);
	}

	return GotNodeType::Success;
}
TryGetNode Parser::GetShortLambdaNode()
{
	LambdaNode* r = LambdaNode::Gen();

	while (TryGetToken()->Type == TokenType::Name)
	{
		NextToken();

		if (TryGetToken()->Type == TokenType::Comma)
		{
			NextToken();
		}
		else
		{
			break;
		}

		NamedParameterNode par;
		par.Name.Token = TryGetToken();
		TypeNode::Gen_Var(par.Type, *par.Name.Token);
		r->Pars.Parameters.push_back(std::move(par));
	}


	auto endtoken = TryGetToken();
	TokenTypeCheck(endtoken, TokenType::Right_Bracket);
	NextToken();

	auto AssmentToken = TryGetToken();
	if (AssmentToken->Type == TokenType::Colon)
	{
		NextToken();
		StatementsNode Statements;
		GetStatementsorStatementNode(Statements);

		r->_Statements = std::move(Statements);
	}
	else if (AssmentToken->Type == TokenType::RightAssignArrow)
	{
		NextToken();
		StatementsNode Statements;

		RetStatementNode* r1 = RetStatementNode::Gen();
		GetExpressionTypeNode(r1->Expression);
		Statements._Nodes.push_back(Unique_ptr<Node>(r1));


		r->_Statements = std::move(Statements);

	}
	else if (AssmentToken->Type == TokenType::Semicolon)
	{
	NextToken();
	}
	else
	{
		TokenTypeCheck(AssmentToken, TokenType::Colon);
	}
	return {GotNodeType::Success, r };
}
GotNodeType Parser::GetTraitNode(TraitNode& out)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWord_trait);
	NextToken();
	GetName(out._Name);

	auto ColonToken = TryGetToken();
	if (ColonToken->Type == TokenType::Semicolon) { NextToken(); return GotNodeType::Success; }

	TokenTypeCheck(ColonToken, TokenType::Colon); NextToken();
	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab); NextToken();


	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();

		TryGetNode V;

		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		case Parser::declareFunc:V = GetFuncNode(); break;
		default:V = GetDeclareVariable();
		}

		if (V.Node)
		{
			out._Nodes.push_back(Unique_ptr<Node>(V.Node));
		}
	}

EndLoop:
	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();

	return GotNodeType::Success;
}
GotNodeType Parser::GetBitCastExpression(BitCastExpression& out)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWord_bitcast);
	NextToken();


	TokenTypeCheck(TryGetToken(), TokenType::lessthan); NextToken();

	GetType(out._Type);

	TokenTypeCheck(TryGetToken(), TokenType::greaterthan); NextToken();


	auto LToken = TryGetToken();
	TokenTypeCheck(LToken, TokenType::Left_Parentheses);
	NextToken();

	GetExpressionTypeNode(out._Expression);

	auto RToken = TryGetToken();
	TokenTypeCheck(RToken, TokenType::Right_Parentheses);
	NextToken();

	return  GotNodeType::Success;
}
GotNodeType Parser::GetPanicNode(PanicNode& out)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWord_panic);

	NextToken();
	auto LToken = TryGetToken();
	TokenTypeCheck(LToken, TokenType::Left_Parentheses);
	
	NextToken();
	auto ExToken = TryGetToken();
	if (ExToken->Type != TokenType::Right_Parentheses)
	{
		ExpressionNodeType V;
		GetExpressionTypeNode(V);
		
		
		out._StringExpression.Value = std::move(V.Value);
	}

	auto RToken = TryGetToken();
	TokenTypeCheck(RToken, TokenType::Right_Parentheses);
	NextToken();


	auto EndToken = TryGetToken();

	TokenTypeCheck(EndToken, TokenType::Semicolon);
	NextToken();

	return GotNodeType::Success;
}
UCodeLangFrontEnd