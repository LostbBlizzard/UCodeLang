#pragma once
#include "UCodeLang/Compliation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"


#include "../Middle/IR.hpp"
#include "UCodeLang/Compliation/Back/BackEndInterface.hpp"
#include "../Middle/Symbol.hpp"
#include "UCodeFrontEndNameSpace.hpp"
UCodeLangFrontStart
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
	UCodeLangForceinline UClib& Get_Lib()
	{
		return _Lib;
	}

	enum class PassType : UInt8
	{
		Null,
		GetTypes,
		FixedTypes,
		BuidCode,
		Done,
	};

	struct FileNodeData
	{
		Vector< const FileNode_t*> _Dependencys;
	};
	const FileNodeData& GetFileData(const FileNode_t* file) const { return _FilesData.at(file); }
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
	//IR Building
	IRBuilder _Builder;
	IRInstruction* _LastExpressionField =0;
	IRFunc* LookingAtIRFunc = nullptr;
	IRBlock* LookingAtIRBlock=nullptr;

	
	
	Vector<FuncPtrInfo> Tep_FuncPtr;


	enum class  ObjectToDropType
	{
		IRInstruction,
		Operator,
	};
	struct ObjectToDrop
	{
		ObjectToDropType DropType= ObjectToDropType::IRInstruction;
		SymbolID ID = 0;


		IROperator _Operator;
		IRInstruction* _Object = nullptr;
		TypeSymbol Type;

	};
	
	struct IRCodeStackFrames
	{
		Vector< ObjectToDrop> OnEndStackFrame;


	};

	Stack<IRInstruction*> IRlocations;//for Constructors
	Vector<IRCodeStackFrames> StackFrames;	
	//

	IRType ConvertToIR(const TypeSymbol& Value);


	Stack<TypeSymbol> LookingForTypes;
	TypeSymbol LastExpressionType;
	TypeSymbol& Get_LookingForType()
	{
		return LookingForTypes.top();
	}
	const Token* LastLookedAtToken = nullptr;
	//
	
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
	//File dependency analysis stuff
	const FileNode* LookingAtFile = nullptr;


	Unordered_map< const FileNode_t*, FileNodeData> _FilesData;


	const FileNode* Get_FileUseingSybol(Symbol* Syb);
	void AddDependencyToCurrentFile(Symbol* Syb);
	void AddDependencyToCurrentFile(const FileNode* file);
	void AddDependencyToCurrentFile(const TypeSymbol& type);
	Symbol& AddSybol(SymbolType type, const String& Name, const String& FullName);
	
	//
	void Pass();
	void OnFileNode(const FileNode* File);
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
	void OnDeclareStaticVariableNode(const DeclareStaticVariableNode& node);
	void OnDeclareThreadVariableNode(const DeclareThreadVariableNode& node);

	struct GetMemberTypeSymbolFromVar_t
	{
		TypeSymbol Type;
	};

	bool GetMemberTypeSymbolFromVar(const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out);

	Symbol* SystematicAnalysis::GetTepFuncPtrSyb(const String& TepFuncPtr, const FuncInfo* Finfo);

	String GetTepFuncPtrName(Symbol* SymbolVar);

	bool GetMemberTypeSymbolFromVar(const size_t& Start, const ScopedNameNode& node, TypeSymbol& FeildType, Symbol*& FeildTypeAsSymbol);
	
	void OnPostfixVariableNode(const PostfixVariableNode& node);
	void OnCompoundStatementNode(const CompoundStatementNode& node);
	void OnExpressionTypeNode(const Node* node);
	void OnExpressionNode(const ValueExpressionNode& node);
	void OnNewNode(NewExpresionNode* nod);
	void OnAnonymousObjectConstructor(AnonymousObjectConstructorNode*& nod);
	void OnReadVariable(const ReadVariableNode& nod);

	Byte OperatorPrecedenceValue(const Node* node);
	Byte OperatorPrecedence(TokenType V);
	bool SwapForOperatorPrecedence(const Node* nodeA, const Node* nodeB);

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
	ReflectionTypeInfo ConvertToTypeInfo(const TypeSymbol& Type);
	TypeSymbol Convert(const ReflectionTypeInfo& Type);

	bool AreTheSame(const TypeSymbol& TypeA, const TypeSymbol& TypeB);
	bool AreTheSameWithOutimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB);

	bool HasBinaryOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB);
	bool HasCompoundOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB);
	bool HasPostfixOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp);

	TypeSymbol BinaryExpressionShouldRurn(TokenType Op, const TypeSymbol& Ex0Type);

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

	bool DoImplicitConversion(IRInstruction* Ex,const TypeSymbol ExType, const TypeSymbol& ToType);
	void DoExplicitlConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType);

	bool IsSIntType(const TypeSymbol& TypeToCheck);
	bool IsUIntType(const TypeSymbol& TypeToCheck);
	bool IsIntType(const TypeSymbol& TypeToCheck)
	{
		return  IsSIntType(TypeToCheck) || IsUIntType(TypeToCheck);
	}
	bool IsfloatType(const TypeSymbol& TypeToCheck);
	bool IsPrimitive(const TypeSymbol& TypeToCheck);

	bool IsimmutableRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);

	bool IsAddessAndLValuesRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);
	
	bool HasDestructor(const TypeSymbol& TypeToCheck);
	//Generic
	struct GenericFuncInfo
	{
		String GenericFuncName;
		const Vector<TypeSymbol>* GenericInput = nullptr;
		const void* NodeTarget = nullptr;
	};

	Stack<GenericFuncInfo> GenericFuncName;
	
	bool GetSize(const TypeSymbol& Type,UAddress& OutSize);
	bool GetOffset(const ClassInfo& Type,const FieldInfo* Field, UAddress& OutOffset);

	struct Get_FuncInfo
	{
		enum class ThisPar_t : UInt8
		{
			NoThisPar,
			PushFromLast,
			PushFromScopedName,
			OnIRlocationStack,
			PushWasCalled,
		};
		ThisPar_t ThisPar = ThisPar_t::NoThisPar;
		const FuncInfo* Func = nullptr;
	};

	Get_FuncInfo GetFunc(
		const TypeSymbol& Name,
		const ValueParametersNode& Pars);;

	Get_FuncInfo GetFunc(
		const ScopedNameNode& Name,
		const ValueParametersNode& Pars,
		TypeSymbol Ret)
	{
		UseGenericsNode generics;
		return GetFunc(Name, generics, Pars, Ret);
	};

	Get_FuncInfo GetFunc(
		const ScopedNameNode& Name,
		const UseGenericsNode&,
		const ValueParametersNode& Pars,
		TypeSymbol Ret);
	//Generics
	void GetScopedNameRemovedLast(const ScopedNameNode& Name, ScopedNameNode& TepNode);

	void GenericFuncInstantiate(Symbol* Func,const Vector<TypeSymbol>& GenericInput);

	String GetGenericFuncName(Symbol* Func, const Vector<TypeSymbol>& Type);

	void GenericTypeInstantiate(Symbol* Class , const Vector<TypeSymbol>& Type);

	Unordered_map<const void*, Get_FuncInfo> FuncToSyboID;
	//
	struct EvaluatedEx
	{
		TypeSymbol Type;
		RawEvaluatedObject EvaluatedObject;
		bool HasValue()
		{
			return EvaluatedObject.HasValue();
		}
	};
	EvaluatedEx MakeEx(const TypeSymbol& Type);
	RawEvaluatedObject MakeExr(const TypeSymbol& Type);
	void* Get_Object(const TypeSymbol& Input, const RawEvaluatedObject& Input2);
	void* Get_Object(const EvaluatedEx& Input);

	bool ConstantExpressionAbleType(const TypeSymbol& Type);

	
	bool EvaluateDefaultConstructor(EvaluatedEx& Out);
	bool Evaluate(EvaluatedEx& Out, const ValueExpressionNode& node);
	bool Evaluate(EvaluatedEx& Out, const BinaryExpressionNode& node);
	bool Evaluate(EvaluatedEx& Out, const CastNode& node);
	bool Evaluate_t(EvaluatedEx& Out,const Node* node);

	bool EvaluatePostfixOperator(EvaluatedEx& Out,TokenType Op);
	bool HasConstantPostfixOperator(const TypeSymbol& Type, TokenType Op);

	bool CanEvaluateImplicitConversionConstant(const TypeSymbol& Type, const TypeSymbol& ToType);
	bool EvaluateImplicitConversion(EvaluatedEx& In, const TypeSymbol& ToType, EvaluatedEx& out);
	
	//IR
	void DoFuncCall(Get_FuncInfo Func, const ScopedNameNode& Name, const ValueParametersNode& Pars);
	void DoFuncCall(const TypeSymbol& Type, const Get_FuncInfo& Func, ValueParametersNode& ValuePars);
	

	void DoDestructorCall(const ObjectToDrop& Object);


	
	IRInstruction* IR_Load_UIntptr(UAddress Value);
	IRInstruction* IR_Load_SIntptr(SIntNative Value);
	IRInstruction* Build_Add_uIntPtr(IROperator field, IROperator field2);
	IRInstruction* Build_Sub_uIntPtr(IROperator field, IROperator field2);
	IRInstruction* Build_Add_sIntPtr(IROperator field, IROperator field2);
	IRInstruction* Build_Sub_sIntPtr(IROperator field, IROperator field2);
	IRInstruction* Build_Mult_uIntPtr(IROperator field, IROperator field2);
	IRInstruction* Build_Mult_sIntPtr(IROperator field, IROperator field2);
	IRInstruction* Build_Div_uIntPtr(IROperator field, IROperator field2);
	IRInstruction* Build_Div_sIntPtr(IROperator field, IROperator field2);
	IRInstruction* Build_Increment_uIntPtr(UAddress Value);
	IRInstruction* Build_Decrement_uIntPtr(UAddress Value);
	IRInstruction* Build_Increment_sIntPtr(SIntNative Value);
	IRInstruction* Build_Decrement_sIntPtr(SIntNative Value);
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

	void LogTypeMustBeAnConstantExpressionAble(const Token* Token, const TypeSymbol& Type);
	void LogCantFindPostfixOpForTypes_Constant(const Token* BinaryOp, TypeSymbol& Ex0Type);
	void LogCantDoPostfixOpForTypes_Constant(const Token* BinaryOp, TypeSymbol& Ex0Type);
	void LogCantCastImplicitTypes_Constant(const Token* Token, TypeSymbol& Ex1Type, TypeSymbol& UintptrType);
	void LogCantFindNamespace(const Token* Token, const String_view Namespace);
		
	struct ReadVarErrorCheck_t
	{
		bool CantFindVar = false;
		bool VarIsInvalid = false;
	};
	ReadVarErrorCheck_t LogTryReadVar(String_view VarName, const Token* Token, const Symbol* Syb);
	void CheckVarWritingErrors(Symbol* Symbol, const Token* Token, String_view& Name);

};
UCodeLangFrontEnd


