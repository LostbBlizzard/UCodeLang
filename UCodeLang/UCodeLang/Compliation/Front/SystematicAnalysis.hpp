#pragma once
#include "UCodeLang/Compliation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"
#include "UCodeLang/Compliation/Back/BackEndInterface.hpp"
#include "../Middle/Symbol.hpp"
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
	bool Analyze(const Vector<const FileNode*>& Files, const Vector<const UClib*>& Libs);

	bool Analyze(const Vector<Unique_ptr<FileNode>>& Files, const Vector<Unique_ptr<UClib>>& Libs);

	void BuildCode();

	
	UCodeLangForceinline IRBuilder& Get_Output()
	{
		return _Builder;
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


	PassType passtype = PassType::Null;

	const Vector<const FileNode*>* _Files = nullptr;
	const Vector<const UClib*>* _Libs = nullptr;
	SymbolTable _Table;
	Stack<ClassInfo*> _ClassStack;
	Stack<FuncInfo*> _FuncStack;
	Vector<const AttributeNode*> _TepAttributes;
	bool _InStatements = false;
	//
	IRBuilder _Builder;
	IRSeg _LastExpression;
	IRField _LastExpressionField =0;
	//

	Vector<SymbolID> IRParameters;



	Stack<TypeSymbol> LookingForTypes;
	TypeSymbol LastExpressionType;
	TypeSymbol& Get_LookingForType()
	{
		return LookingForTypes.top();
	}
	const Token* LastLookedAtToken = nullptr;
	//
	struct ObjectToDrop
	{
		IRField Object;
		TypeSymbol Type;
	};
	
	struct IRCodeStackFrames
	{
		Vector< ObjectToDrop> OnEndStackFrame;


	};
	Vector<IRCodeStackFrames> StackFrames;
	void PushNewStackFrame();
	void PopStackFrame();

	struct NewFuncData
	{
		Unique_ptr<Vector<TypeSymbol>> Pointer;
	};
	Vector< NewFuncData> TepFuncs;
	Vector<const ClassInfo*> ClassDependencies;
	inline bool IsDependencies(const ClassInfo* Value)
	{
		for (auto Item : ClassDependencies)
		{
			if (Item == Value)
			{
				return true;
			}
		}
		return false;
	}
	inline void PushClassDependencie(const ClassInfo* Value)
	{
		ClassDependencies.push_back(Value);
	}
	inline void PopClassDependencie()
	{
		ClassDependencies.pop_back();
	}

	SymbolID GetSymbolID(const Node& node);

	void Pass();
	void OnFileNode(const FileNode* const& File);
	void OnClassNode(const ClassNode& node);
	void OnAliasNode(const AliasNode& node);
	void OnUseingNode(const UsingNode& node);
	void OnFuncNode(const FuncNode& node);
	void OnStatement(const Unique_ptr<UCodeLang::Node>& node2);
	void OnRetStatement(const RetStatementNode& node);
	void OnEnum(const EnumNode& node);
	void OnNamespace(const NamespaceNode& node);
	void OnAttributeNode(const AttributeNode& node);
	void OnNonAttributeable(size_t Line, size_t Pos);
	String GetScopedNameAsString(const ScopedNameNode& node);
	void OnDeclareVariablenode(const DeclareVariableNode& node);
	void CantgussTypesTheresnoassignment(const Token* Token);
	void OnAssignVariableNode(const AssignVariableNode& node);
	void OnIfNode(const IfNode& node);
	void OnWhileNode(const WhileNode& node);
	void OnDoNode(const DoNode& node);
		
	struct GetMemberTypeSymbolFromVar_t
	{
		UAddress Offset =NullAddress;
		TypeSymbol Type;
	};

	bool GetMemberTypeSymbolFromVar(const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out);
	
	void OnPostfixVariableNode(const PostfixVariableNode& node);
	void OnCompoundStatementNode(const CompoundStatementNode& node);
	void OnExpressionTypeNode(const Node* node);
	void OnExpressionNode(const ValueExpressionNode& node);
	void OnNewNode(NewExpresionNode* nod);
	void OnAnonymousObjectConstructor(AnonymousObjectConstructorNode*& nod);
	void DoFuncCall(TypeSymbol& Type, const const UCodeLang::FuncInfo*& Func,ValueParametersNode& ValuePars);
	void OnReadVariable(const ReadVariableNode& nod);
	void BindTypeToLastIR(TypeSymbol& Type);
	void OnExpressionNode(const BinaryExpressionNode& node);
	void OnExpressionNode(const CastNode& node);
	void OnFuncCallNode(const FuncCallNode& node);
	void OnDropStatementNode(const DropStatementNode& node);
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
	void AddClass_tToAssemblyInfo(const ClassInfo* data);


	bool AreTheSame(const TypeSymbol& TypeA, const TypeSymbol& TypeB);
	bool AreTheSameWithOutimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB);

	bool HasBinaryOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB);
	bool HasCompoundOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB);
	bool HasPostfixOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp);


	String ToString(const TypeSymbol& Type);
	String ToString(const TokenType& Type)
	{
		return StringHelper::ToString(Type);
	}
	

	void Convert(const TypeNode& V, TypeSymbol& Out);
	
	Symbol* GetSymbol(const ClassInfo* Info)
	{
		return GetSymbol(Info->FullName,SymbolType::Type_class);
	}
	Symbol* GetSymbol(const FuncInfo* Info)
	{
		return GetSymbol(Info->FullName, SymbolType::Func);
	}
	
	bool IsVaidType(TypeSymbol& Out);
	bool CanBeImplicitConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);
	bool CanBeExplicitlyConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);

	bool DoImplicitConversion(IROperand Ex,const TypeSymbol ExType, const TypeSymbol& ToType);
	void DoExplicitlConversion(IROperand Ex, const TypeSymbol ExType, const TypeSymbol& ToType);

	bool IsSIntType(const TypeSymbol& TypeToCheck);
	bool IsUIntType(const TypeSymbol& TypeToCheck);
	bool IsIntType(const TypeSymbol& TypeToCheck)
	{
		return  IsSIntType(TypeToCheck) || IsUIntType(TypeToCheck);
	}
	bool IsPrimitive(const TypeSymbol& TypeToCheck);

	bool IsimmutableRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);

	bool IsAddessAndLValuesRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);
	
	bool HasDestructor(const TypeSymbol& TypeToCheck);
	//Generic
	struct GenericFuncInfo
	{
		String GenericFuncName;
		const Vector<TypeSymbol>* GenericInput = nullptr;
		void* NodeTarget = nullptr;
	};

	Stack<GenericFuncInfo> GenericFuncName;
	
	bool GetSize(const TypeSymbol& Type,UAddress& OutSize);
	bool GetOffset(const ClassInfo& Type,const FieldInfo* Field, UAddress& OutOffset);


	void DoFuncCall(const FuncInfo* Func, const ScopedNameNode& Name, const ValueParametersNode& Pars);

	void DoDestructorCall(const ObjectToDrop& Object);

	FuncInfo* GetFunc(
		const TypeSymbol& Name,
		const ValueParametersNode& Pars);;

	FuncInfo* GetFunc(
		const ScopedNameNode& Name,
		const ValueParametersNode& Pars,
		TypeSymbol Ret)
	{
		UseGenericsNode generics;
		return GetFunc(Name, generics, Pars, Ret);
	};

	FuncInfo* GetFunc(
		const ScopedNameNode& Name,
		const UseGenericsNode&,
		const ValueParametersNode& Pars,
		TypeSymbol Ret);

	void GenericFuncInstantiate(Symbol* Func,const Vector<TypeSymbol>& GenericInput);

	String GetGenericFuncName(Symbol* Func, const Vector<TypeSymbol>& Type);

	void GenericTypeInstantiate(Symbol* Class , const Vector<TypeSymbol>& Type);

	Unordered_map<const void*,const FuncInfo*> FuncToSyboID;


	//uintptr
	void Build_Assign_uIntPtr(UAddress Value);
	void Build_Assign_sIntPtr(SIntNative Value);
	void Build_Add_uIntPtr(IROperand field, IROperand field2);
	void Build_Sub_uIntPtr(IROperand field, IROperand field2);
	void Build_Add_sIntPtr(IROperand field, IROperand field2);
	void Build_Sub_sIntPtr(IROperand field, IROperand field2);
	void Build_Mult_uIntPtr(IROperand field, IROperand field2);
	void Build_Mult_sIntPtr(IROperand field, IROperand field2);
	void Build_Div_uIntPtr(IROperand field, IROperand field2);
	void Build_Div_sIntPtr(IROperand field, IROperand field2);
	void Build_Increment_uIntPtr(UAddress Value);
	void Build_Decrement_uIntPtr(UAddress Value);
	void Build_Increment_sIntPtr(SIntNative Value);
	void Build_Decrement_sIntPtr(SIntNative Value);
	

	//Errors

	void LogCantFindCompoundOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type);
	void LogCantFindPostfixOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type);
	void LogCantFindBinaryOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type);

	void ExpressionMustbeAnLocationValueError(const Token* Token, TypeSymbol& Ex0Type);
	void YouMustReturnSomethingError(const Token* Token);
	void CantguessVarTypeError(const Token* Token);
	void CantUseThisKeyWordHereError(const Token* NameToken);
	void LogCantCastImplicitTypes(const Token* Token, TypeSymbol& Ex1Type, TypeSymbol& UintptrType);
	void LogReadingFromInvaidVariable(const Token* Token, String_view Str);
	void LogCantFindVarError(const Token* Token, String_view Str);
	void LogCantFindVarMemberError(const Token* Token, String_view Str,const TypeSymbol& OnType);
	void LogCantModifyiMutableError(const Token* Token, String_view Name);
	void LogCantCastExplicityTypes(const Token* Token, TypeSymbol& Ex0Type, TypeSymbol& ToTypeAs);
	void LogCantFindTypeError(const Token* Token, String_view Name);
	void LogTypeDependencyCycle(const Token* Token, const ClassInfo* Value);
	void LogCantUseThisHere(const Token* Token);

	void LogCanIncorrectParCount(const Token* Token, String_view FuncName, size_t Count, size_t FuncCount);
	void LogCanIncorrectGenericCount(const Token* Token, String_view FuncName, size_t Count, size_t FuncCount);
	
	void LogCantFindFuncError(const Token* Token, String_view FuncName,
		const Vector<TypeSymbol>& Generics,
		const Vector<TypeSymbol>& WithTypes,
	    const TypeSymbol& RetType);

	struct ReadVarErrorCheck_t
	{
		bool CantFindVar = false;
		bool VarIsInvalid = false;
	};
	ReadVarErrorCheck_t LogTryReadVar(String_view VarName, const Token* Token, const Symbol* Syb);
	void CheckVarWritingErrors(Symbol* Symbol, const Token* Token, String_view& Name);
public://Only for backends
		
		UAddress GetTypeSize(const TypeSymbol& Type)
		{
			UAddress r;
			GetSize(Type, r);
			return r;
		}
};
UCodeLangEnd

