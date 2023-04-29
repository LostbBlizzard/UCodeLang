#pragma once
#include "../../LangCore.hpp"
#include "../Helpers/CompilerTypes.hpp"
#include "../NodeTypes/TreeNode.hpp"
#include "../NodeTypes/TreeNodeTypes.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../CompliationSettings.hpp"
#include "../../LangCore/TypeNames.hpp"

#include "UCodeFrontEndNameSpace.hpp"
UCodeLangFrontStart
class Parser
{	
public:
	static constexpr TokenType declareFunc = TokenType::bitwise_or;
	static constexpr TokenType declareFuncParsStart = TokenType::Left_Bracket;
	static constexpr TokenType declareFuncParsEnd = TokenType::Right_Bracket;
	
	static constexpr TokenType AnonymousObjectStart = TokenType::Left_Bracket;
	static constexpr TokenType AnonymousObjectEnd = TokenType::Right_Bracket;

	static constexpr TokenType FuncCallStart = TokenType::Left_Parentheses;
	static constexpr TokenType FuncCallEnd = TokenType::Right_Parentheses;

	static constexpr TokenType CastStart = TokenType::Left_Bracket;
	static constexpr TokenType CastEnd = TokenType::Right_Bracket;
	
	static constexpr TokenType SizeofStart = TokenType::Left_Parentheses;
	static constexpr TokenType SizeofEnd = TokenType::Right_Parentheses;
	
	

	Parser(){}
	~Parser(){}
	void Reset();
	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V){_ErrorsOutput = V;}
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) { _Settings = V; }

	
	void Parse(const Vector<Token>& Tokens);
	UCodeLangForceinline bool Get_ParseSucces() { return _ParseSuccess; }
	UCodeLangForceinline FileNode& Get_Tree() { return _Tree; }
