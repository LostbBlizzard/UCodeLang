#include "UCodeLang/Compilation/Helpers/CompilerTypes.hpp"
#ifndef UCodeLangNoCompiler

#include "Parser.hpp"
#include "../Helpers/KeyWords.hpp"
#include "../UAssembly/UAssembly_Parser.hpp"
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



void Parser::Parse(const String_view FileText, const Vector<Token>& Tokens)
{
	Reset();
	_Nodes = &Tokens;

	_Tree.FileText = FileText;


	AttributeStart();
	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();
		TryGetNode V;

		if (T->Type != TokenType::KeyWord_Import)
		{
			_PassedImportFileSection = true;
		}

		if (T->Type == TokenType::Name)
		{
			size_t Index = _TokenIndex;

			ScopedNameNode Name;
			GetName(Name);

			auto NextToken = TryGetToken();
			if (NextToken->Type == TokenType::Colon)
			{
				_TokenIndex = Index;
				V = GetNamespaceNode();

				if (V.Node)
				{
					_Tree._Nodes.push_back(std::move(V.Node));
				}
				else { break; }
				if (V.GotNode != GotNodeType::Success) { break; }
				continue;
			}
			else
			{
				_TokenIndex = Index;
			}

		}

		switch (T->Type)
		{
		case TokenType::KeyWord_export:
		{
			NextToken();
			auto etoken = TryGetToken();
			bool isvar = etoken->Type != declareFunc;
			_TokenIndex--;

			if (isvar)
			{
				switch (etoken->Type)
				{
				case TokenType::KeyWord_static:V = GetDeclareStaticVariable(); break;
				case TokenType::KeyWord_Thread:V = GetDeclareThreadVariable(); break;
				case TokenType::KeyWord_imut:V = GetimutVariableDeclare(); break;
				case TokenType::KeyWord_eval:V = GetEvalDeclare(); break;
				default: GetDeclareVariableNoObject(V); break;
				}
			}
			else
			{
				V = GetFuncNode();
			}
		}
		break;

		case TokenType::Class:V = GetClassNode(); break;
		case TokenType::KeyWord_unsafe:
		case TokenType::KeyWord_extern:
		case Parser::declareFunc:V = GetFuncNode(); break;
		case TokenType::KeyWord_use:V = GetUseNode(); break;
		case TokenType::Left_Bracket:V = GetAttribute(); break;
		case TokenType::KeyWord_static:V = GetDeclareStaticVariable(); break;
		case TokenType::KeyWord_Thread:V = GetDeclareThreadVariable(); break;
		case TokenType::KeyWord_imut:V = GetimutVariableDeclare(); break;
		case TokenType::KeyWord_eval:V = GetEvalDeclare(); break;
		case TokenType::KeyWord_Import:V = GetImportStatement(); break;
		case TokenType::KeyWord_ClassIf:V = GetCompileTimeIf(false); break;
		case TokenType::KeyWord_ClassFor:V = GetForTypeNode(); break;
		default: GetDeclareVariableNoObject(V); break;
		}

		if (T->Type != TokenType::Left_Bracket)
		{
			AttributeCheck();
		}
		else
		{
			continue;
		}
		if (V.Node)
		{
			_Tree._Nodes.push_back(std::move(V.Node));
		}
		else { break; }


		if (V.GotNode != GotNodeType::Success) { break; }
	}
	AttributeEnd();
	_ParseSuccess = !_ErrorsOutput->Has_Errors();
}


void Parser::TokenTypeCheck(const Token* Value, TokenType Type)
{
	if (Value->Type != Type)
	{
		if (!_HasTripped)
		{
     		auto& Error = _ErrorsOutput->AddError(ErrorCodes::ExpectingToken, Value->OnLine, Value->OnPos);

			if (Value->Type == TokenType::Name) {

				Error._Msg = "Expecting " + (String)TokenStringHelper::ToString(Type) +
					" Got " + (String)Value->Value._String;
			}
			else
			{
				Error._Msg = "Expecting " + (String)TokenStringHelper::ToString(Type) +
					" Got " + (String)TokenStringHelper::ToString(Value->Type);
			}
			Tripped();
		}
	}
}

GotNodeType Parser::GetNamespaceNode(NamespaceNode& out)
{
	auto NamespaceToken = TryGetToken(); TokenTypeCheck(NamespaceToken, TokenType::Name);
	NextToken();

	auto ScopeResolutionToken = TryGetToken();
	ScopedName V;
	V._token = NamespaceToken;

	out._NamespaceName._ScopedName.push_back(std::move(V));

	NamespaceNode* tepout = &out;

	while (ScopeResolutionToken->Type == TokenType::ScopeResolution)
	{
		NextToken();

		Unique_ptr<NamespaceNode> TepNamespace = NamespaceNode::Gen();
		auto Ptr = TepNamespace.get();
		tepout->_Nodes.push_back(std::move(TepNamespace));


		tepout = Ptr;

		//out.NamespaceName.ScopedName.back().Operator =ScopedName::Operator_t::ScopeResolution;
		GetName(tepout->_NamespaceName);
		ScopeResolutionToken = TryGetToken();
	}



	auto ColonToken = TryGetToken(); TokenTypeCheck(ColonToken, TokenType::Colon);
	NextToken();

	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);
	NextToken();

	AttributeStart();
	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();
		TryGetNode V;


		if (T->Type == TokenType::Name)
		{
			size_t Index = _TokenIndex;

			ScopedNameNode Name;
			GetName(Name);

			auto NextToken = TryGetToken();
			if (NextToken->Type == TokenType::Colon)
			{
				_TokenIndex = Index;
				V = GetNamespaceNode();

				if (V.Node)
				{
					_Tree._Nodes.push_back(std::move(V.Node));
				}
				else { break; }
				if (V.GotNode != GotNodeType::Success) { break; }
				continue;
			}


		}
		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		case TokenType::Class:V = GetClassNode(); break;
		case TokenType::KeyWord_unsafe:
		case TokenType::KeyWord_extern:
		case TokenType::KeyWord_export:
		case Parser::declareFunc:V = GetFuncNode(); break;
		case TokenType::KeyWord_use:V = GetUseNode(); break;
		case TokenType::Left_Bracket:V = GetAttribute(); break;
		case TokenType::KeyWord_static:V = GetDeclareStaticVariable(); break;
		case TokenType::KeyWord_Thread:V = GetDeclareThreadVariable(); break;
		case TokenType::KeyWord_imut:V = GetimutVariableDeclare(); break;
		case TokenType::KeyWord_eval:V = GetEvalDeclare(); break;
		case TokenType::KeyWord_ClassFor:V = GetForTypeNode(); break;
		default: GetDeclareVariableNoObject(V); break;
		}

		if (T->Type != TokenType::Left_Bracket)
		{
			AttributeCheck();
		}
		else
		{
			continue;
		}
		if (V.Node)
		{
			tepout->_Nodes.push_back(std::move(V.Node));
		}
		else { break; }
		if (V.GotNode != GotNodeType::Success) { break; }
	}
EndLoop:
	AttributeEnd();
	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();

	return GotNodeType::Success;
}
GotNodeType Parser::GetAlias(const Token* AliasName, GenericValuesNode&& AliasGenerics, AliasNode& out)
{
	out._AliasName.token = AliasName;
	out.Generic = std::move(AliasGenerics);
	out._Access = GetModifier();


	auto ClassToken = TryGetToken(); TokenTypeCheck(ClassToken, TokenType::equal);
	NextToken();

	if (TryGetToken()->Type == TokenType::Not)
	{
		out._IsHardAlias = true;
		NextToken();
	}

	GotNodeType r;
	auto FirtToken = TryGetToken();
	if (FirtToken->Type == declareFunc)
	{
		NextToken();

		out._AliasType = AliasType::Func;
		auto Func = new AliasNode_Func();
		out._Node.reset(Func);

		auto LPToken = TryGetToken();
		TokenTypeCheck(LPToken, declareFuncParsStart);
		NextToken();

		auto Parameters = GetNamedParametersNode(Func->_Parameters);

		auto RPToken = TryGetToken();
		TokenTypeCheck(RPToken, declareFuncParsEnd);
		NextToken();

		auto Arrow = TryGetToken();
		TokenTypeCheck(Arrow, TokenType::RightArrow);
		NextToken();

		r = GetType(Func->_ReturnType);
	}
	else if (FirtToken->Type == TokenType::KeyWord_match)
	{
		out._AliasType = AliasType::Match;
		auto ex = new MatchExpression();
		out._Node.reset(ex);
		r = GetMatchExpression(*ex);
	}
	else
	{
		r = GetType(out._Type, false, false);
	}
	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	out.EndOfClass = TryGetToken();
	NextToken();


	return r;
}
GotNodeType Parser::GetClassTypeNode(Unique_ptr<Node>& out)
{
	auto ClassToken = TryGetToken(); TokenTypeCheck(ClassToken, TokenType::Class);

	SetNotTopScope();

	const String_view& ClassName = ClassToken->Value._String;


	NextToken();

	GenericValuesNode TepGenerics;

	TryGetGeneric(TepGenerics);

	InheritedTypeData InheritedTypes;

	auto LeftBracket = TryGetToken();
	if (LeftBracket && LeftBracket->Type == TokenType::Left_Bracket)
	{
		NextToken();
		do
		{
			InheritedTypeValue Inher;
			if (TypeNode::IsType(TryGetToken()->Type))
			{
				Inher._Name.token = TryGetToken();
				NextToken();
			}
			else
			{
				TokenTypeCheck(TryGetToken(), TokenType::Name);
			}

			TryGetGeneric(Inher._generic);
			InheritedTypes._values.push_back(std::move(Inher));


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
	}




	bool isexport = false;
	auto ColonToken = TryGetToken();

	auto oldindex = _TokenIndex;	
	if (ColonToken->Type == TokenType::KeyWord_export)
	{
		NextToken();
		if (TryGetToken()->Type == TokenType::equal ||
			TryGetToken()->Type == TokenType::Semicolon)
		{
			isexport = true;
			ColonToken = TryGetToken();
		}
		else
		{
			_TokenIndex = oldindex;
		}
	}

	if (ColonToken->Type == TokenType::equal)
	{
		auto V = AliasNode::Gen();
		V->IsExport = isexport;
		 
		auto r = GetAlias(ClassToken, std::move(TepGenerics), *V);
		out = std::move(V);
		return r;
	}
	else if (ColonToken->Type == TokenType::Semicolon)
	{
		NextToken();

		auto output = ClassNode::Gen(); 
		output->_className.token = ClassToken;
		output->_generic = std::move(TepGenerics);
		output->_Inherited = std::move(InheritedTypes);
		output->_Access = GetModifier();
		output->_Attributes = Get_TepAttributes();
		output->EndOfClass = ColonToken;
		output->_IsExport = isexport;

		
		out = std::move(output);
		return GotNodeType::Success;
	}
	else if (ColonToken->Type == TokenType::KeyWord_Enum)
	{
		auto output = EnumNode::Gen(); 
		auto r = DoEnumType(output.get(), ClassToken, TepGenerics, InheritedTypes);
	
		out = std::move(output);
		return r;
	}
	else if (ColonToken->Type == TokenType::KeyWord_trait)
	{
		auto output = TraitNode::Gen();
		auto r = DoTraitType(output.get(), ClassToken, TepGenerics, InheritedTypes);

		out = std::move(output);
		return r;
	}
	else if (ColonToken->Type == TokenType::KeyWord_Tag)
	{
		auto output = TagTypeNode::Gen(); 

		auto r = DoTagType(output.get(), ClassToken, TepGenerics, InheritedTypes);

		out = std::move(output);
		
		return r;
	}
	else
	{
		auto output = ClassNode::Gen();
		output->_Inherited = std::move(InheritedTypes);

		auto r = DoClassType(output.get(), ClassToken, TepGenerics, ColonToken);
		
		out = std::move(output);
		return r;
	}


	UnSetTopScope();
}

GotNodeType Parser::DoClassType(ClassNode* output, const Token* ClassToken, GenericValuesNode& TepGenerics, const Token* ColonToken)
{
	output->_className.token = ClassToken;
	output->_generic = std::move(TepGenerics);
	output->_Access = GetModifier();
	output->_Attributes = Get_TepAttributes();


	if (TryGetToken()->Type == TokenType::KeyWord_export)
	{
		output->_IsExport = true;
		NextToken();

		ColonToken = TryGetToken();
	}


	if (ColonToken->Type == TokenType::KeyWord_extern)
	{
		NextToken();
		auto strtoken = TryGetToken();

		TokenTypeCheck(strtoken, TokenType::String_literal);
		if (strtoken->Type == TokenType::String_literal)
		{
			if (strtoken->Value._String == "c")
			{
				NextToken();

				output->IsExternCStruct = true;
				ColonToken = TryGetToken();
			}
			else
			{
				_ErrorsOutput->AddError(ErrorCodes::InValidName, strtoken->OnLine, strtoken->OnPos, "String Must be 'c'");
			}

		}
	}

	TokenTypeCheck(ColonToken, TokenType::Colon);
	NextToken();

	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);
	NextToken();

	AccessStart();
	AttributeStart();
	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();
		TryGetNode V;

		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		case TokenType::Class:V = GetClassNode(); break;
		case TokenType::KeyWord_export:
		case TokenType::KeyWord_unsafe:
		case TokenType::KeyWord_extern:
		case Parser::declareFunc:V = GetFuncNode(); break;
		case TokenType::KeyWord_use:V = GetUseNode(); break;
		case TokenType::Left_Bracket:V = GetAttribute(); break;
		case TokenType::KeyWord_static:V = GetDeclareStaticVariable(); break;
		case TokenType::KeyWord_Thread:V = GetDeclareThreadVariable(); break;
		case TokenType::KeyWord_imut:V = GetimutVariableDeclare(); break;
		case TokenType::KeyWord_eval:V = GetEvalDeclare(); break;
		case TokenType::KeyWord_ClassFor:V = GetForTypeNode(); break;
		case TokenType::KeyWorld_public:
		{
			NextToken(); TokenTypeCheck(TryGetToken(), TokenType::Colon); NextToken();
			_AccessModifier.top() = AccessModifierType::Public;
			TokenTypeCheck(TryGetToken(), TokenType::StartTab); NextToken();

			ClassTypeAccessModifierInerScope(output->_Nodes);

			TokenTypeCheck(TryGetToken(), TokenType::EndTab); NextToken(); continue;
		};
		break;
		case TokenType::KeyWorld_private:
		{
			NextToken(); TokenTypeCheck(TryGetToken(), TokenType::Colon); NextToken();
			_AccessModifier.top() = AccessModifierType::Private;
			TokenTypeCheck(TryGetToken(), TokenType::StartTab); NextToken();

			ClassTypeAccessModifierInerScope(output->_Nodes);

			TokenTypeCheck(TryGetToken(), TokenType::EndTab); NextToken(); continue;
		};
		break;
		default:V = GetDeclareVariable();
		}
		if (T->Type != TokenType::Left_Bracket)
		{
			AttributeCheck();
		}
		else
		{
			continue;
		}
		if (V.Node)
		{
			output->_Nodes.push_back(std::move(V.Node));
		}
		else { break; }
		if (V.GotNode != GotNodeType::Success) { break; }
	}


EndLoop:
	AttributeEnd();
	AccessEnd();
	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();
	output->EndOfClass = EndToken;

	return GotNodeType::Success;
}
void Parser::ClassTypeAccessModifierInerScope(Vector<Unique_ptr<Node>>& Out)
{
	AttributeStart();
	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();
		TryGetNode V;

		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		case TokenType::Class:V = GetClassNode(); break;
		case TokenType::KeyWord_export:
		case TokenType::KeyWord_unsafe:
		case TokenType::KeyWord_extern:
		case Parser::declareFunc:V = GetFuncNode(); break;
		case TokenType::KeyWord_use:V = GetUseNode(); break;
		case TokenType::Left_Bracket:V = GetAttribute(); break;
		case TokenType::KeyWord_static:V = GetDeclareStaticVariable(); break;
		case TokenType::KeyWord_Thread:V = GetDeclareThreadVariable(); break;
		case TokenType::KeyWord_imut:V = GetimutVariableDeclare(); break;
		case TokenType::KeyWord_eval:V = GetEvalDeclare(); break;
		case TokenType::KeyWord_ClassIf:V = GetCompileTimeIf(false); break;
		default:V = GetDeclareVariable();
		}
		if (T->Type != TokenType::Left_Bracket)
		{
			AttributeCheck();
		}
		else
		{
			continue;
		}
		if (V.Node)
		{
			Out.push_back(std::move(V.Node));
		}
	}
