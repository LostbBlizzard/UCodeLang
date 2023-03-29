#pragma once
#include "../../LangCore.hpp"
#include "..//..//LangCore/TypeNames.hpp"
#include "../..//LangCore/ScopeHelper.hpp"
#include "../Front/UCodeFrontEndNameSpace.hpp"
UCodeLangFrontStart

struct StringliteralNode :Node
{
	StringliteralNode() :Node(NodeType::StringliteralNode)
	{

	}

		

	AddforNode(StringliteralNode);

	const Token* Token = nullptr;
};
struct BoolliteralNode :Node
{
	BoolliteralNode() :Node(NodeType::BoolliteralNode)
	{

	}
	AddforNode(BoolliteralNode);


	const Token* Token = nullptr;
	inline bool Get_Value() const
	{
		return Token->Type == TokenType::KeyWorld_True;
	}
};
struct NumberliteralNode :Node
{
	NumberliteralNode() :Node(NodeType::NumberliteralNode)
	{

	}
	AddforNode(NumberliteralNode);

	const Token* Token = nullptr;
};

struct FloatliteralNode :Node
{
	FloatliteralNode() :Node(NodeType::FloatliteralNode)
	{

	}
	AddforNode(FloatliteralNode);

	const Token* Token = nullptr;
};

struct CharliteralNode :Node
{
	CharliteralNode() :Node(NodeType::CharliteralNode)
	{

	}
	AddforNode(CharliteralNode);

	const Token* Token = nullptr;
};

struct NameNode :Node
{
	NameNode() : Node(NodeType::NameNode)
	{

	}
	AddforNode(NameNode);

	const Token* Token = nullptr;
	String AsString()const
	{
		return  String(Token->Value._String);
	}
	String_view AsStringView() const
	{
		return Token->Value._String;
	}
};


struct UseGenericsNode;

struct ScopedName
{
	enum class Operator_t :UInt8
	{
		Null,
		ScopeResolution,// (::)
		Dot,// (.)
		IndirectMember,// (~>)
		OptionalChain,// (?.)
		ExclamationMember, // (!.)
	};


	
	const Token* token = nullptr;
	Operator_t Operator = Operator_t::Null;
	Shared_ptr<UseGenericsNode> Generic;//C++ doesn't like circular dependencies and this need to be copy able

	inline void GetScopedName(String& out) const
	{
		Token::PushString(out, *token);
	}
	static bool Get_IsScoped(TokenType Type)
	{
		return Get_Scoped(Type) != Operator_t::Null;
	}

	static Operator_t GetOverloadable_Scoped(TokenType Type)
	{
		switch (Type)
		{
		case TokenType::IndirectMember:return Operator_t::IndirectMember;
		case TokenType::OptionalDot:return Operator_t::OptionalChain;
		case TokenType::ExclamationDot:return Operator_t::ExclamationMember;
		default:return Operator_t::Null;
		}
	}

	static Operator_t Get_Scoped(TokenType Type)
	{
		switch (Type)
		{
		case TokenType::Dot:return Operator_t::Dot;
		case TokenType::ScopeResolution:return Operator_t::ScopeResolution;
		default:return GetOverloadable_Scoped(Type);
		}
	}
	
	UseGenericsNode& Get_Generic()
	{
		return *Generic.get();
	}
};

struct ScopedNameNode :Node
{
	ScopedNameNode() : Node(NodeType::ScopedNameNode)
	{

	}
	AddforNode(ScopedNameNode);

	Vector<ScopedName> ScopedName;

	
	inline void GetScopedName(String& out,size_t Start =0 ) const
	{
		for (size_t i = Start; i < ScopedName.size(); i++)
		{
			auto& item = ScopedName[i];
			item.GetScopedName(out);
			if (&item != &ScopedName.back())
			{
				switch (item.Operator)
				{
				case ScopedName::Operator_t::ScopeResolution:
					out += ScopeHelper::_ScopeSep;
					break;
				case ScopedName::Operator_t::Dot:
					out += ".";
					break;
				case ScopedName::Operator_t::IndirectMember:
					out += "->";
					break;
				default:
					throw std::exception();
					break;
				}
			}

		}
	}
};

struct ReadVariableNode :Node
{
	ReadVariableNode() : Node(NodeType::ReadVariableNode)
	{

	}
	AddforNode(ReadVariableNode);

	ScopedNameNode VariableName;
};
struct NamespaceNode :Node
{
	NamespaceNode() : Node(NodeType::NamespaceNode)
	{

	}
	NamespaceNode(NamespaceNode&& Source) = default;
	AddforNodeAndWithList(NamespaceNode);

