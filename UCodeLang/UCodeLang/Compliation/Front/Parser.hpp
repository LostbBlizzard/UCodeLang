#pragma once
#include "../../LangCore.hpp"
#include "../Helpers/CompilerTypes.hpp"
#include "../NodeTypes/TreeNode.hpp"
#include "../NodeTypes/TreeNodeTypes.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../CompliationSettings.hpp"
#include "../../LangCore/TypeNames.hpp"
UCodeLangStart


class Parser
{	
public:
	static constexpr TokenType declareFunc = TokenType::bitwise_or;
	static constexpr TokenType declareFuncParsStart = TokenType::Left_Bracket;
	static constexpr TokenType declareFuncParsEnd = TokenType::Right_Bracket;
	

	static constexpr TokenType FuncCallStart = TokenType::Left_Parentheses;
	static constexpr TokenType FuncCallEnd = TokenType::Right_Parentheses;

	static constexpr TokenType CastStart = TokenType::Left_Bracket;
	static constexpr TokenType CastEnd = TokenType::Right_Bracket;
	
	static constexpr TokenType SizeofStart = TokenType::Left_Parentheses;
	static constexpr TokenType SizeofEnd = TokenType::Right_Parentheses;
	
	static constexpr TokenType IfToken = TokenType::bitwise_not;
	static constexpr TokenType ElseToken = TokenType::bitwise_XOr;

	Parser(){}
	~Parser(){}
	void Reset();
	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V){_ErrorsOutput = V;}
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) { _Settings = V; }

	struct FileData
	{
		String_view Text;//For AsmBlock to Work
		String_view FilePath;
	};

	void Parse(const FileData& Data,const Vector<Token>& Tokens);
	UCodeLangForceinline bool Get_ParseSucces() { return _ParseSuccess; }
	UCodeLangForceinline FileNode& Get_Tree() { return _Tree; }
