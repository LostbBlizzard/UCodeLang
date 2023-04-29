#pragma once
#include "UCodeLang/Compliation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"


#include "../Middle/IR.hpp"
#include "UCodeLang/Compliation/Back/BackEndInterface.hpp"
#include "../Middle/Symbol.hpp"
#include "UCodeFrontEndNameSpace.hpp"
UCodeLangFrontStart


struct Systematic_BinaryOverloadData
{
public:
	struct Data
	{
		TokenType token;
		String CompilerName;
		FuncInfo::FuncType Type;
		Data(TokenType t, String compilerName, FuncInfo::FuncType f)
			:token(t), CompilerName(compilerName),Type(f)
		{

		}
	};
	inline static const Array<Data,2> Data =
	{
		Data(TokenType::plus,Overload_Plus_Func,FuncInfo::FuncType::plus),
		Data(TokenType::minus,Overload_minus_Func,FuncInfo::FuncType::minus),
	};
	static bool IsBinaryOverload(FuncInfo::FuncType Type)
	{
		for (auto& Item : Data)
		{
			if (Item.Type == Type)
			{
				return true;
			}
		}
		return false;
	}
};

struct Systematic_PostfixOverloadData
{
public:
	struct Data
	{
		TokenType token;
		String CompilerName;
		FuncInfo::FuncType Type;
		Data(TokenType t, String compilerName, FuncInfo::FuncType f)
			:token(t), CompilerName(compilerName), Type(f)
		{

		}
	};
	inline static const Array<Data, 2> Data =
	{
		Data(TokenType::increment,Overload_increment_Func,FuncInfo::FuncType::increment),
		Data(TokenType::decrement,Overload_decrement_Func,FuncInfo::FuncType::decrement),
	};
	static bool IsPostfixOverload(FuncInfo::FuncType Type)
	{
		for (auto& Item : Data)
		{
			if (Item.Type == Type)
			{
				return true;
			}
		}
		return false;
	}
};

struct Systematic_UrinaryOverloadData
{
public:
	struct Data
	{
		TokenType token;
		String CompilerName;
		FuncInfo::FuncType Type;
		Data(TokenType t, String compilerName, FuncInfo::FuncType f)
			:token(t), CompilerName(compilerName), Type(f)
		{

		}
	};
	inline static const Array<Data, 3> Data =
	{
		Data(TokenType::Not,Overload_Not_Func ,FuncInfo::FuncType::Not),
		Data(TokenType::bitwise_not,Overload_Bitwise_Not  ,FuncInfo::FuncType::Not),
		Data(TokenType::QuestionMark, Overload_exist_Func,FuncInfo::FuncType::Not),
	};
	static bool IsUrinaryOverload(FuncInfo::FuncType Type)
	{
		for (auto& Item : Data)
		{
			if (Item.Type == Type)
			{
				return true;
			}
		}
		return false;
	}
};

struct Systematic_CompoundOverloadData
{
public:
	struct Data
	{
		TokenType token;
		String CompilerName;
		FuncInfo::FuncType Type;
		Data(TokenType t, String compilerName, FuncInfo::FuncType f)
			:token(t), CompilerName(compilerName), Type(f)
		{

		}
	};
	inline static const Array<Data, 2> Data =
	{
		Data(TokenType::CompoundAdd,Overload_Compoundplus_Func ,FuncInfo::FuncType::Compound_plus),
		Data(TokenType::CompoundSub,Overload_CompoundSub_Func,FuncInfo::FuncType::Compound_Minus),
	};
	static bool IsCompoundOverload(FuncInfo::FuncType Type)
	{
		for (auto& Item : Data)
		{
			if (Item.Type == Type)
			{
				return true;
			}
		}
		return false;
	}
};

struct Systematic_MemberOverloadData
{
public:
	struct Data_t
	{
		TokenType token;
		String CompilerName;
		FuncInfo::FuncType Type;
		Data_t(TokenType t, String compilerName, FuncInfo::FuncType f)
			:token(t), CompilerName(compilerName), Type(f)
		{

		}
	};
	inline static const Array<Data_t, 3> Data =
	{
		Data_t(TokenType::IndirectMember,Overload_IndirectMember_Func ,FuncInfo::FuncType::IndirectMember),
		Data_t(TokenType::OptionalDot, Overload_OptionalDot_Func,FuncInfo::FuncType::OptionalDot),
		Data_t(TokenType::ExclamationDot, Overload_ExclamationDot_Func,FuncInfo::FuncType::ExclamationDot),
	};
	static bool IsMemerOverload(FuncInfo::FuncType Type)
	{
		for (auto& Item : Data)
		{
			if (Item.Type == Type)
			{
				return true;
			}
		}
		return false;
	}

