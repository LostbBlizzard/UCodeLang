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
	Vector<Token> _Nodes;
	FileNode _Tree;
	bool _ParseSuccess = false;
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;

	inline Token* TryPeekNextToken(size_t offset)
	{
		size_t Index = _TokenIndex + offset;
		if (Index < _Nodes.size()) 
		{
			return &_Nodes[Index];
		}
		else
		{
			return nullptr;
		}
	}
	UCodeLangForceinline Token* TryPeekNextToken() { return   TryPeekNextToken(1); }
	UCodeLangForceinline Token* TryGetToken() { return TryPeekNextToken(0); }
	UCodeLangForceinline void NextToken() { _TokenIndex++; }
	UCodeLangForceinline void NextToken(size_t offfset) { _TokenIndex += offfset; }
	
	inline bool IsPostfixOperator(Token* Token)
	{
		return Token->Type == TokenType::increment
			|| Token->Type == TokenType::decrement;
	}
	inline bool IsCompoundOperator(Token* Token)
	{
		return Token->Type == TokenType::CompoundAdd
			|| Token->Type == TokenType::CompoundSub
			|| Token->Type == TokenType::CompoundMult
			|| Token->Type == TokenType::CompoundDiv;
	}
	inline bool IsUnaryOperator(Token* Token)
	{
		return Token->Type == TokenType::plus
			|| Token->Type == TokenType::minus
			|| Token->Type == TokenType::KeyWorld_Sizeof
		    || Token->Type == TokenType::KeyWorld_Nameof
			|| Token->Type == TokenType::KeyWorld_typeof
			|| Token->Type == TokenType::Not
			|| Token->Type == TokenType::bitwise_not;
	}
	inline bool IsOverLoadableOperator(Token* Token)
	{
		return Token->Type == TokenType::equal_Comparison
			|| Token->Type == TokenType::Notequal_Comparison
			|| Token->Type == TokenType::greaterthan
			|| Token->Type == TokenType::lessthan
			|| Token->Type == TokenType::greater_than_or_equalto
			|| Token->Type == TokenType::less_than_or_equalto;
	}
	inline bool IsBinaryOperator(Token* Token)
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
			|| Token->Type == TokenType::bitwise_XOr;
			
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
		ClassNode* V = ClassNode::Gen();
		auto r = GetClassNode(*V);
		return { r,V->As() };
	}
	GotNodeType GetClassNode(ClassNode& out);
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
		case UCodeLang::TokenType::KeyWorld_asm:return true;
		case UCodeLang::TokenType::StartTab:return true;
		case UCodeLang::TokenType::KeyWorld_use:return true;
		case UCodeLang::TokenType::Class:return true;
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
	GotNodeType TryGetGeneric(GenericValuesNode& out);

	GotNodeType GetName(ScopedNameNode& out);
	GotNodeType GetName(NameNode& out);
	GotNodeType GetType(TypeNode& out);
	GotNodeType GetTypeWithVoid(TypeNode& out);
	GotNodeType GetNumericType(TypeNode& out);
	
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
};
UCodeLangEnd