	ScopedNameNode NamespaceName;
};
struct GenericValueNode :Node
{
	GenericValueNode() : Node(NodeType::Null)
	{

	}
	const Token* Token = nullptr;
	bool IsConstantExpression = false;

	String_view AsStringView() const
	{
		return Token->Value._String;
	}
	String AsString() const
	{
		return (String)AsStringView();
	}
};
struct GenericValuesNode :Node
{
	GenericValuesNode() : Node(NodeType::Null)
	{

	}
	Vector<GenericValueNode> Values;

	UCodeLangForceinline bool HasGeneric()
	{
		return Values.size();
	}
};


struct ClassNode :Node
{
	ClassNode() : Node(NodeType::ClassNode)
	{

	}
	ClassNode(ClassNode&& Source) = default;
	AddforNodeAndWithList(ClassNode);

	NameNode ClassName;
	GenericValuesNode Generic;
};


struct UsingNode :Node
{

	UsingNode() : Node(NodeType::UsingNode)
	{

	}
	AddforNode(UsingNode);

	ScopedNameNode ScopedName;
};

struct TypeNode;

struct UseGenericsNode :Node
{
	UseGenericsNode() : Node(NodeType::Null)
	{

	}
	Vector<TypeNode> Values;

	UCodeLangForceinline bool HasGeneric()
	{
		return Values.size();
	}
};

struct TypeNode :Node
{
	TypeNode() : Node(NodeType::TagTypeNode)
	{

	}
	NameNode Name;
	UseGenericsNode Generic;
	Unique_ptr<Node> node = nullptr;

	static constexpr bool IsType(TokenType Type)
	{
		if (IsPrimitive(Type)) { return true; }
		else if (Type == TokenType::KeyWorld_var) { return true; }
		else if (Type == TokenType::Name) { return true; }
		else if (Type == TokenType::KeyWord_This) { return true; }

		return false;
	}

	static constexpr bool IsPrimitive(TokenType Type)
	{
		switch (Type)
		{
		case TokenType::KeyWord_Bool:
		case TokenType::KeyWord_Char:
		case TokenType::KeyWord_UInt8:
		case TokenType::KeyWord_UInt16:
		case TokenType::KeyWord_UInt32:
		case TokenType::KeyWord_UInt64:
		case TokenType::KeyWord_SInt8:
		case TokenType::KeyWord_SInt16:
		case TokenType::KeyWord_SInt32:
		case TokenType::KeyWord_SInt64:
		case TokenType::KeyWord_uintptr:
		case TokenType::KeyWord_sintptr:
		case TokenType::KeyWord_float32:
		case TokenType::KeyWord_float64:
		return true;
		default:return false;
		}

	}


	static void Gen_Type(TypeNode& Out, TokenType Type, const Token& ToGetLinesFrom)
	{
		auto T =std::make_unique<Token>();
		T->Type = Type;
		T->OnLine = ToGetLinesFrom.OnLine;
		T->OnPos = ToGetLinesFrom.OnPos;
		Out.GenToken = std::move(T);
		Out.Name.Token =Out.GenToken.get();
	}
	static void Gen_void(TypeNode& Out, const Token& ToGetLinesFrom)
	{
		return Gen_Type(Out, TokenType::Void, ToGetLinesFrom);
	}
	static void Gen_Var(TypeNode& Out, const Token& ToGetLinesFrom)
	{
		return Gen_Type(Out, TokenType::KeyWorld_var, ToGetLinesFrom);
	}
	static void Gen_ThisMemberFunc(TypeNode& Out, const Token& ToGetLinesFrom)
	{
		Out.SetAsAddess();
		return Gen_Type(Out, TokenType::KeyWord_This, ToGetLinesFrom);
	}
	static void Gen_Byte(TypeNode& Out, const Token& ToGetLinesFrom)
	{
		return Gen_Type(Out, TokenType::KeyWord_UInt8, ToGetLinesFrom);
	}
	static void Gen_Expression(TypeNode& Out, const Token& ToGetLinesFrom)
	{
		return Gen_Type(Out, TokenType::internal_Constant_expression, ToGetLinesFrom);
	}

	bool IsThisMemberFunc() const
	{
		return Name.Token->Type == TokenType::KeyWord_This
			&& IsAddess;
	}
	String AsString() const
	{
		auto T = Name.Token->Type;
		if (IsPrimitive(T))
		{
			return StringHelper::ToString(T);
		}
		return String(Name.Token->Value._String);
	}
	TypeNode(const TypeNode& ToCopyFrom) = default;
	TypeNode(TypeNode&& source) = default;
	TypeNode& operator=(TypeNode&& source) = default;
	~TypeNode() = default;
	void SetAsAddess()
	{
		IsAddess = true;
	}
	void SetAsArrayAddess()
	{
		IsAddessArray = true;
	}
	void SetAsimmutable()
	{
		Isimmutable = true;
	}
	void SetAsStaticArray()
	{
		IsStackArray = true;
	}