	static Optional<const Data_t*> GetOverloadData(TokenType type)
	{
		for (auto& Item : Data)
		{
			if (Item.token== type)
			{
				return { &Item };
			}
		}
		return {};
	}
	static Optional<const Data_t*> GetOverloadData(ScopedName::Operator_t type)
	{
		return GetOverloadData(To(type));
	}

	static TokenType To(ScopedName::Operator_t type)
	{
		switch (type)
		{
		case ScopedName::Operator_t::IndirectMember:return TokenType::IndirectMember;
		case ScopedName::Operator_t::OptionalChain:return TokenType::OptionalDot;
		case ScopedName::Operator_t::ExclamationMember:return TokenType::ExclamationDot;
		default:return TokenType::Null;
		}
	}
};

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



	struct FileNodeData
	{
		Vector< const FileNode_t*> _Dependencys;
	};
	const FileNodeData& GetFileData(const FileNode_t* file) const { return _FilesData.at(file); }
private:
	//Types
	struct BinaryExpressionNode_Data
	{
		TypeSymbol Op0;
		TypeSymbol Op1;

		//func to call
		Symbol* FuncToCall = nullptr;
		bool IsBuitIn()
		{
			return  FuncToCall == nullptr;
		}
	};
	using IndexedExpresion_Data = BinaryExpressionNode_Data;
	struct PostFixExpressionNode_Data
	{
		TypeSymbol Op0;

		//func to call
		Symbol* FuncToCall = nullptr;
		bool IsBuitIn()
		{
			return  FuncToCall == nullptr;
		}
	};
	using CompoundExpresion_Data = BinaryExpressionNode_Data;
	struct ForExpresion_Data
	{
		Symbol* FuncGetLoopAble = nullptr;
		Symbol* FuncToGet = nullptr;
		Symbol* FuncToCheck = nullptr;
	};
	using CastExpressionNode_Data = PostFixExpressionNode_Data;
	using AssignExpression_Data = BinaryExpressionNode_Data;

	enum class  ObjectToDropType
	{
		IRInstruction,
		IRInstructionNoMod,
		Operator,
	};
	struct ObjectToDrop
	{
		ObjectToDropType DropType = ObjectToDropType::IRInstruction;
		SymbolID ID = 0;


		IROperator _Operator;
		IRInstruction* _Object = nullptr;
		TypeSymbol Type;

	};
	struct NewFuncData
	{
		Unique_ptr<Vector<TypeSymbol>> Pointer;
	};
	struct IRCodeStackFrames
	{
		Vector< ObjectToDrop> OnEndStackFrame;


	};
	struct FuncStackInfo
	{
		FuncInfo* Pointer = nullptr;
		bool IsOnRetStatemnt = false;
		FuncStackInfo(FuncInfo* V)
		{
			Pointer = V;
		}
	};
	struct IRLocation_Cotr
	{
		IRInstruction* Value = nullptr;
		bool UsedlocationIR = false;
	};
	struct GetMemberTypeSymbolFromVar_t
	{
		Symbol* Symbol = nullptr;
		TypeSymbol Type;

		//state
		void* V1 = nullptr;
		void* V2 = nullptr;

		const ScopedName* Start = nullptr;
		size_t End = 0;

		template<typename T> void Set_V1(T* Value)
		{
			V1 = Value;
		}
		template<typename T>  T* Get_V1() const
		{
			return (T*)V1;
		}
		template<typename T>  void Set_V2(T* Value)
		{
			V2 = Value;
		}
		template<typename T> T* Get_V2() const
		{
			return (T*)V2;
		}
	};
	enum class GetValueMode
	{
		Read,
		Write,
		ReadAndWrite,
	};

	struct Get_FuncInfo
	{
		enum class ThisPar_t : UInt8
		{
			NoThisPar,
			PushFromLast,
			PushFromScopedName,
			FullScopedName,
			OnIRlocationStack,
			OnIRlocationStackNonedef,
			PushWasCalled,
			AutoPushThis,
			PushFromScopedNameDynamicTrait,
			NoThisPar_GetValue_EnumVariant,
		};
		ThisPar_t ThisPar = ThisPar_t::NoThisPar;


		const FuncInfo* Func = nullptr;
		Symbol* SymFunc = nullptr;
	};
	struct ReadVarErrorCheck_t
	{
		bool CantFindVar = false;
		bool VarIsInvalid = false;
	};
	struct EvaluatedEx
	{
		TypeSymbol Type;
		RawEvaluatedObject EvaluatedObject;
		bool HasValue()
		{
			return EvaluatedObject.HasValue();
		}
	};
	struct GenericFuncInfo
	{
		String GenericFuncName;
		const Vector<TypeSymbol>* GenericInput = nullptr;
		const void* NodeTarget = nullptr;
	};
	struct BinaryOverLoadWith_t
	{
		bool HasValue = false;
		Optional<Symbol*> Value;
	};
	using IndexOverLoadWith_t = BinaryOverLoadWith_t;
	using PostFixOverLoadWith_t = BinaryOverLoadWith_t;
	using CompoundOverLoadWith_t = BinaryOverLoadWith_t;
	using ForOverLoadWith_t = BinaryOverLoadWith_t;
	using CastOverLoadWith_t = BinaryOverLoadWith_t;
	using UrinaryOverLoadWith_t = BinaryOverLoadWith_t;


	struct ClassStackInfo
	{
		ClassInfo* Info = nullptr;
		bool _InStatements = false;
	};

	enum class Jumps_t
	{
		Continue,
		Break,
	};
	struct JumpsData
	{
		Jumps_t Type = Jumps_t::Continue;
		IRInstruction* JumpIns = nullptr;
	};
	enum class NodeSyb_t
	{
		Any,
		Varable,
		Parameter,
		ClassFeild,
		Ret,
	};

	//Members
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	UClib _Lib;

	PassType passtype = PassType::Null;

	const Vector<const FileNode*>* _Files = nullptr;
	const Vector<const UClib*>* _Libs = nullptr;
	SymbolTable _Table;


	Stack<ClassStackInfo> _ClassStack;


	const FileNode* LookingAtFile = nullptr;
	BinaryVectorMap<const FileNode_t*, FileNodeData> _FilesData;
	BinaryVectorMap<const void*, BinaryExpressionNode_Data> BinaryExpressionNode_Datas;
	BinaryVectorMap<const void*, IndexedExpresion_Data> IndexedExpresion_Datas;
	BinaryVectorMap<const void*, PostFixExpressionNode_Data> PostFix_Datas;
	BinaryVectorMap<const void*, CompoundExpresion_Data> Compound_Datas;
	BinaryVectorMap<const void*, ForExpresion_Data> For_Datas;
	BinaryVectorMap<const void*, CastExpressionNode_Data> CastDatas;
	BinaryVectorMap<const void*, AssignExpression_Data > AssignExpressionDatas;
	BinaryVectorMap<const void*, bool> ValidNodes;

	Vector<FuncStackInfo> _FuncStack;

	const Token* LastLookedAtToken = nullptr;
	enum class DeclareStaticVariableNode_t
	{
		Stack,
		Static,
		Thread,
		ClassField,
	};
	struct VarableUseData
	{
		String Scope;
		Vector<Symbol*> _UsedSymbols;
		Vector<Symbol*> _SymbolsToPassBecauseInerLamdba;
	};
	Vector<NodeType> NodeTypeStack;

	//
	Stack<TypeSymbol> LookingForTypes;
	TypeSymbol LastExpressionType;
	BinaryVectorMap<const void*, Get_FuncInfo> FuncToSyboID;
	Vector< NewFuncData> TepFuncs;
	Vector<const ClassInfo*> ClassDependencies;
	Vector< FuncInfo*>_RetLoopStack;
	Vector<const AttributeNode*> _TepAttributes;
	//IR Building
	IRBuilder _Builder;
	IRInstruction* _LastExpressionField = 0;
	IRFunc* LookingAtIRFunc = nullptr;
	IRBlock* LookingAtIRBlock = nullptr;
	IROperator _LastStoreField;
	Stack<IRLocation_Cotr> IRlocations;//for Constructors
	Vector<IRCodeStackFrames> StackFrames;


	BinaryVectorMap<void*, SymbolID> _ConstantExpressionMap;
	Stack<GetValueMode> GetExpressionMode;


	Stack<GenericFuncInfo> GenericFuncName;

	Vector< JumpsData> _Jumps;


	Stack<VarableUseData> _Varable;

	//Funcs

	static bool IsWrite(GetValueMode Value)
	{
		return Value == GetValueMode::Write || Value == GetValueMode::ReadAndWrite;
	}
	static bool IsRead(GetValueMode Value)
	{
		return Value == GetValueMode::Read || Value == GetValueMode::ReadAndWrite;
	}
	void WriteTo(IRInstruction* IR, const IROperator& Value);
	void WriteToDef(IRInstruction* IR, const IROperator& Value);

	void WriteTo(IRInstruction* IR, const TypeSymbol& Type, const IROperator& Value);

	size_t GetJumpsIndex();
	void RemoveJumps(size_t Index);

	IRidentifierID ConveToIRClassIR(const Symbol& Class);
	IRidentifierID ConveToStaticArray(const Symbol& Class);
	IRidentifierID ConveToIRVariantEnum(const Symbol& Class);

	IRType ConvertToIR(const TypeSymbol& Value);
	BinaryVectorMap<SymbolID, IRidentifierID> SybToIRMap;
	void PushNewStackFrame();
	void PopStackFrame();
	TypeSymbol& Get_LookingForType()
	{
		return LookingForTypes.top();
	}
	SymbolID GetSymbolID(const Node& node);

	//File dependency analysis stuff

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
	inline bool IsDependencies(const FuncInfo* Value)
	{
		for (auto& Item : _FuncStack)
		{
			if (Item.Pointer == Value)
			{
				return true;
			}
		}
		return false;
	}



	inline void PushToNodeScope(const Node& node)
	{
		NodeTypeStack.push_back(node.Get_Type());
	}
	inline void PopNodeScope()
	{
		NodeTypeStack.pop_back();
	}


	inline FuncStackInfo* GetDependencies(const FuncInfo* Value)
	{
		for (auto& Item : _FuncStack)
		{
			if (Item.Pointer == Value)
			{
				return &Item;
			}
		}
		return nullptr;
	}
	inline bool IsRetDependencies(const FuncInfo* Value)
	{
		for (auto& Item : _RetLoopStack)
		{
			if (Item == Value)
			{
				return true;
			}
		}
		return false;
	}
	const FileNode* Get_FileUseingSybol(Symbol* Syb);
	void AddDependencyToCurrentFile(Symbol* Syb);
	void AddDependencyToCurrentFile(const FileNode* file);
	void AddDependencyToCurrentFile(const TypeSymbol& type);
	Symbol& AddSybol(SymbolType type, const String& Name, const String& FullName, AccessModifierType Access);
	bool IsVarableType(SymbolType type);
	void Pass();
	void OnFileNode(const FileNode* File);
	void OnClassNode(const ClassNode& node);
	void OnAliasNode(const AliasNode& node);
	void OnUseingNode(const UsingNode& node);
	void OnFuncNode(const FuncNode& node);
	void SetInStatetements(bool Value);
	void FuncGetName(const Token* NameToken, std::string_view& FuncName, FuncInfo::FuncType& FuncType);

	String GetEnumVariantUnionName(const String& FullName);
	String GetUnrefencedableName(const String& FullName);

	void OnStatement(const Node& node2);
	void OnRetStatement(const RetStatementNode& node);
	void OnEnum(const EnumNode& node);
	void OnNamespace(const NamespaceNode& node);
	void OnAttributeNode(const AttributeNode& node);
	void OnNonAttributeable(size_t Line, size_t Pos);
	String GetScopedNameAsString(const ScopedNameNode& node);
	void OnDeclareVariablenode(const DeclareVariableNode& node, DeclareStaticVariableNode_t type);
	void OnStoreVarable(bool IsStructObjectPassRef, IRInstruction* OnVarable, Symbol* syb, const SymbolID& sybId);
	void AddDestructorToStack(Symbol* syb, const SymbolID& sybId, IRInstruction* OnVarable);
	void AddDestructorToStack(const TypeSymbol& Type, IRInstruction* OnVarable);
	void ExDeclareVariableTypeCheck(TypeSymbol& VarType, TypeSymbol& Ex, const Token* Token);
	void ExTypeDeclareVarableCheck(TypeSymbol& VarType, const Node* Ex, const Token* Token);
	void CantgussTypesTheresnoassignment(const Token* Token);
	void OnAssignExpressionNode(const AssignExpressionNode& node);
	void OnIfNode(const IfNode& node);
	void OnWhileNode(const WhileNode& node);
	void OnDoNode(const DoNode& node);
	void OnDeclareStaticVariableNode(const DeclareStaticVariableNode& node);
	void OnDeclareThreadVariableNode(const DeclareThreadVariableNode& node);
	void FuncRetCheck(const Token& Name, const Symbol* FuncSyb, const FuncInfo* Func);
	void OnForNode(const ForNode& node);

	void DoJumpsBreakAndContiunes(size_t JumpIndex, size_t BoolCode, size_t BreakCode);

	void OnContinueNode(const ContinueNode& node);
	void OnBreakNode(const BreakNode& node);
	void OnLambdaNode(const LambdaNode& node);
	void OnTrait(const TraitNode& node);
	void OnTag(const TagTypeNode& node);
	void OnBitCast(const BitCastExpression& node);

	String GetClassWithTraitVTableName(const String& ClassFullName, const String& TraitFullName);
	String GetTraitVTableName(const String& TraitName);
	String GetTraitVStructTableName(const String& TraitName);

	void InheritTrait(Symbol* Syb, ClassInfo* ClassInfo, Symbol* Trait, const Token* ClassNameToken);

	void BuildTrait(Symbol* Syb, ClassInfo* ClassInfo, Symbol* Trait, const Token* ClassNameToken);

	void LogMissingFuncionforTrait(String_view& FuncName, FuncInfo* Info, Symbol* Trait, const Token* ClassNameToken);


	Symbol* NewDropFuncSymbol(ClassInfo* ClassInfo, TypeSymbol& ClassAsType);
	void BuildFuncDropUsesingFields(const ClassInfo* ClassInfo, const IRType& ThisPar);


	void OnStatementsWithSetableRet(const StatementsNode& node, TypeSymbol& RetOut, const Token* Token);

	String GetLambdaFuncTepName(const String& LambdaName)
	{
		return LambdaName + "ptr";
	}

	void TypeDoesNotHaveForOverload(const Token* Token, TypeSymbol& ExType);

	bool ISStructPassByRef(Symbol* syb);

	bool GetMemberTypeSymbolFromVar(size_t Start, size_t End, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out);
	bool GetMemberTypeSymbolFromVar(size_t Start, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out)
	{
		return GetMemberTypeSymbolFromVar(Start, -1, node, Out);
	}
	bool GetMemberTypeSymbolFromVar(const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out);
	void BuildMemberDereferencStore(const GetMemberTypeSymbolFromVar_t& In, IRInstruction* Value);
	void BuildMember_Store(const GetMemberTypeSymbolFromVar_t& In, IRInstruction* Value);

	IROperator BuildMember_Store(const GetMemberTypeSymbolFromVar_t& In);
	IROperator BuildMember_DereferencStore(const GetMemberTypeSymbolFromVar_t& In);
	IROperator BuildMember_Store(const GetMemberTypeSymbolFromVar_t& In, const TypeSymbol& Type);

	IRInstruction* BuildMember_GetPointer(const GetMemberTypeSymbolFromVar_t& In);
	IRInstruction* BuildMember_GetValue(const GetMemberTypeSymbolFromVar_t& In);
	IRInstruction* BuildMember_DereferenceValue(const GetMemberTypeSymbolFromVar_t& In);
	IRInstruction* BuildMember_AsPointer(const GetMemberTypeSymbolFromVar_t& In);
	IRInstruction* BuildMember_AsValue(const GetMemberTypeSymbolFromVar_t& In);
	void BuildMember_Access(const GetMemberTypeSymbolFromVar_t& In, IRInstruction*& Output);
	void BuildMember_Reassignment(const GetMemberTypeSymbolFromVar_t& In, const TypeSymbol& Type, IRInstruction* Value);
	Symbol* GetTepFuncPtrSyb(const String& TepFuncPtr, const FuncInfo* Finfo);
	String GetTepFuncPtrName(FuncInfo* SymbolVar);
	String_view GetTepFuncPtrNameAsName(const String_view Str);

	void OnPostfixVariableNode(const PostfixVariableNode& node);
	void OnCompoundStatementNode(const CompoundStatementNode& node);
	void OnExpressionTypeNode(const Node* node, GetValueMode Mode);
	void OnExpressionNode(const ValueExpressionNode& node);
	void OnMovedNode(MoveNode* nod);
	void OnNumberliteralNode(NumberliteralNode* num);
	void OnBoolliteralNode(BoolliteralNode* num);
	void OnCharliteralNode(CharliteralNode* num);
	void OnFloatLiteralNode(FloatliteralNode* num);
	void OnStringLiteral(StringliteralNode* nod, bool& retflag);
	void OnSizeofNode(SizeofExpresionNode* nod);
	void OnNewNode(NewExpresionNode* nod);
	void OnAnonymousObjectConstructor(AnonymousObjectConstructorNode*& nod);
	void OnReadVariable(const ReadVariableNode& nod);
	Byte OperatorPrecedenceValue(const Node* node);
	Byte OperatorPrecedence(TokenType V);
	bool SwapForOperatorPrecedence(const Node* nodeA, const Node* nodeB);
	void OnExpressionNode(const BinaryExpressionNode& node);
	void OnExpressionNode(const CastNode& node);
	void OnExpressionNode(const IndexedExpresionNode& node);
	void OnFuncCallNode(const FuncCallNode& node);
	void OnDropStatementNode(const DropStatementNode& node);
	void OnInvalidNode(const InvalidNode& node);
	void OnvalidNode(const ValidNode& node);

	void PushTepAttributesInTo(Vector<AttributeData>& Input);
	void LoadLibSymbols();
	void LoadLibSymbols(const UClib& lib);
	auto OutputType()
	{
		return  _Settings->_Type;
	}
	auto OutputTypeAsLibType()
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

	struct StrExELav
	{

		String OwnedStr;
		String_view Msg;
		bool IsConstantExpression = false;
	};


	BinaryOverLoadWith_t HasBinaryOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB);
	CompoundOverLoadWith_t HasCompoundOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB);
	PostFixOverLoadWith_t HasPostfixOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp);
	IndexOverLoadWith_t  HasIndexedOverLoadWith(const TypeSymbol& TypeA, const TypeSymbol& TypeB);
	ForOverLoadWith_t HasForOverLoadWith(const TypeSymbol& TypeA);
	UrinaryOverLoadWith_t HasUrinaryOverLoadWith(const TypeSymbol& TypeA, TokenType Op);

	TypeSymbol BinaryExpressionShouldRurn(TokenType Op, const TypeSymbol& Ex0Type);

	String ToString(const TypeSymbol& Type);
	String ToString(const TokenType& Type)
	{
		return StringHelper::ToString(Type);
	}
	


	void Convert(const TypeNode& V, TypeSymbol& Out);
	
	void ConvertAndValidateType(const TypeNode& V, TypeSymbol& Out,NodeSyb_t Syb);
	TypeSymbol ConvertAndValidateType(const TypeNode& V, NodeSyb_t Syb);
	bool ValidateType(const TypeSymbol& V,const Token* Token,NodeSyb_t Syb);

	

	Symbol* GetSymbol(const ClassInfo* Info)
	{
		return GetSymbol(Info->FullName, SymbolType::Type_class);
	}
	Symbol* GetSymbol(const FuncInfo* Info)
	{
		return GetSymbol(Info->FullName, SymbolType::Func);
	}
	Symbol* GetSymbol(const TypeSymbol& Info)
	{
		if (Info._Type == TypesEnum::CustomType) {
			return GetSymbol(Info._CustomTypeSymbol);
		}
		return nullptr;
	}
	Symbol* GetSymbol(const SymbolID Info)
	{
		return &_Table.GetSymbol(Info);
	}

	bool _InSideClass()
	{
		return  _ClassStack.size() && _ClassStack.top()._InStatements == false;
	}
	bool IsInThisFuncCall()
	{
		if (_FuncStack.size()) {
			return IsThisFuncCall(_FuncStack.back().Pointer);
		}
		return false;
	}
	bool IsThisFuncCall(const FuncInfo* Func)
	{
		if (auto ObjectType = Func->GetObjectForCall())
		{
			auto objecttypesyb = GetSymbol(*ObjectType);
			ClassInfo* V = objecttypesyb->Get_Info<ClassInfo>();

			return _ClassStack.top().Info== V;
		}

		return false;
	}
	

	void DoSymbolRedefinitionCheck(const Symbol* Syb, const Token* Value);
	void DoSymbolRedefinitionCheck(const String_view FullName, SymbolType Type, const Token* Value);
	void DoSymbolRedefinitionCheck(const Symbol* Syb, const FuncInfo* Fvalue, const Token* Value);

	bool CanDoTypeToTrait(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);

	bool IsVaidType(TypeSymbol& Out);
	bool CanBeImplicitConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type, bool ReassignMode = true);
	CastOverLoadWith_t  CanBeExplicitlyConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);
	Optional<FuncInfo*> GetAnExplicitlyConvertedFunc(const TypeSymbol& TypeToCheck);

	bool DoImplicitConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType);
	void DoExplicitlConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType,const CastExpressionNode_Data& Data);

	bool IsSIntType(const TypeSymbol& TypeToCheck);
	bool IsUIntType(const TypeSymbol& TypeToCheck);
	bool IsIntType(const TypeSymbol& TypeToCheck)
	{
		return  IsSIntType(TypeToCheck) || IsUIntType(TypeToCheck);
	}
	bool IsfloatType(const TypeSymbol& TypeToCheck);
	bool IsPrimitive(const TypeSymbol& TypeToCheck);
	bool IsPrimitiveNotIncludingPointers(const TypeSymbol& TypeToCheck);
	bool IsStaticArray(const TypeSymbol& TypeToCheck);

	bool IsimmutableRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);

	bool IsAddessAndLValuesRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type, bool ReassignMode);


	bool HasDestructor(const TypeSymbol& TypeToCheck);

	void Update_ClassSym_ToFixedTypes(Symbol* Sym);
	void Update_FuncSym_ToFixedTypes(Symbol* Sym);

	Optional<size_t> GetSize(const TypeSymbol& Type)
	{
		UAddress V;
		if (GetSize(Type, V))
		{
			return V;
		}
		return {};
	}
	bool GetSize(const TypeSymbol& Type, UAddress& OutSize);

	Optional<size_t> GetOffset(const ClassInfo& Type, const FieldInfo* Field)
	{
		UAddress V;
		if (GetOffset(Type, Field, V))
		{
			return V;
		}
		return {};
	}
	bool GetOffset(const ClassInfo& Type, const FieldInfo* Field, UAddress& OutOffset);



	Get_FuncInfo GetFunc(const TypeSymbol& Name,const ValueParametersNode& Pars);
	Get_FuncInfo GetFunc(const ScopedNameNode& Name,const ValueParametersNode& Pars,TypeSymbol Ret)
	{
		UseGenericsNode generics;
		return GetFunc(Name, generics, Pars, Ret);
	};
	Get_FuncInfo GetFunc(const ScopedNameNode& Name,const UseGenericsNode&,const ValueParametersNode& Pars,TypeSymbol Ret);


	Get_FuncInfo GetEnumVariantFunc(Symbol* EnumSyb, size_t FeildIndex, Symbol* EnumFieldSyb, const ValueParametersNode& Pars,const Token* Token,const Vector<TypeSymbol>& ValueTypes);



	void SetFuncRetAsLastEx(Get_FuncInfo& Info);
	
	struct  IsCompatiblePar
	{
		const Vector<TypeSymbol>* Pars;
		const TypeSymbol* Ret;
		Symbol* Item;
		void SetAsFuncInfo(Symbol* Item)
		{
			FuncInfo* Info = Item->Get_Info<FuncInfo>();
			Pars = &Info->Pars;
			Ret = &Info->Ret;
			this->Item = Item;
		}
		void SetAsFuncPtrInfo(Symbol* Item)
		{
			FuncPtrInfo* Info = Item->Get_Info<FuncPtrInfo>();
			Pars = &Info->Pars;
			Ret = &Info->Ret;
			this->Item = Item;
		}
	};

	bool IsCompatible(const IsCompatiblePar& FuncPar, const Vector<TypeSymbol>& ValueTypes, bool _ThisTypeIsNotNull, const Token* Token);

	int GetCompatibleScore(const TypeSymbol& ParFunc, const TypeSymbol& Value);
	int GetCompatibleScore(const IsCompatiblePar& Func, const Vector<TypeSymbol>& ValueTypes);

	bool AccessCheck(const Symbol* Syb, const  Token* Token, const String_view Scope);
	bool AccessCheck(const Symbol* Syb, const  Token* Token)
	{
		return AccessCheck(Syb, Token, this->_Table._Scope.ThisScope);
	}

	//Generics

	void GenericFuncInstantiate(Symbol* Func, const Vector<TypeSymbol>& GenericInput);

	String GetGenericFuncName(Symbol* Func, const Vector<TypeSymbol>& Type);
	

	void GenericTypeInstantiate(Symbol* Class, const Vector<TypeSymbol>& Type);

	EvaluatedEx MakeEx(const TypeSymbol& Type);
	RawEvaluatedObject MakeExr(const TypeSymbol& Type);
	void* Get_Object(const TypeSymbol& Input, const RawEvaluatedObject& Input2);
	void* Get_Object(const EvaluatedEx& Input);

	StrExELav GetStrEVal(const Node* node);
	bool ConstantExpressionAbleType(const TypeSymbol& Type);
	bool EvaluateDefaultConstructor(EvaluatedEx& Out);
	bool Evaluate(EvaluatedEx& Out, const ValueExpressionNode& node);
	bool Evaluate(EvaluatedEx& Out, const BinaryExpressionNode& node);
	bool Evaluate(EvaluatedEx& Out, const CastNode& node);
	bool Evaluate(EvaluatedEx& Out, const ReadVariableNode& nod);
	bool Evaluate_t(EvaluatedEx& Out, const Node* node);
	bool EvaluatePostfixOperator(EvaluatedEx& Out, TokenType Op);
	bool HasConstantPostfixOperator(const TypeSymbol& Type, TokenType Op);
	bool CanEvaluateImplicitConversionConstant(const TypeSymbol& Type, const TypeSymbol& ToType);
	bool EvaluateImplicitConversion(EvaluatedEx& In, const TypeSymbol& ToType, EvaluatedEx& out);
	


	bool Evaluate(EvaluatedEx& Out, const TypeSymbol& MustBeType, const ExpressionNodeType& node);
	Optional<EvaluatedEx> Evaluate(const TypeSymbol& MustBeType, const ExpressionNodeType& node);

	bool EvaluateToAnyType(EvaluatedEx& Out, const ExpressionNodeType& node);
	Optional<EvaluatedEx> EvaluateToAnyType(const ExpressionNodeType& node);
	String ToString(const TypeSymbol& Type, const RawEvaluatedObject& Data);
	//IR
	void DoFuncCall(Get_FuncInfo Func, const ScopedNameNode& Name, const ValueParametersNode& Pars);
	void DoFuncCall(const TypeSymbol& Type, const Get_FuncInfo& Func, ValueParametersNode& ValuePars);

	void DoDestructorCall(const ObjectToDrop& Object);

	IRInstruction* IR_Load_UIntptr(UAddress Value);
	IRInstruction* IR_Load_SIntptr(SIntNative Value);
	IRInstruction* Build_Add_uIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* Build_Sub_uIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* Build_Add_sIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* Build_Sub_sIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* Build_Mult_uIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* Build_Mult_sIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* Build_Div_uIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* Build_Div_sIntPtr(IRInstruction* field, IRInstruction* field2);
	void Build_Increment_uIntPtr(IRInstruction* field, UAddress Value);
	void Build_Decrement_uIntPtr(IRInstruction* field, UAddress Value);
	void Build_Increment_sIntPtr(IRInstruction* field, SIntNative Value);
	void Build_Decrement_sIntPtr(IRInstruction* field, SIntNative Value);
	IRInstruction* LoadEvaluatedEx(const RawEvaluatedObject& Value, const TypeSymbol& ValueType);
	//Errors

	void LogCantFindCompoundOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type);
	void LogCantFindPostfixOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type);
	void LogCantFindBinaryOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type);

	void ExpressionMustbeAnLocationValueError(const Token* Token, TypeSymbol& Ex0Type);
	void YouMustReturnSomethingError(const Token* Token);
	void CantguessVarTypeError(const Token* Token);
	void CantUseThisKeyWordHereError(const Token* NameToken);
	void LogCantCastImplicitTypes(const Token* Token, const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType, bool ReassignMode);
	void LogReadingFromInvaidVariable(const Token* Token, String_view Str);
	void LogCantFindVarError(const Token* Token, String_view Str);
	void LogCantFindVarMemberError(const Token* Token, String_view Str, const TypeSymbol& OnType);
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
	void LogCantCastImplicitTypes_Constant(const Token* Token, const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType);
	void LogCantFindNamespace(const Token* Token, const String_view Namespace);
	void LogFuncMustBe(const Token* Token, const String_view FuncName, TypeSymbol& TypeSybToBe);
	void LogSymbolRedefinition(const Token* Token, const Symbol* Symbol);
	void LogUseingVarableBeforDeclared(const Token* Token);
	void LogBeMoreSpecifiicForRetType(const String_view FuncName, const Token* Token);
	void LogCantBeIndexWithType(const Token* Token, const  TypeSymbol& Ex0Type, const  TypeSymbol& IndexType);
	void LogCantUseThisInStaticFunction(const Token* Token);
	void LogFuncDependencyCycle(const Token* Token, const FuncInfo* Value);

	void LogGenericInputWantsaExpressionNotType(const Token* Token,const String_view NameOfPar);
	void LogGenericInputWantsaTypeNotExpression(const Token* Token, const String_view NameOfPar);
	
	void LogExpectedSymbolToBea(const Token* Token, const Symbol& Syb, SymbolType Value);
	
	void LogCanIncorrectStaticArrCount(const Token* Token, const TypeSymbol& Type, size_t Count, size_t FuncCount);
	void LogBeMoreSpecifiicWithStaticArrSize(const Token* Token, const TypeSymbol& Type);
	void LogBinaryOverloadPars(const Token& Name, const FuncInfo* Func);
	void LogIndexOverloadPars(const Token& Name, const FuncInfo* Func);
	void LogPostfixOverloadPars(const Token& Name, const FuncInfo* Func);
	void LogCantOverLoadOverload(const Token* NameToken);
	void LogCantFindMemberOverloadForType(const Token* Item, TokenType Op, const TypeSymbol& Out);
	void LogCantUseMoveTypeHere(const Token* Token);
	void LogMustMakeEnumLikeafuncion(EnumInfo* Einfo, size_t Index, const Token* Token);
	void LogDynamicMustBeRrait(const TypeNode& V, const TypeSymbol& Out);
	void TraitCantBeAlone(const Token* Token);
	void LogWantedAType(const TypeNode& V, Symbol* SybV);
	void LogOutCanOnlyBeInControlFlow(const Token* Token);
	void LogParamterMustBeAnOutExpression(const Token* Token, const size_t& i);
	void LogInvalidNodeError(const Token* Token, String_view ErrStr);
	void LogEmptyInvalidError(const Token* Token);

	String ToString(SymbolType Value);
	ReadVarErrorCheck_t LogTryReadVar(String_view VarName, const Token* Token, const Symbol* Syb);
	void CheckVarWritingErrors(Symbol* Symbol, const Token* Token, String_view& Name);
	void LogWantedAVariable(const Token* const& Item, Symbol* TepSyb);
	ClassData* SystematicAnalysis::GetAssemblyClass(const String& FullName);

	String MangleName(const FuncInfo* Func);
	IRidentifierID GetIRID(const FuncInfo* Func);
};
UCodeLangFrontEnd


