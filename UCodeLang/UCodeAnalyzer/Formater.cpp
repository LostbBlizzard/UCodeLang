#include "Formater.hpp"

UCodeAnalyzerStart

Formater::Formater()
{

}
Formater::~Formater()
{

}
void Formater::Reset()
{
	this->~Formater();
	new (this) Formater();
}

constexpr const char* ToName(UCodeLang::TokenType Type)
{
	return UCodeLang::TokenStringHelper::ToString(Type);
}

Optional<String> Formater::Format(StrScope scope, StringView Text)
{
	_Output.clear();
	String Tep;

	Tep += Text;
	switch (scope)
	{
	case StrScope::FileScope:

		break;
	case StrScope::NameSpace:
		Tep += Text;
		break;
	case StrScope::ClassSpace:
		break;
	case StrScope::FuncSpace:
		break;
	case StrScope::StatementsSpace:
		break;
	default:
		break;
	}

	UCodeLang::CompliationErrors _Err;
	UCodeLang::CompliationSettings _Set;


	UCodeLang::FrontEnd::Lexer _Lexer;
	UCodeLang::FrontEnd::Parser _Parser;

	_Lexer.Set_ErrorsOutput(&_Err);
	_Lexer.Set_Settings(&_Set);
	_Lexer.Lex(Tep);
	if (_Err.Has_Errors())
	{
		return {};
	}

	_Parser.Set_ErrorsOutput(&_Err);
	_Parser.Set_Settings(&_Set);
	_Parser.Parse(_Lexer.Get_Tokens());
	if (_Err.Has_Errors())
	{
		return {};
	}

	_InputString = Tep;
	ReFormat(_Parser.Get_Tree());

	return _Output;
}

void Formater::ReFormat(const UCodeLang::FileNode& Node)
{
	for (auto& Item : Node._Nodes)
	{
		switch (Item->Get_Type())
		{
		case UCodeLang::NodeType::ImportStatement:ReFormat(*UF::ImportStatement::As(Item.get())); break;
		case UCodeLang::NodeType::ClassNode:ReFormat(*UF::ClassNode::As(Item.get())); break;
		case UCodeLang::NodeType::DeclareVariableNode:ReFormat(*UF::DeclareVariableNode::As(Item.get())); break;
		case UCodeLang::NodeType::DeclareEvalVariableNode:ReFormat(*UF::DeclareEvalVariableNode::As(Item.get())); break;
		case UCodeLang::NodeType::DeclareStaticVariableNode:ReFormat(*UF::DeclareStaticVariableNode::As(Item.get())); break;
		case UCodeLang::NodeType::DeclareThreadVariableNode:ReFormat(*UF::DeclareThreadVariableNode::As(Item.get())); break;
		default:
			CantReFormat();
			break;
		}
	}
}
void Formater::ReFormat(const UF::ImportStatement& Node)
{
	//Import Hello::{Type = T,Type};
	//Import {Type = T::T,Type};

	AddToOutput(ToName(UCodeLang::TokenType::KeyWord_Import));
	AddToOutput(" ");

	if (Node._StartingNameSpace.has_value())
	{
		auto& NameSpace = Node._StartingNameSpace.value();
		ReFormat(NameSpace);

		AddToOutput(ToName(UCodeLang::TokenType::ScopeResolution));
	}

	AddToOutput(ToName(UCodeLang::TokenType::Left_Brace));

	for (auto& Item : Node._Imports)
	{
		if (Item._AliasName.has_value())
		{
			AddToOutput(Item._AliasName.value()->Value._String);
			AddToOutput(" ");
			AddToOutput(ToName(UCodeLang::TokenType::equal));
			AddToOutput(" ");
		}
		ReFormat(Item._ImportedSybol);
	}

	AddToOutput(ToName(UCodeLang::TokenType::Right_Brace));

	AddToOutput(ToName(UCodeLang::TokenType::Semicolon));

	AddNextLine();
	AddIndentsFromCount();
}
void Formater::ReFormat(const UF::ClassNode& Node)
{
	//$ClassName<T>[A,B,C]:
	ReFormat(Node._Access);

	AddToOutput("$");
	AddToOutput(Node._className.token->Value._String);

	ReFormat(Node._generic);
	ReFormat(Node._Inherited);

	AddToOutput(ToName(UCodeLang::TokenType::Colon));
	
	AddIndentCount();
	{
		NextLineAndIndentsFromCount();

		SetAccessIndent();

		for (auto& Item : Node._Nodes)
		{
			switch (Item->Get_Type())
			{
			case UCodeLang::NodeType::ClassNode:ReFormat(*UF::ClassNode::As(Item.get())); break;
			case UCodeLang::NodeType::DeclareVariableNode:ReFormat(*UF::DeclareVariableNode::As(Item.get())); break;
			case UCodeLang::NodeType::DeclareEvalVariableNode:ReFormat(*UF::DeclareEvalVariableNode::As(Item.get())); break;
			case UCodeLang::NodeType::DeclareStaticVariableNode:ReFormat(*UF::DeclareStaticVariableNode::As(Item.get())); break;
			case UCodeLang::NodeType::DeclareThreadVariableNode:ReFormat(*UF::DeclareThreadVariableNode::As(Item.get())); break;
			default:
				CantReFormat();
				break;
			}
		}

		RemoveAccessIndent();
	}
	RemoveIndentCount();
}