	bool IsAddess = false;
	bool IsAddessArray = false;
	bool Isimmutable = false;
	bool IsStackArray = false;
private:
	Unique_ptr<Token> GenToken;
};

struct NamedParameterNode :Node
{
	NamedParameterNode() : Node(NodeType::Null)
	{

	}
	TypeNode Type;
	NameNode Name;
};
struct NamedParametersNode :Node
{
	NamedParametersNode() : Node(NodeType::NamedParametersNode)
	{

	}
	AddforNode(NamedParametersNode);
	Vector<NamedParameterNode> Parameters;
};
struct ValueParametersNode :Node
{
	ValueParametersNode() : Node(NodeType::ValueParametersNode)
	{

	}
	ValueParametersNode(ValueParametersNode&& Source) = default;
	AddforNodeAndWithList(ValueParametersNode);
};
struct AttributeNode :Node
{
	AttributeNode() : Node(NodeType::AttributeNode)
	{

	}
	AddforNode(AttributeNode);

	ScopedNameNode ScopedName;
	ValueParametersNode Parameters;
};
struct StatementsNode :Node
{
	StatementsNode() : Node(NodeType::StatementsNode)
	{

	}
	AddforNodeAndWithList(StatementsNode);
	StatementsNode(StatementsNode&& source) = default;
	StatementsNode& operator=(StatementsNode&& source) = default;
};
struct FuncSignatureNode :Node
{
	NameNode Name;
	GenericValuesNode Generic;
	NamedParametersNode Parameters;
	TypeNode ReturnType;
};
struct FuncBodyNode :Node
{
	StatementsNode Statements;
};
struct FuncNode :Node
{
	FuncNode() : Node(NodeType::FuncNode)
	{

	}
	~FuncNode()
	{

	}

		
	AddforNode(FuncNode);
	FuncSignatureNode Signature;
	Optional<FuncBodyNode> Body;
};

struct AsmBlockNode :Node
{
	AsmBlockNode() : Node(NodeType::AsmBlockNode)
	{

	}
	AddforNode(AsmBlockNode);
	String_view AsmText;
};

struct ExpressionNodeType :Node
{
	ExpressionNodeType() : Node(NodeType::ExpressionNodeType)
	{

	}
	AddforNode(ExpressionNodeType);
	Unique_ptr<Node> Value = nullptr;
	ExpressionNodeType(const ExpressionNodeType& ToCopyFrom) = default;
	ExpressionNodeType(ExpressionNodeType&& source) = default;
	~ExpressionNodeType() = default;

	inline static bool IsPostfixOperator(const Token* Token)
	{
		return Token->Type == TokenType::increment
			|| Token->Type == TokenType::decrement;
	}
	inline static bool IsCompoundOperator(const Token* Token)
	{
		return Token->Type == TokenType::CompoundAdd
			|| Token->Type == TokenType::CompoundSub
			|| Token->Type == TokenType::CompoundMult
			|| Token->Type == TokenType::CompoundDiv;
	}
	inline static bool IsUnaryOperator(const Token* Token)
	{
		return Token->Type == TokenType::plus
			|| Token->Type == TokenType::minus
			|| Token->Type == TokenType::KeyWorld_Sizeof
			|| Token->Type == TokenType::KeyWorld_Nameof
			|| Token->Type == TokenType::KeyWorld_typeof
			|| Token->Type == TokenType::Not
			|| Token->Type == TokenType::bitwise_not
			|| Token->Type == TokenType::QuestionMark;
	}
	inline static bool IsOverLoadableOperator(const Token* Token)
	{
		return  IsBinaryOperator(Token)
			|| IsPostfixOperator(Token)
			|| IsUnaryOperator(Token)
			|| IsCompoundOperator(Token)
			|| IsMemberAccessOperator(Token);
	}
	inline static bool IsMemberAccessOperator(const Token* Token)
	{
		return ScopedName::Get_Scoped(Token->Type) != ScopedName::Operator_t::Null;
	}