EndLoop:
	AttributeEnd();
	return;
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
		Unique_ptr<Node> nodeptr = nullptr;

		auto r = GetStatement(nodeptr);
		if (nodeptr)
		{
			out._Nodes.push_back(std::move(nodeptr));
		}
		TrippedCheck(r);
		return r;
	}
}

GotNodeType Parser::GetStatement(Unique_ptr<Node>& out)
{
	auto StatementTypeToken = TryGetToken();
	switch (StatementTypeToken->Type)
	{
	case TokenType::EndTab:return GotNodeType::EndLoop;
	case TokenType::KeyWord_block:
	{
		NextToken();
		TokenTypeCheck(TryGetToken(), TokenType::Colon);
		NextToken();

		auto r = GetStatements();

		out = std::move(r.Node);
		return r.GotNode;
	};
	case TokenType::Class:
	{
		auto r = GetClassNode();
		out = std::move(r.Node);
		return r.GotNode;
	};
	case TokenType::KeyWord_use:
	{
		auto r = GetUseNode();
		out = std::move(r.Node);
		return r.GotNode;
	};
	case TokenType::KeyWorld_Ret:
	{
		auto r = GetRetStatement();
		out = std::move(r.Node);
		return r.GotNode;
	};
	case TokenType::KeyWord_static:
	{
		auto r = GetDeclareStaticVariable();
		out = std::move(r.Node);
		return r.GotNode;
	}
	case TokenType::KeyWord_Thread:
	{
		auto r = GetDeclareThreadVariable();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case  TokenType::KeyWord_while:
	{
		auto r = GetWhileNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case  TokenType::KeyWord_do:
	{
		auto r = GetDoNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case  TokenType::KeyWord_If:
	{
		auto r = GetIfNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_continue:
	{
		auto r = GetContinueNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_break:
	{
		auto r = GetBreakNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_for:
	{
		auto r = GetForNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_Drop:
	{
		auto r = GetDropStatementNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_imut:
	{
		auto r = GetimutVariableDeclare();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_panic:
	{
		auto r = GetPanicNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_invalid:
	{
		auto r = GetInvalidNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_eval:
	{
		auto r = GetEvalDeclare();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_ClassIf:
	{
		auto r = GetCompileTimeIf();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_ClassFor:
	{
		auto r = GetCompileTimeForNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_match:
	{
		auto r = GetMatchStatement();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_await:
	{
		auto r = GetAwaitStatementNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_yield:
	{
		auto r = GetYieldStatementNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_unsafe:
	{
		auto r = GetUnsafeStatementNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	case TokenType::KeyWord_defer:
	{
		auto r = GetDeferStatementNode();
		out = std::move(r.Node);
		return r.GotNode;
	}
	break;
	default:
	{
		size_t OldIndex = _TokenIndex;

		enum class MyEnum
		{
			DeclareVar,
			Assign,
			Postfix,
			Compound,
			FuncCall,
		};
		MyEnum StatemeType = MyEnum::DeclareVar;

		while (TryGetToken()->Type != TokenType::EndofFile)
		{

			auto TokenV = TryGetToken();
			if (TokenV->Type == TokenType::KeyWord_bind || TokenV->Type == TokenType::KeyWord_dynamic)
			{
				StatemeType = MyEnum::DeclareVar;
				break;
			}

			if (TokenV->Type == TokenType::equal)
			{

				bool NameWasBefor = false;

				auto Start = _TokenIndex - 1;
				for (int i = Start; i >= OldIndex - 1; i--)
				{
					auto& Token = _Nodes->operator[](i);

					if (!NameWasBefor)
					{
						if (Token.Type == TokenType::Name && i == Start)
						{
							NameWasBefor = true;
						}
						else
						{
							StatemeType = MyEnum::Assign;
							goto Done;
						}

					}
					else
					{
						if (Token.Type == TokenType::Name
							|| Token.Type == TokenType::greaterthan
							|| TypeNode::IsType(Token.Type)
							|| Token.Type == TokenType::Right_Bracket
							|| Token.Type == TokenType::bitwise_and
							|| Token.Type == TokenType::bitwise_XOr
							|| Token.Type == TokenType::DollarSign
							|| Token.Type == TokenType::QuestionMark)
						{
							StatemeType = MyEnum::DeclareVar;
						}
						else
						{
							StatemeType = MyEnum::Assign;
						}
						goto Done;
					}
				}

				break;
			}
			else if (TokenV->Type == TokenType::Semicolon)
			{
				StatemeType = MyEnum::DeclareVar;

				auto Start = _TokenIndex - 1;
				auto& Token = _Nodes->operator[](Start);
				if (Token.Type == TokenType::Right_Parentheses)
				{
					StatemeType = MyEnum::FuncCall;
				}

				break;
			}
			else if (ExpressionNodeType::IsPostfixOperator(TokenV))
			{
				StatemeType = MyEnum::Postfix;
				break;
			}
			else if (ExpressionNodeType::IsCompoundOperator(TokenV))
			{
				StatemeType = MyEnum::Compound;
				break;
			}

			NextToken();
		}
	Done:
		TryGetNode r;
		size_t NewIndex = _TokenIndex;
		_TokenIndex = OldIndex;


		switch (StatemeType)
		{
		case MyEnum::DeclareVar:
			r = GetDeclareVariable();
			break;
		case MyEnum::Compound:
			r = GetCompoundStatement();
			break;
		case MyEnum::Assign:
			r = GetAssignExpression();
			break;
		case MyEnum::Postfix:
			r = GetPostfixStatement();
			break;
		case MyEnum::FuncCall:
			r = GetFuncCallStatementNode2();
			break;
		default:
			UCodeLangUnreachable();
			break;
		}
		out = std::move(r.Node);
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

		Unique_ptr<Node> V = nullptr;
		auto CompilerRet = GetStatement(V);
		if (V) { out._Nodes.push_back(std::move(V)); }

		TrippedCheck(CompilerRet);
		if (CompilerRet != GotNodeType::Success) { break; }
	}


	auto EndToken = TryGetToken();
	TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();
	return GotNodeType::Success;
}

GotNodeType Parser::GetFuncNode(FuncNode& out)
{
	auto V = GetFuncSignatureNode(out._Signature);
	out._Attributes = Get_TepAttributes();
	auto ColonToken = TryGetToken();

	switch (ColonToken->Type)
	{
	case TokenType::Semicolon:
		NextToken();
		out._Body = {};

		out.EndOfFunc = ColonToken;
		break;
	case TokenType::Colon:
	{
		NextToken();
		FuncBodyNode V;
		GetFuncBodyNode(V);
		out._Body = Opt(std::move(V));

		out.EndOfFunc = TryPeekNextToken(-1);
	}break;
	case TokenType::RightAssignArrow:
	{
		NextToken();
		FuncBodyNode V;

		V._Statements._Nodes.push_back(nullptr);

		auto r = RetStatementNode::Gen();
		r->_RetToken = ColonToken;
		GetExpressionTypeNode(r->_Expression);
		V._Statements._Nodes.back() = Unique_ptr<Node>(r.release());

		out._Body = Opt(std::move(V));

		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
		NextToken();

		out.EndOfFunc = SemicolonToken;
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

	if (funcToken->Type == TokenType::KeyWord_export)
	{
		out._IsExport = true;
		NextToken();
		funcToken = TryGetToken();
	}

	if (funcToken->Type == TokenType::KeyWord_dynamic)
	{
		out._HasDynamicKeyWordForTrait = true;
		NextToken();
		funcToken = TryGetToken();
	}

	if (funcToken->Type == TokenType::KeyWord_unsafe)
	{
		out._HasUnsafeKeyWord = true;
		NextToken();
		funcToken = TryGetToken();
	}

	if (funcToken->Type == TokenType::KeyWord_extern)
	{
		out.Externtype = ExternType::ExternUCode;
		NextToken();

		funcToken = TryGetToken();
		if (funcToken->Type == TokenType::KeyWord_dynamic)
		{
			out._HasDynamicKeyWord = true;
			NextToken();
			funcToken = TryGetToken();
		}
		else if (funcToken->Type == TokenType::String_literal)
		{
			auto& str = funcToken->Value._String;
			if (str == "c")
			{
				out.Externtype = ExternType::ExternC;
			}
			else if (str == "ucode")
			{
				out.Externtype = ExternType::ExternUCode;
			}
			else if (str == "system")
			{
				out.Externtype = ExternType::ExternSystem;
			}
			else if (str == "C")
			{
				_ErrorsOutput->AddError(ErrorCodes::InValidName, funcToken->OnLine, funcToken->OnPos, "did you mean 'extern c'");
			}
			NextToken();
			funcToken = TryGetToken();
		}
	}
	else if (funcToken->Type == TokenType::KeyWord_eval)
	{
		out._HasEvalKeyWord = true;
		NextToken();
		funcToken = TryGetToken();
	}

	TokenTypeCheck(funcToken, Parser::declareFunc);
	NextToken();

	out._Access = GetModifier();

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
	out._Name.token = NameToken;
	TryGetGeneric(out._generic);

	auto LPToken = TryGetToken();
	TokenTypeCheck(LPToken, declareFuncParsStart);
	NextToken();

	auto Parameters = GetNamedParametersNode(out._Parameters, true);

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
			out._ReturnType._node = Unique_ptr<Node>(node.release());
			GetNamedParametersNode(node->_Fields);


			auto AnonymousObjectEndToken = TryGetToken();
			TokenTypeCheck(AnonymousObjectEndToken, AnonymousObjectEnd);
			NextToken();
		}
		else
		{
			GetType(out._ReturnType, false, false);
		}
	}
	else  if (Arrow->Type == TokenType::RightAssignArrow)
	{
		TypeNode::Gen_Var(out._ReturnType, *Arrow);
	}
	else if (Arrow->Type == TokenType::Colon)
	{
		TypeNode::Gen_Var(out._ReturnType, *Arrow);
	}
	else if (Arrow->Type == TokenType::Semicolon)
	{
		TypeNode::Gen_Var(out._ReturnType, *Arrow);
	}
	else if (Arrow->Type == TokenType::equal)
	{
		NextToken();

		TokenTypeCheck(TryGetToken(), TokenType::KeyWord_invalid);

		NextToken();

		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);


		out._IsRemoved = true;
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
	return GetStatementsorStatementNode(out._Statements);
}
GotNodeType Parser::GetExpressionNode(ValueExpressionNode& out)
{
	auto nodeptr = Unique_ptr<Node>(nullptr);
	auto r = GetExpressionNode(nodeptr);
	out._Value = std::move(nodeptr);
	return r;
}
GotNodeType Parser::GetExpressionTypeNode(ExpressionNodeType& out)
{
	auto nodeptr = Unique_ptr<Node>(nullptr);
	auto r = GetExpressionTypeNode(nodeptr);

	out._Value = std::move(nodeptr);
	return r;
}
GotNodeType Parser::GetExpressionNode(Unique_ptr<Node>& out)
{
	auto StatementTypeToken = TryGetToken();
	switch (StatementTypeToken->Type)
	{
	case TokenType::String_literal:
	{
		NextToken();
		auto r = StringliteralNode::Gen();
		r->token = StatementTypeToken;
		out = std::move(r);
		return GotNodeType::Success;
	}
	case TokenType::Number_literal:
	{
		NextToken();
		auto r = NumberliteralNode::Gen();
		r->token = StatementTypeToken;
		out = std::move(r);
		return GotNodeType::Success;
	}
	case TokenType::Float_literal:
	{
		NextToken();
		auto r = FloatliteralNode::Gen();
		r->token = StatementTypeToken;
		out = std::move(r);
		return GotNodeType::Success;
	}
	case TokenType::Char_literal:
	{
		NextToken();
		auto r = CharliteralNode::Gen();
		r->token = StatementTypeToken;
		out = std::move(r);
		return GotNodeType::Success;
	}
	case TokenType::KeyWorld_True:
	case TokenType::KeyWorld_False:
	{
		NextToken();
		auto r = BoolliteralNode::Gen();
		r->token = StatementTypeToken;
		out = std::move(r);
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
			out = std::move(r.Node);
			return r.GotNode;
		}
		else
		{
			NextToken();
			auto r = GetShortLambdaNode();
			out = std::move(r.Node);
			return r.GotNode;
		}
	}
TokenType_Name:
	case TokenType::Name:
	{
		size_t OldIndex = _TokenIndex;
		ScopedNameNode Tep;
		auto Name = GetNameCheck2(Tep);
		auto _Token = TryGetToken();

		_TokenIndex = OldIndex;
		if (Name.IsExpression)
		{
			auto r = ReadVariableNode::Gen();
			r->_VariableName = std::move(Tep);

			NextToken();
			
			out = std::move(r);
			return GotNodeType::Success;
		}
		else
		{
			if (_Token->Type == TokenType::lessthan ||
				_Token->Type == FuncCallStart)
			{
				auto r = GetFuncCallNode();
				
				out = std::move(r.Node);
				return r.GotNode;
			}
			else
			{
				auto r = ReadVariableNode::Gen();
				GetName(r->_VariableName, true);
	
				out = std::move(r);
				return GotNodeType::Success;
			}
		}
	}
	case TokenType::Left_Parentheses:
	{
		NextToken();

		auto V = ParenthesesExpresionNode::Gen();
		auto r = GetExpressionTypeNode(V->_Expression);

		auto endtoken = TryGetToken();
		TokenTypeCheck(endtoken, TokenType::Right_Parentheses);
		NextToken();
		
		out = std::move(V);
		return r;
	}
	break;
	case TokenType::KeyWorld_Sizeof:
	{
		NextToken();
		auto V = SizeofExpresionNode::Gen();

		auto ParToken = TryGetToken();
		TokenTypeCheck(ParToken, FuncCallStart);
		NextToken();

		GetType(V->_Type,false,false);

		auto Par2Token = TryGetToken();
		TokenTypeCheck(Par2Token, FuncCallEnd);
		NextToken();
		
		out = std::move(V);
		return GotNodeType::Success;
	}
	break;
	case TokenType::KeyWord_new:
	{
		auto V = GetNewExpresionNode();
	
		out = std::move(V.Node);
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_move:
	{
		NextToken();
		auto V = MoveNode::Gen();	
		GetExpressionTypeNode(V->_expression);

		out = std::move(V);
		return  GotNodeType::Success;
	}
	break;
	case Parser::declareFunc:
	{
		auto V = GetLambdaNode();
		
		out = std::move(V.Node);
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_bitcast:
	{
		auto V = GetBitCastExpression();
		
		out = std::move(V.Node);
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_valid:
	{
		auto V = GetValidNode();
	
		out = std::move(V.Node);
		return V.GotNode;
	}
	break;
	case TokenType::KeyWorld_typeof:
	{
		auto V = GetExpressionToTypeValue();
	
		out = std::move(V.Node);
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_type:
	{
		auto V = GeTypeExNode();
		
		out = std::move(V.Node);
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_match:
	{
		auto V = GetMatchExpression();
	
		out = std::move(V.Node);
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_unq:
	{
		auto V = GetUnqExpresionNode();
	
		out = std::move(V.Node);
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_shr:
	{
		auto V = GetShrExpresionNode();
		
		out = std::move(V.Node);
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_await:
	{
		auto V = GetAwaitExpresionNode();
	
		out = std::move(V.Node);
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_yield:
	{
		auto V = GetYieldExpresionNode();
		
		out = std::move(V.Node);
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_unsafe:
	{
		auto V = GetUnsafeExpression();
		out = std::move(V.Node);
		return V.GotNode;
	}
	break;
	case TokenType::KeyWord_RangeOperator:
	{
		auto V = GetRangeExpression();
	
		out = std::move(V.Node);
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

//https://en.cppreference.com/w/c/language/operator_precedence
int Operator_precedenceBinary(TokenType type)
{
	switch (type)
	{

	case TokenType::star:
	case TokenType::forwardslash:
	case TokenType::modulo:
		return 3;
	case TokenType::plus:
	case TokenType::minus:return 4;

	case TokenType::bitwise_LeftShift:
	case TokenType::bitwise_RightShift:return 5;

	case TokenType::greaterthan:
	case TokenType::lessthan:
	case TokenType::less_than_or_equalto:
	case TokenType::greater_than_or_equalto:
		return 6;

	case TokenType::equal_Comparison:
	case TokenType::Notequal_Comparison:
		return 7;

	case TokenType::bitwise_and:
		return 8;


	case TokenType::bitwise_XOr:
		return 9;
	case TokenType::logical_and:
		return 11;
	default:
		UCodeLangUnreachable();
		break;
	}
}

GotNodeType Parser::GetExpressionTypeNode(Unique_ptr<Node>& out)
{
	Unique_ptr<Node> ExNode = nullptr;

	auto tokenu = TryGetToken();

	if (tokenu && ExpressionNodeType::IsUnaryOperator(tokenu))
	{
		NextToken();
	}
	else
	{
		tokenu = nullptr;
	}

	auto Ex = GetExpressionNode(ExNode);
	GotNodeType r_t = GotNodeType::Null;

	auto Ptr = ValueExpressionNode::Gen();
	Ptr->_Value = std::move(ExNode);
	Unique_ptr<Node> r_out = std::move(Ptr);

	if (tokenu)
	{
		bool isnumberliteral = false;
		if (Ex == GotNodeType::Success)
		{
			if (
				(tokenu->Type == TokenType::minus || tokenu->Type == TokenType::plus)
				&& r_out->Get_Type() == NodeType::ValueExpressionNode
			   )
			{
				auto valueex = ValueExpressionNode::As(r_out.get());

				bool isplus = tokenu->Type == TokenType::plus;
				if (valueex->_Value->Get_Type() == NodeType::NumberliteralNode)
				{
					auto num = Unique_ptr<NumberliteralNode>((NumberliteralNode*)valueex->_Value.release());
					String newstring;
					if (!isplus)
					{
						newstring += "-";
					}
					newstring += num->token->Value._String;
					_Tree.TemporaryStrings.push_back(std::make_unique<String>(std::move(newstring)));

					isnumberliteral = true;
					auto newtoken = std::make_unique<Token>();
					newtoken->OnLine = num->token->OnLine;
					newtoken->OnPos = num->token->OnPos;
					newtoken->Type = num->token->Type;
					newtoken->Value._String = *_Tree.TemporaryStrings.back();

					num->token = newtoken.get();
					_Tree.TemporaryTokens.push_back(std::move(newtoken));

					r_out = std::move(num);
					r_t = GotNodeType::Success;
				}
				else if (valueex->_Value->Get_Type() == NodeType::FloatliteralNode)
				{
					auto num = Unique_ptr<FloatliteralNode>((FloatliteralNode*)valueex->_Value.release());
					String newstring;
					if (!isplus)
					{
						newstring += "-";
					}
					newstring += num->token->Value._String;
					_Tree.TemporaryStrings.push_back(std::make_unique<String>(std::move(newstring)));

					isnumberliteral = true;
					auto newtoken = std::make_unique<Token>();
					newtoken->OnLine = num->token->OnLine;
					newtoken->OnPos = num->token->OnPos;
					newtoken->Type = num->token->Type;
					newtoken->Value._String = *_Tree.TemporaryStrings.back();


					num->token = newtoken.get();
					_Tree.TemporaryTokens.push_back(std::move(newtoken));
					
					r_out = std::move(num);
					r_t = GotNodeType::Success;
				}
			}
		}

		if (isnumberliteral == false)
		{
			auto r = UnaryExpressionNode::Gen();
			r->_Value0._Value = std::move(r_out);

			r->_UnaryOp = tokenu;
			r_t = GotNodeType::Success;
			r_out = std::move(r);
		}
		else 
		{
			auto Ptr2 = ValueExpressionNode::Gen();
			Ptr2->_Value = std::move(r_out);

			r_t = GotNodeType::Success;
			r_out = std::move(Ptr2);
		}
	}

	auto Token2 = TryGetToken();

	while (Token2->Type == TokenType::RightArrow
		|| Token2->Type == TokenType::Left_Bracket
		|| ScopedName::Get_IsScoped(Token2->Type))
	{
		NextToken();
		if (Token2->Type == TokenType::RightArrow)
		{
			auto cast = CastNode::Gen();



			auto Type = GetType(cast->_ToType);
			cast->_Expression._Value = std::move(r_out);
			r_t = Merge(Ex, Type);
			r_out = std::move(cast);
			Token2 = TryGetToken();
		}
		else if (Token2->Type == TokenType::Left_Bracket)
		{
			auto IndexExNode = IndexedExpresionNode::Gen();
			auto V = GetExpressionTypeNode(IndexExNode->_IndexExpression);
			IndexExNode->_SourceExpression._Value = std::move(r_out);

			IndexExNode->_token = Token2;
			r_out = std::move(IndexExNode);

			Token2 = TryGetToken();
			TokenTypeCheck(Token2, TokenType::Right_Bracket);
			NextToken();

			Token2 = TryGetToken();
		}
		else if (ScopedName::Get_IsScoped(Token2->Type))
		{
			ScopedNameNode Name;
			GetName(Name);

			if (TryGetToken()->Type == TokenType::Left_Parentheses
				|| TryGetToken()->Type == TokenType::lessthan)
			{
				auto ExtendedNode = ExtendedFuncExpression::Gen();
				ExtendedNode->_Expression._Value = std::move(r_out);
				ExtendedNode->_Operator = ScopedName::Get_Scoped(Token2->Type);
				ExtendedNode->_Extended._FuncName = std::move(Name);

				TokenTypeCheck(TryGetToken(), TokenType::Left_Parentheses);

				{
					NextToken();
					auto Pars = GetValueParametersNode(ExtendedNode->_Extended.Parameters);

					auto Par2Token = TryGetToken();
					TokenTypeCheck(Par2Token, FuncCallEnd);
					NextToken();
				}


				r_out = std::move(ExtendedNode);
			}
			else
			{
				auto ExtendedNode = ExtendedScopeExpression::Gen();
				ExtendedNode->_Expression._Value = std::move(r_out);
				ExtendedNode->_Operator = ScopedName::Get_Scoped(Token2->Type);
				ExtendedNode->_Extended = std::move(Name);

				r_out = std::move(ExtendedNode);

			}
			Token2 = TryGetToken();
		}
		else
		{
			UCodeLangUnreachable();
		}
	}

	auto token = TryGetToken();

	if (token->Type == TokenType::KeyWord_RangeOperator)
	{
		NextToken();

		auto newtoken = TryGetToken();

		String_view funcname = UCode_RangeFunction;
		if (newtoken->Type == TokenType::equal)
		{
			funcname = UCode_RangeInclusiveFunction;
			NextToken();
		}

		auto Extoken = TryGetToken();

		if (Extoken->Type == TokenType::Number_literal 
			|| Extoken->Type == TokenType::Name
			|| Extoken->Type == TokenType::Left_Parentheses
			|| funcname == UCode_RangeInclusiveFunction
			|| TypeNode::IsType(Extoken->Type))
		{
			Unique_ptr<Node> Other = nullptr;
			auto Ex2 = GetExpressionTypeNode(Other);

			auto call = FuncCallNode::Gen();

			{
				call->Parameters._Nodes.push_back(std::move(r_out));
			}
			{
				auto Ptr = ValueExpressionNode::Gen();
				call->Parameters._Nodes.push_back(std::move(Other));
			}
			//0..10 = Range<bind(typeof(0))>(0,10)
			{
				ScopedName namenode;


				auto funcnametoken = std::make_unique<Token>(Token());

				funcnametoken->OnLine = token->OnLine;
				funcnametoken->OnPos = token->OnPos;
				funcnametoken->Type = TokenType::Name;
				funcnametoken->Value._String = funcname;

				namenode._token = funcnametoken.get();

				namenode._generic = std::make_shared<UseGenericsNode>();

				/*
				TypeNode typenode;
				typenode._IsTypeBinding = true;
				typenode.Gen_Type(typenode, TokenType::internal_Constant_expression, *token);


				auto expression = std::make_unique<ExpressionToTypeValueNode>();
				expression->_TypeEx._Value = Unique_ptr<Node>(ExNode);//dub free
				ValueExpressionNode valuetypenode;

				valuetypenode._Value.reset(expression.release());

				typenode._node = std::make_unique<ValueExpressionNode>(std::move(valuetypenode));

				namenode._generic->_Values.push_back(std::move(typenode));
				*/

				call->_FuncName._ScopedName.push_back(std::move(namenode));
				_Tree.TemporaryTokens.push_back(std::move(funcnametoken));
			}
			auto Ptr = ValueExpressionNode::Gen();
			Ptr->_Value = std::move(call);
			r_out = std::move(Ptr);
			r_t = Ex;
		}
		else 
		{

			auto call = FuncCallNode::Gen();

			{
				call->Parameters._Nodes.push_back(std::move(r_out));
			}
			//0.. = RangeFrom(0)
			{
				ScopedName namenode;


				auto funcnametoken = std::make_unique<Token>(Token());

				funcnametoken->OnLine = token->OnLine;
				funcnametoken->OnPos = token->OnPos;
				funcnametoken->Type = TokenType::Name;
				funcnametoken->Value._String = UCode_RangeFromFunction ;

				namenode._token = funcnametoken.get();

				namenode._generic = std::make_shared<UseGenericsNode>();

				call->_FuncName._ScopedName.push_back(std::move(namenode));
				_Tree.TemporaryTokens.push_back(std::move(funcnametoken));
			}
			auto Ptr = ValueExpressionNode::Gen();
			Ptr->_Value = std::move(call);
			r_out = std::move(Ptr);
			r_t = Ex;
		}
	}
	else if (token && ExpressionNodeType::IsBinaryOperator(token))
	{
		//
		if (token->Type == TokenType::bitwise_and
			&& TryPeekNextToken(1)->Type == TokenType::equal)
		{
			goto ElseCase;
		}
		//
		NextToken();
		Unique_ptr<Node> Other = nullptr;
		auto Ex2 = GetExpressionTypeNode(Other);

		auto binarynod = BinaryExpressionNode::Gen();

		auto r = binarynod.get();

		binarynod->_Value0._Value = std::move(r_out);
		
		binarynod->_BinaryOp = token;
		binarynod->_Value1._Value = std::move(Other);
		r_out = std::move(binarynod);
		r_t = Merge(Ex, Ex2);

		{
			if (r->_Value1._Value->Get_Type() == NodeType::BinaryExpressionNode)
			{
				auto val1 = BinaryExpressionNode::As(r->_Value1._Value.get());


				auto opnum = Operator_precedenceBinary(r->_BinaryOp->Type);
				auto opnum2 = Operator_precedenceBinary(val1->_BinaryOp->Type);

				if (opnum < opnum2)
				{
					//this code is so hard to visualize in your head
					std::swap(r->_BinaryOp, val1->_BinaryOp);
					std::swap(r->_Value0._Value, val1->_Value1._Value);
					std::swap(val1->_Value1._Value, val1->_Value0._Value);
				}
			}
			else if (r->_Value0._Value->Get_Type() == NodeType::ValueExpressionNode)
			{
				auto valex = ValueExpressionNode::As(r->_Value0._Value.get());
				if (valex->_Value->Get_Type() == NodeType::ParenthesesExpresionNode)
				{
					auto val1 = ParenthesesExpresionNode::As(valex->_Value.get());


					auto opnum = Operator_precedenceBinary(r->_BinaryOp->Type);
					auto opnum2 = 0;

					if (opnum < opnum2)
					{
						std::swap(r->_Value0._Value, r->_Value1._Value);
					}
				}
			}
		}

	}
	else
	{
	ElseCase:
		r_t = Ex;
	}



	out = std::move(r_out);
	return r_t;
}
GotNodeType Parser::GetValueParameterNode(Unique_ptr<Node>& out)
{
	if (TryGetToken()->Type == TokenType::KeyWord_out)
	{
		NextToken();

		size_t oldindex = this->_TokenIndex;
		NextToken();

		bool HasType = false;


		auto ParToken = TryGetToken();
		HasType = !(ParToken->Type == TokenType::Comma || ParToken->Type == TokenType::Right_Parentheses);


		this->_TokenIndex = oldindex;


		auto B = OutExpression::Gen();

		if (HasType)
		{
			GetType(B->_Type);
		}
		else
		{
			TypeNode::Gen_Var(B->_Type, *TryGetToken());
		}


		auto r =GetName(B->_Name);
		out = std::move(B);
		return r;
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


		Unique_ptr<Node> node = nullptr;
		auto Ex = GetValueParameterNode(node);
		out._Nodes.push_back(std::move(node));

		auto Token = TryGetToken();

		if (Token == nullptr || Token->Type != TokenType::Comma)
		{
			break;
		}
		NextToken();
	}
	return GotNodeType::Success;
}
GotNodeType Parser::GetNamedParametersNode(NamedParametersNode& out, bool CanHaveOutPar)
{

	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		NamedParameterNode Tep;
		auto Token = TryGetToken();
		if (!Token || Token->Type == TokenType::Right_Bracket) { break; }

		if (CanHaveOutPar && Token->Type == TokenType::KeyWord_out)
		{
			Tep._IsOutVarable = true;
			NextToken();
		}

		GetType(Tep._Type, false, false);

		auto Token2 = TryGetToken();
		if (Token2->Type == TokenType::Comma || Token2->Type == TokenType::Right_Bracket)
		{
			if (out._Parameters.size() != 0)
			{
				_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos, "this& Must be the first parameter");
			}

		}
		else
		{
			GetName(Tep._Name);
		}
		out._Parameters.push_back(std::move(Tep));

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

			if (TryGetToken()->Type == TokenType::Left_Bracket)
			{
				NextToken();

				auto currenttoken = TryGetToken();

				if (currenttoken->Type == TokenType::KeyWord_functor)
				{
					NextToken();

					FunctorNode functor;

					auto LPToken = TryGetToken();
					TokenTypeCheck(LPToken, declareFuncParsStart);
					NextToken();

					functor._Base = std::make_unique<NamedParametersNode>();

					auto Parameters = GetNamedParametersNode(*functor._Base);

					auto RPToken = TryGetToken();
					TokenTypeCheck(RPToken, declareFuncParsEnd);
					NextToken();

					auto Arrow = TryGetToken();
					TokenTypeCheck(Arrow, TokenType::RightArrow);
					NextToken();

					functor._ReturnType = std::make_unique<TypeNode>();

					GetType(*functor._ReturnType);

					Item._BaseOrRuleScopeName = std::move(functor);
				}
				else
				{
					ScopedNameNode sym;
					GetName(sym);
					bool isrule = false;

					if (TryGetToken()->Type == TokenType::Left_Parentheses)
					{
						NextToken();

						TokenTypeCheck(TryGetToken(), TokenType::Right_Parentheses);

						NextToken();
					}

					Item._BaseOrRuleScopeName = std::move(sym);
					Item.IsRule = isrule;

				}

				TokenTypeCheck(TryGetToken(), TokenType::Right_Bracket);
				NextToken();

			}
			auto NameToken = TryGetToken();


			if (NameToken->Type == TokenType::Left_Parentheses)
			{
				NextToken();
				NameToken = TryGetToken();
				TokenTypeCheck(NameToken, TokenType::Name);

				NextToken();
				auto RPar = TryGetToken();
				TokenTypeCheck(RPar, TokenType::Right_Parentheses);

				Item.token = NameToken;
				Item._Generictype = GenericValueNode::GenericType::ConstantExpression;
				out._Values.push_back(std::move(Item));
			}
			else
			{
				TokenTypeCheck(NameToken, TokenType::Name);

				Item.token = NameToken;


				auto Token = TryPeekNextToken(1);
				if (Token->Type == TokenType::ellipses)
				{
					Item._Generictype = GenericValueNode::GenericType::Pack;
					NextToken();
				}


				out._Values.push_back(std::move(Item));
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

				Item._node.reset(node);

				auto Rtoken = TryGetToken();
				TokenTypeCheck(Rtoken, TokenType::Right_Parentheses);
				NextToken();
				out._Values.push_back(std::move(Item));
			}
			else
			{
				TypeNode Item;
				GetType(Item,false,false);
				out._Values.push_back(std::move(Item));
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

GotNodeType Parser::GetName(ScopedNameNode& out, bool CanHaveVarableName)
{

	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		ScopedName V;
		auto NameToken = TryGetToken();

		if (!TypeNode::IsType(NameToken->Type))
		{
			bool V;
			if (CanHaveVarableName)
			{

				V = NameToken->Type != TokenType::Class;
			}
			else
			{
				V = true;
			}

			if (V)
			{
				TokenTypeCheck(NameToken, TokenType::Name);
			}
		}



		V._token = NameToken;


		NextToken();
		V._generic.reset(new UseGenericsNode());
		TryGetGeneric(*V._generic);


		auto Token = TryGetToken();
		if (Token == nullptr || !ScopedName::Get_IsScoped(Token->Type))
		{
			out._ScopedName.push_back(std::move(V));
			break;
		}

		V._operator = ScopedName::Get_Scoped(Token->Type);
		out._ScopedName.push_back(std::move(V));
		NextToken();
	}
	return GotNodeType::Success;
}

GotNodeType Parser::GetName(NameNode& out)
{
	auto NameToken = TryGetToken();
	TokenTypeCheck(NameToken, TokenType::Name);

	out.token = NameToken;
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

	out.token = NameToken;
	NextToken();
	return GotNodeType::Success;
}
Parser::GetNameCheck_ret2 Parser::GetNameCheck2(ScopedNameNode& out, bool CanHaveVarableName)
{
	NameCheck_t LookingAtT = NameCheck_t::Name;


	auto func = [this](UseGenericsNode& out) -> GetNameCheck_ret2
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

						Item._node.reset(node);

						auto Rtoken = TryGetToken();
						TokenTypeCheck(Rtoken, TokenType::Right_Parentheses);
						NextToken();
						out._Values.push_back(std::move(Item));
					}
					else
					{
						auto token = TryGetToken();
						if (TypeNode::IsType(token->Type))
						{
							TypeNode Item;
							GetType(Item);
							out._Values.push_back(std::move(Item));
						}
						else
						{
							return { GotNodeType::failed,true };
						}
					}
					auto Token = TryGetToken();
					if (Token == nullptr || Token->Type != TokenType::Comma)
					{
						break;
					}
					NextToken();
				}

				auto endtoken = TryGetToken();

				if (out._Values.size() == 1 && endtoken->Type != TokenType::greaterthan)
				{
					return { GotNodeType::failed,true };
				}
				else {
					TokenTypeCheck(endtoken, TokenType::greaterthan);
					NextToken();
				}
			}
			return { GotNodeType::Success,{} };
		};

	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		ScopedName V;
		UseGenericsNode Generic;
		auto NameToken = TryGetToken();

		if (NameToken == nullptr
			||
			(
				NameToken->Type != TokenType::Name &&
				NameToken->Type != TokenType::KeyWord_compiler &&
				!(NameToken->Type == TokenType::KeyWord_This && out._ScopedName.size() == 0)
				&& !TypeNode::IsPrimitive(NameToken->Type)
				)
			)
		{
			NextToken();
			return { GotNodeType::failed,{} };
		}

		V._token = NameToken;

		NextToken();
		auto r = func(Generic);
		if (r.IsExpression)
		{
			out._ScopedName.push_back(std::move(V));
			return r;
		}



		auto Token = TryGetToken();


		if (Token)
		{
			V._operator = ScopedName::Get_Scoped(Token->Type);
		}
		V._generic = std::move(std::make_unique<UseGenericsNode>(std::move(Generic)));

		bool MemAccess = V._operator == ScopedName::Operator_t::Dot
			|| V._operator == ScopedName::Operator_t::IndirectMember
			|| V._operator == ScopedName::Operator_t::OptionalChain;

		if (out._ScopedName.size())
		{
			auto& Back = out._ScopedName.back();

			if (LookingAtT == NameCheck_t::MemberAccess && V._generic.get()->_Values.size())
			{
				//auto Token = out._ScopedName[0]._token;
				//_ErrorsOutput->AddError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos,
					//"generic can't be with a Memberaccess operator '" + (String)TokenStringHelper::ToString(Token->Type) + "'");
			}
		}
		if (MemAccess)
		{
			LookingAtT = NameCheck_t::MemberAccess;
		}

		out._ScopedName.push_back(std::move(V));
		if (Token == nullptr || !ScopedName::Get_IsScoped(Token->Type))
		{
			break;
		}





		NextToken();
	}
	return{ GotNodeType::Success,{} };
}
Parser::GetNameCheck_ret Parser::GetNameCheck(ScopedNameNode& out, bool CanHaveVarableName)
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
				NameToken->Type != TokenType::Name && !(NameToken->Type == TokenType::KeyWord_This && out._ScopedName.size() == 0)
				)
			)
		{
			NextToken();
			return { LookingAtT,GotNodeType::failed };
		}

		V._token = NameToken;

		NextToken();
		TryGetGeneric(Generic);




		auto Token = TryGetToken();


		if (Token)
		{
			V._operator = ScopedName::Get_Scoped(Token->Type);
		}
		V._generic = std::move(std::make_unique<UseGenericsNode>(std::move(Generic)));

		bool MemAccess = V._operator == ScopedName::Operator_t::Dot
			|| V._operator == ScopedName::Operator_t::IndirectMember
			|| V._operator == ScopedName::Operator_t::OptionalChain;

		if (out._ScopedName.size())
		{
			auto& Back = out._ScopedName.back();

			if (LookingAtT == NameCheck_t::MemberAccess && V._generic.get()->_Values.size())
			{
				auto Token = out._ScopedName[0]._token;
				_ErrorsOutput->AddError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos,
					"generic can't be with a Memberaccess operator '" + (String)TokenStringHelper::ToString(Token->Type) + "'");
			}
		}
		if (MemAccess)
		{
			LookingAtT = NameCheck_t::MemberAccess;
		}

		out._ScopedName.push_back(std::move(V));
		if (Token == nullptr || !ScopedName::Get_IsScoped(Token->Type))
		{
			break;
		}





		NextToken();
	}
	return{ LookingAtT,GotNodeType::Success };
}

GotNodeType Parser::GetType(TypeNode*& out, bool ignoreRighthandOFtype, bool ignoreleftHandType, bool CanUseInlineEnum)
{
	GotNodeType r = GotNodeType::Success;
	auto Token = TryGetToken();
	bool Isasync = false;
	bool addeddynmaiccloseing = false;
	bool hadsdynamic = false;
	
	if (!ignoreleftHandType)
	{
		if (Token->Type == TokenType::KeyWord_imut)
		{
			out->SetAsimmutable();
			NextToken();
			Token = TryGetToken();
		}
		else if (Token->Type == TokenType::KeyWord_moved)
		{
			out->SetMovedType();
			NextToken();
			Token = TryGetToken();
		}
		else if (Token->Type == TokenType::KeyWord_async)//async<int> => Future<int>
		{
			NextToken();
			Token = TryGetToken();

			TokenTypeCheck(Token, TokenType::lessthan);
			NextToken();

			Token = TryGetToken();
			Isasync = true;
		}


		if (Token->Type == TokenType::KeyWord_bind)
		{
			out->SetAsBinding();
			NextToken();

			TokenTypeCheck(TryGetToken(), TokenType::Left_Parentheses);
			NextToken();

			auto ExNode = new ExpressionNodeType();
			out->_node.reset(ExNode);
			auto Ex = GetExpressionTypeNode(*ExNode);

			ScopedName V;
			V._token = Token;
			out->_name._ScopedName.push_back(std::move(V));

			TokenTypeCheck(TryGetToken(), TokenType::Right_Parentheses);
			NextToken();
			return  GotNodeType::Success;
		}

		if (Token->Type == TokenType::KeyWord_dynamic)
		{
			hadsdynamic = true;
			out->SetDynamic();
			NextToken();
			Token = TryGetToken();

			TokenTypeCheck(Token, TokenType::lessthan);
			NextToken();

			Token = TryGetToken();	
		}
	}
	else
	{
		if (Token->Type == TokenType::KeyWord_async)//async<int> => Future<int>
		{
			NextToken();
			Token = TryGetToken();

			TokenTypeCheck(Token, TokenType::lessthan);
			NextToken();

			Token = TryGetToken();
			Isasync = true;
		}
	}

	if (Token->Type == TokenType::Name)
	{
		auto A = GetName(out->_name);
		r = A;
	}
	else if (TypeNode::IsType(Token->Type))
	{
		NextToken();

		ScopedName V;
		V._token = Token;
		out->_name._ScopedName.push_back(std::move(V));
		r = GotNodeType::Success;
	}
	else if (Token->Type == TokenType::Void)
	{
		NextToken();
		ScopedName V;
		V._token = Token;
		out->_name._ScopedName.push_back(std::move(V));
		r = GotNodeType::Success;
	}
	else
	{
		TokenTypeCheck(Token, TokenType::Name);
	}
	
	if (hadsdynamic && addeddynmaiccloseing == false && TryGetToken()->Type == TokenType::greaterthan)
	{
		addeddynmaiccloseing = true;
		NextToken();
	}

	auto ChangeTypeNodes = [](TypeNode* New,TypeNode* out)
		{
			New->_Isimmutable = out->_Isimmutable;
			out->_Isimmutable = false;
		};

	auto Token2 = TryGetToken();
	while (!ignoreRighthandOFtype && Token2)
	{
		if (Token2->Type == TokenType::bitwise_and
			&& out->_IsAddess == false)//int&
		{
			NextToken();
			out->SetAsAddess();
			Token2 = TryGetToken();
		}
		else if (Token2 && Token2->Type == TokenType::Left_Bracket)
		{
			bool Break = false;
			NextToken();
			Token2 = TryGetToken();
			if (addeddynmaiccloseing == false 
				&& out->_IsDynamic
				&& Token2->Type == TokenType::greaterthan)
			{
				addeddynmaiccloseing = true;
				NextToken();
				Token = TryGetToken();
			}
			else if (Token2->Type == TokenType::bitwise_and)//int[&]
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
					out->_node.reset(ExNode);
					auto Ex = GetExpressionTypeNode(*ExNode);
				}
				out->SetAsStaticArray();
				Break = true;
			}

			else if (Token2->Type == TokenType::Colon)//int[:] => View<int>
			{
				NextToken();
				TypeNode* New = new TypeNode();


				auto NameToken = new UCodeLang::Token();
				NameToken->OnLine = Token2->OnLine;
				NameToken->OnPos = Token2->OnPos;
				NameToken->Type = TokenType::Name;
				NameToken->Value._String = UCode_SpanType;

				_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

				ScopedName V;
				V._token = NameToken;
				V._generic.reset(new UseGenericsNode());

				New->_Isimmutable = out->_Isimmutable;
				out->_Isimmutable = false;
				V._generic->_Values.push_back(std::move(*out));


				New->_name._ScopedName.push_back(std::move(V));
				out = New;
			}
			else if (Token2->Type == TokenType::Right_Bracket)//int[] => Vector<int>
			{
				//NextToken();
				TypeNode* New = new TypeNode();
				
				ChangeTypeNodes(New, out);

				auto NameToken = new UCodeLang::Token();
				NameToken->OnLine = Token2->OnLine;
				NameToken->OnPos = Token2->OnPos;
				NameToken->Type = TokenType::Name;
				NameToken->Value._String = UCode_VectorType;

				_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

				ScopedName V;
				V._token = NameToken;
				V._generic.reset(new UseGenericsNode());
				V._generic->_Values.push_back(std::move(*out));

				New->_name._ScopedName.push_back(std::move(V));
				out = New;
			}
			else if (Token2->Type == TokenType::bitwise_XOr)//int[^] => Unique_Array<int>
			{
				NextToken();
				TypeNode* New = new TypeNode();

				ChangeTypeNodes(New, out);

				auto NameToken = new UCodeLang::Token();
				NameToken->OnLine = Token2->OnLine;
				NameToken->OnPos = Token2->OnPos;
				NameToken->Type = TokenType::Name;
				NameToken->Value._String = UCode_Unique_Array;

				_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

				ScopedName V;
				V._token = NameToken;
				V._generic.reset(new UseGenericsNode());
				V._generic->_Values.push_back(std::move(*out));

				New->_name._ScopedName.push_back(std::move(V));
				out = New;
			}
			else if (Token2->Type == TokenType::DollarSign)//int[^] => Unique_Array<int>
			{
				NextToken();
				TypeNode* New = new TypeNode();

				ChangeTypeNodes(New, out);
				
				auto NameToken = new UCodeLang::Token();
				NameToken->OnLine = Token2->OnLine;
				NameToken->OnPos = Token2->OnPos;
				NameToken->Type = TokenType::Name;
				NameToken->Value._String = UCode_Shared_Array;

				_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

				ScopedName V;
				V._token = NameToken;
				V._generic.reset(new UseGenericsNode());
				V._generic->_Values.push_back(std::move(*out));


				New->_name._ScopedName.push_back(std::move(V));
				out = New;
			}
			else			
			{
				//NextToken();
				
				bool isarrary = Token2->Type != TokenType::Name;

				if (isarrary) //int[0] => Array<int,(0)>
				{
					TypeNode* New = new TypeNode();
					{
						ChangeTypeNodes(New, out);
						
						auto NameToken = new UCodeLang::Token();
						NameToken->OnLine = Token2->OnLine;
						NameToken->OnPos = Token2->OnPos;
						NameToken->Type = TokenType::Name;
						NameToken->Value._String = UCode_ArrayType;

						_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

						ScopedName V;
						V._token = NameToken;
						V._generic.reset(new UseGenericsNode());
						V._generic->_Values.push_back(std::move(*out));


						New->_name._ScopedName.push_back(std::move(V));
					}

					{//Array expression
						auto ExTypeNode = TypeNode();
						TypeNode::Gen_Expression(ExTypeNode, *TryGetToken());


						auto ExNode = new ExpressionNodeType();
						ExTypeNode._node.reset(ExNode);
						auto Ex = GetExpressionTypeNode(*ExNode);


						auto& _generic = *New->_name._ScopedName.front()._generic;

						_generic._Values.push_back(std::move(ExTypeNode));
					}


					out = New;
				}
				else //int[String] => Map<String,int> // Map<Key,Value>
				{
					TypeNode* New = new TypeNode();

					TypeNode valuetype = std::move(*out);
					TypeNode keytype = {};
					GetType(keytype);

					{
						ChangeTypeNodes(New, out);
					
						auto NameToken = new UCodeLang::Token();
						NameToken->OnLine = Token2->OnLine;
						NameToken->OnPos = Token2->OnPos;
						NameToken->Type = TokenType::Name;
						NameToken->Value._String = UCode_MapType;

						_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

						ScopedName V;
						V._token = NameToken;
						V._generic.reset(new UseGenericsNode());

						New->_name._ScopedName.push_back(std::move(V));
					}
					
					{//Key
						auto ExTypeNode = std::move(keytype);
					

						auto& _generic = *New->_name._ScopedName.front()._generic;
						_generic._Values.push_back(std::move(ExTypeNode));
					}

					{//Value
						auto ExTypeNode = std::move(valuetype);
					

						auto& _generic = *New->_name._ScopedName.front()._generic;
						_generic._Values.push_back(std::move(ExTypeNode));
					}
					out = New;
				}
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

			ChangeTypeNodes(New, out);
			
			auto NameToken = new UCodeLang::Token();
			NameToken->OnLine = Token2->OnLine;
			NameToken->OnPos = Token2->OnPos;
			NameToken->Type = TokenType::Name;
			NameToken->Value._String = UCode_OptionalType;

			_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

			ScopedName V;
			V._token = NameToken;
			V._generic.reset(new UseGenericsNode());
			V._generic->_Values.push_back(std::move(*out));

			New->_name._ScopedName.push_back(std::move(V));
			out = New;

			Token2 = TryGetToken();
		}
		else if (Token2->Type == TokenType::bitwise_XOr)//int^ => Unique_ptr<int>
		{
			NextToken();
			TypeNode* New = new TypeNode();

			ChangeTypeNodes(New, out);

			auto NameToken = new UCodeLang::Token();
			NameToken->OnLine = Token2->OnLine;
			NameToken->OnPos = Token2->OnPos;
			NameToken->Type = TokenType::Name;
			NameToken->Value._String = UCode_Unique_ptr;

			_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

			ScopedName V;
			V._token = NameToken;
			V._generic.reset(new UseGenericsNode());
			V._generic->_Values.push_back(std::move(*out));

			New->_name._ScopedName.push_back(std::move(V));
			out = New;

			Token2 = TryGetToken();
		}
		else if (Token2->Type == TokenType::DollarSign)//int$ => Shared_ptr<int>
		{
			NextToken();
			TypeNode* New = new TypeNode();

			ChangeTypeNodes(New, out);
			
			auto NameToken = new UCodeLang::Token();
			NameToken->OnLine = Token2->OnLine;
			NameToken->OnPos = Token2->OnPos;
			NameToken->Type = TokenType::Name;
			NameToken->Value._String = UCode_Shared_ptr;

			_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

			ScopedName V;
			V._token = NameToken;
			V._generic.reset(new UseGenericsNode());
			V._generic->_Values.push_back(std::move(*out));

			New->_name._ScopedName.push_back(std::move(V));
			out = New;

			Token2 = TryGetToken();
		}
		else if (Token2->Type == TokenType::Not)//int!Error => Result<int,Error>
		{
			NextToken();


			bool hasother = true;

			TypeNode othertype;
			if (hasother)
			{
				GetType(othertype);
			}
			else
			{

			}
			TypeNode* New = new TypeNode();

			ChangeTypeNodes(New, out);
			
			auto NameToken = new UCodeLang::Token();
			NameToken->OnLine = Token2->OnLine;
			NameToken->OnPos = Token2->OnPos;
			NameToken->Type = TokenType::Name;
			NameToken->Value._String = UCode_ResultType;

			_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

			ScopedName V;
			V._token = NameToken;
			V._generic.reset(new UseGenericsNode());
			V._generic->_Values.push_back(std::move(*out));
			V._generic->_Values.push_back(std::move(othertype));

			New->_name._ScopedName.push_back(std::move(V));
			out = New;

			Token2 = TryGetToken();
		}
		else if (Token2->Type == TokenType::logical_or && CanUseInlineEnum)//int||bool||char => InlineEnum<int,bool,char>
		{
			NextToken();
			Vector<TypeNode> Typelist;
			Typelist.resize(2);
			Typelist[0] = std::move(*out);
			GetType(Typelist[1], false, true, false);

			while (TryGetToken()->Type == TokenType::logical_or)
			{
				NextToken();
				GetType(Typelist.emplace_back(), false, true, false);
			}


			auto NameToken = new UCodeLang::Token();
			NameToken->OnLine = Token2->OnLine;
			NameToken->OnPos = Token2->OnPos;
			NameToken->Type = TokenType::internal_InlineEnumVariant;

			_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

			TypeNode* New = new TypeNode();
			ScopedName V;
			V._token = NameToken;
			V._generic.reset(new UseGenericsNode());
			V._generic->_Values = std::move(Typelist);

			New->_name._ScopedName.push_back(std::move(V));

			out = New;
			Token2 = TryGetToken();
		}
		else
		{
			break;
		}

	}

	if (Isasync)
	{
		TokenTypeCheck(TryGetToken(), TokenType::greaterthan);
		NextToken();

		auto NameToken = new UCodeLang::Token();
		NameToken->OnLine = Token->OnLine;
		NameToken->OnPos = Token->OnPos;
		NameToken->Type = TokenType::Name;
		NameToken->Value._String = UCode_FutureType;
		_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

		TypeNode* New = new TypeNode();

		out->_Isimmutable = false;

		ScopedName V;
		V._token = NameToken;
		V._generic.reset(new UseGenericsNode());
		V._generic->_Values.push_back(std::move(*out));
		New->_name._ScopedName.push_back(std::move(V));

		out = New;

	}


	if (addeddynmaiccloseing == false && hadsdynamic)
	{
		TokenTypeCheck(TryGetToken(), TokenType::greaterthan);
		NextToken();
	}

	return r;
}

GotNodeType Parser::GetType(TypeNode& out, bool ignoreRighthandOFtype, bool ignoreleftHandType, bool CanUseInlineEnum)
{
	TypeNode tep;
	TypeNode* tepptr = &tep;
	auto r = GetType(tepptr, ignoreRighthandOFtype, ignoreleftHandType, CanUseInlineEnum);

	out = std::move(*tepptr);

	return r;
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

	auto ScopedName = GetName(out._ScopedName);

	auto ParToken = TryGetToken();

	out._Access = GetModifier();

	if (ParToken->Type == FuncCallStart)
	{
		NextToken();
		auto Pars = GetValueParametersNode(out._Parameters);

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

	GetName(out._ScopedName);


	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();

	return GotNodeType::Success;
}


GotNodeType Parser::GetRetStatement(RetStatementNode& out)
{
	auto RetToken = TryGetToken();
	TokenTypeCheck(RetToken, TokenType::KeyWorld_Ret);
	NextToken();

	out._RetToken = RetToken;

	auto Token = TryGetToken();
	if (Token->Type == TokenType::Semicolon)
	{
		NextToken();
		out._Expression._Value = nullptr;
		return GotNodeType::Success;
	}
	else
	{
		GetExpressionTypeNode(out._Expression);
		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
		NextToken();
	}

	return GotNodeType::Success;
}

GotNodeType Parser::GetDeclareStaticVariable(DeclareStaticVariableNode& out, bool ignoreleftHandType)
{
	auto RetToken = TryGetToken();
	if (RetToken->Type == TokenType::KeyWord_export)
	{
		out._Variable._IsExport = true;
		NextToken();
		RetToken = TryGetToken();
	}

	TokenTypeCheck(RetToken, TokenType::KeyWord_static);
	NextToken();

	size_t OldIndex = _TokenIndex;

	NameNode NameValue;
	GetNameCheck(NameValue);
	auto Token3 = TryGetToken();
	if (Token3->Type == TokenType::equal)
	{
		NextToken();

		TypeNode::Gen_Var(out._Variable._Type, *NameValue.token);
		out._Variable._Name = std::move(NameValue);
		GetExpressionTypeNode(out._Variable._Expression);

		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
		NextToken();
		return GotNodeType::Success;
	}
	else
	{
		_TokenIndex = OldIndex;
		return GetDeclareVariable(out._Variable, ignoreleftHandType);
	}
}

GotNodeType Parser::GetDeclareThreadVariable(DeclareThreadVariableNode& out, bool ignoreleftHandType)
{
	auto RetToken = TryGetToken();
	if (RetToken->Type == TokenType::KeyWord_export)
	{
		out._Variable._IsExport = true;
		NextToken();
		RetToken = TryGetToken();
	}

	TokenTypeCheck(RetToken, TokenType::KeyWord_Thread);
	NextToken();


	size_t OldIndex = _TokenIndex;

	NameNode NameValue;
	GetNameCheck(NameValue);
	auto Token3 = TryGetToken();
	if (Token3->Type == TokenType::equal)
	{
		NextToken();

		TypeNode::Gen_Var(out._Variable._Type, *NameValue.token);
		out._Variable._Name = std::move(NameValue);
		GetExpressionTypeNode(out._Variable._Expression);

		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
		NextToken();
		return GotNodeType::Success;
	}
	else
	{
		_TokenIndex = OldIndex;
		return GetDeclareVariable(out._Variable, ignoreleftHandType);
	}
}

void Parser::GetDeclareVariableNoObject(TryGetNode& out)
{
	DeclareVariableNode node;

	if (TryGetToken()->Type == TokenType::KeyWord_export)
	{
		node._IsExport = true;
		NextToken();
	}
	out.GotNode = GetDeclareVariable(node);
	if (out.GotNode == GotNodeType::Success)
	{
		auto ptr = DeclareThreadVariableNode::Gen();
		//ptr->Variable = std::move(node);
		ptr->_Variable._Type = std::move(node._Type);
		ptr->_Variable._Name = std::move(node._Name);
		ptr->_Variable._Expression._Value = std::move(node._Expression._Value);

		out.Node = std::move(ptr);
	}
}

GotNodeType Parser::GetDeclareVariable(DeclareVariableNode& out, bool ignoreleftHandType)
{
	auto Type = GetType(out._Type, false, ignoreleftHandType);
	auto Name = GetName(out._Name);
	out._Access = GetModifier();

	auto Token = TryGetToken();
	if (Token && Token->Type == TokenType::equal)
	{
		NextToken();
		GetExpressionTypeNode(out._Expression);
	}

	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();

	return Merge(Type, Name);
}

GotNodeType Parser::GetAssignExpression(AssignExpressionNode& out)
{
	auto Name = GetExpressionTypeNode(out._ToAssign);


	auto Token = TryGetToken();

	if (Token->Type == TokenType::Colon && TryPeekNextToken(1)->Type == TokenType::equal)
	{
		_ErrorsOutput->AddError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Found ':=' did you mean '=:'(address reassignment operator)?");

		NextToken();
		out._Token = Token;

		Token = TryGetToken();
		out._ReassignAddress = true;
		NextToken();
		Token = TryGetToken();


		auto Ex = GetExpressionTypeNode(out._Expression);


		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
		NextToken();
		return Merge(Name, Ex);
	}
	else
	{
		TokenTypeCheck(Token, TokenType::equal);
		NextToken();
		out._Token = Token;

		Token = TryGetToken();
		if (Token->Type == TokenType::Colon)
		{
			out._ReassignAddress = true;
			NextToken();
			Token = TryGetToken();
		}

		auto Ex = GetExpressionTypeNode(out._Expression);


		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
		NextToken();
		return Merge(Name, Ex);
	}

}
GotNodeType Parser::GetIfNode(IfNode& out)
{
	auto RetToken = TryGetToken();
	TokenTypeCheck(RetToken, TokenType::KeyWord_If);
	NextToken();

	auto GetEx = GetExpressionTypeNode(out._Expression);

	auto Token3 = TryGetToken();
	TokenTypeCheck(Token3, TokenType::Colon);
	NextToken();

	auto Statements = GetStatementsorStatementNode(out._Body);


	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();

		if (T->Type != TokenType::KeyWord_Else) { break; }

		NextToken();
		auto T2 = TryGetToken();

		if (T2->Type != TokenType::Colon)
		{//else if or Item();
			auto elseNode = std::make_unique<ElseNode>();

			//auto RetToken2 = TryGetToken();
			//TokenTypeCheck(RetToken2, TokenType::KeyWorld_If);

			Unique_ptr<Node> V;
			auto Statements = GetStatement(V);
			if (V)
			{
				elseNode->_Body._Nodes.push_back(std::move(V));
			}

			out._Else = std::move(elseNode);
			break;
		}
		else
		{//else
			auto elseNode = std::make_unique<ElseNode>();

			auto ColonToken = TryGetToken();
			TokenTypeCheck(ColonToken, TokenType::Colon);
			NextToken();

			auto Statements = GetStatementsorStatementNode(elseNode->_Body);

			out._Else = std::move(elseNode);

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

	auto GetEx = GetExpressionTypeNode(out._Expression);

	auto Token3 = TryGetToken();
	if (Token3->Type != TokenType::Semicolon)
	{
		TokenTypeCheck(Token3, TokenType::Colon);
		NextToken();

		auto Statements = GetStatementsorStatementNode(out._Body);
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
		auto Statements = GetStatementsorStatementNode(out._Body);
	}

	auto Token3 = TryGetToken();
	TokenTypeCheck(Token3, TokenType::KeyWord_while);
	NextToken();

	auto GetEx = GetExpressionTypeNode(out._Expression);

	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();
	return GotNodeType::Success;
}
GotNodeType Parser::DoEnumType(EnumNode* output, const Token* ClassToken, GenericValuesNode& TepGenerics, InheritedTypeData& Inherited)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWord_Enum);
	NextToken();

	if (TryGetToken()->Type == TokenType::KeyWord_export)
	{
		output->_IsExport = true;
		NextToken();
	}

	output->_EnumName.token = ClassToken;
	output->_generic = std::move(TepGenerics);
	output->_Access = GetModifier();
	if (Inherited._values.size() > 1)
	{
		_ErrorsOutput->AddError(ErrorCodes::ExpectingSequence, Token->OnLine, Token->OnPos, "Cant Inherit more then one type for enum's.");
	}
	if (Inherited._values.size())
	{
		ScopedName V;
		V._token = std::move(Inherited._values.front()._Name.token);
		output->_BaseType._name._ScopedName.push_back(std::move(V));
	}
	else
	{
		TypeNode::Gen_Byte(output->_BaseType, *ClassToken);
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

		output->_Values.push_back(EnumValueNode());
		EnumValueNode& EnumValue = output->_Values.back();
		GetEnumValueNode(EnumValue);


		auto ColonToken = TryGetToken();
		if (ColonToken == nullptr || ColonToken->Type != TokenType::Comma) { break; }
		NextToken();
	}

	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();
	output->EndOfClass = EndToken;

	return GotNodeType::Success;
}

GotNodeType Parser::GetEnumValueNode(EnumValueNode& out)
{
	GetName(out._Name);


	auto BToken = TryGetToken();
	Unique_ptr<AnonymousTypeNode> ClassType = nullptr;
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
					pars._Name.token = CToken;
					NextToken();
				}
				else
				{
					pars._Name.token = out._Name.token;
				}

				if (ClassType == nullptr) {
					ClassType = AnonymousTypeNode::Gen();
				}



				pars._Type = std::move(V);
				ClassType->_Fields._Parameters.push_back(std::move(pars));


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
					pars._Name.token = out._Name.token;
					pars._Type = std::move(V);
					ClassType->_Fields._Parameters.push_back(std::move(pars));
					NextToken();
					continue;
				}

				out._VariantType = Opt(std::move(V));
				break;
			}



		}
		if (ClassType)
		{
			TypeNode V;
			V._node = std::move(ClassType);
			out._VariantType = Opt(std::move(V));
		}

		auto RToken = TryGetToken();
		TokenTypeCheck(RToken, TokenType::Right_Bracket);
		NextToken();
	}

	auto EqualToken = TryGetToken();
	if (EqualToken && EqualToken->Type == TokenType::equal)
	{
		NextToken();
		return GetExpressionTypeNode(out._Expression);
	}
	return GotNodeType::Success;
}
GotNodeType Parser::DoTagType(TagTypeNode* output, const Token* ClassToken, GenericValuesNode& TepGenerics, InheritedTypeData& Inherited)
{
	TokenTypeCheck(TryGetToken(), TokenType::KeyWord_Tag);
	NextToken();
	output->_AttributeName.token = ClassToken;
	output->_generic = std::move(TepGenerics);

	if (TryGetToken()->Type == TokenType::KeyWord_export)
	{
		output->_IsExport = true;
		NextToken();
	}

	auto ColonToken = TryGetToken();
	if (ColonToken->Type == TokenType::Semicolon) { NextToken(); return GotNodeType::Success; }

	

	TokenTypeCheck(ColonToken, TokenType::Colon); NextToken();
	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab); NextToken();

	AttributeStart();
	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();

		TryGetNode V;

		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		case TokenType::KeyWord_unsafe:
		case Parser::declareFunc:V = GetFuncNode(); break;
		default:V = GetDeclareVariable();
		}
		if (T->Type != TokenType::Left_Bracket)
		{
			AttributeCheck();
		}
		else
		{
			continue;
		}
		if (V.Node)
		{
			output->_Nodes.push_back(std::move(V.Node));
		}
	}

EndLoop:
	AttributeEnd();
	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();
	output->EndOfClass = EndToken;


	return GotNodeType::Success;
}
GotNodeType Parser::GetFuncCallStatementNode(FuncCallStatementNode& out)
{
	GetFuncCallNode(out._Base);

	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();
	return GotNodeType::Success;
}
GotNodeType Parser::GetFuncCallStatementNode2(Unique_ptr<Node>& out)
{
	FuncCallNode node;
	GetFuncCallNode(node);

	auto SemicolonToken = TryGetToken();

	if (SemicolonToken->Type == TokenType::Semicolon)
	{
		NextToken();

		auto V = FuncCallStatementNode::Gen();

		V->_Base = std::move(node);
		out = std::move(V);

		return GotNodeType::Success;
	}
	else if (ScopedName::Get_IsScoped(SemicolonToken->Type))
	{
		auto Operator = SemicolonToken->Type;

		auto valex = ValueExpressionNode::Gen();
		valex->_Value = std::move(std::make_unique<FuncCallNode>(std::move(node)));
		Unique_ptr<Node> Ex = std::move(valex);

		do
		{

			NextToken();

			ScopedNameNode node;
			GetName(node);
	
			bool isfunc = false;
			auto DotToken = TryGetToken();
			if (DotToken->Type == TokenType::Left_Parentheses)
			{
				NextToken();

				auto ExtendedNode = ExtendedFuncExpression::Gen();
				ExtendedNode->_Expression._Value = std::move(Ex);
				ExtendedNode->_Operator = ScopedName::Get_Scoped(Operator);
				ExtendedNode->_Extended._FuncName._ScopedName = std::move(node._ScopedName);

				GetValueParametersNode(ExtendedNode->_Extended.Parameters);

				TokenTypeCheck(TryGetToken(), TokenType::Right_Parentheses);
				NextToken();

				Ex = std::move(ExtendedNode);

				isfunc = true;
			}
			else
			{

				auto ExtendedNode = ExtendedScopeExpression::Gen();
				ExtendedNode->_Expression._Value = std::move(Ex);
				ExtendedNode->_Operator = ScopedName::Get_Scoped(Operator);
				ExtendedNode->_Extended._ScopedName = std::move(node._ScopedName);

				Ex = std::move(ExtendedNode);
			}

			auto nexttoken = TryGetToken();

			if (nexttoken->Type == TokenType::Semicolon && isfunc)
			{
				NextToken();
				
				auto ptr = (ExtendedFuncExpression*)Ex.get();
				auto NewEx = std::make_unique<ExtendedFuncStatement>(std::move(*ptr));
				
				out = std::move(NewEx);
				return GotNodeType::Success;
			}
			else if (ScopedName::Get_IsScoped(nexttoken->Type))
			{
				continue;
			}
			else
			{
				UCodeLangToDo();
			}

		} while (true);

	}
	else
	{
		TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	}

	return GotNodeType::Success;
}
GotNodeType Parser::GetFuncCallNode(FuncCallNode& out)
{
	GetName(out._FuncName, true);

	auto ParToken = TryGetToken();

	TokenTypeCheck(ParToken, FuncCallStart);
	NextToken();

	GetValueParametersNode(out.Parameters);

	auto Par2Token = TryGetToken();
	TokenTypeCheck(Par2Token, FuncCallEnd);
	NextToken();

	return GotNodeType::Success;
}

GotNodeType Parser::GetPostfixStatement(PostfixVariableNode& out, bool DoSemicolon)
{
	auto Name = GetExpressionTypeNode(out._ToAssign);
	auto Token = TryGetToken();
	out._PostfixOp = Token;
	NextToken();

	if (DoSemicolon) {
		auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
		NextToken();
	}
	return GotNodeType::Success;
}
GotNodeType Parser::GetCompoundStatement(CompoundStatementNode& out)
{
	auto Name = GetExpressionTypeNode(out._ToAssign);
	auto Token = TryGetToken();
	out._CompoundOp = Token;
	NextToken();

	GetExpressionTypeNode(out._Expession);

	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, TokenType::Semicolon);
	NextToken();
	return GotNodeType::Success;
}

GotNodeType Parser::GetAnonymousObjectConstructorNode(AnonymousObjectConstructorNode& out)
{
	auto SemicolonToken = TryGetToken(); TokenTypeCheck(SemicolonToken, AnonymousObjectStart);
	NextToken();

	GetValueParametersNode(out._Fields);

	auto SemicolonToken2 = TryGetToken(); TokenTypeCheck(SemicolonToken2, AnonymousObjectEnd);
	NextToken();

	out.token = SemicolonToken;
	return GotNodeType::Success;
}

GotNodeType Parser::GetDropStatementNode(DropStatementNode& out)
{
	auto DropToken = TryGetToken(); TokenTypeCheck(DropToken, TokenType::KeyWord_Drop);
	NextToken();
	out._KeywordToken = DropToken;

	auto ParToken = TryGetToken();
	TokenTypeCheck(ParToken, FuncCallStart);
	NextToken();

	Unique_ptr<Node> Ptr = nullptr;
	GetExpressionTypeNode(Ptr);
	out._expression._Value = std::move(Ptr);

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

	out._KeywordToken = NewToken;

	GetType(out._Type, true);



	auto ParToken = TryGetToken();
	if (ParToken->Type == FuncCallStart) {

		NextToken();

		GetValueParametersNode(out._Parameters);

		auto Par2Token = TryGetToken();
		TokenTypeCheck(Par2Token, FuncCallEnd);
		NextToken();
	}
	else if (ParToken->Type == TokenType::Left_Bracket)
	{
		NextToken();

		GetExpressionTypeNode(out._Arrayexpression);

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
GotNodeType Parser::GetimutVariableDeclare(Unique_ptr<Node>& out)
{
	auto NewToken = TryGetToken(); TokenTypeCheck(NewToken, TokenType::KeyWord_imut);
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
			auto V = DeclareThreadVariableNode::Gen();
			
			r = GetDeclareThreadVariable(*V, true);
			Tnode->SetAsimmutable();
			
			out = std::move(V);
		}
		else
		{
			auto V = DeclareVariableNode::Gen();

			TypeNode::Gen_Var(V->_Type, *NameValue.token);
			V->_Name = std::move(NameValue);
			GetExpressionTypeNode(V->_Expression);

			auto SemicolonToken2 = TryGetToken(); TokenTypeCheck(SemicolonToken2, TokenType::Semicolon);
			NextToken();

			Tnode = &V->_Type;
			Tnode->SetAsimmutable();
			
			r = GotNodeType::Success;
			out = std::move(V);
		}
	}
	else if (Token3->Type == TokenType::Semicolon)
	{

		auto Token = NameValue.token;
		_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
			, "can't guess type theres no '=' [expression]");
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
			auto V = DeclareThreadVariableNode::Gen();
			r = GetDeclareThreadVariable(*V, true);
			
			Tnode = &V->_Variable._Type;
			Tnode->SetAsimmutable();
			
			out = std::move(V);
		}
		break;
		case TokenType::KeyWord_static:
		{
			auto V = DeclareStaticVariableNode::Gen();
			r = GetDeclareStaticVariable(*V, true);
			
			Tnode = &V->_Variable._Type;
			Tnode->SetAsimmutable();
		
			out = std::move(V);
		}break;
		default:
		{
			auto V = DeclareVariableNode::Gen();
			r = GetDeclareVariable(*V, true);
			
			Tnode = &V->_Type;
			Tnode->SetAsimmutable();
			
			out = std::move(V);
		}break;
		}
	}
	return r;
}

void Parser::ForNodeModernIntLoop(ForNode& out, const Token* OtherToken, const Token* NameToken, TokenType BinaryOp, TokenType PostFixOp)
{
	ExpressionNodeType WhenToEnd;
	GetExpressionTypeNode(WhenToEnd);


	out._Type = ForNode::ForType::Traditional;

	out._Traditional_Assignment_Expression._Value = std::move(out._Modern_List._Value);

	if (PostFixOp == TokenType::decrement)
	{
		std::swap(out._Traditional_Assignment_Expression._Value, WhenToEnd._Value);
	}

	{
		BinaryExpressionNode* BinExpression = new BinaryExpressionNode();
		out._BoolExpression._Value.reset(BinExpression);

		{
			auto BinToken = Token();
			BinToken.OnLine = OtherToken->OnLine;
			BinToken.OnPos = OtherToken->OnPos;
			BinToken.Type = BinaryOp;



			auto V = std::make_unique<Token>(std::move(BinToken));
			BinExpression->_BinaryOp = V.get();
			_Tree.TemporaryTokens.push_back(std::move(V));
		}

		{
			ValueExpressionNode* ValuNode = new ValueExpressionNode();
			BinExpression->_Value0._Value.reset(ValuNode);

			ReadVariableNode* ReadVarNode = new ReadVariableNode();
			ValuNode->_Value.reset(ReadVarNode);

			ScopedName ScopeName;
			ScopeName._token = NameToken;
			ReadVarNode->_VariableName._ScopedName.push_back(std::move(ScopeName));

		}

		BinExpression->_Value1._Value = std::move(WhenToEnd._Value);

		if (PostFixOp == TokenType::decrement)
		{
			BinaryExpressionNode* BinExpression2 = new BinaryExpressionNode();

			BinExpression2->_Value0._Value = std::move(BinExpression->_Value1._Value);


			BinExpression->_Value1._Value.reset(BinExpression2);

			{
				auto BinToken = Token();
				BinToken.OnLine = OtherToken->OnLine;
				BinToken.OnPos = OtherToken->OnPos;
				BinToken.Type = TokenType::minus;


				auto V = std::make_unique<Token>(std::move(BinToken));
				BinExpression2->_BinaryOp = V.get();
				_Tree.TemporaryTokens.push_back(std::move(V));
			}


			{
				ValueExpressionNode* NumNode = new ValueExpressionNode();
				BinExpression2->_Value1._Value.reset(NumNode);

				NumberliteralNode* NumLiteral = new NumberliteralNode();
				NumNode->_Value.reset(NumLiteral);


				auto BinToken = Token();
				BinToken.OnLine = OtherToken->OnLine;
				BinToken.OnPos = OtherToken->OnPos;
				BinToken.Type = TokenType::Number_literal;
				BinToken.Value._String = "1";


				auto V = std::make_unique<Token>(std::move(BinToken));
				NumLiteral->token = V.get();
				_Tree.TemporaryTokens.push_back(std::move(V));
			}
		}


		//
	}


	{

		ValueExpressionNode* ValuNode = new ValueExpressionNode();
		out._OnNextStatement._ToAssign._Value.reset(ValuNode);


		ReadVariableNode* ReadVarNode = new ReadVariableNode();
		ValuNode->_Value.reset(ReadVarNode);

		ScopedName ScopeName;
		ScopeName._token = NameToken;
		ReadVarNode->_VariableName._ScopedName.push_back(std::move(ScopeName));


		auto BinToken = Token();
		BinToken.OnLine = OtherToken->OnLine;
		BinToken.OnPos = OtherToken->OnPos;
		BinToken.Type = PostFixOp;

		auto V = std::make_unique<Token>(std::move(BinToken));
		out._OnNextStatement._PostfixOp = V.get();
		_Tree.TemporaryTokens.push_back(std::move(V));
	}
}


GotNodeType Parser::GetForNode(ForNode& out)
{
	//auto ForToken = TryGetToken(); TokenTypeCheck(ForToken, TokenType::KeyWord_for);
	NextToken();

	auto LeftBracket = TryGetToken(); TokenTypeCheck(LeftBracket, TokenType::Left_Bracket);
	NextToken();

	bool didoneloop = false;
	do
	{
		if (didoneloop)
		{
			NextToken();//to pass the , token
		}

		
		bool isjustname = false;
		auto ind = _TokenIndex;

		{
			NextToken();
			auto token = TryGetToken();
			if (token->Type == TokenType::Colon
				|| token->Type == TokenType::Comma
				|| token->Type == TokenType::equal)
			{
				isjustname = true;
			}

			_TokenIndex = ind;
		}


		auto NameToken = TryGetToken(); 		
		TypeNode V;
		if (isjustname)
		{
			TokenTypeCheck(NameToken, TokenType::Name);
			TypeNode::Gen_Var(V,*NameToken);
		}
		else
		{
			GetType(V, false, false);
			NameToken = TryGetToken();
		}
		NextToken();

		if (didoneloop)
		{
			ForNode::ForVarable val;
			val._Name = NameToken;
			val._typeNode = std::move(V);

			out._OtherVarables.push_back(std::move(val));
		}
		else
		{
			out._Name = NameToken;
			out._typeNode = std::move(V);
		}

		didoneloop = true;

	} while (TryGetToken()->Type ==TokenType::Comma);

	auto Assemnt = TryGetToken();
	if (Assemnt->Type == TokenType::equal)
	{
		if (out._OtherVarables.size())
		{
			TokenTypeCheck(Assemnt, TokenType::Colon);
		}

		out._Type = ForNode::ForType::Traditional;

		NextToken();
		GetExpressionTypeNode(out._Traditional_Assignment_Expression);

		TokenTypeCheck(TryGetToken(), TokenType::Semicolon); NextToken();

		GetExpressionTypeNode(out._BoolExpression);

		TokenTypeCheck(TryGetToken(), TokenType::Semicolon); NextToken();

		GetPostfixStatement(out._OnNextStatement, false);
	}
	else if (Assemnt->Type == TokenType::Colon)
	{
		NextToken();
		out._Type = ForNode::ForType::modern;

		GetExpressionTypeNode(out._Modern_List);




		auto OtherToken = TryGetToken();

		if (OtherToken->Type == TokenType::RightAssignArrow)
		{
			NextToken();

			ForNodeModernIntLoop(out, OtherToken,out._Name, TokenType::lessthan, TokenType::increment);
		}
		else if (OtherToken->Type == TokenType::equal)
		{
			NextToken();
			TokenTypeCheck(TryGetToken(), TokenType::lessthan);
			NextToken();

			ForNodeModernIntLoop(out, OtherToken,out._Name, TokenType::Notequal_Comparison, TokenType::decrement);
		}
	}
	else
	{
		TokenTypeCheck(Assemnt, TokenType::equal);
	}

	auto RightBracket = TryGetToken(); TokenTypeCheck(RightBracket, TokenType::Right_Bracket); NextToken();

	auto Token3 = TryGetToken();
	if (Token3->Type != TokenType::Semicolon)
	{
		TokenTypeCheck(Token3, TokenType::Colon);
		NextToken();

		auto Statements = GetStatementsorStatementNode(out._Body);
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

	out._token = KeyToken;
	return  GotNodeType::Success;
}
GotNodeType Parser::GetBreakNode(BreakNode& out)
{
	auto KeyToken = TryGetToken(); TokenTypeCheck(KeyToken, TokenType::KeyWord_break);
	NextToken();

	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::Semicolon);
	NextToken();

	out._token = KeyToken;
	return  GotNodeType::Success;
}
GotNodeType Parser::GetLambdaNode(LambdaNode& out)
{
	auto KeyToken = TryGetToken(); TokenTypeCheck(KeyToken, Parser::declareFunc);
	NextToken();


	auto LeftBracket = TryGetToken(); TokenTypeCheck(LeftBracket, TokenType::Left_Bracket);
	NextToken();

	out._LambdaStart = KeyToken;
	//
	{
		const Token* LoopToken = TryGetToken();
		while (LoopToken->Type != TokenType::Right_Bracket && LoopToken->Type != TokenType::EndofFile)
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
				GetType(par._Type, false, false);
				GetName(par._Name);
				out._Pars._Parameters.push_back(std::move(par));
			}
			else
			{
				NamedParameterNode par;
				GetName(par._Name);

				TypeNode::Gen_Var(par._Type, *par._Name.token);

				out._Pars._Parameters.push_back(std::move(par));
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
	if (ParToken->Type == TokenType::Left_Brace)
	{
		NextToken();
		LambdaCapturesData Captures;

		out._Capture = Opt(std::move(Captures));
		auto OtherPar = TryGetToken(); TokenTypeCheck(OtherPar, TokenType::Right_Bracket); NextToken();
	}


	auto AssmentToken = TryGetToken();
	if (AssmentToken->Type == TokenType::Colon)
	{
		NextToken();
		StatementsNode Statements;
		GetStatementsorStatementNode(Statements);

		out._Statements = Opt(std::move(Statements));
	}
	else if (AssmentToken->Type == TokenType::RightAssignArrow)
	{
		NextToken();
		StatementsNode Statements;

		auto r = RetStatementNode::Gen();
		r->_RetToken = AssmentToken;
		GetExpressionTypeNode(r->_Expression);
		Statements._Nodes.push_back(std::move(r));


		out._Statements = Opt(std::move(Statements));

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
GotNodeType Parser::GetShortLambdaNode(LambdaNode& out)
{
	while (TryGetToken()->Type == TokenType::Name)
	{
		auto nametoken = TryGetToken();
		NextToken();

		if (TryGetToken()->Type == TokenType::Comma)		
		{
			NextToken();
		}
		else if (TryGetToken()->Type == TokenType::Right_Bracket)
		{

		}
		else
		{
			break;
		}

		NamedParameterNode par;
		par._Name.token = nametoken;
		TypeNode::Gen_Var(par._Type, *par._Name.token);
		out._Pars._Parameters.push_back(std::move(par));
	}


	auto endtoken = TryGetToken();
	TokenTypeCheck(endtoken, TokenType::Right_Bracket);
	NextToken();

	out._LambdaStart = endtoken;

	auto AssmentToken = TryGetToken();
	if (AssmentToken->Type == TokenType::Colon)
	{
		NextToken();
		StatementsNode Statements;
		GetStatementsorStatementNode(Statements);

		out._Statements = Opt(std::move(Statements));
	}
	else if (AssmentToken->Type == TokenType::RightAssignArrow)
	{
		NextToken();
		StatementsNode Statements;

		auto r1 = RetStatementNode::Gen();
		r1->_RetToken = AssmentToken;
		GetExpressionTypeNode(r1->_Expression);
		Statements._Nodes.push_back(std::move(r1));


		out._Statements = Opt(std::move(Statements));

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
GotNodeType Parser::DoTraitType(TraitNode* output, const Token* ClassToken, GenericValuesNode& TepGenerics, InheritedTypeData& Inherited)
{
	TokenTypeCheck(TryGetToken(), TokenType::KeyWord_trait);
	NextToken();
	output->_Name.token = ClassToken;
	output->_generic = std::move(TepGenerics);

	if (TryGetToken()->Type == TokenType::KeyWord_export)
	{
		output->_IsExport = true;
		NextToken();
	}

	auto ColonToken = TryGetToken();
	if (ColonToken->Type == TokenType::Semicolon)
	{
		output->EndOfClass = ColonToken;
		NextToken();
		return GotNodeType::Success;
	}

	

	TokenTypeCheck(ColonToken, TokenType::Colon); NextToken();
	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab); NextToken();

	AccessStart();
	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();

		TryGetNode V;

		if (T->Type == TokenType::KeyWord_dynamic)
		{

			auto i = _TokenIndex;

			NextToken();
			if (TryGetToken()->Type == Parser::declareFunc)
			{
				_TokenIndex = i;
				V = GetFuncNode();
				if (T->Type != TokenType::Left_Bracket)
				{
					AttributeCheck();
				}
				else
				{
					continue;
				}
				if (V.Node)
				{
					output->_Nodes.push_back(std::move(V.Node));
				}
				continue;
			}
			else
			{
				_TokenIndex = i;
			}
		}

		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		case TokenType::KeyWord_export:
		case TokenType::KeyWord_extern:
		case TokenType::KeyWord_unsafe:
		case Parser::declareFunc:V = GetFuncNode(); break;
		case TokenType::Class:V = GetClassNode(); break;
		case TokenType::KeyWorld_public:
		{
			NextToken(); TokenTypeCheck(TryGetToken(), TokenType::Colon); NextToken();
			_AccessModifier.top() = AccessModifierType::Public;
			TokenTypeCheck(TryGetToken(), TokenType::StartTab); NextToken();

			TraitAccessModifierInerScope(output->_Nodes);

			TokenTypeCheck(TryGetToken(), TokenType::EndTab); NextToken();
		};
		break;
		case TokenType::KeyWorld_private:
		{
			NextToken(); TokenTypeCheck(TryGetToken(), TokenType::Colon); NextToken();
			_AccessModifier.top() = AccessModifierType::Private;
			TokenTypeCheck(TryGetToken(), TokenType::StartTab); NextToken();

			TraitAccessModifierInerScope(output->_Nodes);

			TokenTypeCheck(TryGetToken(), TokenType::EndTab); NextToken();
		};
		break;
		default:V = GetDeclareVariable();
		}
		if (T->Type != TokenType::Left_Bracket)
		{
			AttributeCheck();
		}
		else
		{
			continue;
		}
		if (V.Node)
		{
			output->_Nodes.push_back(std::move(V.Node));
		}
	}


EndLoop:
	AccessEnd();
	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();
	output->EndOfClass = EndToken;

	return GotNodeType::Success;
}
void  Parser::TraitAccessModifierInerScope(Vector< Unique_ptr<Node>>& Out)
{
	AttributeStart();
	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		TryGetNode V;
		auto T = TryGetToken();
		switch (T->Type)
		{
		case TokenType::EndTab:goto EndLoop;
		case TokenType::KeyWord_unsafe:
		case Parser::declareFunc:V = GetFuncNode(); break;

		default:V = GetDeclareVariable();
		}
		if (T->Type != TokenType::Left_Bracket)
		{
			AttributeCheck();
		}
		else
		{
			continue;
		}
		if (V.Node)
		{
			Out.push_back(std::move(V.Node));
		}
	}
EndLoop:
	AttributeEnd();
	return;
}
GotNodeType Parser::GetBitCastExpression(BitCastExpression& out)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWord_bitcast);
	NextToken();
	out._KeywordToken = Token;

	TokenTypeCheck(TryGetToken(), TokenType::lessthan); NextToken();

	GetType(out._Type, false, false);

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

		out._KeyWord = Token;
		out._StringExpression._Value = std::move(V._Value);
	}

	auto RToken = TryGetToken();
	TokenTypeCheck(RToken, TokenType::Right_Parentheses);
	NextToken();


	auto EndToken = TryGetToken();

	TokenTypeCheck(EndToken, TokenType::Semicolon);
	NextToken();

	return GotNodeType::Success;
}
GotNodeType Parser::GetInvalidNode(InvalidNode& out)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWord_invalid);
	out._KeyWord = Token;

	NextToken();
	auto LToken = TryGetToken();
	TokenTypeCheck(LToken, TokenType::Left_Parentheses);

	NextToken();
	auto ExToken = TryGetToken();
	if (ExToken->Type != TokenType::Right_Parentheses)
	{
		ExpressionNodeType V;
		GetExpressionTypeNode(V);


		out._StringExpression._Value = std::move(V._Value);
	}

	auto RToken = TryGetToken();
	TokenTypeCheck(RToken, TokenType::Right_Parentheses);
	NextToken();


	auto EndToken = TryGetToken();

	TokenTypeCheck(EndToken, TokenType::Semicolon);
	NextToken();

	return GotNodeType::Success;
}
GotNodeType Parser::GetValidNode(ValidNode& out)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWord_valid);

	NextToken();
	auto LToken = TryGetToken();
	TokenTypeCheck(LToken, TokenType::Left_Parentheses);

	NextToken();
	auto ExToken = TryGetToken();
	if (ExToken->Type != TokenType::Right_Parentheses)
	{

		if (ExToken->Type == TokenType::Colon)
		{
			NextToken();
			StatementsNode V;
			GetStatementsorStatementNode(V);

			out._StatementToCheck = std::move(V);
			out._IsExpression = false;
		}
		else
		{
			ExpressionNodeType V;
			GetExpressionTypeNode(V);


			out._ExpressionToCheck._Value = std::move(V._Value);
			out._IsExpression = true;
		}
	}

	auto RToken = TryGetToken();
	TokenTypeCheck(RToken, TokenType::Right_Parentheses);
	NextToken();


	return GotNodeType::Success;

}
GotNodeType Parser::GeTypeExNode(Unique_ptr<Node>& out)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWord_type);

	NextToken();
	auto LToken = TryGetToken();
	TokenTypeCheck(LToken, TokenType::Left_Parentheses);
	NextToken();

	{
		TypeNode Op1;
		GetType(Op1, false, false);


		auto CMPToken = TryGetToken();
		if (CMPTypesNode::IsOp(CMPToken->Type))
		{
			NextToken();
			auto V = CMPTypesNode::Gen();
			V->_TypeOp0 = std::move(Op1);
			V->_Op = CMPToken;
			GetType(V->_TypeOp1, false, false);
			out = std::move(V);
		}
		else
		{
			auto V = TypeToValueNode::Gen();
			V->TypeOp = std::move(Op1);
			out = std::move(V);
		}



	}

	auto RToken = TryGetToken();
	TokenTypeCheck(RToken, TokenType::Right_Parentheses);
	NextToken();


	return GotNodeType::Success;
}
GotNodeType Parser::GetExpressionToTypeValue(ExpressionToTypeValueNode& out)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWorld_typeof);

	NextToken();
	auto LToken = TryGetToken();
	TokenTypeCheck(LToken, TokenType::Left_Parentheses);
	NextToken();

	{
		GetExpressionTypeNode(out._TypeEx);
	}
	auto RToken = TryGetToken();
	TokenTypeCheck(RToken, TokenType::Right_Parentheses);
	NextToken();


	return GotNodeType::Success;
}
GotNodeType Parser::GetEvalDeclare(Unique_ptr<Node>& out)
{
	size_t EvalIndex = _TokenIndex;

	auto NewToken = TryGetToken(); TokenTypeCheck(NewToken, TokenType::KeyWord_eval);
	NextToken();

	auto Token2 = TryGetToken();
	if (Token2->Type == Parser::declareFunc)
	{
		_TokenIndex = EvalIndex;
		auto V = GetFuncNode();
		out = std::move(V.Node);
		return V.GotNode;
	}
	else
	{


		bool isexport = false;
		if (TryGetToken()->Type == TokenType::KeyWord_export)
		{
			isexport = true;
			NextToken();
		}
		size_t OldIndex = _TokenIndex;


		NameNode NameValue;
		GetNameCheck(NameValue);


		TypeNode* Tnode = nullptr;
		auto Token3 = TryGetToken();

		GotNodeType r;

		if (Token3->Type == TokenType::equal)
		{
			NextToken();
			auto V = DeclareEvalVariableNode::Gen();

			if (isexport)
			{
				V->_Variable._IsExport = true;
			}

			TypeNode::Gen_Var(V->_Variable._Type, *NameValue.token);
			V->_Variable._Name = std::move(NameValue);
			GetExpressionTypeNode(V->_Variable._Expression);

			auto SemicolonToken2 = TryGetToken(); TokenTypeCheck(SemicolonToken2, TokenType::Semicolon);
			NextToken();

			Tnode = &V->_Variable._Type;
			
			out = std::move(V);
			r = GotNodeType::Success;
		}
		else if (Token3->Type == TokenType::Semicolon)
		{

			auto Token = NameValue.token;
			_ErrorsOutput->AddError(ErrorCodes::InValidType, Token->OnLine, Token->OnPos
				, "can't guess type theres no '=' [expression]");
			NextToken();
			return GotNodeType::Success;
		}
		else
		{
			_TokenIndex = OldIndex;
			auto V = DeclareEvalVariableNode::Gen();
			r = GetDeclareVariable(V->_Variable, true);
			out = std::move(V);
		}
		return r;
	}
}
GotNodeType Parser::GetCompileTimeIf(CompileTimeIfNode& out, bool IsInFunc)
{
	auto Token = TryGetToken();
	TokenTypeCheck(Token, TokenType::KeyWord_ClassIf);

	NextToken();

	GetExpressionTypeNode(out._Expression);

	auto Token3 = TryGetToken();
	TokenTypeCheck(Token3, TokenType::Colon);
	NextToken();

	GotNodeType Statements;
	if (IsInFunc)
	{
		Statements = GetStatementsorStatementNode(out._Body);
	}
	else
	{
		auto TabToken = TryGetToken();

		if (TabToken->Type == TokenType::StartTab)
		{
			NextToken();
		}

		while (TryGetToken()->Type != TokenType::EndofFile)
		{
			auto T = TryGetToken();
			TryGetNode V;
			if (T->Type == TokenType::EndTab)
			{
				break;
			}

			switch (T->Type)
			{
			case TokenType::Class:V = GetClassNode(); break;
			case TokenType::KeyWord_unsafe:
			case TokenType::KeyWord_extern:
			case Parser::declareFunc:V = GetFuncNode(); break;
			case TokenType::KeyWord_static:V = GetDeclareStaticVariable(); break;
			case TokenType::KeyWord_Thread:V = GetDeclareThreadVariable(); break;
			case TokenType::KeyWord_imut:V = GetimutVariableDeclare(); break;
			case TokenType::KeyWord_eval:V = GetEvalDeclare(); break;
			case TokenType::Left_Bracket:V = GetAttribute(); break;
			case TokenType::KeyWord_ClassFor:V = GetForTypeNode(); break;
			default:
				UCodeLangUnreachable();
				break;
			}

			if (T->Type != TokenType::Left_Bracket)
			{
				AttributeCheck();
			}
			else
			{
				continue;
			}

			if (V.Node)
			{
				out._Body._Nodes.push_back(std::move(V.Node));
			}
			else { break; }


			if (V.GotNode != GotNodeType::Success) { break; }
		}


		auto TabToken2 = TryGetToken();

		if (TabToken2->Type == TokenType::EndTab)
		{
			NextToken();
		}
		Statements = GotNodeType::Success;
	}
	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		auto T = TryGetToken();

		if (T->Type != TokenType::KeyWord_ClassElse) { break; }

		NextToken();
		auto T2 = TryGetToken();

		if (T2->Type != TokenType::Colon)
		{//else if or Item();
			auto elseNode = std::make_unique<ElseNode>();

			//auto RetToken2 = TryGetToken();
			//TokenTypeCheck(RetToken2, TokenType::KeyWorld_If);

			Unique_ptr<Node> V;
			auto Statements = GetStatement(V);
			if (V)
			{
				elseNode->_Body._Nodes.push_back(std::move(V));
			}

			out._Else = std::move(elseNode);
			break;
		}
		else
		{//else
			auto elseNode = std::make_unique<ElseNode>();

			auto ColonToken = TryGetToken();
			TokenTypeCheck(ColonToken, TokenType::Colon);
			NextToken();

			auto Statements = GetStatementsorStatementNode(elseNode->_Body);

			out._Else = std::move(elseNode);

			break;
		}

	}

	return Statements;
}
GotNodeType Parser::GetCompileTimeForNode(CompileTimeForNode& out)
{
	ForNode& f = *(ForNode*)&out;
	return GetForNode(f);
}
void Parser::CompileTimeForNodeModernIntLoop(CompileTimeForNode& out, const Token* OtherToken, const Token* NameToken, TokenType BinaryOp, TokenType PostFixOp)
{
	ExpressionNodeType WhenToEnd;
	GetExpressionTypeNode(WhenToEnd);


	out._Type = CompileTimeForNode::ForType::Traditional;

	out._Traditional_Assignment_Expression._Value = std::move(out._Modern_List._Value);

	if (PostFixOp == TokenType::decrement)
	{
		std::swap(out._Traditional_Assignment_Expression._Value, WhenToEnd._Value);
	}

	{
		BinaryExpressionNode* BinExpression = new BinaryExpressionNode();
		out._BoolExpression._Value.reset(BinExpression);

		{
			auto BinToken = Token();
			BinToken.OnLine = OtherToken->OnLine;
			BinToken.OnPos = OtherToken->OnPos;
			BinToken.Type = BinaryOp;



			auto V = std::make_unique<Token>(std::move(BinToken));
			BinExpression->_BinaryOp = V.get();
			_Tree.TemporaryTokens.push_back(std::move(V));
		}

		{
			ValueExpressionNode* ValuNode = new ValueExpressionNode();
			BinExpression->_Value0._Value.reset(ValuNode);

			ReadVariableNode* ReadVarNode = new ReadVariableNode();
			ValuNode->_Value.reset(ReadVarNode);

			ScopedName ScopeName;
			ScopeName._token = NameToken;
			ReadVarNode->_VariableName._ScopedName.push_back(std::move(ScopeName));

		}

		BinExpression->_Value1._Value = std::move(WhenToEnd._Value);

		if (PostFixOp == TokenType::decrement)
		{
			BinaryExpressionNode* BinExpression2 = new BinaryExpressionNode();

			BinExpression2->_Value0._Value = std::move(BinExpression->_Value1._Value);


			BinExpression->_Value1._Value.reset(BinExpression2);

			{
				auto BinToken = Token();
				BinToken.OnLine = OtherToken->OnLine;
				BinToken.OnPos = OtherToken->OnPos;
				BinToken.Type = TokenType::minus;


				auto V = std::make_unique<Token>(std::move(BinToken));
				BinExpression2->_BinaryOp = V.get();
				_Tree.TemporaryTokens.push_back(std::move(V));
			}


			{
				ValueExpressionNode* NumNode = new ValueExpressionNode();
				BinExpression2->_Value1._Value.reset(NumNode);

				NumberliteralNode* NumLiteral = new NumberliteralNode();
				NumNode->_Value.reset(NumLiteral);


				auto BinToken = Token();
				BinToken.OnLine = OtherToken->OnLine;
				BinToken.OnPos = OtherToken->OnPos;
				BinToken.Type = TokenType::Number_literal;
				BinToken.Value._String = "1";


				auto V = std::make_unique<Token>(std::move(BinToken));
				NumLiteral->token = V.get();
				_Tree.TemporaryTokens.push_back(std::move(V));
			}
		}


		//
	}


	{

		ValueExpressionNode* ValuNode = new ValueExpressionNode();
		out._OnNextStatement._ToAssign._Value.reset(ValuNode);


		ReadVariableNode* ReadVarNode = new ReadVariableNode();
		ValuNode->_Value.reset(ReadVarNode);

		ScopedName ScopeName;
		ScopeName._token = NameToken;
		ReadVarNode->_VariableName._ScopedName.push_back(ScopeName);


		auto BinToken = Token();
		BinToken.OnLine = OtherToken->OnLine;
		BinToken.OnPos = OtherToken->OnPos;
		BinToken.Type = PostFixOp;

		auto V = std::make_unique<Token>(std::move(BinToken));
		out._OnNextStatement._PostfixOp = V.get();
		_Tree.TemporaryTokens.push_back(std::move(V));
	}
}
GotNodeType Parser::GetMatchStatement(MatchStatement& out)
{
	auto MatchToken = TryGetToken(); TokenTypeCheck(MatchToken, TokenType::KeyWord_match);
	NextToken();
	out._Token = MatchToken;

	GetExpressionTypeNode(out._Expression);

	TokenTypeCheck(TryGetToken(), TokenType::Colon);
	NextToken();

	TokenTypeCheck(TryGetToken(), TokenType::StartTab);
	NextToken();

	while (TryGetToken()->Type != TokenType::EndTab
		 && TryGetToken()->Type != TokenType::EndofFile)
	{

		auto Token = TryGetToken();
		if (Token->Type == TokenType::KeyWord_invalid)
		{
			NextToken();
			TokenTypeCheck(TryGetToken(), TokenType::Colon);
			NextToken();

			StatementsNode V;
			GetStatementsorStatementNode(V);


			if (out._InvaidCase.has_value())
			{
				_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Cant have more than one invalid cases in match");
			}
			else
			{
				out._InvaidCase = Opt(std::move(V));
			}

		}
		else
		{
			MatchStatementArm V;
			GetExpressionTypeNode(V._Expression);

			TokenTypeCheck(TryGetToken(), TokenType::Colon);
			NextToken();

			GetStatementsorStatementNode(V._Statements);

			out._Arms.push_back(std::move(V));
		}

	}
	TokenTypeCheck(TryGetToken(), TokenType::EndTab);
	NextToken();

	return GotNodeType::Success;
}
GotNodeType Parser::GetMatchExpression(MatchExpression& out)
{
	auto MatchToken = TryGetToken(); TokenTypeCheck(MatchToken, TokenType::KeyWord_match);
	NextToken();

	out._Token = MatchToken;

	GetExpressionTypeNode(out._Expression);

	TokenTypeCheck(TryGetToken(), TokenType::Colon);
	NextToken();

	TokenTypeCheck(TryGetToken(), TokenType::StartTab);
	NextToken();

	while (TryGetToken()->Type != TokenType::EndTab
		&& TryGetToken()->Type != TokenType::EndofFile)
	{

		auto Token = TryGetToken();
		if (Token->Type == TokenType::KeyWord_invalid)
		{
			NextToken();
			TokenTypeCheck(TryGetToken(), TokenType::RightAssignArrow);
			NextToken();

			ExpressionNodeType V;
			GetExpressionTypeNode(V);


			if (out._InvaidCase.has_value())
			{
				_ErrorsOutput->AddError(ErrorCodes::InValidName, Token->OnLine, Token->OnPos, "Cant have more than one invalid cases in match");
			}
			else
			{
				out._InvaidCase = Opt(std::move(V));
			}
		}
		else
		{
			MatchExpressionArm V;
			GetExpressionTypeNode(V._Expression);

			TokenTypeCheck(TryGetToken(), TokenType::RightAssignArrow);
			NextToken();

			GetExpressionTypeNode(V._AssignmentExpression);

			out._Arms.push_back(std::move(V));
		}

		TokenTypeCheck(TryGetToken(), TokenType::Semicolon);
		NextToken();

	}
	TokenTypeCheck(TryGetToken(), TokenType::EndTab);
	NextToken();

	return GotNodeType::Success;
}
GotNodeType Parser::GetUnqExpresionNode(Unique_ptr<Node>& out)
{
	auto Token = TryGetToken(); TokenTypeCheck(Token, TokenType::KeyWord_unq);
	NextToken();

	TypeNode TypeNode;
	auto gottype = GetType(TypeNode, true);

	auto ParToken = TryGetToken();
	if (ParToken->Type == FuncCallStart) {

		NextToken();

		ValueParametersNode Pars;

		GetValueParametersNode(Pars);

		auto Par2Token = TryGetToken();
		TokenTypeCheck(Par2Token, FuncCallEnd);
		NextToken();


		auto OutNode = FuncCallNode::Gen();

		SmartPointerNewToFuncName(TypeNode, Pars, Token, OutNode, UCode_Unique_ptr, UCode_Unique_Ptr_MakeFunction);
		out = std::move(OutNode);
	}
	else if (ParToken->Type == TokenType::Left_Bracket)
	{
		NextToken();

		Unique_ptr<Node> ArrayCountexpression = {};

		GetExpressionTypeNode(ArrayCountexpression);

		auto Par2Token = TryGetToken();
		TokenTypeCheck(Par2Token, TokenType::Right_Bracket);
		NextToken();

		//
		auto OutNode = FuncCallNode::Gen();
		SmartPointerNewArray(TypeNode, Token, OutNode, ArrayCountexpression, UCode_Unique_Array, UCode_Unique_Array_MakeFunction);
		out = std::move(OutNode);
		//
	}
	else
	{
		TokenTypeCheck(ParToken, FuncCallStart);
		return GotNodeType::Error;
	}
	return gottype;
}
void Parser::SmartPointerNewArray(TypeNode& TypeNode, const Token* Token, Unique_ptr<FuncCallNode>& OutNode,Unique_ptr<Node>& ArrayCountexpression, String_view SmartPointerName, String_view SmartPoinerMakeFunc)
{

	ScopedName ClassName;
	UseGenericsNode Generics;
	Generics._Values.push_back(std::move(TypeNode));

	auto NameToken = new UCodeLang::Token();
	NameToken->OnLine = Token->OnLine;
	NameToken->OnPos = Token->OnPos;
	NameToken->Type = TokenType::Name;
	NameToken->Value._String = SmartPointerName;
	_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

	ClassName._token = NameToken;
	ClassName._operator = ScopedName::Operator_t::ScopeResolution;

	ClassName._generic = Shared_ptr<UseGenericsNode>(new UseGenericsNode(std::move(Generics)));
	OutNode->_FuncName._ScopedName.push_back(std::move(ClassName));



	auto FuncNameToken = new UCodeLang::Token();
	FuncNameToken->OnLine = Token->OnLine;
	FuncNameToken->OnPos = Token->OnPos;
	FuncNameToken->Type = TokenType::Name;
	FuncNameToken->Value._String = SmartPoinerMakeFunc;
	_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(FuncNameToken));

	ScopedName FuncName;
	FuncName._token = FuncNameToken;

	OutNode->_FuncName._ScopedName.push_back(std::move(FuncName));


	OutNode->Parameters._Nodes.push_back(std::move(ArrayCountexpression));
}
void Parser::SmartPointerNewToFuncName(TypeNode& TypeNode, ValueParametersNode& Pars, const Token* Token,Unique_ptr<FuncCallNode>& OutNode, String_view SmartPointerName, String_view SmartPoinerMakeFunc)
{
	ScopedName ClassName;
	UseGenericsNode Generics;
	Generics._Values.push_back(std::move(TypeNode));

	auto NameToken = new UCodeLang::Token();
	NameToken->OnLine = Token->OnLine;
	NameToken->OnPos = Token->OnPos;
	NameToken->Type = TokenType::Name;
	NameToken->Value._String = SmartPointerName;
	_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(NameToken));

	ClassName._token = NameToken;
	ClassName._operator = ScopedName::Operator_t::ScopeResolution;

	ClassName._generic = Shared_ptr<UseGenericsNode>(new UseGenericsNode(std::move(Generics)));
	OutNode->_FuncName._ScopedName.push_back(std::move(ClassName));



	auto FuncNameToken = new UCodeLang::Token();
	FuncNameToken->OnLine = Token->OnLine;
	FuncNameToken->OnPos = Token->OnPos;
	FuncNameToken->Type = TokenType::Name;
	FuncNameToken->Value._String = SmartPoinerMakeFunc;
	_Tree.TemporaryTokens.push_back(Unique_ptr<UCodeLang::Token>(FuncNameToken));

	ScopedName FuncName;
	FuncName._token = FuncNameToken;

	OutNode->_FuncName._ScopedName.push_back(std::move(FuncName));

	OutNode->Parameters = std::move(Pars);
}
GotNodeType Parser::GetShrExpresionNode(Unique_ptr<Node>& out)
{
	auto Token = TryGetToken(); TokenTypeCheck(Token, TokenType::KeyWord_shr);
	NextToken();

	TypeNode TypeNode;
	GetType(TypeNode, true);

	auto ParToken = TryGetToken();
	if (ParToken->Type == FuncCallStart) {

		NextToken();

		ValueParametersNode Pars;

		GetValueParametersNode(Pars);

		auto Par2Token = TryGetToken();
		TokenTypeCheck(Par2Token, FuncCallEnd);
		NextToken();


		auto OutNode = FuncCallNode::Gen();

		SmartPointerNewToFuncName(TypeNode, Pars, Token, OutNode, UCode_Shared_ptr, UCode_Shared_Ptr_MakeFunction);
		out = std::move(OutNode);
	}
	else if (ParToken->Type == TokenType::Left_Bracket)
	{
		NextToken();

		Unique_ptr<Node> ArrayCountexpression = nullptr;

		GetExpressionTypeNode(ArrayCountexpression);

		auto Par2Token = TryGetToken();
		TokenTypeCheck(Par2Token, TokenType::Right_Bracket);
		NextToken();


		//
		auto OutNode = FuncCallNode::Gen();
		SmartPointerNewArray(TypeNode, Token, OutNode, ArrayCountexpression, UCode_Shared_Array, UCode_Shared_Array_MakeFunction);
		out = std::move(OutNode);
		//
	}
	else
	{
		TokenTypeCheck(ParToken, FuncCallStart);
	}
	return GotNodeType::Success;
}
GotNodeType Parser::GetImportStatement(ImportStatement& out)
{
	out._Token = TryGetToken(); TokenTypeCheck(out._Token, TokenType::KeyWord_Import);
	NextToken();


	auto StartParToken = TryGetToken();
	if (StartParToken->Type == TokenType::Name)
	{
		ScopedNameNode StartNameSpace;
		{
			ScopedName ScopedName;
			ScopedName._token = StartParToken;
			StartNameSpace._ScopedName.push_back(std::move(ScopedName));


		}
		NextToken();

		while (TryGetToken()->Type == TokenType::ScopeResolution)
		{

			auto Name = TryPeekNextToken(1);
			if (Name->Type != TokenType::Name)
			{
				break;
			}
			StartNameSpace._ScopedName.back()._operator = ScopedName::Operator_t::ScopeResolution;
			ScopedName ScopedName;
			ScopedName._token = Name;
			StartNameSpace._ScopedName.push_back(std::move(ScopedName));

			NextToken();
			NextToken();

		}

		auto ScopeToken = TryGetToken();
		TokenTypeCheck(ScopeToken, TokenType::ScopeResolution);
		NextToken();

		out._StartingNameSpace = Opt(std::move(StartNameSpace));
	}


	auto Par2Token = TryGetToken();
	TokenTypeCheck(Par2Token, TokenType::Left_Brace);
	NextToken();


	while (TryGetToken()->Type != TokenType::EndofFile)
	{
		size_t ValueIndex = _TokenIndex;
		auto ValueName = TryGetToken(); NextToken();


		if (TryGetToken()->Type == TokenType::equal)
		{
			NextToken();


			ImportValue importvalue;
			importvalue._AliasName = Opt<const Token*>(ValueName);
			GetName(importvalue._ImportedSymbol);

			out._Imports.push_back(std::move(importvalue));
		}
		else
		{
			_TokenIndex = ValueIndex;

			ImportValue importvalue;
			GetName(importvalue._ImportedSymbol);

			out._Imports.push_back(std::move(importvalue));
		}


		if (TryGetToken()->Type == TokenType::Comma)
		{
			NextToken();
		}
		else
		{
			break;
		}

	}

	TokenTypeCheck(TryGetToken(), TokenType::Right_Brace);
	NextToken();

	TokenTypeCheck(TryGetToken(), TokenType::Semicolon);
	NextToken();

	if (_PassedImportFileSection)
	{
		_ErrorsOutput->AddError(ErrorCodes::ExpectingSequence, out._Token->OnLine, out._Token->OnPos,
			"This Import Statement is not at the top of the file.");

	}
	return GotNodeType::Success;
}
GotNodeType Parser::GetAwaitExpresionNode(AwaitExpression& out)
{
	out._Token = TryGetToken(); TokenTypeCheck(out._Token, TokenType::KeyWord_await);
	NextToken();

	auto Token = TryGetToken();
	if (Token->Type == TokenType::Left_Bracket)
	{
		NextToken();
		GetShortLambdaNode(out._Lambda);
	}
	else
	{
		out._IsFunc = true;
		GetFuncCallNode(out._Func);
	}


	return GotNodeType::Success;
}
GotNodeType Parser::GetAwaitStatementNode(AwaitStatement& out)
{
	auto o = GetAwaitExpresionNode(out._Base);

	TokenTypeCheck(TryGetToken(), TokenType::Semicolon);
	NextToken();

	return o;
}
GotNodeType Parser::GetYieldExpresionNode(YieldExpression& out)
{
	out._Token = TryGetToken(); TokenTypeCheck(out._Token, TokenType::KeyWord_yield);
	NextToken();

	GetExpressionTypeNode(out._Expression);

	return GotNodeType::Success;
}
GotNodeType Parser::GetYieldStatementNode(YieldStatement& out)
{
	auto o = GetYieldExpresionNode(out._Base);

	TokenTypeCheck(TryGetToken(), TokenType::Semicolon);
	NextToken();

	return o;
}
GotNodeType Parser::GetUnsafeStatementNode(UnsafeStatementsNode& out)
{
	TokenTypeCheck(TryGetToken(), TokenType::KeyWord_unsafe);
	NextToken();

	auto token = TryGetToken();
	if (token->Type == TokenType::Colon)
	{
		NextToken();
		return GetStatements(out._Base);
	}

	Unique_ptr<Node> nodeptr = nullptr;
	auto r = GetStatement(nodeptr);
	out._Base._Nodes.push_back(std::move(nodeptr));
	return r;
}
GotNodeType Parser::GetUnsafeExpression(UnsafeExpression& out)
{
	TokenTypeCheck(TryGetToken(), TokenType::KeyWord_unsafe);
	NextToken();

	return GetExpressionTypeNode(out._Base);
}
GotNodeType Parser::GetDeferStatementNode(DeferStatementNode& out)
{
	TokenTypeCheck(TryGetToken(), TokenType::KeyWord_defer);
	NextToken();

	auto token = TryGetToken();
	if (token->Type == TokenType::Colon)
	{
		NextToken();
		return GetStatements(out._Base);
	}

	Unique_ptr<Node> nodeptr = nullptr;
	auto r = GetStatement(nodeptr);
	out._Base._Nodes.push_back(std::move(nodeptr));
	return r;
}
GotNodeType Parser::GetForTypeNode(ForTypeNode& out)
{
	TokenTypeCheck(TryGetToken(), TokenType::KeyWord_ClassFor);
	NextToken();

	TryGetGeneric(out._generic);

	GetType(out._typetoaddto);

	if (TryGetToken()->Type == TokenType::KeyWord_export)
	{
		out._IsExport = true;
		NextToken();
	}

	TokenTypeCheck(TryGetToken(), TokenType::Colon);
	NextToken();

	auto StartToken = TryGetToken(); TokenTypeCheck(StartToken, TokenType::StartTab);
	NextToken();

	while (TryGetToken()->Type != TokenType::EndTab && TryGetToken()->Type != TokenType::EndofFile)
	{
		auto f =new FuncNode();
		
		out._Nodes.push_back(Unique_ptr<FuncNode>(f));
		
		GetFuncNode(*f);
	}

	auto EndToken = TryGetToken(); TokenTypeCheck(EndToken, TokenType::EndTab);
	NextToken();

	return GotNodeType::Success;
}
GotNodeType Parser::GetRangeExpression(FuncCallNode& out)
{
	auto token = TryGetToken();
	TokenTypeCheck(token, TokenType::KeyWord_RangeOperator);
	NextToken();

	
	auto nexttoken = TryGetToken();

	if (nexttoken->Type == TokenType::Number_literal 
		|| nexttoken->Type == TokenType::Left_Parentheses
		|| nexttoken->Type == TokenType::Name
		|| TypeNode::IsType(token->Type)
		|| nexttoken->Type == TokenType::equal)
	{

		bool iseq = false;
		if (nexttoken->Type == TokenType::equal)
		{
			iseq = true;
			NextToken();
		}

		Unique_ptr<Node> nod = nullptr;
		GetExpressionTypeNode(nod);

		out.Parameters._Nodes.push_back(std::move(nod));
		
		auto funcnametoken = std::make_unique<Token>();

		funcnametoken->OnLine = token->OnLine;
		funcnametoken->OnPos = token->OnPos;
		funcnametoken->Type = TokenType::Name;
		funcnametoken->Value._String = iseq ? UCode_RangeToInclusiveFunction : UCode_RangeToFunction;



		{
			ScopedName scope;
			scope._token = funcnametoken.get();
			out._FuncName._ScopedName.push_back(std::move(scope));
		}
		_Tree.TemporaryTokens.push_back(std::move(funcnametoken));

	}
	else
	{
		_TokenIndex--;
	
		auto funcnametoken = std::make_unique<Token>();

		funcnametoken->OnLine = token->OnLine;
		funcnametoken->OnPos = token->OnPos;
		funcnametoken->Type = TokenType::Name;
		funcnametoken->Value._String = UCode_RangeFullFunction;


		{
			ScopedName scope;
			scope._token = funcnametoken.get();
			out._FuncName._ScopedName.push_back(std::move(scope));
		}
		_Tree.TemporaryTokens.push_back(std::move(funcnametoken));

		NextToken();
	}
	return GotNodeType::Success;
}
UCodeLangFrontEnd

#endif
