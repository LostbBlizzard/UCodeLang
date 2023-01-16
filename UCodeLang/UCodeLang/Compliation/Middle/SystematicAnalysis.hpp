#pragma once
#include "UCodeLang/Compliation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"
#include "UCodeLang/Compliation/Back/BackEndInterface.hpp"
#include "Symbol.hpp"
UCodeLangStart

class SystematicAnalysis
{

public:
	SystematicAnalysis() {}
	~SystematicAnalysis() {}
	void Reset();
	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) { _Settings = V; }


	bool Analyze(const FileNode& File);
	bool Analyze(const Vector<FileNode*>& Files, const Vector<UClib*>& Libs);

	void BuildCode();

	UCodeLangForceinline UClib& Get_Output()
	{
		return _Lib;
	}
	UCodeLangForceinline void Set_BackEnd(const BackEndInterface* Value)
	{
		BackEnd = Value;
	}

	enum class PassType : UInt8
	{
		Null,
		GetTypes,
		FixedTypes,
		BuidCode,
		Done,
	};
	
private:
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	UClib _Lib;
	const BackEndInterface* BackEnd = nullptr;
	BackEndInterface::BackendPtr BackEndObject = nullptr;


	PassType passtype = PassType::Null;

	const Vector<FileNode*>* _Files = nullptr;
	const Vector<UClib*>* _Libs = nullptr;
	SymbolTable _Table;
	Stack<ClassData*> _ClassStack;
	Vector<const AttributeNode*> _TepAttributes;
	bool _InStatements = false;
	//
	IRBuilder _Builder;
	IRSeg _LastExpression;
	IRField _LastExpressionField =0;
	//
	Stack<TypeSymbol> LookingForTypes;
	TypeSymbol LastExpressionType;
	TypeSymbol& Get_LookingForType()
	{
		return LookingForTypes.top();
	}
	const Token* LastLookedAtToken = nullptr;
	//

	void Pass();
	void OnFileNode(UCodeLang::FileNode* const& File);
	void OnClassNode(const ClassNode& node);
	void OnAliasNode(const AliasNode& node);
	void OnUseingNode(const UsingNode& node);
	void OnFuncNode(const FuncNode& node);
	void OnRetStatement(const RetStatementNode& node);
	void OnEnum(const EnumNode& node);
	void OnNamespace(const NamespaceNode& node);
	void OnAttributeNode(const AttributeNode& node);
	void OnNonAttributeable(size_t Line, size_t Pos);
	String GetScopedNameAsString(const ScopedNameNode& node);
	void OnDeclareVariablenode(const DeclareVariableNode& node);
	void CantgussTypesTheresnoassignment(const UCodeLang::Token* Token);
	void OnAssignVariableNode(const AssignVariableNode& node);
	void OnPostfixVariableNode(const PostfixVariableNode& node);
	void OnCompoundStatementNode(const CompoundStatementNode& node);
	void OnExpressionTypeNode(const Node* node);
	void OnExpressionNode(const ValueExpressionNode& node);
	void OnExpressionNode(const BinaryExpressionNode& node);
	void OnExpressionNode(const CastNode& node);
	void OnFuncCallNode(const FuncCallNode& node);
	void LogCantFindFuncError(const UCodeLang::Token* Token, UCodeLang::String_view& FuncName);
	void OnDropStatementNode(const DropStatementNode& node);
	void CheckBackEnd();
	void PushTepAttributesInTo(Vector<AttributeData>& Input);
	void LoadLibSymbols();
	void LoadLibSymbols(const UClib& lib);
	UCodeLangForceinline auto OutputType()
	{
		return  _Settings->_Type;
	}
	UCodeLangForceinline auto OutputTypeAsLibType()
	{
		return   OutputType() == OutPutType::Lib ? LibType::Lib : LibType::Dll;
	}
	Symbol* GetSymbol(String_view Name, SymbolType Type);
	static String GetFuncAnonymousObjectFullName(const String& FullFuncName);
	void GetTypesClass(ClassData& data);


	bool AreTheSame(const TypeSymbol& TypeA, const TypeSymbol& TypeB);
	bool AreTheSameWithOutimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB);
	bool HasBinaryOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB);

	String ToString(const TypeSymbol& Type);
	String ToString(const TokenType& Type)
	{
		return StringHelper::ToString(Type);
	}
	void Convert(const TypeNode& V, TypeSymbol& Out);
	
	bool IsVaidType(TypeSymbol& Out);
	bool CanBeImplicitConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);
	bool CanBeExplicitlyConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);

	bool IsSIntType(const UCodeLang::TypeSymbol& TypeToCheck);
	bool IsUIntType(const UCodeLang::TypeSymbol& TypeToCheck);
	bool IsIntType(const UCodeLang::TypeSymbol& TypeToCheck)
	{
		return  IsSIntType(TypeToCheck) || IsUIntType(TypeToCheck);
	}


	bool IsimmutableRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);

	bool IsAddessAndLValuesRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);
	//Generic
	struct GenericFuncInfo
	{
		String GenericFuncName;
		const Vector<TypeSymbol>* TypeToChage=nullptr;
		const Vector<TypeSymbol>* Type = nullptr;
	};

	Stack<GenericFuncInfo> GenericFuncName;
	
	bool GetSize(TypeSymbol& Type,UAddress& OutSize);

	void GenericFuncInstantiate(Symbol* Func,const FuncNode& FuncBase,
		const Vector<TypeSymbol>& TypeToChage, 
		const Vector<TypeSymbol>& Type);
	String GetGenericFuncName(Symbol* Func, const Vector<TypeSymbol>& Type);

	void GenericTypeInstantiate(Symbol* Class , const Vector<TypeSymbol>& Type);

	unordered_map<const FuncCallNode*, SymbolID> FuncToSyboID;

	void Build_Assign_uIntPtr(UAddress Value)
	{
		_Builder.Build_Assign(IROperand::AsInt64(Value));
	}
	void Build_Assign_sIntPtr(SIntNative Value)
	{
		_Builder.Build_Assign(IROperand::AsInt64(Value));
	}
	void Build_Add_uIntPtr(IROperand field, IROperand field2)
	{
		_Builder.MakeAdd64(field, field2);
	}
	void Build_Sub_uIntPtr(IROperand field, IROperand field2)
	{
		_Builder.MakeSub64(field, field2);
	}
	void Build_Mult_uIntPtr(IROperand field, IROperand field2)
	{
		_Builder.MakeUMult64(field, field2);
	}
	
	
	void Build_Increment_uIntPtr(UAddress Value)
	{
		_Builder.Build_Increment64(Value);
	}
	void Build_Decrement_uIntPtr(UAddress Value)
	{
		_Builder.Build_Decrement64(Value);
	}

	void Build_Increment_sIntPtr(SIntNative Value)
	{
		_Builder.Build_Increment64(Value);
	}
	void Build_Decrement_sIntPtr(SIntNative Value)
	{
		_Builder.Build_Decrement64(Value);
	}
	
	//Errors
	void LogCantFindBinaryOpForTypes(const UCodeLang::Token* BinaryOp, UCodeLang::TypeSymbol& Ex0Type, UCodeLang::TypeSymbol& Ex1Type);
	void ExpressionMustbeAnLocationValueError(const UCodeLang::Token* Token, UCodeLang::TypeSymbol& Ex0Type);
	void YouMustReturnSomethingError(const UCodeLang::Token* Token);
	void CantguessVarTypeError(const UCodeLang::Token* Token);
	void CantUseThisKeyWordHereError(const UCodeLang::Token* NameToken);
	void LogCantCastImplicitTypes(const UCodeLang::Token* Token, UCodeLang::TypeSymbol& Ex1Type, UCodeLang::TypeSymbol& UintptrType);
	void LogReadingFromInvaidVariable(const UCodeLang::Token* Token, UCodeLang::String& Str);
	void LogCantFindVarError(const UCodeLang::Token* Token, UCodeLang::String& Str);
	void LogCantModifyiMutableError(const UCodeLang::Token* Token, UCodeLang::String_view& Name);
	void LogCantCastExplicityTypes(const UCodeLang::Token* Token, UCodeLang::TypeSymbol& Ex0Type, UCodeLang::TypeSymbol& ToTypeAs);
	void LogCantFindTypeError(const UCodeLang::Token* Token, UCodeLang::String_view& Name);
	
	struct ReadVarErrorCheck_t
	{
		bool CantFindVar = false;
		bool VarIsInvalid = false;
	};
	ReadVarErrorCheck_t LogTryReadVar(String_view VarName, const UCodeLang::Token* Token, const Symbol* Syb);
	void CheckVarWritingErrors(UCodeLang::Symbol* Symbol, const UCodeLang::Token* Token, UCodeLang::String_view& Name);
public://Only for backends
		
		UAddress GetTypeSize(TypeSymbol& Type)
		{
			UAddress r;
			GetSize(Type, r);
			return r;
		}
};
UCodeLangEnd

