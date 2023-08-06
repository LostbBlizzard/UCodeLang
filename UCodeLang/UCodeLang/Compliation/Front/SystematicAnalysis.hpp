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

class SystematicAnalysis;
struct Systematic_BuiltInFunctions
{
public:

	enum class ID 
	{
		Null,
		TypeInfo_GetName,
		TypeInfo_GetFullName,

		Max,
	};
	
	struct FunctionPar
	{
		bool IsOutPar =false;
		TypeSymbol Type;
		const Node* ExpressionNode = nullptr;
	};
	struct FunctionData
	{
		String FuncName;
		ID _ID = ID::Null;

		bool IsMemberFuncion = false;
		Vector<FunctionPar> Pars;
		TypeSymbol Ret;
		FunctionData(const String& Str, ID ID)
		{
			FuncName = Str;
			_ID = ID;
		}
	};

	struct Func
	{
		struct OutParData
		{
			TypeSymbol Type;
			Optional<RawEvaluatedObject> EvalObject;
		};

		TypeSymbol RetType;
		Optional<RawEvaluatedObject> EvalObject;
		bool EvalAsCString = false;
		Vector<OutParData> _OutPars;
	};

	struct ClassField
	{
		const ClassInfo* _ClassInfo =nullptr;
		const FieldInfo* Field = nullptr;
	};
	struct EnumField
	{
		const EnumInfo* _EnumInfo = nullptr;
		const EnumFieldInfo* Field = nullptr;
	};

	static Optional<Func> GetFunction(const String_view Name,const Vector<FunctionPar>& Pars, SystematicAnalysis& This);
};



class SystematicAnalysis
{

public:
	friend Systematic_BuiltInFunctions;
	struct ImportLib
	{
		const Path* Name;
	};
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
		return _IR_Builder;
	}
	UCodeLangForceinline UClib& Get_Lib()
	{
		return _Lib;
	}


	
	struct FileNodeData
	{
		Vector< const FileNode_t*> _Dependencys;
	
		struct ImportData
		{
			String ImportSymbolFullName;
			Vector<Symbol*> _AliasSymbols;


			Vector<String> _TepSymbolFullNames;
			bool* IsImportUsed =nullptr;
		};
		Vector<ImportData> _Imports;
		struct SpanData
		{
			size_t Index = 0;
			size_t Count = 0;
	

			static SpanData NewWithNewIndex(size_t Index, size_t EndIndex)
			{
				SpanData R;
				R.Index = Index;
				R.Count = EndIndex - Index;
				return R;
			}
		};
		SpanData AssemblyInfoSpan;
		SpanData GlobalObjectMethodInfoSpan;
		
		SpanData IRInitStaticSpan;
		SpanData IRInitThreadSpan;
		SpanData IRDeInitStaticSpan;
		SpanData IRDeInitThreadSpan;

		SpanData IRFuncsSpan;
		SpanData IRSymbolSpan;
		SpanData IRConstStringSpan;
	};
	const FileNodeData& GetFileDataPub(const FileNode_t* file) const { return *_FilesData.at(file); }
	void SetLibNames(const Vector <Path>* LibsNames)
	{
		_LibsNames = LibsNames;
	}
	void ToIntFile(FileNode_t* File, const Path& path);
