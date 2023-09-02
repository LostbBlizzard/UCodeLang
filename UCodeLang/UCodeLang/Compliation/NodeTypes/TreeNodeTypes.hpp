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

	const Token* token = nullptr;
};
struct BoolliteralNode :Node
{
	BoolliteralNode() :Node(NodeType::BoolliteralNode)
	{

	}
	AddforNode(BoolliteralNode);


	const Token* token = nullptr;
	inline bool Get_Value() const
	{
		return token->Type == TokenType::KeyWorld_True;
	}
};
struct NumberliteralNode :Node
{
	NumberliteralNode() :Node(NodeType::NumberliteralNode)
	{

	}
	AddforNode(NumberliteralNode);

	const Token* token = nullptr;
};

struct FloatliteralNode :Node
{
	FloatliteralNode() :Node(NodeType::FloatliteralNode)
	{

	}
	AddforNode(FloatliteralNode);

	const Token* token = nullptr;
};

struct CharliteralNode :Node
{
	CharliteralNode() :Node(NodeType::CharliteralNode)
	{

	}
	AddforNode(CharliteralNode);

	const Token* token = nullptr;
};

struct NameNode :Node
{
	NameNode() : Node(NodeType::NameNode)
	{

	}
	AddforNode(NameNode);

	const Token* token = nullptr;
	String AsString()const
	{
		return  String(token->Value._String);
	}
	String_view AsStringView() const
	{
		return token->Value._String;
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


	
	const Token* _token = nullptr;
	Operator_t _operator = Operator_t::Null;
	Shared_ptr<UseGenericsNode> _generic;//C++ doesn't like circular dependencies and this need to be copy able

	inline void GetScopedName(String& out) const
	{
		Token::PushString(out, *_token);
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
		return *_generic.get();
	}
};

struct ScopedNameNode :Node
{
	ScopedNameNode() : Node(NodeType::ScopedNameNode)
	{

	}
	AddforNode(ScopedNameNode);