void Formater::ReFormat(const UF::ScopedNameNode& Node)
{
}

void Formater::ReFormat(const UF::GenericValuesNode& Node)
{
}

void Formater::ReFormat(const UF::GenericValueNode& Node)
{
}

void Formater::ReFormat(const UF::InheritedTypeData& Node)
{
}

void Formater::ReFormat(const UF::DeclareVariableNode& Node)
{
	ReFormat(Node.Access);
	ReFormat(Node._Type);

	AddToOutput(" ");
	AddToOutput(Node.Name.token->Value._String);

	if (Node.Expression._Value.get()) 
	{
		AddToOutput(" ");
		AddToOutput(ToName(UCodeLang::TokenType::equal));
		AddToOutput(" ");

		ReFormat(Node.Expression);

		AddToOutput(ToName(UCodeLang::TokenType::Semicolon));
	}
	else
	{
		AddToOutput(ToName(UCodeLang::TokenType::Semicolon));
	}
	NextLineAndIndentsFromCount();

}

void Formater::ReFormat(const UF::DeclareStaticVariableNode& Node)
{
	AddToOutput(ToName(UCodeLang::TokenType::KeyWord_static));
	ReFormat(Node.Variable);
}

void Formater::ReFormat(const UF::DeclareThreadVariableNode& Node)
{
	AddToOutput(ToName(UCodeLang::TokenType::KeyWord_Thread));
	ReFormat(Node.Variable);
}

void Formater::ReFormat(const UF::DeclareEvalVariableNode& Node)
{
	AddToOutput(ToName(UCodeLang::TokenType::KeyWord_eval));
	ReFormat(Node.Variable);
}

void Formater::ReFormat(const UF::TypeNode& Node)
{
	if (Node._name.token->Type == UCodeLang::TokenType::Name) 
	{
		AddToOutput(Node._name.token->Value._String);
	}
	else
	{
		if (_InputString[Node._name.token->OnPos - 3] == 'i')
		{
			AddToOutput("int");
		}
		else
		{
			AddToOutput(ToName(Node._name.token->Type));
		}
	}
	ReFormat(Node._generic);
}

void Formater::ReFormat(const UF::UseGenericsNode& Node)
{
	for (auto& Item : Node._Values)
	{
		
		if (Item._name.token->Type == UCodeLang::TokenType::internal_Constant_expression)
		{
			AddToOutput(ToName(UCodeLang::TokenType::Left_Parentheses));


			AddToOutput(ToName(UCodeLang::TokenType::Right_Parentheses));
		}
		else
		{
			ReFormat(Item);

			if (&Item != &Node._Values.back())
			{
				AddToOutput(ToName(UCodeLang::TokenType::Comma));
			}
		}

	}
}

void Formater::ReFormat(const UF::ExpressionNodeType& Node)
{
	switch (Node._Value->Get_Type())
	{
	case UCodeLang::NodeType::ValueExpressionNode:ReFormat(*UF::ValueExpressionNode::As(Node._Value.get()));break;
	default:
		CantReFormat();
		break;
	}
}

void Formater::ReFormat(const UF::ValueExpressionNode& Node)
{
	switch (Node._Value->Get_Type())
	{
	case UCodeLang::NodeType::NumberliteralNode: 
		ReFormat(*UF::NumberliteralNode::As(Node._Value.get()));
		break;
	default:
		CantReFormat();
		break;
	}
}

void Formater::ReFormat(const UF::NumberliteralNode& Node)
{
	AddToOutput(Node.token->Value._String);
}

void Formater::ReFormat(UCodeLang::AccessModifierType Type)
{
	if (Get_CurintAccess() != Type)
	{	
		if (GetAccessIndent() != IndentCount)
		{
			RemoveIndent();
		}
		
		if (Type == UCodeLang::AccessModifierType::Public)
		{
			AddToOutput(ToName(UCodeLang::TokenType::KeyWorld_public));
		}
		else
		{
			AddToOutput(ToName(UCodeLang::TokenType::KeyWorld_private));
		}
		AddToOutput(ToName(UCodeLang::TokenType::Colon));


		if (GetAccessIndent() != IndentCount)
		{
		

			NextLineAndIndentsFromCount();
		}
		else
		{
			AddIndentCount();
			NextLineAndIndentsFromCount();
		}
		
		if (_CurintAccess.size()) 
		{
			_CurintAccess.top() = Type;
		}

	}
}
void ReFormat(const UF::TypeNode& Node)
{

}
UCodeAnalyzerEnd