	inline static bool IsBinaryOperator(const Token* Token)
	{
		return Token->Type == TokenType::plus
			|| Token->Type == TokenType::minus
			|| Token->Type == TokenType::star
			|| Token->Type == TokenType::forwardslash
			|| Token->Type == TokenType::modulo

			|| Token->Type == TokenType::equal_Comparison
			|| Token->Type == TokenType::Notequal_Comparison
			|| Token->Type == TokenType::greaterthan
			|| Token->Type == TokenType::lessthan
			|| Token->Type == TokenType::greater_than_or_equalto
			|| Token->Type == TokenType::less_than_or_equalto

			|| Token->Type == TokenType::logical_and
			|| Token->Type == TokenType::logical_or

			|| Token->Type == TokenType::bitwise_and
			|| Token->Type == TokenType::bitwise_or
			|| Token->Type == TokenType::bitwise_LeftShift
			|| Token->Type == TokenType::bitwise_RightShift
			|| Token->Type == TokenType::bitwise_XOr

			|| Token->Type == TokenType::approximate_Comparison;

	}
};

struct ValueExpressionNode :Node
{
	ValueExpressionNode() : Node(NodeType::ValueExpressionNode)
	{

	}
	AddforNode(ValueExpressionNode);
	Unique_ptr<Node> Value = nullptr;
	ValueExpressionNode(const ValueExpressionNode& ToCopyFrom) = default;
	ValueExpressionNode(ValueExpressionNode&& source) = default;
	
	~ValueExpressionNode() = default;
};
struct BinaryExpressionNode :Node
{
	BinaryExpressionNode() : Node(NodeType::BinaryExpressionNode)
	{

	}
	AddforNode(BinaryExpressionNode);

	ExpressionNodeType Value0;
	const Token* BinaryOp =nullptr;
	ExpressionNodeType Value1;
}; 

struct RetStatementNode :Node
{
	RetStatementNode() : Node(NodeType::RetStatementNode)
	{

	}
	AddforNode(RetStatementNode);
	ExpressionNodeType Expression;
};

struct DeclareVariableNode :Node
{
	DeclareVariableNode() : Node(NodeType::DeclareVariableNode)
	{

	}
	AddforNode(DeclareVariableNode);
	TypeNode Type;
	NameNode Name;
	ExpressionNodeType Expression;
	DeclareVariableNode(DeclareVariableNode&& source) = default;
	DeclareVariableNode& operator=(DeclareVariableNode&& source) = default;
};

struct AssignVariableNode :Node
{
	AssignVariableNode() : Node(NodeType::AssignVariableNode)
	{

	}
	AddforNode(AssignVariableNode);
	ScopedNameNode Name;
	ExpressionNodeType Expression;
};

struct DeclareStaticVariableNode :Node
{
	DeclareStaticVariableNode() : Node(NodeType::DeclareStaticVariableNode)
	{

	}
	AddforNode(DeclareStaticVariableNode);
	DeclareVariableNode Variable;
};
struct DeclareThreadVariableNode :Node
{
	DeclareThreadVariableNode() : Node(NodeType::DeclareThreadVariableNode)
	{

	}
	AddforNode(DeclareThreadVariableNode);
	DeclareVariableNode Variable;
};

enum class AliasType
{
	Type,
	Func,
};

struct AliasNode :Node
{
	AliasNode() : Node(NodeType::AliasNode)
	{

	}
	AddforNode(AliasNode);

	NameNode AliasName;
	GenericValuesNode Generic;
	TypeNode Type;

	
	bool IsHardAlias = false;
	
	AliasType _Type = AliasType::Type;
	Unique_ptr<Node> _Node;
};


struct EnumValueNode :Node
{
	
	NameNode Name;
	ExpressionNodeType Expression;
};
struct EnumNode :Node
{
	EnumNode() : Node(NodeType::EnumNode)
	{

	}
	AddforNode(EnumNode);

	NameNode EnumName;
	Vector<EnumValueNode> Values;
	TypeNode BaseType;
};

struct TagTypeNode :Node
{
	TagTypeNode() : Node(NodeType::TagTypeNode)
	{

	}
	AddforNodeAndWithList(TagTypeNode);

	NameNode AttributeName;

	TagTypeNode(TagTypeNode&& source) = default;
	TagTypeNode& operator=(TagTypeNode&& source) = default;
};


struct IfNode :Node
{
	IfNode() : Node(NodeType::IfNode)
	{

	}
	AddforNode(IfNode);

	ExpressionNodeType Expression;
	StatementsNode Body;
	Unique_ptr<Node> Else;
};
struct ElseNode :Node
{
	ElseNode() : Node(NodeType::ElseNode)
	{

	}
	AddforNode(ElseNode);

	
	StatementsNode Body;
};
struct WhileNode :Node
{
	WhileNode() : Node(NodeType::WhileNode)
	{

	}
	AddforNode(WhileNode);