private:
	size_t _TokenIndex = 0;
	const Vector<Token>* _Nodes = nullptr;
	FileNode _Tree;
	bool _ParseSuccess = false;
	String_view _Text;
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;

	inline const Token* TryPeekNextToken(size_t offset)
	{
		size_t Index = _TokenIndex + offset;
		if (Index < _Nodes->size()) 
		{
			return &_Nodes->at(Index);
		}
		else
		{
			return nullptr;
		}
	}
	UCodeLangForceinline const Token* TryPeekNextToken() { return   TryPeekNextToken(1); }
	UCodeLangForceinline const Token* TryGetToken() { return TryPeekNextToken(0); }
	UCodeLangForceinline void NextToken() { _TokenIndex++; }
	UCodeLangForceinline void NextToken(size_t offfset) { _TokenIndex += offfset; }
	
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
			|| Token->Type == TokenType::bitwise_not;
	}
	inline static bool IsOverLoadableOperator(const Token* Token)
	{
		return Token->Type == TokenType::equal_Comparison
			|| Token->Type == TokenType::Notequal_Comparison
			|| Token->Type == TokenType::greaterthan
			|| Token->Type == TokenType::lessthan
			|| Token->Type == TokenType::greater_than_or_equalto
			|| Token->Type == TokenType::less_than_or_equalto;
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
		return { r,V->As() };
	}
	GotNodeType GetNamespaceNode(NamespaceNode& out);
	TryGetNode GetClassNode()
	{
		Node* V = nullptr;
		auto r = GetClassTypeNode(V);
		return { r,V};
	}
	GotNodeType GetClassTypeNode(Node*& out);
	TryGetNode GetFuncNode()
	{
		FuncNode* V = FuncNode::Gen();
		auto r = GetFuncNode(*V);
		return {r,V->As()};
	}
	GotNodeType GetStatementsorStatementNode(StatementsNode& out);
	GotNodeType GetStatement(Node*& out);

	bool IsStartofaStatement(TokenType type)
	{
		switch (type)
		{
		case TokenType::KeyWorld_asm:return true;
		case TokenType::StartTab:return true;
		case TokenType::KeyWorld_use:return true;
		case TokenType::Class:return true;
		case Parser::IfToken:return true;
		case TokenType::Left_Bracket:return true;
		default:return false;
		}
	}
	TryGetNode GetStatements()
	{
		StatementsNode* V = StatementsNode::Gen();
		auto r = GetStatements(*V);
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

	GotNodeType GetName(ScopedNameNode& out);
	GotNodeType GetName(NameNode& out);
	GotNodeType GetNameCheck(NameNode& out);
	GotNodeType GetType(TypeNode& out);
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
		return { r,V->As() };
	}
	GotNodeType GetAttribute(AttributeNode& out);

	TryGetNode GetUseNode()
	{
		UsingNode* V = UsingNode::Gen();
		auto r = GetUseNode(*V);
		return { r,V->As() };
	}
	GotNodeType GetUseNode(UsingNode& out);
	//Statements
	TryGetNode GetAsmBlock()
	{
		AsmBlockNode* V = AsmBlockNode::Gen();
		auto r = GetAsmBlock(*V);
		return { r,V->As() };
	}
	GotNodeType GetAsmBlock(AsmBlockNode& out);

	TryGetNode GetRetStatement()
	{
		RetStatementNode* V = RetStatementNode::Gen();
		auto r = GetRetStatement(*V);
		return { r,V->As() };
	}
	GotNodeType GetRetStatement(RetStatementNode& out);

	TryGetNode GetDeclareStaticVariable()
	{
		DeclareStaticVariableNode* V = DeclareStaticVariableNode::Gen();
		auto r = GetDeclareStaticVariable(*V);
		return { r,V->As() };
	}
	GotNodeType GetDeclareStaticVariable(DeclareStaticVariableNode& out);
	TryGetNode GetDeclareThreadVariable()
	{
		DeclareThreadVariableNode* V = DeclareThreadVariableNode::Gen();
		auto r = GetDeclareThreadVariable(*V);
		return { r,V->As() };
	}
	GotNodeType  GetDeclareThreadVariable(DeclareThreadVariableNode& out);


	TryGetNode GetDeclareVariable()
	{
		DeclareVariableNode* V = DeclareVariableNode::Gen();
		auto r = GetDeclareVariable(*V);
		return { r,V->As() };
	}
	GotNodeType GetDeclareVariable(DeclareVariableNode& out);

	TryGetNode GetAssignVariable()
	{
		AssignVariableNode* V = AssignVariableNode::Gen();
		auto r = GetAssignVariable(*V);
		return { r,V->As() };
	}
	GotNodeType GetAssignVariable(AssignVariableNode& out);

	TryGetNode GetPostfixStatement()
	{
		PostfixVariableNode* V = PostfixVariableNode::Gen();
		auto r = GetPostfixStatement(*V);
		return { r,V->As() };
	}
	GotNodeType GetPostfixStatement(PostfixVariableNode& out);

	TryGetNode GetCompoundStatement()
	{
		CompoundStatementNode* V = CompoundStatementNode::Gen();
		auto r = GetCompoundStatement(*V);
		return { r,V->As() };
	}
	GotNodeType GetCompoundStatement(CompoundStatementNode& out);

	void GetDeclareVariableNoObject(TryGetNode& out);

	GotNodeType GetAlias(const Token* AliasName, GenericValuesNode& AliasGenerics, AliasNode& out);

	TryGetNode GetIfNode()
	{
		IfNode* V = IfNode::Gen();
		auto r = GetIfNode(*V);
		return { r,V->As() };
	}
	GotNodeType GetIfNode(IfNode& out);

	TryGetNode GetEnumNode()
	{
		EnumNode* V = EnumNode::Gen();
		auto r = GetEnumNode(*V);
		return { r,V->As() };
	}
	GotNodeType GetEnumNode(EnumNode& out);
	GotNodeType GetEnumValueNode(EnumValueNode& out);

	TryGetNode GetTagNode()
	{
		TagTypeNode* V = TagTypeNode::Gen();
		auto r = GetTagNode(*V);
		return { r,V->As() };
	}
	GotNodeType GetTagNode(TagTypeNode& out);

	TryGetNode GetFuncCallStatementNode()
	{
		FuncCallStatementNode* V = FuncCallStatementNode::Gen();
		auto r = GetFuncCallStatementNode(*V);
		return { r,V->As() };
	}
	GotNodeType GetFuncCallStatementNode(FuncCallStatementNode& out);

	TryGetNode GetFuncCallNode()
	{
		FuncCallNode* V = FuncCallNode::Gen();
		auto r = GetFuncCallNode(*V);
		return { r,V->As() };
	}
	GotNodeType GetFuncCallNode(FuncCallNode& out);
};
UCodeLangEnd