	Vector<ScopedName> _ScopedName;

	
	inline void GetScopedName(String& out,size_t Start =0 ) const
	{
		for (size_t i = Start; i < _ScopedName.size(); i++)
		{
			auto& item = _ScopedName[i];
			item.GetScopedName(out);
			if (&item != &_ScopedName.back())
			{
				switch (item._operator)
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
				UCodeLangUnreachable();
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

	ScopedNameNode _VariableName;
};
struct NamespaceNode :Node
{
	NamespaceNode() : Node(NodeType::NamespaceNode)
	{

	}
	NamespaceNode(NamespaceNode&& Source) = default;
	AddforNodeAndWithList(NamespaceNode);

	ScopedNameNode _NamespaceName;
};



struct GenericValueNode :Node
{

	enum class GenericType
	{
		Name,
		ConstantExpression,
		Pack,
	};

	GenericValueNode() : Node(NodeType::Null)
	{

	}
	const Token* token = nullptr;
	GenericType _Generictype = GenericType::Name;


	String_view AsStringView() const
	{
		return token->Value._String;
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
	Vector<GenericValueNode> _Values;

	UCodeLangForceinline bool HasGeneric()
	{
		return _Values.size();
	}
};

struct TypeNode;

struct UseGenericsNode :Node
{
	UseGenericsNode() : Node(NodeType::Null)
	{

	}
	Vector<TypeNode> _Values;

	UCodeLangForceinline bool HasGeneric()
	{
		return _Values.size();
	}
};

struct InheritedTypeValue
{
	NameNode _Name;
	UseGenericsNode _generic;
};
struct InheritedTypeData
{
	Vector<InheritedTypeValue> _values;
};
struct AttributeNode;

struct ClassNode :Node
{
	ClassNode() : Node(NodeType::ClassNode)
	{

	}
	ClassNode(ClassNode&& Source) = default;
	AddforNodeAndWithList(ClassNode);

	NameNode _className;
	GenericValuesNode _generic;
	InheritedTypeData _Inherited;
	AccessModifierType _Access = AccessModifierType::Default;
	Vector<Unique_ptr<AttributeNode>> _Attributes;
};


struct UsingNode :Node
{

	UsingNode() : Node(NodeType::UsingNode)
	{

	}
	AddforNode(UsingNode);

	ScopedNameNode _ScopedName;
};


struct TypeNode :Node
{
	TypeNode() : Node(NodeType::TagTypeNode)
	{

	}
	NameNode _name;
	UseGenericsNode _generic;
	Unique_ptr<Node> _node = nullptr;

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
		case TokenType::KeyWord_TypeInfo:
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
		Out._GenToken = std::move(T);
		Out._name.token =Out._GenToken.get();
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
		return _name.token->Type == TokenType::KeyWord_This
			&& _IsAddess;
	}
	String AsString() const
	{
		auto T = _name.token->Type;
		if (IsPrimitive(T))
		{
			return TokenStringHelper::ToString(T);
		}
		return String(_name.token->Value._String);
	}
	TypeNode(const TypeNode& ToCopyFrom) = default;
	TypeNode(TypeNode&& source) = default;
	TypeNode& operator=(TypeNode&& source) = default;
	~TypeNode() = default;
	void SetAsAddess()
	{
		_IsAddess = true;
	}
	void SetAsArrayAddess()
	{
		_IsAddessArray = true;
	}
	void SetAsimmutable()
	{
		_Isimmutable = true;
	}
	void SetAsStaticArray()
	{
		_IsStackArray = true;
	}
	void SetMovedType()
	{
		_IsTypedMoved = true;
	}

	void SetDynamic()
	{
		_IsDynamic = true;
	}

	void SetAsBinding()
	{
		_IsTypeBinding = true;
	}

	bool _IsAddess = false;
	bool _IsAddessArray = false;
	bool _Isimmutable = false;
	bool _IsStackArray = false;
	bool _IsTypedMoved = false;
	bool _IsDynamic = false;
	bool _IsTypeBinding = false;
private:
	Unique_ptr<Token> _GenToken;
};

struct NamedParameterNode :Node
{
	NamedParameterNode() : Node(NodeType::Null)
	{

	}
	TypeNode _Type;
	NameNode _Name;
	bool _IsOutVarable = false;
};
struct NamedParametersNode :Node
{
	NamedParametersNode() : Node(NodeType::NamedParametersNode)
	{

	}
	AddforNode(NamedParametersNode);
	Vector<NamedParameterNode> _Parameters;
};
struct ValueParametersNode :Node
{
	ValueParametersNode() : Node(NodeType::ValueParametersNode)
	{

	}
	ValueParametersNode(ValueParametersNode&& Source) = default;
	ValueParametersNode& operator=(ValueParametersNode&& source) = default;
	AddforNodeAndWithList(ValueParametersNode);
};
struct AttributeNode :Node
{
	AttributeNode() : Node(NodeType::AttributeNode)
	{

	}
	AddforNode(AttributeNode);

	ScopedNameNode _ScopedName;
	ValueParametersNode _Parameters;

	AccessModifierType _Access = AccessModifierType::Default;
};
struct StatementsNode :Node
{
	StatementsNode() : Node(NodeType::StatementsNode)
	{

	}
	AddforNodeAndWithList(StatementsNode);
	StatementsNode(StatementsNode&& source) = default;
	StatementsNode& operator=(StatementsNode&& source) = default;
	StatementsNode(StatementsNode& source) = default;
	StatementsNode& operator=(StatementsNode& source) = default;
};
struct FuncSignatureNode :Node
{
	NameNode _Name;
	GenericValuesNode _generic;
	NamedParametersNode _Parameters;
	TypeNode _ReturnType;
	AccessModifierType _Access = AccessModifierType::Default;

	bool _HasExternKeyWord = false;
	bool _HasDynamicKeyWord = false;
	bool _HasEvalKeyWord = false;
	bool _HasUnsafeKeyWord = false;
};
struct FuncBodyNode :Node
{
	StatementsNode _Statements;
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
	FuncSignatureNode _Signature;
	Optional<FuncBodyNode> _Body;
	Vector<Unique_ptr<AttributeNode>> _Attributes;
};

struct AsmBlockNode :Node
{
	AsmBlockNode() : Node(NodeType::AsmBlockNode)
	{

	}
	AddforNode(AsmBlockNode);
	String_view _AsmText;
};

struct ExpressionNodeType :Node
{
	ExpressionNodeType() : Node(NodeType::ExpressionNodeType)
	{

	}
	AddforNode(ExpressionNodeType);
	Unique_ptr<Node> _Value = nullptr;
	ExpressionNodeType(const ExpressionNodeType& ToCopyFrom) = default;
	ExpressionNodeType(ExpressionNodeType&& source) = default;
	~ExpressionNodeType() = default;
	ExpressionNodeType& operator=(ExpressionNodeType&& source) = default;

	inline static bool IsPostfixOperator(const Token* token)
	{
		return token->Type == TokenType::increment
			|| token->Type == TokenType::decrement;
	}
	inline static bool IsCompoundOperator(const Token* token)
	{
		return token->Type == TokenType::CompoundAdd
			|| token->Type == TokenType::CompoundSub
			|| token->Type == TokenType::CompoundMult
			|| token->Type == TokenType::CompoundDiv;
	}
	inline static bool IsUnaryOperator(const Token* token)
	{
		return token->Type == TokenType::plus
			|| token->Type == TokenType::minus
			|| token->Type == TokenType::KeyWorld_Sizeof
			|| token->Type == TokenType::KeyWorld_Nameof
			|| token->Type == TokenType::KeyWorld_typeof
			|| token->Type == TokenType::Not
			|| token->Type == TokenType::bitwise_not
			|| token->Type == TokenType::QuestionMark;
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

	inline static bool IsBinaryOperator(const Token* token)
	{
		return token->Type == TokenType::plus
			|| token->Type == TokenType::minus
			|| token->Type == TokenType::star
			|| token->Type == TokenType::forwardslash
			|| token->Type == TokenType::modulo

			|| token->Type == TokenType::equal_Comparison
			|| token->Type == TokenType::Notequal_Comparison
			|| token->Type == TokenType::greaterthan
			|| token->Type == TokenType::lessthan
			|| token->Type == TokenType::greater_than_or_equalto
			|| token->Type == TokenType::less_than_or_equalto

			|| token->Type == TokenType::logical_and
			|| token->Type == TokenType::logical_or

			|| token->Type == TokenType::bitwise_and
			|| token->Type == TokenType::bitwise_or
			|| token->Type == TokenType::bitwise_LeftShift
			|| token->Type == TokenType::bitwise_RightShift
			|| token->Type == TokenType::bitwise_XOr

			|| token->Type == TokenType::approximate_Comparison;

	}
};

struct ValueExpressionNode :Node
{
	ValueExpressionNode() : Node(NodeType::ValueExpressionNode)
	{

	}
	AddforNode(ValueExpressionNode);
	Unique_ptr<Node> _Value = nullptr;
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

	ExpressionNodeType _Value0;
	const Token* _BinaryOp =nullptr;
	ExpressionNodeType _Value1;
}; 

struct RetStatementNode :Node
{
	RetStatementNode() : Node(NodeType::RetStatementNode)
	{

	}
	AddforNode(RetStatementNode);
	ExpressionNodeType _Expression;
};

struct DeclareVariableNode :Node
{
	DeclareVariableNode() : Node(NodeType::DeclareVariableNode)
	{

	}
	AddforNode(DeclareVariableNode);
	TypeNode _Type;
	NameNode _Name;
	ExpressionNodeType _Expression;
	AccessModifierType _Access = AccessModifierType::Default;

	DeclareVariableNode(DeclareVariableNode&& source) = default;
	DeclareVariableNode& operator=(DeclareVariableNode&& source) = default;
};

struct AssignExpressionNode :Node
{
	AssignExpressionNode() : Node(NodeType::AssignExpressionNode)
	{

	}
	AddforNode(AssignExpressionNode);
	ExpressionNodeType _ToAssign;
	ExpressionNodeType _Expression;
	bool _ReassignAddress = false;
	const Token* _Token = nullptr;
};

struct DeclareStaticVariableNode :Node
{
	DeclareStaticVariableNode() : Node(NodeType::DeclareStaticVariableNode)
	{

	}
	AddforNode(DeclareStaticVariableNode);
	DeclareVariableNode _Variable;
};
struct DeclareThreadVariableNode :Node
{
	DeclareThreadVariableNode() : Node(NodeType::DeclareThreadVariableNode)
	{

	}
	AddforNode(DeclareThreadVariableNode);
	DeclareVariableNode _Variable;
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

	NameNode _AliasName;
	GenericValuesNode Generic;
	TypeNode _Type;

	
	bool _IsHardAlias = false;
	
	AliasType _AliasType = AliasType::Type;
	Unique_ptr<Node> _Node;

	AccessModifierType _Access = AccessModifierType::Default;
};


struct EnumValueNode :Node
{
	
	NameNode _Name;
	ExpressionNodeType _Expression;
	Optional<TypeNode> _VariantType;
};
struct EnumNode :Node
{
	EnumNode() : Node(NodeType::EnumNode)
	{

	}
	AddforNode(EnumNode);

	NameNode _EnumName;
	GenericValuesNode _generic;
	Vector<EnumValueNode> _Values;
	TypeNode _BaseType;
	AccessModifierType _Access = AccessModifierType::Default;
};

struct TagTypeNode :Node
{
	TagTypeNode() : Node(NodeType::TagTypeNode)
	{

	}
	AddforNodeAndWithList(TagTypeNode);

	NameNode _AttributeName;
	GenericValuesNode _generic;
	AccessModifierType _Access = AccessModifierType::Default;

	TagTypeNode(TagTypeNode&& source) = default;
	TagTypeNode& operator=(TagTypeNode&& source) = default;
};


struct IfNode :Node
{
	IfNode() : Node(NodeType::IfNode)
	{

	}
	AddforNode(IfNode);

	ExpressionNodeType _Expression;
	StatementsNode _Body;
	Unique_ptr<Node> _Else;
};
struct ElseNode :Node
{
	ElseNode() : Node(NodeType::ElseNode)
	{

	}
	AddforNode(ElseNode);

	
	StatementsNode _Body;
};
struct WhileNode :Node
{
	WhileNode() : Node(NodeType::WhileNode)
	{

	}
	AddforNode(WhileNode);

	ExpressionNodeType _Expression;
	StatementsNode _Body;
};

struct DoNode :Node
{
	DoNode() : Node(NodeType::DoNode)
	{

	}
	AddforNode(DoNode);

	
	StatementsNode _Body;
	ExpressionNodeType _Expression;
};

struct PostfixVariableNode :Node
{
	PostfixVariableNode() : Node(NodeType::PostfixVariableNode)
	{

	}
	AddforNode(PostfixVariableNode);
	ExpressionNodeType _ToAssign;
	const Token* _PostfixOp = nullptr;
};

struct CompoundStatementNode :Node
{
	CompoundStatementNode() : Node(NodeType::CompoundStatementNode)
	{

	}
	AddforNode(CompoundStatementNode);
	ExpressionNodeType _ToAssign;
	const Token* _CompoundOp = nullptr;
	ExpressionNodeType _Expession;
};

struct FuncCallNode :Node
{
	FuncCallNode() : Node(NodeType::FuncCallNode)
	{

	}
	AddforNode(FuncCallNode);
	ScopedNameNode _FuncName;
	ValueParametersNode Parameters;
};
struct FuncCallStatementNode :Node
{
	FuncCallStatementNode() : Node(NodeType::FuncCallStatementNode)
	{

	}
	AddforNode(FuncCallStatementNode);
	FuncCallNode _Base;
};

struct AnonymousTypeNode :Node
{
	AnonymousTypeNode() : Node(NodeType::AnonymousTypeNode)
	{

	}
	AddforNode(AnonymousTypeNode);
	NamedParametersNode _Fields;
};

struct AnonymousObjectConstructorNode :Node
{
	AnonymousObjectConstructorNode() : Node(NodeType::AnonymousObjectConstructorNode)
	{

	}
	AddforNode(AnonymousObjectConstructorNode);
	ValueParametersNode _Fields;
};
struct CastNode :Node
{
	CastNode() : Node(NodeType::CastNode)
	{

	}
	AddforNode(CastNode);

	ExpressionNodeType _Expression;
	TypeNode _ToType;
};
struct ParenthesesExpresionNode :Node
{
	ParenthesesExpresionNode() : Node(NodeType::ParenthesesExpresionNode)
	{

	}
	AddforNode(ParenthesesExpresionNode);

	ExpressionNodeType _Expression;
};
struct SizeofExpresionNode :Node
{
	SizeofExpresionNode() : Node(NodeType::SizeofExpresionNode)
	{

	}
	AddforNode(SizeofExpresionNode);

	TypeNode _Type;
};

struct NewExpresionNode :Node
{
	NewExpresionNode() : Node(NodeType::NewExpresionNode)
	{

	}
	AddforNode(NewExpresionNode);


	const Token* _KeywordToken = nullptr;
	TypeNode _Type;
	ValueParametersNode _Parameters;

	ExpressionNodeType _Arrayexpression;//can be null
};

struct DropStatementNode :Node
{
	DropStatementNode() : Node(NodeType::DropStatementNode)
	{

	}
	AddforNode(DropStatementNode);

	const Token* _KeywordToken =nullptr;
	ExpressionNodeType _expression;
};


struct AliasNode_Func :Node
{
	NamedParametersNode _Parameters;
	TypeNode _ReturnType;
};


struct IndexedExpresionNode :Node
{
	IndexedExpresionNode() : Node(NodeType::IndexedExpresionNode)
	{

	}
	AddforNode(IndexedExpresionNode);

	ExpressionNodeType _SourceExpression;// ex[SomeEx]
	ExpressionNodeType _IndexExpression;// SomeVar[ex]
};

struct ForNode :Node
{
	enum class ForType
	{
		Traditional,//for [int a = 0;a < 10;a++];
		modern,//for [var& Item : List];
	};

	ForNode() : Node(NodeType::ForNode)
	{

	}
	AddforNode(ForNode);


	ForType _Type = ForType::Traditional;

	//Traditional
	ExpressionNodeType _Traditional_Assignment_Expression;
	ExpressionNodeType _BoolExpression;
	PostfixVariableNode _OnNextStatement;
	//Modern
	ExpressionNodeType _Modern_List;
	//Both
	TypeNode _typeNode;
	const Token* _Name = nullptr;
	StatementsNode _Body;
};


struct ContinueNode :Node
{

	ContinueNode() : Node(NodeType::ContinueNode)
	{

	}
	AddforNode(ContinueNode);
	
	const Token* _token = nullptr;
};

struct BreakNode :Node
{

	BreakNode() : Node(NodeType::BreakNode)
	{

	}
	AddforNode(BreakNode);

	const Token* _token = nullptr;
};

struct MoveNode :Node
{
	MoveNode() : Node(NodeType::MoveNode)
	{

	}
	AddforNode(MoveNode);

	ExpressionNodeType _expression;
};

struct LambdaCapture
{
	Optional<TypeNode> _Type;
	NameNode _Name;
	ExpressionNodeType _Expression;
};

struct LambdaCapturesData
{
	Vector<LambdaCapture> _Captures;
};
struct LambdaNode :Node
{
	LambdaNode() : Node(NodeType::LambdaNode)
	{

	}
	AddforNode(LambdaNode);

	const Token* _LambdaStart = nullptr;
	NamedParametersNode _Pars;
	Optional<LambdaCapturesData> _Capture;
	Optional<StatementsNode> _Statements;
};

struct TraitNode :Node
{
	TraitNode() : Node(NodeType::TraitNode)
	{

	}
	NameNode _Name;
	AccessModifierType _Access = AccessModifierType::Default;
	GenericValuesNode _generic;
	AddforNodeAndWithList(TraitNode);

};
struct OutExpression :Node
{
	OutExpression() : Node(NodeType::OutExpression)
	{

	}
	NameNode _Name;
	TypeNode _Type;
	AddforNode(OutExpression);
};

struct BitCastExpression :Node
{
	BitCastExpression() : Node(NodeType::BitCastExpression)
	{

	}

	const Token* _KeywordToken = nullptr;
	TypeNode _Type;
	ExpressionNodeType _Expression;
	AddforNode(BitCastExpression);
};

struct PanicNode :Node
{
	PanicNode() : Node(NodeType::PanicNode)
	{

	}
	ExpressionNodeType  _StringExpression;
	AddforNode(PanicNode);
};

struct InvalidNode :Node
{
	InvalidNode() : Node(NodeType::InvalidNode)
	{

	}
	ExpressionNodeType  _StringExpression;


	const Token* _KeyWord = nullptr;
	AddforNode(InvalidNode);
};

struct ValidNode :Node
{
	ValidNode() : Node(NodeType::ValidNode)
	{

	}
	bool _IsExpression = true;

	ExpressionNodeType  _ExpressionToCheck;
	StatementsNode _StatementToCheck;

	AddforNode(ValidNode);
};

struct CMPTypesNode :Node
{
	CMPTypesNode() noexcept : Node(NodeType::CMPTypesNode)
	{

	}
	
	TypeNode _TypeOp0;
	const Token* _Op=nullptr;
	TypeNode _TypeOp1;

	static bool IsOp(TokenType Type)
	{
		switch (Type)
		{
		case TokenType::equal_Comparison:
		case TokenType::Notequal_Comparison:
		case TokenType::greaterthan:
			return true;
		default:
			return false;
		}
	}

	AddforNode(CMPTypesNode);
};

struct TypeToValueNode :Node
{
	TypeToValueNode() : Node(NodeType::TypeToValueNode)
	{

	}

	TypeNode TypeOp;

	AddforNode(TypeToValueNode);
};

struct ExpressionToTypeValueNode :Node
{
	ExpressionToTypeValueNode() : Node(NodeType::ExpressionToTypeValueNode)
	{

	}

	ExpressionNodeType _TypeEx;

	AddforNode(ExpressionToTypeValueNode);
};

struct DeclareEvalVariableNode :Node
{
	DeclareEvalVariableNode() : Node(NodeType::DeclareEvalVariableNode)
	{

	}
	AddforNode(DeclareEvalVariableNode);
	DeclareVariableNode _Variable;
};
struct CompileTimeIfNode :Node
{
	CompileTimeIfNode() : Node(NodeType::CompileTimeIfNode)
	{

	}
	AddforNode(CompileTimeIfNode);

	ExpressionNodeType _Expression;
	StatementsNode _Body;
	Unique_ptr<Node> _Else;
};

struct CompileTimeForNode :Node
{
	enum class ForType
	{
		Traditional,//for [int a = 0;a < 10;a++];
		modern,//for [var& Item : List];
	};

	CompileTimeForNode() : Node(NodeType::CompileTimeForNode)
	{

	}
	AddforNode(CompileTimeForNode);


	ForType _Type = ForType::Traditional;

	//Traditional
	ExpressionNodeType _Traditional_Assignment_Expression;
	ExpressionNodeType _BoolExpression;
	PostfixVariableNode _OnNextStatement;
	//Modern
	ExpressionNodeType _Modern_List;
	//Both
	TypeNode _TypeNode;
	const Token* _Name = nullptr;
	StatementsNode _body;
};

struct ExtendedScopeExpression : Node
{
	ExpressionNodeType _Expression;
	ScopedName::Operator_t _Operator = ScopedName::Operator_t::Null;
	ScopedNameNode _Extended;

	AddforNode(ExtendedScopeExpression);

	ExtendedScopeExpression() : Node(NodeType::ExtendedScopeExpression)
	{

	}
};

struct ExtendedFuncExpression : Node
{
	ExpressionNodeType _Expression;
	ScopedName::Operator_t _Operator = ScopedName::Operator_t::Null;
	FuncCallNode _Extended;

	AddforNode(ExtendedFuncExpression);

	ExtendedFuncExpression() : Node(NodeType::ExtendedFuncExpression)
	{

	}
};


struct MatchStatementArm
{
	ExpressionNodeType _Expression;
	StatementsNode _Statements;
};

struct MatchStatement : Node
{
	ExpressionNodeType _Expression;
	Vector<MatchStatementArm> _Arms;
	Optional<StatementsNode> _InvaidCase;

	AddforNode(MatchStatement);

	MatchStatement() : Node(NodeType::MatchStatement)
	{

	}
};

struct MatchExpressionArm
{
	ExpressionNodeType _Expression;
	ExpressionNodeType _AssignmentExpression;
};
struct MatchExpression : Node
{
	ExpressionNodeType _Expression;
	Vector<MatchExpressionArm> _Arms;
	Optional<ExpressionNodeType> _InvaidCase;

	AddforNode(MatchExpression);

	MatchExpression() : Node(NodeType::MatchExpression)
	{

	}
};

struct ImportValue
{
	Optional<const Token*> _AliasName;
	ScopedNameNode _ImportedSybol;
};

struct ImportStatement : Node
{
	const Token* _Token = nullptr;
	Vector<ImportValue> _Imports;
	Optional<ScopedNameNode> _StartingNameSpace;
	AddforNode(ImportStatement);
	ImportStatement() : Node(NodeType::ImportStatement)
	{

	}
};

struct AwaitExpression : Node
{
	const Token* _Token = nullptr;
	
	bool _IsFunc = false;
	FuncCallNode _Func;
	LambdaNode _Lambda;

	AddforNode(AwaitExpression);
	AwaitExpression() : Node(NodeType::AwaitExpression)
	{

	}
};
struct AwaitStatement : Node
{
	AwaitExpression _Base;

	AddforNode(AwaitStatement);
	AwaitStatement() : Node(NodeType::AwaitStatement)
	{

	}
};

struct YieldExpression : Node
{
	const Token* _Token = nullptr;
	ExpressionNodeType _Expression;

	AddforNode(YieldExpression);
	YieldExpression() : Node(NodeType::YieldExpression)
	{

	}
};
struct YieldStatement : Node
{
	YieldExpression _Base;

	AddforNode(YieldStatement);
	YieldStatement() : Node(NodeType::YieldStatement)
	{

	}
};
struct UnsafeStatementsNode : Node
{
	StatementsNode _Base;

	AddforNode(UnsafeStatementsNode);
	UnsafeStatementsNode() : Node(NodeType::UnsafeStatementsNode)
	{

	}
};
UCodeLangFrontEnd