	ExpressionNodeType Expression;
	StatementsNode Body;
};

struct DoNode :Node
{
	DoNode() : Node(NodeType::DoNode)
	{

	}
	AddforNode(DoNode);

	
	StatementsNode Body;
	ExpressionNodeType Expression;
};

struct PostfixVariableNode :Node
{
	PostfixVariableNode() : Node(NodeType::PostfixVariableNode)
	{

	}
	AddforNode(PostfixVariableNode);
	ScopedNameNode Name;
	const Token* PostfixOp = nullptr;
};

struct CompoundStatementNode :Node
{
	CompoundStatementNode() : Node(NodeType::CompoundStatementNode)
	{

	}
	AddforNode(CompoundStatementNode);
	ScopedNameNode VariableName;
	const Token* CompoundOp = nullptr;
	ExpressionNodeType Expession;
};

struct FuncCallNode :Node
{
	FuncCallNode() : Node(NodeType::FuncCallNode)
	{

	}
	AddforNode(FuncCallNode);
	ScopedNameNode FuncName;
	UseGenericsNode Generics;
	ValueParametersNode Parameters;
};
struct FuncCallStatementNode :Node
{
	FuncCallStatementNode() : Node(NodeType::FuncCallStatementNode)
	{

	}
	AddforNode(FuncCallStatementNode);
	FuncCallNode Base;
};

struct AnonymousTypeNode :Node
{
	AnonymousTypeNode() : Node(NodeType::AnonymousTypeNode)
	{

	}
	AddforNode(AnonymousTypeNode);
	NamedParametersNode Fields;
};

struct AnonymousObjectConstructorNode :Node
{
	AnonymousObjectConstructorNode() : Node(NodeType::AnonymousObjectConstructorNode)
	{

	}
	AddforNode(AnonymousObjectConstructorNode);
	ValueParametersNode Fields;
};
struct CastNode :Node
{
	CastNode() : Node(NodeType::CastNode)
	{

	}
	AddforNode(CastNode);

	ExpressionNodeType Expression;
	TypeNode ToType;
};
struct ParenthesesExpresionNode :Node
{
	ParenthesesExpresionNode() : Node(NodeType::ParenthesesExpresionNode)
	{

	}
	AddforNode(ParenthesesExpresionNode);

	ExpressionNodeType Expression;
};
struct SizeofExpresionNode :Node
{
	SizeofExpresionNode() : Node(NodeType::SizeofExpresionNode)
	{

	}
	AddforNode(SizeofExpresionNode);

	TypeNode Type;
};

struct NewExpresionNode :Node
{
	NewExpresionNode() : Node(NodeType::NewExpresionNode)
	{

	}
	AddforNode(NewExpresionNode);

	TypeNode Type;
	ValueParametersNode Parameters;

	ExpressionNodeType Arrayexpression;//can be null
};

struct DropStatementNode :Node
{
	DropStatementNode() : Node(NodeType::DropStatementNode)
	{

	}
	AddforNode(DropStatementNode);

	ExpressionNodeType expression;
};


struct AliasNode_Func :Node
{
	NamedParametersNode Parameters;
	TypeNode ReturnType;
};


struct IndexedExpresionNode :Node
{
	IndexedExpresionNode() : Node(NodeType::IndexedExpresionNode)
	{

	}
	AddforNode(IndexedExpresionNode);

	ExpressionNodeType SourceExpression;// ex[SomeEx]
	ExpressionNodeType IndexExpression;// SomeVar[ex]
};

struct ForNode :Node
{
	//for [int a = 0;a < 10;a++;]
	enum class ForType
	{
		Traditional,//for [int a = 0;a < 10;a++];
		modern,//for [var& Item : List];
	};

	ForNode() : Node(NodeType::ForNode)
	{

	}
	AddforNode(ForNode);


	ForType Type = ForType::Traditional;

	//Traditional
	ExpressionNodeType Traditional_Assignment_Expression;
	ExpressionNodeType BoolExpression;
	PostfixVariableNode OnNextStatement;
	//Modern
	ExpressionNodeType Modern_List;
	//Both
	TypeNode TypeNode;
	const Token* Name = nullptr;
	StatementsNode Body;
};


struct ContinueNode :Node
{

	ContinueNode() : Node(NodeType::ContinueNode)
	{

	}
	AddforNode(ContinueNode);
	
	const Token* token = nullptr;
};

struct BreakNode :Node
{

	BreakNode() : Node(NodeType::BreakNode)
	{

	}
	AddforNode(BreakNode);

	const Token* token = nullptr;
};

UCodeLangFrontEnd