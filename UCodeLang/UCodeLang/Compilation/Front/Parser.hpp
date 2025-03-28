#pragma once
#ifndef UCodeLangNoCompiler

#include "../../LangCore.hpp"
#include "../Helpers/CompilerTypes.hpp"
#include "../NodeTypes/TreeNode.hpp"
#include "../NodeTypes/TreeNodeTypes.hpp"
#include "../Helpers/CompilationErrors.hpp"
#include "../CompilationSettings.hpp"
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



	Parser() {}
	~Parser() {}
	void Reset();
	UCodeLangForceinline void Set_ErrorsOutput(CompilationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompilationSettings* V) { _Settings = V; }


	void Parse(const String_view FileText, const Vector<Token>& Tokens);
	UCodeLangForceinline bool Get_ParseSucces() { return _ParseSuccess; }
	UCodeLangForceinline FileNode& Get_Tree() { return _Tree; }
private:
	size_t _TokenIndex = 0;
	const Vector<Token>* _Nodes = nullptr;
	FileNode _Tree;
	bool _ParseSuccess = false;
	CompilationErrors* _ErrorsOutput = nullptr;
	CompilationSettings* _Settings = nullptr;
	Stack<AccessModifierType> _AccessModifier;
	bool _PassedImportFileSection = false;
	size_t TopScope = 0;
	Stack<Vector<Unique_ptr<AttributeNode>>> _TepAttributes;

	void AttributeStart()
	{
		_TepAttributes.push({});
	}
	void AttributeEnd()
	{
		AttributeCheck();
		_TepAttributes.pop();
	}
	Vector<Unique_ptr<AttributeNode>> Get_TepAttributes()
	{
		if (_TepAttributes.size())
		{
			auto R = std::move(_TepAttributes.top());
			return R;
		}
		else
		{
			return {};
		}
	}
	void AttributeCheck()
	{
		auto& Item = _TepAttributes.top();
		if (Item.size())
		{
			auto& F = Item.front();
			auto Token = F->_ScopedName._ScopedName.front()._token;
			_ErrorsOutput->AddError(ErrorCodes::TreeAnalyerError, Token->OnLine, Token->OnPos, "You can't put the Tag '" + (String)F->_ScopedName._ScopedName.front()._token->Value._String + "' here.");
		}
	}

	void AccessStart()
	{
		_AccessModifier.push(AccessModifierType::Default);
	}

	void AccessEnd()
	{
		_AccessModifier.pop();
	}


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
		auto V = NamespaceNode::Gen();
		auto r = GetNamespaceNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetNamespaceNode(NamespaceNode& out);
	TryGetNode GetClassNode()
	{
		Unique_ptr<Node> V = nullptr;
		auto r = GetClassTypeNode(V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetClassTypeNode(Unique_ptr<Node>& out);
	GotNodeType DoClassType(ClassNode* output, const Token* ClassToken, GenericValuesNode& TepGenerics, const Token* ColonToken);
	void ClassTypeAccessModifierInerScope(Vector<Unique_ptr<Node>>& Out);

	TryGetNode GetFuncNode()
	{
		auto V = FuncNode::Gen();
		auto r = GetFuncNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetStatementsorStatementNode(StatementsNode& out);
	GotNodeType GetStatement(Unique_ptr<Node>& out);

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
		auto V = StatementsNode::Gen();
		auto r = GetStatements(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetStatements(StatementsNode& out);

	GotNodeType GetFuncNode(FuncNode& out);
	GotNodeType GetFuncSignatureNode(FuncSignatureNode& out);
	GotNodeType GetFuncBodyNode(FuncBodyNode& out);
	GotNodeType GetNamedParametersNode(NamedParametersNode& out, bool CanHaveOutPar = false);

	GotNodeType GetValueParameterNode(Unique_ptr<Node>& out);
	GotNodeType GetValueParametersNode(ValueParametersNode& out);
	GotNodeType TryGetGeneric(GenericValuesNode& out);

	GotNodeType TryGetGeneric(UseGenericsNode& out);

	GotNodeType GetName(ScopedNameNode& out, bool CanHaveVarableName = false);
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
	GetNameCheck_ret GetNameCheck(ScopedNameNode& out, bool CanHaveVarableName = false);

	struct GetNameCheck_ret2
	{
		GotNodeType Type;
		bool IsExpression = false;
	};
	GetNameCheck_ret2 GetNameCheck2(ScopedNameNode& out, bool CanHaveVarableName = false);

	GotNodeType GetType(TypeNode*& out, bool ignoreRighthandOFtype = false, bool ignoreleftHandType = true, bool CanUseInlineEnum = true);
	GotNodeType GetType(TypeNode& out, bool ignoreRighthandOFtype = false, bool ignoreleftHandType = true, bool CanUseInlineEnum = true);
	GotNodeType GetNumericType(TypeNode& out);


	GotNodeType GetExpressionNode(Unique_ptr<Node>& out);
	GotNodeType GetExpressionNode(ValueExpressionNode& out);

	GotNodeType GetExpressionTypeNode(Unique_ptr<Node>& out);
	GotNodeType GetExpressionTypeNode(ExpressionNodeType& out);


	TryGetNode GetAttribute()
	{
		auto V = AttributeNode::Gen();
		auto r = GetAttribute(*V);
		TrippedCheck(r);
		_TepAttributes.top().push_back(std::move(V));
		return { r,nullptr };
	}
	GotNodeType GetAttribute(AttributeNode& out);

	TryGetNode GetUseNode()
	{
		auto V = UsingNode::Gen();
		auto r = GetUseNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetUseNode(UsingNode& out);
	//Statements

	TryGetNode GetRetStatement()
	{
		auto V = RetStatementNode::Gen();
		auto r = GetRetStatement(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetRetStatement(RetStatementNode& out);

	TryGetNode GetDeclareStaticVariable()
	{
		auto V = DeclareStaticVariableNode::Gen();
		auto r = GetDeclareStaticVariable(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetDeclareStaticVariable(DeclareStaticVariableNode& out, bool ignoreleftHandType = false);
	TryGetNode GetDeclareThreadVariable()
	{
		auto V = DeclareThreadVariableNode::Gen();
		auto r = GetDeclareThreadVariable(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType  GetDeclareThreadVariable(DeclareThreadVariableNode& out, bool ignoreleftHandType = false);


	TryGetNode GetDeclareVariable()
	{
		auto V = DeclareVariableNode::Gen();
		auto r = GetDeclareVariable(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetDeclareVariable(DeclareVariableNode& out, bool ignoreleftHandType = false);



	TryGetNode GetAssignExpression()
	{
		auto V = AssignExpressionNode::Gen();
		auto r = GetAssignExpression(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetAssignExpression(AssignExpressionNode& out);

	TryGetNode GetPostfixStatement(bool DoSemicolon = true)
	{
		auto V = PostfixVariableNode::Gen();
		auto r = GetPostfixStatement(*V, DoSemicolon);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetPostfixStatement(PostfixVariableNode& out, bool DoSemicolon = true);

	TryGetNode GetCompoundStatement()
	{
		auto V = CompoundStatementNode::Gen();
		auto r = GetCompoundStatement(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetCompoundStatement(CompoundStatementNode& out);

	void GetDeclareVariableNoObject(TryGetNode& out);

	GotNodeType GetAlias(const Token* AliasName, GenericValuesNode&& AliasGenerics, AliasNode& out);

	TryGetNode GetIfNode()
	{
		auto V = IfNode::Gen();
		auto r = GetIfNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetIfNode(IfNode& out);

	TryGetNode GetWhileNode()
	{
		auto V = WhileNode::Gen();
		auto r = GetWhileNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetWhileNode(WhileNode& out);

	TryGetNode GetDoNode()
	{
		auto V = DoNode::Gen();
		auto r = GetDoNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetDoNode(DoNode& out);


	GotNodeType DoEnumType(EnumNode* output, const Token* ClassToken, GenericValuesNode& TepGenerics, InheritedTypeData& Inherited);
	GotNodeType GetEnumValueNode(EnumValueNode& out);

	GotNodeType DoTagType(TagTypeNode* output, const Token* ClassToken, GenericValuesNode& TepGenerics, InheritedTypeData& Inherited);


	TryGetNode GetFuncCallStatementNode()
	{
		auto V = FuncCallStatementNode::Gen();
		auto r = GetFuncCallStatementNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetFuncCallStatementNode(FuncCallStatementNode& out);

	TryGetNode GetFuncCallStatementNode2()
	{
		Unique_ptr<Node> V = nullptr;
		auto r = GetFuncCallStatementNode2(V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetFuncCallStatementNode2(Unique_ptr<Node>& out);

	TryGetNode GetFuncCallNode()
	{
		auto V = FuncCallNode::Gen();
		auto r = GetFuncCallNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetFuncCallNode(FuncCallNode& out);

	TryGetNode GetAnonymousObjectConstructorNode()
	{
		auto V = AnonymousObjectConstructorNode::Gen();
		auto r = GetAnonymousObjectConstructorNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetAnonymousObjectConstructorNode(AnonymousObjectConstructorNode& out);

	TryGetNode GetDropStatementNode()
	{
		auto V = DropStatementNode::Gen();
		auto r = GetDropStatementNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetDropStatementNode(DropStatementNode& out);

	TryGetNode GetNewExpresionNode()
	{
		auto V = NewExpresionNode::Gen();
		auto r = GetNewExpresionNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}

	GotNodeType GetNewExpresionNode(NewExpresionNode& out);

	TryGetNode GetimutVariableDeclare()
	{
		Unique_ptr<Node> V = nullptr;
		auto r = GetimutVariableDeclare(V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetimutVariableDeclare(Unique_ptr<Node>& out);


	TryGetNode GetForNode()
	{
		auto V = ForNode::Gen();
		auto r = GetForNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetForNode(ForNode& out);

	void ForNodeModernIntLoop(ForNode& out, const Token* OtherToken, const Token* NameToken, TokenType BinaryOp, TokenType PostFixOp);

	TryGetNode GetContinueNode()
	{
		auto V = ContinueNode::Gen();
		auto r = GetContinueNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetContinueNode(ContinueNode& out);

	TryGetNode GetBreakNode()
	{
		auto V = BreakNode::Gen();
		auto r = GetBreakNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetBreakNode(BreakNode& out);


	TryGetNode GetLambdaNode()
	{
		auto V = LambdaNode::Gen();
		auto r = GetLambdaNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetLambdaNode(LambdaNode& out);

	TryGetNode GetShortLambdaNode()
	{
		auto V = LambdaNode::Gen();
		auto r = GetShortLambdaNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetShortLambdaNode(LambdaNode& out);

	GotNodeType DoTraitType(TraitNode* output, const Token* ClassToken, GenericValuesNode& TepGenerics, InheritedTypeData& Inherited);


	void TraitAccessModifierInerScope(Vector< Unique_ptr<Node>>& Out);

	TryGetNode GetBitCastExpression()
	{
		auto V = BitCastExpression::Gen();
		auto r = GetBitCastExpression(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetBitCastExpression(BitCastExpression& out);


	TryGetNode GetPanicNode()
	{
		auto V = PanicNode::Gen();
		auto r = GetPanicNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetPanicNode(PanicNode& out);

	AccessModifierType GetModifier()
	{
		return _AccessModifier.size() ? _AccessModifier.top() : AccessModifierType::Default;
	}


	TryGetNode GetInvalidNode()
	{
		auto V = InvalidNode::Gen();
		auto r = GetInvalidNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetInvalidNode(InvalidNode& out);

	TryGetNode GetValidNode()
	{
		auto V = ValidNode::Gen();
		auto r = GetValidNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetValidNode(ValidNode& out);


	TryGetNode GeTypeExNode()
	{
		Unique_ptr<Node> V = nullptr;
		auto r = GeTypeExNode(V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GeTypeExNode(Unique_ptr<Node>& out);

	TryGetNode GetExpressionToTypeValue()
	{
		auto V = ExpressionToTypeValueNode::Gen();
		auto r = GetExpressionToTypeValue(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetExpressionToTypeValue(ExpressionToTypeValueNode& out);

	TryGetNode GetEvalDeclare()
	{
		Unique_ptr<Node> V = nullptr;
		auto r = GetEvalDeclare(V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetEvalDeclare(Unique_ptr<Node>& out);


	TryGetNode GetCompileTimeIf(bool IsInFunc = true)
	{
		auto V = CompileTimeIfNode::Gen();
		auto r = GetCompileTimeIf(*V, IsInFunc);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetCompileTimeIf(CompileTimeIfNode& out, bool IsInFunc = true);


	TryGetNode GetCompileTimeForNode()
	{
		auto V = CompileTimeForNode::Gen();
		auto r = GetCompileTimeForNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetCompileTimeForNode(CompileTimeForNode& out);

	void CompileTimeForNodeModernIntLoop(CompileTimeForNode& out, const Token* OtherToken, const Token* NameToken, TokenType BinaryOp, TokenType PostFixOp);


	TryGetNode GetMatchStatement()
	{
		auto V = MatchStatement::Gen();
		auto r = GetMatchStatement(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetMatchStatement(MatchStatement& out);

	TryGetNode GetMatchExpression()
	{
		auto V = MatchExpression::Gen();
		auto r = GetMatchExpression(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetMatchExpression(MatchExpression& out);

	TryGetNode GetUnqExpresionNode()
	{
		Unique_ptr<Node> V = nullptr;
		auto r = GetUnqExpresionNode(V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}

	GotNodeType GetUnqExpresionNode(Unique_ptr<Node>& out);

	void SmartPointerNewArray(TypeNode& TypeNode, const Token* Token,Unique_ptr<FuncCallNode>& OutNode, Unique_ptr<Node>& ArrayCountexpression, String_view SmartPointerName, String_view SmartPoinerMakeFunc);

	void SmartPointerNewToFuncName(TypeNode& TypeNode, ValueParametersNode& Pars, const Token* Token,Unique_ptr<FuncCallNode>& OutNode, String_view SmartPointerName, String_view SmartPoinerMakeFunc);

	TryGetNode GetShrExpresionNode()
	{
		Unique_ptr<Node> V = nullptr;
		auto r = GetShrExpresionNode(V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}

	GotNodeType GetShrExpresionNode(Unique_ptr<Node>& out);


	TryGetNode GetImportStatement()
	{
		auto V = ImportStatement::Gen();
		auto r = GetImportStatement(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetImportStatement(ImportStatement& out);

	TryGetNode GetAwaitExpresionNode()
	{
		auto V = AwaitExpression::Gen();
		auto r = GetAwaitExpresionNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}

	GotNodeType GetAwaitExpresionNode(AwaitExpression& out);

	TryGetNode GetAwaitStatementNode()
	{
		auto V = AwaitStatement::Gen();
		auto r = GetAwaitStatementNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}

	GotNodeType GetAwaitStatementNode(AwaitStatement& out);


	TryGetNode GetYieldExpresionNode()
	{
		auto V = YieldExpression::Gen();
		auto r = GetYieldExpresionNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}

	GotNodeType GetYieldExpresionNode(YieldExpression& out);

	TryGetNode GetYieldStatementNode()
	{
		auto V = YieldStatement::Gen();
		auto r = GetYieldStatementNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}

	GotNodeType GetYieldStatementNode(YieldStatement& out);


	TryGetNode GetUnsafeStatementNode()
	{
		auto V = UnsafeStatementsNode::Gen();
		auto r = GetUnsafeStatementNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}

	GotNodeType GetUnsafeStatementNode(UnsafeStatementsNode& out);


	TryGetNode GetUnsafeExpression()
	{
		auto V = UnsafeExpression::Gen();
		auto r = GetUnsafeExpression(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetUnsafeExpression(UnsafeExpression& out);


	TryGetNode GetDeferStatementNode()
	{
		auto V = DeferStatementNode::Gen();
		auto r = GetDeferStatementNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}

	GotNodeType GetDeferStatementNode(DeferStatementNode& out);

	TryGetNode GetForTypeNode()
	{
		auto V = ForTypeNode::Gen();
		auto r = GetForTypeNode(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}

	GotNodeType GetForTypeNode(ForTypeNode& out);

	TryGetNode GetRangeExpression()
	{
		auto V = FuncCallNode::Gen();
		auto r = GetRangeExpression(*V);
		TrippedCheck(r);
		return { r,std::move(V) };
	}
	GotNodeType GetRangeExpression(FuncCallNode& out);
};
UCodeLangFrontEnd

#endif