private:
	FileNodeData& GetFileData(const FileNode_t* file) { return *_FilesData.at(file); }

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
		SymbolID ID ;


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
		Optional<Systematic_BuiltInFunctions::Func> _BuiltFunc;
		bool CantCheckBecauseIsUnMaped = false;
		bool HasFunc()
		{
			return Func || _BuiltFunc.has_value();
		}
		static bool AddOneToGetParNode(ThisPar_t ThisPar)
		{
			return ThisPar != Get_FuncInfo::ThisPar_t::NoThisPar;
		}
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
		String _IR_GenericFuncName;
		const Vector<TypeSymbol>* GenericInput = nullptr;
		const void* NodeTarget = nullptr;
		Optional<SymbolID> Pack;
	};
	struct BinaryOverLoadWith_t
	{
		bool HasValue = false;
		Optional<Symbol*> Value;
	};
	struct EvalFuncData
	{
		VectorMap<SymbolID, RawEvaluatedObject> Pars;
		RawEvaluatedObject Ret;

		VectorMap<SymbolID,RawEvaluatedObject> Vars;

		const Symbol* FuncSyb = nullptr;
		const FuncInfo* Get_funcInfo()
		{
			return FuncSyb->Get_Info<FuncInfo>();
		}
		const Token* CallFrom = nullptr;
	};
	using IndexOverLoadWith_t = BinaryOverLoadWith_t;
	using PostFixOverLoadWith_t = BinaryOverLoadWith_t;
	using CompoundOverLoadWith_t = BinaryOverLoadWith_t;
	using ForOverLoadWith_t = BinaryOverLoadWith_t;
	using CastOverLoadWith_t = BinaryOverLoadWith_t;
	using UrinaryOverLoadWith_t = BinaryOverLoadWith_t;


	struct MatchAutoPassEnum
	{
		FuncCallNode Func;
		ExpressionNodeType NewNode;
		ValueExpressionNode NewValNode;
		MatchAutoPassEnum()
		{

		}
		~MatchAutoPassEnum()
		{

		}
	};
	struct MatchArm
	{
		Shared_ptr<MatchAutoPassEnum> _AutoPassEnum;
		//c++ does not like this not copying but it need to work for Vectors should be Unique_ptr or just it self.
	
		MatchAutoPassEnum& Get_AutoPassEnum()
		{
			return *_AutoPassEnum.get();
		}
		
	};
	struct MatchArmData
	{
		Vector<MatchArm> Arms;
	};

	struct MatchStatementData
	{

		MatchArmData ArmData;
	};
	struct MatchExpressionData
	{
		TypeSymbol  MatchAssignmentType;
		MatchArmData ArmData;
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
	struct CompileTimeforNode
	{
		Symbol* SybToLoopOver=nullptr;
		Vector<Symbol*> SybItems;
	};
	struct VarableMemberData
	{
		String MemberString;
	};
	enum class DeclareStaticVariableNode_t
	{
		Stack,
		Static,
		Thread,
		ClassField,
		Eval,
	};
	struct VarableUseData
	{
		String Scope;
		Vector<Symbol*> _UsedSymbols;
		Vector<Symbol*> _SymbolsToPassBecauseInerLamdba;
	};
	//Bad Name
	struct LibLoadTypeSeter
	{
		const TypeSymbol* ToGetTypeFrom = nullptr;
		TypeSymbol* TypeToFix = nullptr;
	};
	//Members
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	UClib _Lib;

	PassType _PassType = PassType::Null;
	
	const Vector<const FileNode*>* _Files = nullptr;
	const Vector<const UClib*>* _Libs = nullptr;
	const Vector<Path>* _LibsNames = nullptr;
	Vector<FileNode> _LibsFiles;
	SymbolTable _Table;

	VectorMap<String,BinaryVectorMap<const void*, SymbolID>> _SybIdMap;
	uintptr_t _IDIndex = 0;
	//Args
	bool _ForceImportArgWasPassed = false;
	bool _RemoveUnSafeArgWasPassed = false;
	bool _ImmutabilityIsForced = false;
	Optional<String> _StartingNameSpace;

	Stack<ClassStackInfo> _ClassStack;


	const FileNode* _LookingAtFile = nullptr;
	BinaryVectorMap<const FileNode_t*,Shared_ptr<FileNodeData>> _FilesData;
	BinaryVectorMap<SymbolID, BinaryExpressionNode_Data> _BinaryExpressionNode_Datas;
	BinaryVectorMap<SymbolID, IndexedExpresion_Data> _IndexedExpresion_Datas;
	BinaryVectorMap<SymbolID, PostFixExpressionNode_Data> _PostFix_Datas;
	BinaryVectorMap<SymbolID, CompoundExpresion_Data> _Compound_Datas;
	BinaryVectorMap<SymbolID, ForExpresion_Data> _For_Datas;
	BinaryVectorMap<SymbolID, CastExpressionNode_Data> _CastDatas;
	BinaryVectorMap<SymbolID, AssignExpression_Data > _AssignExpressionDatas;
	BinaryVectorMap<SymbolID, bool> _ValidNodes;
	BinaryVectorMap<SymbolID, CompileTimeforNode> _ForNodes;
	BinaryVectorMap<SymbolID, MatchStatementData> _MatchStatementDatas;
	BinaryVectorMap<SymbolID, MatchExpressionData> _MatchExpressionDatas;

	

	BinaryVectorMap<SymbolID, VarableMemberData> _VarableMemberDatas;//Var.$Item

	Vector<FuncStackInfo> _FuncStack;

	const Token* _LastLookedAtToken = nullptr;

	Vector<NodeType> _NodeTypeStack;
	Vector<String> _ExtendedErr;
	//
	Stack<TypeSymbol> _LookingForTypes;
	TypeSymbol _LastExpressionType;
	BinaryVectorMap<SymbolID, Get_FuncInfo> _FuncToSyboID;
	Vector< NewFuncData> _TepFuncs;
	Vector<const ClassInfo*> _ClassDependencies;
	Vector< FuncInfo*>_RetLoopStack;
	//IR Building
	IRBuilder _IR_Builder;
	IRInstruction* _IR_LastExpressionField = 0;
	IRFunc* _IR_LookingAtIRFunc = nullptr;
	IRBlock* _IR_LookingAtIRBlock = nullptr;
	IROperator _IR_LastStoreField;
	Stack<IRLocation_Cotr> _IR_IRlocations;//for Constructors
	Vector<IRCodeStackFrames> _IR_StackFrames;
	Stack<GenericFuncInfo> _IR_GenericFuncName;
	Vector< JumpsData> _IR_Jumps;

	BinaryVectorMap<void*, SymbolID> _ConstantExpressionMap;
	Stack<GetValueMode> _GetExpressionMode;
	Stack<VarableUseData> _Varable;
	Vector<Unique_ptr<EvalFuncData>> _Eval_FuncStackFrames;
	
	
	//To Fix Types being Loaded out of order.
	Vector<LibLoadTypeSeter> _Lib_TypesToFix;

	BinaryVectorMap<SymbolID, IRidentifierID> SybToIRMap;
	//Funcs
	SymbolContext Save_SymbolContext() const
	{
		SymbolContext R;
		R.File = _LookingAtFile;
		R.Scope = _Table._Scope;
		R.Useings = _Table.Useings;
		R._ClassStack = _ClassStack;

		return R;
	}
	SymbolContext Save_SymbolContextRemoveOneScopeName() const
	{
		SymbolContext R;
		R.File = _LookingAtFile;
		R.Scope.ThisScope =ScopeHelper::GetReMoveScope(_Table._Scope.ThisScope);
		R.Useings = _Table.Useings;
		R._ClassStack = _ClassStack;

		return R;
	}
	SymbolContext SaveAndMove_SymbolContext()
	{
		SymbolContext R;
		R.File = std::move(_LookingAtFile);
		R.Scope = std::move(_Table._Scope);
		R.Useings = std::move(_Table.Useings);
		R._ClassStack = std::move(_ClassStack);

		return R;
	}

	void Set_SymbolConext(const SymbolContext& Contex)
	{
		_LookingAtFile = Contex.File;
		_Table._Scope = Contex.Scope;
		_Table.Useings = Contex.Useings;
		_ClassStack = Contex._ClassStack;
	}
	void Set_SymbolConext(SymbolContext&& Contex)
	{
		_LookingAtFile = std::move(Contex.File);
		_Table._Scope = std::move(Contex.Scope);
		_Table.Useings = std::move(Contex.Useings);
		_ClassStack = std::move(Contex._ClassStack);
	}


	void Push_ExtendedErr(String Err,const Token* token)
	{
		_ExtendedErr.push_back(Err + ". On line " + std::to_string(token->OnLine));
	}
	void Pop_ExtendedErr()
	{
		_ExtendedErr.pop_back();
	}

	void Lib_BuildLibs();
	void Lib_BuildLib(const UClib& lib, const Path& LibName);
	static bool IsWrite(GetValueMode Value)
	{
		return Value == GetValueMode::Write || Value == GetValueMode::ReadAndWrite;
	}
	static bool IsRead(GetValueMode Value)
	{
		return Value == GetValueMode::Read || Value == GetValueMode::ReadAndWrite;
	}
	void IR_WriteTo(IRInstruction* IR, const IROperator& Value);
	void IR_WriteToDef(IRInstruction* IR, const IROperator& Value);

	void IR_WriteTo(IRInstruction* IR, const TypeSymbol& Type, const IROperator& Value);

	size_t IR_GetJumpsIndex();
	void IR_RemoveJumps(size_t Index);

	IRidentifierID IRType_ConvertToIRClassIR(const Symbol& Class);
	IRidentifierID IRType_ConvertToStaticArray(const Symbol& Class);
	IRidentifierID IRType_ConveToIRVariantEnum(const Symbol& Class);



	IRType IRType_ConvertToIRType(const ParInfo& Value)
	{
		if (Value.IsOutPar)
		{
			return IRType(IRTypes::pointer);
		}
		return IRType_ConvertToIRType(Value.Type);
	}
	IRType IRType_ConvertToIRType(const TypeSymbol& Value);
	void Push_NewStackFrame();
	void Pop_StackFrame();
	TypeSymbol& Type_Get_LookingForType() 
	{
		return _LookingForTypes.top();
	}
	const TypeSymbol& Type_Get_LookingForType() const
	{
		return _LookingForTypes.top();
	}
	SymbolID Symbol_GetSymbolID(const Node& node)
	{
		return  Symbol_GetSymbolID((const void*)&node);
	}
	SymbolID Symbol_GetSymbolID(const void* Item);
	//File dependency analysis stuff

	inline bool IsDependencies(const ClassInfo* Value) const
	{
		for (auto Item : _ClassDependencies)
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
		_ClassDependencies.push_back(Value);
	}
	inline void PopClassDependencie()
	{
		_ClassDependencies.pop_back();
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
		_NodeTypeStack.push_back(node.Get_Type());
	}
	inline void PopNodeScope()
	{
		_NodeTypeStack.pop_back();
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

	String _LastIRFileName;
	size_t LastLineNumber = -1;
	IRBlockDebugInfo* LastLookAtDebugBlock = nullptr;
	void Add_SetLineNumber(const Token* token, size_t InsInBlock);
	void Add_SetVarableInfo(const Symbol& Syb, size_t InsInBlock);

	const FileNode* Get_FileUseingSybol(const Symbol* Syb);
	void AddDependencyToCurrentFile(const Symbol* Syb);
	void AddDependencyToCurrentFile(const FileNode* file);
	void AddDependencyToCurrentFile(const TypeSymbol& type);
	Symbol& AddSybol(SymbolType type, const String& Name, const String& FullName, AccessModifierType Access);
	bool IsVarableType(SymbolType type);
	void Pass();
	void OnFileNode(const FileNode* File);
	void OnClassNode(const ClassNode& node);
	void OnAliasNode(const AliasNode& node);
	void OnUseingNode(const UsingNode& node);

	void InitGenericalias(const GenericValuesNode& GenericList, bool IsgenericInstantiation, Generic& Out);
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
	void OnAttributesNode(const Vector<Unique_ptr<AttributeNode>>& nodes);
	String GetScopedNameAsString(const ScopedNameNode& node);
	void OnDeclareVariablenode(const DeclareVariableNode& node, DeclareStaticVariableNode_t type);
	void LogCantUseTypeVoidHere(const Token* Token);
	void LogUseingTypeinfoInNonEvalVarable(const Token* Token);
	void LogUseingTypeinfoInEvalFuncPar(const Token* Token);
	void OnStoreVarable(bool IsStructObjectPassRef, IRInstruction* OnVarable, Symbol* syb, const SymbolID& sybId);
	void AddDestructorToStack(const Symbol* syb, const SymbolID& sybId, IRInstruction* OnVarable);
	void AddDestructorToStack(const TypeSymbol& Type, IRInstruction* OnVarable);
	void ExDeclareVariableTypeCheck(TypeSymbol& VarType,const TypeSymbol& Ex, const Token* Token);
	void ExTypeDeclareVarableCheck(TypeSymbol& VarType, const Node* Ex, const Token* Token);
	void CantgussTypesTheresnoassignment(const Token* Token);
	void OnAssignExpressionNode(const AssignExpressionNode& node);
	void OnIfNode(const IfNode& node);
	void OnWhileNode(const WhileNode& node);
	void OnDoNode(const DoNode& node);
	void OnDeclareStaticVariableNode(const DeclareStaticVariableNode& node);
	void OnDeclareThreadVariableNode(const DeclareThreadVariableNode& node);

	void OnDeclareEvalVariableNode(const DeclareEvalVariableNode& node);
	void FuncRetCheck(const Token& Name, const Symbol* FuncSyb, const FuncInfo* Func);
	void LogCantUseOutInOverloadFunc(const UCodeLang::Token& Name);
	void OnForNode(const ForNode& node);

	void DoJumpsBreakAndContiunes(size_t JumpIndex, size_t BoolCode, size_t BreakCode);

	void OnContinueNode(const ContinueNode& node);
	void OnBreakNode(const BreakNode& node);
	void OnLambdaNode(const LambdaNode& node);
	void OnTrait(const TraitNode& node);
	void OnTag(const TagTypeNode& node);
	void OnBitCast(const BitCastExpression& node);

	void OnCompileTimeIfNode(const CompileTimeIfNode& node);

	void OnCompileTimeforNode(const CompileTimeForNode& node);
	void CompileTimeforNodeEvaluateStatements(const CompileTimeForNode& node);

	Optional<SymbolID> UnMapTypeSybol;
	TypeSymbol GetUnMapType();
	bool IsUnMapType(const TypeSymbol& Type);

	void OnImportNode(const ImportStatement& node);

	void OnCMPTypesNode(const CMPTypesNode& node);
	bool CMPGetValue(const TypeSymbol& Type0, const TypeSymbol& Type1, const Token* Value);

	

	String GetClassWithTraitVTableName(const String& ClassFullName, const String& TraitFullName);
	String GetTraitVTableName(const String& TraitName);
	String GetTraitVStructTableName(const String& TraitName);

	void InheritTrait(Symbol* Syb, ClassInfo* ClassInfo,const Symbol* Trait, const Token* ClassNameToken);

	void BuildTrait(const Symbol* Syb,ClassInfo* ClassInfo, const Symbol* Trait, const Token* ClassNameToken);

	void LogMissingFuncionforTrait(const String_view& FuncName, const FuncInfo* Info, const Symbol* Trait, const Token* ClassNameToken);


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
	bool GetMemberTypeSymbolFromVar(const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out)
	{
		return GetMemberTypeSymbolFromVar(0, node, Out);
	}
	bool StepGetMemberTypeSymbolFromVar(const ScopedNameNode& node, size_t Index, ScopedName::Operator_t OpType, GetMemberTypeSymbolFromVar_t& Out);

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
	void StepBuildMember_Access(const ScopedName& ITem, TypeSymbol& Last_Type, ScopedName::Operator_t OpType, const GetMemberTypeSymbolFromVar_t& In, IRInstruction*& Output);
	void BuildMember_Reassignment(const GetMemberTypeSymbolFromVar_t& In, const TypeSymbol& Type, IRInstruction* Value);
	Symbol* GetTepFuncPtrSyb(const String& TepFuncPtr, const FuncInfo* Finfo);
	String GetTepFuncPtrName(FuncInfo* SymbolVar);
	String_view GetTepFuncPtrNameAsName(const String_view Str);

	void OnPostfixVariableNode(const PostfixVariableNode& node);
	void OnCompoundStatementNode(const CompoundStatementNode& node);
	void OnExpressionTypeNode(const Node* node, GetValueMode Mode);
	void OnExpressionTypeNode(const ExpressionNodeType& node, GetValueMode Mode)
	{
		OnExpressionTypeNode(node.Value.get(), Mode);
	}
	void OnExpressionNode(const ValueExpressionNode& node);
	void OnMovedNode(const MoveNode* nod);
	void OnNumberliteralNode(const NumberliteralNode* num);
	void OnBoolliteralNode(const BoolliteralNode* num);
	void OnCharliteralNode(const CharliteralNode* num);
	void OnFloatLiteralNode(const FloatliteralNode* num);
	void OnStringLiteral(const StringliteralNode* nod, bool& retflag);
	bool IsStaticCharArr(const TypeSymbol& Type);
	void OnSizeofNode(const SizeofExpresionNode* nod);
	void OnNewNode(const NewExpresionNode* nod);
	void OnAnonymousObjectConstructor(const AnonymousObjectConstructorNode* nod);
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
	void OnExpressionNode(const ExtendedScopeExpression& node);
	void OnExpressionNode(const ExtendedFuncExpression& node);

	TypeSymbol ExtendedFuncExpressionGetTypeToStart(const TypeSymbol& ExpressionType, const ExtendedFuncExpression& node);

	void OnTypeToValueNode(const TypeToValueNode& node);
	void LogCantOutputTypeinfo(const Token* Token);
	void OnExpressionToTypeValueNode(const ExpressionToTypeValueNode& node);

	void OnMatchStatement(const MatchStatement& node);
	void CanMatch(const TypeSymbol& MatchItem, const ExpressionNodeType& MatchValueNode, const ExpressionNodeType& node, MatchArmData& Data);
	

	bool MatchShouldOutPassEnumValue(const ExpressionNodeType& node);
	void MatchAutoPassEnumValueStart(MatchAutoPassEnum& V, const ExpressionNodeType& node, const ValueExpressionNode* Val, const FuncCallNode* Call);
	void MatchAutoPassEnd(MatchAutoPassEnum& V);
	void CheckAllValuesAreMatched(const TypeSymbol& MatchItem, const MatchArmData& Data);
	
	struct BuildMatch_ret
	{
		IRBlock::NewConditionalFalseJump_t JumpToUpdateIFMatchTrue;
		IRInstruction* JumpToUpdateEndIndex = 0;
	};
	struct BuildMatch_State
	{
		Vector<BuildMatch_ret> MatchList;
	};

	BuildMatch_ret BuildMatch(const TypeSymbol& MatchItem, const ExpressionNodeType& MatchValueNode,IRInstruction* Item, BuildMatch_State& State, MatchArm& Arm, const ExpressionNodeType& ArmEx);
	BuildMatch_ret BuildInvaildMatch(const TypeSymbol& MatchItem, IRInstruction* Item, BuildMatch_State& State);
	void EndMatch(BuildMatch_ret& Value, BuildMatch_State& State);
	void EndMatchState(BuildMatch_State& State);

	void OnMatchExpression(const MatchExpression& node);

	enum class LoadLibMode
	{
		GetTypes,
		FixTypes,
		Done
	};

	void ConvertAttributes(const Vector<Unique_ptr<AttributeNode>>& nodes,Vector<UsedTagValueData>& Out);
	void ConvertAttribute(const AttributeNode& nodes, UsedTagValueData Out);
	void LoadLibSymbols();
	void LoadLibSymbols(const UClib& lib, LoadLibMode Mode);

	void LoadClassSymbol(const Class_Data& Item,const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode);
	void LoadEnumSymbol(const Enum_Data& Item,const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode);

	void LoadSymbol(const ClassMethod& Item, SystematicAnalysis::LoadLibMode Mode);
	void LoadType(const ReflectionTypeInfo& Item, TypeSymbol& Out);
	TypeSymbol LoadType(const ReflectionTypeInfo& Item);

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
	void AddClass_tToAssemblyInfo(const Vector<Unique_ptr<AttributeNode>>& attributes,const Symbol* ClassSyb);
	ReflectionTypeInfo ConvertToTypeInfo(const TypeSymbol& Type);

	TypeSymbolID GetTypeID(TypesEnum Type, SymbolID SymbolId);
	TypeSymbol GetStaticArrayType(const TypeSymbol& BaseType, size_t Size);

	bool AreTheSame(const TypeSymbol& TypeA, const TypeSymbol& TypeB);
	bool AreTheSame(const ParInfo& TypeA, const ParInfo& TypeB);
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


	String ToString(const ParInfo& Type)
	{
		String R;

		if (Type.IsOutPar) 
		{
			R += "out ";
		}
		R += ToString(Type.Type);
		return R;
	}

	String ToString(const TypeSymbol& Type);
	String ToString(const TokenType& Type)
	{
		return StringHelper::ToString(Type);
	}
	


	void Convert(const TypeNode& V, TypeSymbol& Out);

	Optional<Symbol*> InstantiateOrFindGenericSymbol(const Token* Token, const UseGenericsNode& GenericsVals, const String_view& Name);

	void LogCantBindTypeItNotTypeInfo(const UCodeLang::Token* Token, UCodeLang::FrontEnd::TypeSymbol& Type);
	
	void ConvertAndValidateType(const TypeNode& V, TypeSymbol& Out,NodeSyb_t Syb);
	TypeSymbol ConvertAndValidateType(const TypeNode& V, NodeSyb_t Syb);
	bool ValidateType(const TypeSymbol& V,const Token* Token,NodeSyb_t Syb);

	

	
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
			if (IsUnMapType(*ObjectType))//Generic Test
			{
				return true;
			}


			return _ClassStack.top().Syb == objecttypesyb;
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
	bool IsCharType(const TypeSymbol& TypeToCheck);
	bool IsPrimitive(const TypeSymbol& TypeToCheck);
	bool IsPrimitiveNotIncludingPointers(const TypeSymbol& TypeToCheck);
	bool IsStaticArray(const TypeSymbol& TypeToCheck);

	bool IsimmutableRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);

	bool IsAddessAndLValuesRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type, bool ReassignMode);


	bool HasDestructor(const TypeSymbol& TypeToCheck);

	void Update_ClassSym_ToFixedTypes(Symbol* Sym);
	void Update_FuncSym_ToFixedTypes(Symbol* Sym);
	void Update_EnumSym_ToFixedTypes(Symbol* Sym);
	void Update_TraitSym_ToFixedTypes(Symbol* Sym);
	void Update_TagSym_ToFixedTypes(Symbol* Sym);
	void Update_AliasSym_ToFixedTypes(Symbol* Sym);
	void Update_EvalSym_ToFixedTypes(Symbol* Sym);

	void Update_Sym_ToFixedTypes(Symbol* Sym);

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
	Get_FuncInfo GetFunc(const ScopedNameNode& Name,const ValueParametersNode& Pars,TypeSymbol Ret);

	void RemoveTypeattributes(UCodeLang::FrontEnd::TypeSymbol& tep_);


	Get_FuncInfo GetEnumVariantFunc(Symbol* EnumSyb, size_t FeildIndex,Symbol* EnumFieldSyb, const ValueParametersNode& Pars,const Token* Token,const Vector<ParInfo>& ValueTypes);
	void SetOutExpression(const OutExpression* Ex, const TypeSymbol& TypeToSet);

	Symbol* GetSymbolFromExpression(const OutExpression* Ex);

	void SetFuncRetAsLastEx(const Get_FuncInfo& Info);
	
	struct  IsCompatiblePar
	{
		const Vector<ParInfo>* Pars;
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

	bool IsCompatible(const IsCompatiblePar& FuncPar, const Vector<ParInfo>& ValueTypes, bool _ThisTypeIsNotNull, const Token* Token);

	int GetCompatibleScore(const ParInfo& ParFunc, const ParInfo& Value);
	int GetCompatibleScore(const IsCompatiblePar& Func, const Vector<ParInfo>& ValueTypes);

	bool AccessCheck(const Symbol* Syb, const  Token* Token, const String_view Scope);
	bool AccessCheck(const Symbol* Syb, const  Token* Token)
	{
		return AccessCheck(Syb, Token, this->_Table._Scope.ThisScope);
	}

	//Generics

	void GenericFuncInstantiate(const Symbol* Func, const Vector<TypeSymbol>& GenericInput);

	String GetGenericExtendedErrValue(const Generic& Generic, const GenericValuesNode GenericAsNode, const Vector<TypeSymbol>& GenericInput);
	Optional<SymbolID> MakeTypePackSymbolIfNeeded(const String& NewName, const Vector<TypeSymbol>& GenericInput, const Generic& Generic);

	String GetGenericFuncFullName(const Symbol* Func, const Vector<TypeSymbol>& Type);
	String GetGenericFuncName(const Symbol* Func, const Vector<TypeSymbol>& Type);

	void GenericTypeInstantiate(const Symbol* Class, const Vector<TypeSymbol>& Type);
	void GenericTypeInstantiate_Trait(const Symbol* Trait, const Vector<TypeSymbol>& Type);
	void GenericTypeInstantiate_Alias(const Symbol* Alias, const Vector<TypeSymbol>& Type);
	void GenericTypeInstantiate_Enum(const Symbol* Enum, const Vector<TypeSymbol>& Type);
	void GenericTypeInstantiate_Tag(const Symbol* Trait, const Vector<TypeSymbol>& Type);

	EvaluatedEx MakeEx(const TypeSymbol& Type);
	RawEvaluatedObject MakeExr(const TypeSymbol& Type);
	void* Get_Object(const TypeSymbol& Input, const RawEvaluatedObject& Input2);
	void* Get_Object(const EvaluatedEx& Input);
	template<typename T> T* Get_ObjectAs(const TypeSymbol& Input, const RawEvaluatedObject& Input2)
	{
		if (Input2.ObjectSize == sizeof(T))
		{
			return (T*)Get_Object(Input, Input2);
		}
		else
		{
			String TepStr = "type miss-mach when EvaluatedObject To Cpp type '" + (String)typeid(T).name() + "' ";
			throw std::exception(TepStr.c_str());
		}
	}
	template<typename T> T* Get_ObjectAs(const EvaluatedEx& Input)
	{
		return Get_ObjectAs<T>(Input.Type, Input.EvaluatedObject);
	}

	template<typename T> void Set_ObjectAs(const TypeSymbol& Input,RawEvaluatedObject& Input2, const T& Value)
	{
		if (Input2.ObjectSize == sizeof(T))
		{
			auto Ptr = (T*)Get_Object(Input, Input2);
			*Ptr = Value;
		}
		else
		{
			String TepStr = "type miss-mach when Cpp type To EvaluatedObject'" + (String)typeid(T).name() + "' ";
			throw std::exception(TepStr.c_str());
		}
	}
	template<typename T> void Set_ObjectAs(EvaluatedEx& Input,const T& Value)
	{
		return Set_ObjectAs<T>(Input.Type, Input.EvaluatedObject,Value);
	}
	void Set_ObjectAs(EvaluatedEx& Input,const void* Object, size_t ObjectSize)
	{
		return Set_ObjectAs(Input.Type, Input.EvaluatedObject, Object, ObjectSize);
	}
	void Set_ObjectAs(const TypeSymbol& Input, RawEvaluatedObject& Input2,const void* Object,size_t ObjectSize)
	{
		if (Input2.ObjectSize == ObjectSize)
		{
			auto Ptr = (Byte*)Get_Object(Input, Input2);
			memcpy(Ptr, Object, ObjectSize);
		}
		else
		{
			String TepStr = "type miss-mach when Cpp type To EvaluatedObject'";
			throw std::exception(TepStr.c_str());
		}
	}

	using TypeInstantiateFunc = void(SystematicAnalysis::*)(const Symbol* Symbol,const Vector<TypeSymbol>& GenericInput);
	Symbol* InstantiateOrFindGenericSymbol(const Token* Name,const Symbol* Symbol,const GenericValuesNode& SymbolGenericValues,const Generic& GenericData,const UseGenericsNode& UseNode,TypeInstantiateFunc Instantiate)
	{
		if (GenericData._Generic.size() != UseNode.Values.size())
		{
			LogError_CanIncorrectGenericCount(Name, Name->Value._String, UseNode.Values.size(), GenericData._Generic.size());
			return nullptr;
		}


		auto GenericInput = std::make_unique<Vector<TypeSymbol>>();//pointer must be unique so it cant be on the stack
		for (size_t i = 0; i < UseNode.Values.size(); i++)
		{
			const auto& Tnode = UseNode.Values[i];
			const auto& GenericInfo = GenericData._Generic[i];
			TypeSymbol Type;
			ConvertAndValidateType(Tnode, Type, NodeSyb_t::Any);

			{
				bool InputTypeIsConstantExpression = false;

				auto TypeSyb = GetSymbol(Type);
				if (TypeSyb)
				{
					InputTypeIsConstantExpression = TypeSyb->Type == SymbolType::ConstantExpression;
				}

				if (InputTypeIsConstantExpression != GenericInfo.IsConstantExpression())
				{
					const Token* nodeToken = Tnode.Name.Token;
					auto& GenericNo = SymbolGenericValues.Values[i];
					if (InputTypeIsConstantExpression)
					{
						LogError_GenericInputWantsaExpressionNotType(nodeToken, GenericNo.Token->Value._String);
					}
					else
					{
						LogError_GenericInputWantsaExpressionNotType(nodeToken, GenericNo.Token->Value._String);
					}

					return nullptr;
				}
			}


			GenericInput->push_back(Type);
		}

		String NewName = GetGenericFuncFullName(Symbol, *GenericInput);
		auto FuncIsMade = GetSymbol(NewName, SymbolType::Type_class);
		if (!FuncIsMade)
		{
			(*this.*Instantiate)(Symbol, *GenericInput);
			
			_TepFuncs.push_back({ std::move(GenericInput) });
		}

		return GetSymbol(NewName, SymbolType::Type);
	}
	Symbol* InstantiateOrFindGeneric_Class(const Token* Name, const Symbol* Symbol, const GenericValuesNode& SymbolGenericValues, const Generic& GenericData, const UseGenericsNode& UseNode)
	{
		TypeInstantiateFunc Func = &SystematicAnalysis::GenericTypeInstantiate;
		return InstantiateOrFindGenericSymbol(Name, Symbol, SymbolGenericValues, GenericData, UseNode,Func);
	}

	Symbol* InstantiateOrFindGeneric_Trait(const Token* Name, const Symbol* Symbol, const GenericValuesNode& SymbolGenericValues, const Generic& GenericData, const UseGenericsNode& UseNode)
	{
		TypeInstantiateFunc Func = &SystematicAnalysis::GenericTypeInstantiate_Trait;
		return InstantiateOrFindGenericSymbol(Name, Symbol, SymbolGenericValues, GenericData, UseNode, Func);
	}

	Symbol* InstantiateOrFindGeneric_Alias(const Token* Name, const Symbol* Symbol, const GenericValuesNode& SymbolGenericValues, const Generic& GenericData, const UseGenericsNode& UseNode)
	{
		TypeInstantiateFunc Func = &SystematicAnalysis::GenericTypeInstantiate_Alias;
		return InstantiateOrFindGenericSymbol(Name, Symbol, SymbolGenericValues, GenericData, UseNode, Func);
	}
	Symbol* InstantiateOrFindGeneric_Enum(const Token* Name, const Symbol* Symbol, const GenericValuesNode& SymbolGenericValues, const Generic& GenericData, const UseGenericsNode& UseNode)
	{
		TypeInstantiateFunc Func = &SystematicAnalysis::GenericTypeInstantiate_Enum;
		return InstantiateOrFindGenericSymbol(Name, Symbol, SymbolGenericValues, GenericData, UseNode, Func);
	}
	Symbol* InstantiateOrFindGeneric_Tag(const Token* Name, const Symbol* Symbol, const GenericValuesNode& SymbolGenericValues, const Generic& GenericData, const UseGenericsNode& UseNode)
	{
		TypeInstantiateFunc Func = &SystematicAnalysis::GenericTypeInstantiate_Tag;
		return InstantiateOrFindGenericSymbol(Name, Symbol, SymbolGenericValues, GenericData, UseNode, Func);
	}

	StrExELav GetStrEVal(const Node* node);
	bool ConstantExpressionAbleType(const TypeSymbol& Type);
	bool EvaluateDefaultConstructor(EvaluatedEx& Out);
	bool Evaluate(EvaluatedEx& Out, const ValueExpressionNode& node);
	bool Evaluate(EvaluatedEx& Out, const BinaryExpressionNode& node);
	bool Evaluate(EvaluatedEx& Out, const CastNode& node);
	bool Evaluate(EvaluatedEx& Out, const ReadVariableNode& nod);
	bool Evaluate_t(EvaluatedEx& Out, const Node* node, GetValueMode Mode);
	bool Evaluate(EvaluatedEx& Out, const ExpressionNodeType& node, GetValueMode Mode);
	bool EvaluatePostfixOperator(EvaluatedEx& Out, TokenType Op);
	bool HasConstantPostfixOperator(const TypeSymbol& Type, TokenType Op);
	bool CanEvaluateImplicitConversionConstant(const TypeSymbol& Type, const TypeSymbol& ToType);
	bool EvaluateImplicitConversion(EvaluatedEx& In, const TypeSymbol& ToType, EvaluatedEx& out);
	bool EvalutateCMPTypesNode(EvaluatedEx& Out, const CMPTypesNode& node);
	bool EvalutateValidNode(EvaluatedEx& Out, const ValidNode& node);
	bool EvalutateFunc(EvaluatedEx& Out, const FuncCallNode& node);
	bool EvalutateFunc(EvaluatedEx& Out, const Get_FuncInfo& Func, const ScopedNameNode& Name, const Vector<EvaluatedEx>& Pars);
	bool EvalutateFunc(EvaluatedEx& Out, const TypeSymbol& Type, const Get_FuncInfo& Func, const Vector<EvaluatedEx>& ValuePars);
	bool Evaluate(EvaluatedEx& Out, const ExtendedScopeExpression& node);
	bool Evaluate(EvaluatedEx& Out, const ExtendedFuncExpression& node);
	
	EvaluatedEx& Evaluate_GetPointer()
	{

	}

	bool EvalutateFunc(EvalFuncData& State, const Symbol* Func, const Vector<EvaluatedEx>& Pars);
	bool EvalutateStatement(EvalFuncData& State, const Node* node);

		

	bool EvalutateScopedName(EvaluatedEx& Out, size_t Start, size_t End, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& OtherOut);
	bool EvalutateScopedName(EvaluatedEx& Out, size_t Start, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& OtherOut)
	{
		return EvalutateScopedName(Out, Start, -1, node, OtherOut);
	}
	bool EvalutateScopedName(EvaluatedEx& Out, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& OtherOut)
	{
		return EvalutateScopedName(Out, 0, node, OtherOut);
	}
	bool EvalutateStepScopedName(EvaluatedEx& Out, const ScopedNameNode& node, size_t Index, ScopedName::Operator_t OpType, GetMemberTypeSymbolFromVar_t& OtherOut);

	bool CanEvalutateFuncCheck(const Get_FuncInfo& Func);

	bool Evaluate(EvaluatedEx& Out, const TypeSymbol& MustBeType, const ExpressionNodeType& node);
	Optional<EvaluatedEx> Evaluate(const TypeSymbol& MustBeType, const ExpressionNodeType& node);
	Optional<EvaluatedEx> Evaluate(const TypeSymbol& MustBeType, const Node& node)
	{
		return  Evaluate(MustBeType, *ExpressionNodeType::As(&node));
	}

	void SetOutExpressionEval(const OutExpression* Ex, const EvaluatedEx& ObjectToSet);

	bool EvaluateToAnyType(EvaluatedEx& Out, const ExpressionNodeType& node);
	Optional<EvaluatedEx> EvaluateToAnyType(const ExpressionNodeType& node);
	String ToString(const TypeSymbol& Type, const RawEvaluatedObject& Data);
	IRInstruction* RawObjectDataToCString(const RawEvaluatedObject& EvalObject);
	//IR
	void DoFuncCall(Get_FuncInfo Func, const ScopedNameNode& Name, const ValueParametersNode& Pars);
	void DoFuncCall(const TypeSymbol& Type, const Get_FuncInfo& Func, const ValueParametersNode& ValuePars);
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

	void LogError_CantFindCompoundOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type);
	void LogError_CantFindPostfixOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type);
	void LogError_CantFindBinaryOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type);

	void LogError_ExpressionMustbeAnLocationValueError(const Token* Token, TypeSymbol& Ex0Type);
	void LogError_YouMustReturnSomethingError(const Token* Token);
	void LogError_CantguessVarTypeError(const Token* Token);
	void LogError_CantUseThisKeyWordHereError(const Token* NameToken);
	void LogError_CantCastImplicitTypes(const Token* Token, const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType, bool ReassignMode);
	void LogError_ReadingFromInvaidVariable(const Token* Token, String_view Str);
	void LogError_CantFindVarError(const Token* Token, String_view Str);
	void LogError_CantFindVarMemberError(const Token* Token, String_view Str, const TypeSymbol& OnType);
	void LogError_CantModifyiMutableError(const Token* Token, String_view Name);
	void LogError_CantCastExplicityTypes(const Token* Token, TypeSymbol& Ex0Type, TypeSymbol& ToTypeAs);
	void LogError_CantFindTypeError(const Token* Token, String_view Name);
	void LogError_TypeDependencyCycle(const Token* Token, const ClassInfo* Value);
	void LogError_CantUseThisHere(const Token* Token);

	void LogError_CanIncorrectParCount(const Token* Token, String_view FuncName, size_t Count, size_t FuncCount);
	void LogError_CanIncorrectGenericCount(const Token* Token, String_view FuncName, size_t Count, size_t FuncCount);

	void LogError_CantFindFuncError(const Token* Token, String_view FuncName,
		const Vector<TypeSymbol>& Generics,
		const Vector<TypeSymbol>& WithTypes,
		const TypeSymbol& RetType);
	void LogError_CantFindFuncError(const Token* Token, String_view FuncName,
		const Vector<TypeSymbol>& Generics,
		const Vector<ParInfo>& WithTypes,
		const TypeSymbol& RetType);

	void LogError_TypeMustBeAnConstantExpressionAble(const Token* Token, const TypeSymbol& Type);
	void LogError_CantFindPostfixOpForTypes_Constant(const Token* BinaryOp, TypeSymbol& Ex0Type);
	void LogError_Eval_CantDoPostfixOpForTypes(const Token* BinaryOp, TypeSymbol& Ex0Type);
	void LogError_Eval_CantCastImplicitTypes(const Token* Token, const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType);
	void LogError_CantFindNamespace(const Token* Token, const String_view Namespace);
	void LogError_LogFuncMustBe(const Token* Token, const String_view FuncName, TypeSymbol& TypeSybToBe);
	void LogError_SymbolRedefinition(const Token* Token, const Symbol* Symbol);
	void LogError_UseingVarableBeforDeclared(const Token* Token);
	void LogError_BeMoreSpecifiicForRetType(const String_view FuncName, const Token* Token);
	void LogError_CantBeIndexWithType(const Token* Token, const  TypeSymbol& Ex0Type, const  TypeSymbol& IndexType);
	void LogError_CantUseThisInStaticFunction(const Token* Token);
	void LogError_FuncDependencyCycle(const Token* Token, const FuncInfo* Value);

	void LogError_GenericInputWantsaExpressionNotType(const Token* Token,const String_view NameOfPar);
	void LogError_GenericInputWantsaTypeNotExpression(const Token* Token, const String_view NameOfPar);
	
	void LogError_ExpectedSymbolToBea(const Token* Token, const Symbol& Syb, SymbolType Value);
	
	void LogError_CanIncorrectStaticArrCount(const Token* Token, const TypeSymbol& Type, size_t Count, size_t FuncCount);
	void LogError_BeMoreSpecifiicWithStaticArrSize(const Token* Token, const TypeSymbol& Type);
	void LogError_BinaryOverloadPars(const Token& Name, const FuncInfo* Func);
	void LogError_IndexOverloadPars(const Token& Name, const FuncInfo* Func);
	void LogError_PostfixOverloadPars(const Token& Name, const FuncInfo* Func);
	void LogError_CantOverLoadOverload(const Token* NameToken);
	void LogError_CantFindMemberOverloadForType(const Token* Item, TokenType Op, const TypeSymbol& Out);
	void LogError_CantUseMoveTypeHere(const Token* Token);
	void LogError_MustMakeEnumLikeafuncion(EnumInfo* Einfo, size_t Index, const Token* Token);
	void LogError_DynamicMustBeRrait(const TypeNode& V, const TypeSymbol& Out);
	void LogError_TraitCantBeAlone(const Token* Token);
	void LogError_WantedAType(const TypeNode& V, Symbol* SybV);
	void LogError_OutCanOnlyBeInControlFlow(const Token* Token);
	void LogError_ParamterMustBeAnOutExpression(const Token* Token, const size_t& i);
	void LogError_LogInvalidNodeError(const Token* Token, String_view ErrStr);
	void LogError_EmptyInvalidError(const Token* Token);
	void LogError_LogParPackIsNotLast(const Token* Token);
	void LogError_ParPackTypeIsNotLast(const Token* Token);
	void LogError(ErrorCodes Err, const String& MSG, const Token* Token);
	void LogError(ErrorCodes Err,size_t Line,size_t Pos, const String& MSG);

	String ToString(SymbolType Value);
	ReadVarErrorCheck_t LogTryReadVar(String_view VarName, const Token* Token, const Symbol* Syb);
	void CheckVarWritingErrors(Symbol* Symbol, const Token* Token, String_view& Name);
	void LogError_LogWantedAVariable(const Token* const& Item, Symbol* TepSyb);
	Class_Data* GetAssemblyClass(const String& FullName);

	String MangleName(const FuncInfo* Func);
	IRidentifierID GetIRID(const FuncInfo* Func);

	GenericData::Type GenericTypeToGenericDataType(GenericValueNode::GenericType type);
};
UCodeLangFrontEnd