private:
	size_t _TokenIndex = 0;
	const Vector<Token>* _Nodes = nullptr;
	FileNode _Tree;
	bool _ParseSuccess = false;
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	Stack<AccessModifierType> _AccessModifier;

	void AccessStart()
	{
		_AccessModifier.push(AccessModifierType::Default);
	}

	void AccessEnd()
	{
		_AccessModifier.pop();
	}

	size_t TopScope = 0;
	void SetNotTopScope()
	{
		TopScope++;
	}
	void UnSetTopScope()
	{
		TopScope--;
	}
	bool  IsTopScope()
	{
		return TopScope == 0;
	}

	bool _HasTripped = false;
	void Tripped()
	{
		_HasTripped = true;
	}
	void GotVaild()
	{
		_HasTripped = false;
	}
	void TrippedCheck(GotNodeType T)
	{
		if (T != GotNodeType::Error)
		{
			GotVaild();
		}
	}
	inline const Token* TryPeekNextToken(size_t offset)
	{
		size_t Index = _TokenIndex + offset;
		if (Index < _Nodes->size()) 
		{
			return &_Nodes->at(Index);
		}
		else
		{
			return &_Nodes->back();
		}
	}
	UCodeLangForceinline const Token* TryPeekNextToken() { return   TryPeekNextToken(1); }
	UCodeLangForceinline const Token* TryGetToken() { return TryPeekNextToken(0); }
	UCodeLangForceinline void NextToken() { _TokenIndex++; }
	UCodeLangForceinline void NextToken(size_t offfset) { _TokenIndex += offfset; }
	
	void TokenTypeCheck(const Token* Value, TokenType Type);

	inline static GotNodeType Merge(GotNodeType A, GotNodeType B)
	{
		if (A == GotNodeType::Success && B == GotNodeType::Success)
		{
			return GotNodeType::Success;
		}
		return GotNodeType::failed;
	}
	
	TryGetNode GetNamespaceNode()
	{
		NamespaceNode* V = NamespaceNode::Gen();
		auto r = GetNamespaceNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetNamespaceNode(NamespaceNode& out);
	TryGetNode GetClassNode()
	{
		Node* V = nullptr;
		auto r = GetClassTypeNode(V);
		TrippedCheck(r);
		return { r,V};
	}
	GotNodeType GetClassTypeNode(Node*& out);
	GotNodeType DoClassType(ClassNode* output, const Token* ClassToken, GenericValuesNode& TepGenerics, const Token* ColonToken);
	void ClassTypeAccessModifierInerScope(Vector<Unique_ptr<Node>>& Out);
	
	TryGetNode GetFuncNode()
	{
		FuncNode* V = FuncNode::Gen();
		auto r = GetFuncNode(*V);
		TrippedCheck(r);
		return {r,V->As()};
	}
	GotNodeType GetStatementsorStatementNode(StatementsNode& out);
	GotNodeType GetStatement(Node*& out);

	bool IsStartofaStatement(TokenType type)
	{
		switch (type)
		{
		case TokenType::StartTab:return true;
		case TokenType::KeyWord_use:return true;
		case TokenType::Class:return true;
		case TokenType::KeyWord_If:return true;
		case TokenType::Left_Bracket:return true;
		default:return false;
		}
	}
	TryGetNode GetStatements()
	{
		StatementsNode* V = StatementsNode::Gen();
		auto r = GetStatements(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetStatements(StatementsNode& out);
	
	GotNodeType GetFuncNode(FuncNode& out);
	GotNodeType GetFuncSignatureNode(FuncSignatureNode& out);
	GotNodeType GetFuncBodyNode(FuncBodyNode& out);
	GotNodeType GetNamedParametersNode(NamedParametersNode& out);

	GotNodeType GetValueParameterNode(Node*& out);
	GotNodeType GetValueParametersNode(ValueParametersNode& out);
	GotNodeType TryGetGeneric(GenericValuesNode& out);

	GotNodeType TryGetGeneric(UseGenericsNode& out);

	GotNodeType GetName(ScopedNameNode& out);
	GotNodeType GetName(NameNode& out);
	GotNodeType GetNameCheck(NameNode& out);


	enum class NameCheck_t : UInt8
	{
		Null,
		Name,
		MemberAccess,
	};
	struct GetNameCheck_ret
	{
		NameCheck_t Type = NameCheck_t::Null;
		GotNodeType Gotnode = GotNodeType::Null;
	};
	GetNameCheck_ret GetNameCheck(ScopedNameNode& out);

	GotNodeType GetType(TypeNode*& out,bool ignoreRighthandOFtype =false,bool ignoreleftHandType = true);
	GotNodeType GetType(TypeNode& out, bool ignoreRighthandOFtype = false, bool ignoreleftHandType = true);
	GotNodeType GetTypeWithVoid(TypeNode& out);
	GotNodeType GetNumericType(TypeNode& out);


	GotNodeType GetExpressionNode(Node*& out);
	GotNodeType GetExpressionNode(ValueExpressionNode& out);
	
	GotNodeType GetExpressionTypeNode(Node*& out);
	GotNodeType GetExpressionTypeNode(ExpressionNodeType& out);


	TryGetNode GetAttribute()
	{
		AttributeNode* V = AttributeNode::Gen();
		auto r = GetAttribute(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetAttribute(AttributeNode& out);

	TryGetNode GetUseNode()
	{
		UsingNode* V = UsingNode::Gen();
		auto r = GetUseNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetUseNode(UsingNode& out);
	//Statements

	TryGetNode GetRetStatement()
	{
		RetStatementNode* V = RetStatementNode::Gen();
		auto r = GetRetStatement(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetRetStatement(RetStatementNode& out);

	TryGetNode GetDeclareStaticVariable()
	{
		DeclareStaticVariableNode* V = DeclareStaticVariableNode::Gen();
		auto r = GetDeclareStaticVariable(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetDeclareStaticVariable(DeclareStaticVariableNode& out, bool ignoreleftHandType = false);
	TryGetNode GetDeclareThreadVariable()
	{
		DeclareThreadVariableNode* V = DeclareThreadVariableNode::Gen();
		auto r = GetDeclareThreadVariable(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType  GetDeclareThreadVariable(DeclareThreadVariableNode& out, bool ignoreleftHandType = false);


	TryGetNode GetDeclareVariable()
	{
		DeclareVariableNode* V = DeclareVariableNode::Gen();
		auto r = GetDeclareVariable(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetDeclareVariable(DeclareVariableNode& out, bool ignoreleftHandType =false);

	

	TryGetNode GetAssignExpression()
	{
		AssignExpressionNode* V = AssignExpressionNode::Gen();
		auto r = GetAssignExpression(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetAssignExpression(AssignExpressionNode& out);

	TryGetNode GetPostfixStatement(bool DoSemicolon =true)
	{
		PostfixVariableNode* V = PostfixVariableNode::Gen();
		auto r = GetPostfixStatement(*V,DoSemicolon);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetPostfixStatement(PostfixVariableNode& out,bool DoSemicolon = true);

	TryGetNode GetCompoundStatement()
	{
		CompoundStatementNode* V = CompoundStatementNode::Gen();
		auto r = GetCompoundStatement(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetCompoundStatement(CompoundStatementNode& out);

	void GetDeclareVariableNoObject(TryGetNode& out);

	GotNodeType GetAlias(const Token* AliasName, GenericValuesNode&& AliasGenerics, AliasNode& out);

	TryGetNode GetIfNode()
	{
		IfNode* V = IfNode::Gen();
		auto r = GetIfNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetIfNode(IfNode& out);

	TryGetNode GetWhileNode()
	{
		WhileNode* V = WhileNode::Gen();
		auto r = GetWhileNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetWhileNode(WhileNode& out);

	TryGetNode GetDoNode()
	{
		DoNode* V = DoNode::Gen();
		auto r = GetDoNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetDoNode(DoNode& out);

	TryGetNode GetEnumNode()
	{
		EnumNode* V = EnumNode::Gen();
		auto r = GetEnumNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetEnumNode(EnumNode& out);
	GotNodeType GetEnumValueNode(EnumValueNode& out);

	TryGetNode GetTagNode()
	{
		TagTypeNode* V = TagTypeNode::Gen();
		auto r = GetTagNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetTagNode(TagTypeNode& out);

	TryGetNode GetFuncCallStatementNode()
	{
		FuncCallStatementNode* V = FuncCallStatementNode::Gen();
		auto r = GetFuncCallStatementNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetFuncCallStatementNode(FuncCallStatementNode& out);

	TryGetNode GetFuncCallNode()
	{
		FuncCallNode* V = FuncCallNode::Gen();
		auto r = GetFuncCallNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetFuncCallNode(FuncCallNode& out);

	TryGetNode GetAnonymousObjectConstructorNode()
	{
		AnonymousObjectConstructorNode* V = AnonymousObjectConstructorNode::Gen();
		auto r = GetAnonymousObjectConstructorNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetAnonymousObjectConstructorNode(AnonymousObjectConstructorNode& out);

	TryGetNode GetDropStatementNode()
	{
		DropStatementNode* V = DropStatementNode::Gen();
		auto r = GetDropStatementNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetDropStatementNode(DropStatementNode& out);

	TryGetNode GetNewExpresionNode()
	{
		NewExpresionNode* V =NewExpresionNode::Gen();
		auto r = GetNewExpresionNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetNewExpresionNode(NewExpresionNode& out);

	TryGetNode GetumutVariableDeclare()
	{
		Node* V = nullptr;
		auto r = GetumutVariableDeclare(V);
		TrippedCheck(r);
		return { r,V };
	}
	GotNodeType GetumutVariableDeclare(Node*& out);


	TryGetNode GetForNode()
	{
		ForNode* V = ForNode::Gen();
		auto r = GetForNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetForNode(ForNode& out);


	TryGetNode GetContinueNode()
	{
		ContinueNode* V = ContinueNode::Gen();
		auto r = GetContinueNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetContinueNode(ContinueNode& out);

	TryGetNode GetBreakNode()
	{
		BreakNode* V = BreakNode::Gen();
		auto r = GetBreakNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetBreakNode(BreakNode& out);


	TryGetNode GetLambdaNode()
	{
		LambdaNode* V = LambdaNode::Gen();
		auto r = GetLambdaNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetLambdaNode(LambdaNode& out);
	TryGetNode GetShortLambdaNode();

	TryGetNode GetTraitNode()
	{ 
		auto V = TraitNode::Gen();
		auto r = GetTraitNode(*V);
		TrippedCheck(r);
		return { r,V };
	}
	GotNodeType GetTraitNode(TraitNode& out);

	void TraitAccessModifierInerScope(Vector< Unique_ptr<Node>>& Out);

	TryGetNode GetBitCastExpression()
	{
		BitCastExpression* V = BitCastExpression::Gen();
		auto r = GetBitCastExpression(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetBitCastExpression(BitCastExpression& out);


	TryGetNode GetPanicNode()
	{
		PanicNode* V = PanicNode::Gen();
		auto r = GetPanicNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetPanicNode(PanicNode& out);

	AccessModifierType GetModifier()
	{
		return _AccessModifier.size() ? _AccessModifier.top() : AccessModifierType::Default;
	}

	
	TryGetNode GetInvalidNode()
	{
		InvalidNode* V = InvalidNode::Gen();
		auto r = GetInvalidNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetInvalidNode(InvalidNode& out);

	TryGetNode GetValidNode()
	{
		ValidNode* V = ValidNode::Gen();
		auto r = GetValidNode(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetValidNode(ValidNode& out);


	TryGetNode GeTypeExNode()
	{
		Node* V = nullptr;
		auto r = GeTypeExNode(V);
		TrippedCheck(r);
		return { r,V };
	}
	GotNodeType GeTypeExNode(Node*& out);

	TryGetNode GetExpressionToTypeValue()
	{
		ExpressionToTypeValueNode* V = ExpressionToTypeValueNode::Gen();
		auto r = GetExpressionToTypeValue(*V);
		TrippedCheck(r);
		return { r,V->As() };
	}
	GotNodeType GetExpressionToTypeValue(ExpressionToTypeValueNode& out);
};
UCodeLangFrontEnd

