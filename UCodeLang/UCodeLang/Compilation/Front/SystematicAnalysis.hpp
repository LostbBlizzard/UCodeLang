#pragma once
#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"


#include "../Middle/IR.hpp"
#include "UCodeLang/Compilation/Back/BackEndInterface.hpp"
#include "../Middle/Symbol.hpp"
#include "UCodeFrontEndNameSpace.hpp"
#include "UCodeLang/LangCore/DataType/Set.hpp"
UCodeLangFrontStart

#define GenericTestStr CompilerGenerated("___GenericTest")
#define ForTypeScope CompilerGenerated("ForType")
#define TraitClassEnd CompilerGenerated("TraitClass")

constexpr size_t EnumVarantKeyIndex = 0;
constexpr size_t EnumVarantUnionIndex = 1;

constexpr size_t DymTraitIRPointerIndex = 0;
constexpr size_t DymTraitIRVTableIndex = 1;

struct Systematic_BinaryOverloadData
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
	inline static const Array<Data, 4> data =
	{
		Data(TokenType::plus,Overload_Plus_Func,FuncInfo::FuncType::plus),
		Data(TokenType::minus,Overload_minus_Func,FuncInfo::FuncType::minus),

		Data(TokenType::equal_Comparison,Overload_equal_Func,FuncInfo::FuncType::equal),
		Data(TokenType::Notequal_Comparison,Overload_notequal_Func,FuncInfo::FuncType::notequal),
	};
	static bool IsBinaryOverload(FuncInfo::FuncType Type)
	{
		for (auto& Item : data)
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
	inline static const Array<Data, 2> data =
	{
		Data(TokenType::increment,Overload_increment_Func,FuncInfo::FuncType::increment),
		Data(TokenType::decrement,Overload_decrement_Func,FuncInfo::FuncType::decrement),
	};
	static bool IsPostfixOverload(FuncInfo::FuncType Type)
	{
		for (auto& Item : data)
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
	inline static const Array<Data, 3> data =
	{
		Data(TokenType::Not,Overload_Not_Func ,FuncInfo::FuncType::Not),
		Data(TokenType::bitwise_not,Overload_Bitwise_Not  ,FuncInfo::FuncType::Not),
		Data(TokenType::QuestionMark, Overload_exist_Func,FuncInfo::FuncType::Not),
	};
	static bool IsUrinaryOverload(FuncInfo::FuncType Type)
	{
		for (auto& Item : data)
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
	inline static const Array<Data, 2> data =
	{
		Data(TokenType::CompoundAdd,Overload_Compoundplus_Func ,FuncInfo::FuncType::Compound_plus),
		Data(TokenType::CompoundSub,Overload_CompoundSub_Func,FuncInfo::FuncType::Compound_Minus),
	};
	static bool IsCompoundOverload(FuncInfo::FuncType Type)
	{
		for (auto& Item : data)
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
	inline static const Array<Data_t, 3> data =
	{
		Data_t(TokenType::IndirectMember,Overload_IndirectMember_Func ,FuncInfo::FuncType::IndirectMember),
		Data_t(TokenType::OptionalDot, Overload_OptionalDot_Func,FuncInfo::FuncType::OptionalDot),
		Data_t(TokenType::ExclamationDot, Overload_ExclamationDot_Func,FuncInfo::FuncType::ExclamationDot),
	};
	static bool IsMemerOverload(FuncInfo::FuncType Type)
	{
		for (auto& Item : data)
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
		for (auto& Item : data)
		{
			if (Item.token == type)
			{
				return Opt(&Item);
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
		bool IsOutPar = false;
		TypeSymbol Type;
		const Node* ExpressionNode = nullptr;
	};
	struct FunctionData
	{
		String FuncName;
		ID _ID = ID::Null;

		bool IsMemberFunction = false;
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
		const ClassInfo* _ClassInfo = nullptr;
		const FieldInfo* Field = nullptr;
	};
	struct EnumField
	{
		const EnumInfo* _EnumInfo = nullptr;
		const EnumFieldInfo* Field = nullptr;
	};

	static Optional<Func> GetFunction(const String_view Name, const Vector<FunctionPar>& Pars, SystematicAnalysis& This);
};

struct ScopeGenerator
{
	inline String GetScopeLabelName(const String& currentscope, const void* node)
	{
		constexpr const char* ScopePrefix = "_";

		if (!_Data.HasValue(currentscope))
		{
			auto newlist = CurrentScopeInfo();

			_Data.AddValue(currentscope, std::move(newlist));
		}


		auto& list = _Data.GetValue(currentscope);
		if (list.NodeToIndex.HasValue(node))
		{
			auto index = list.NodeToIndex.GetValue(node);
			return String(ScopePrefix) + std::to_string(index);
		}
		else
		{
			auto index = list.GetNextIndex();
			list.NodeToIndex.AddValue(node, index);
			return String(ScopePrefix) + std::to_string(index);
		}
	}
	void Clear()
	{
		_Data.clear();
	}
private:
	struct CurrentScopeInfo
	{
		UnorderedMap<const void*, size_t> NodeToIndex;
		size_t GetNextIndex()
		{
			return NodeToIndex.size() + 1;
		}
	};
	UnorderedMap<String, CurrentScopeInfo> _Data;
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
	UCodeLangForceinline void Set_ErrorsOutput(CompilationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompilationSettings* V) { _Settings = V; }


	bool Analyze(const FileNode& File);
	bool Analyze(const Vector<NeverNullPtr<FileNode>>& Files, const Vector<NeverNullPtr<UClib>>& Libs);

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
		Vector<NeverNullPtr<FileNode_t>> _Dependencys;

		struct ImportData
		{
			String ImportSymbolFullName;
			Vector<Symbol*> _AliasSymbols;


			Vector<String> _TepSymbolFullNames;
			bool* IsImportUsed = nullptr;
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
	const FileNodeData& GetFileDataPub(const  NeverNullPtr<FileNode_t> file) const { return *_FilesData.GetValue(file); }
	void SetLibNames(const Vector <Path>* LibsNames)
	{
		_LibsNames = LibsNames;
	}
	void ToIntFile(FileNode_t* File, const Path& path);
private:
	FileNodeData& GetFileData(const NeverNullPtr<FileNode_t> file) { return *_FilesData.GetValue(file); }

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
		Symbol* FuncNext = nullptr;
	};
	using CastExpressionNode_Data = PostFixExpressionNode_Data;
	using AssignExpression_Data = BinaryExpressionNode_Data;
	using UnaryExpression_Data = PostFixExpressionNode_Data;
	enum class  ObjectToDropType
	{
		IRInstruction,
		IRInstructionNoMod,
		Operator,
	};
	struct ObjectToDrop
	{
		ObjectToDropType DropType = ObjectToDropType::IRInstruction;
		SymbolID ID;


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
	struct BlockContext
	{
		String Scope;
		bool PassedYield = false;
		bool IsUnSafeBlock = false;
	};
	struct FuncStackInfo
	{
		FuncInfo* Pointer = nullptr;
		bool IsOnRetStatemnt = false;
		Vector<BlockContext> BlockContexts;
		FuncStackInfo(FuncInfo* V)
		{
			Pointer = V;
			BlockContexts.push_back({});
		}
	};
	struct IRLocation_Cotr
	{
		IRInstruction* Value = nullptr;
		bool UsedlocationIR = false;
	};
	struct GetMemberTypeSymbolFromVar_t
	{
		Symbol* _Symbol = nullptr;
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

		WritePointerReassment,
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
		Optional<TypeSymbol> BulitInTypeContructer;
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

	using EvalPointer = UInt32;

	struct EvalSharedState
	{
		struct MemLoc
		{
			TypeSymbol type;
			RawEvaluatedObject* pointer;
		};
		UnorderedMap<EvalPointer, MemLoc> ActivePointers;

		EvalPointer _NextEvalPointer = 1;//must non non-zero because people will use 0 as a invaild pointer


		EvalPointer GetNewEvalPointer()
		{
			auto v = _NextEvalPointer;
			_NextEvalPointer++;
			return v;
		}

		EvalPointer GivePointerAccess(TypeSymbol type, RawEvaluatedObject* pointer)
		{
			auto r = GetNewEvalPointer();

			ActivePointers.AddValue(r, { type,pointer });

			return r;
		}
		EvalPointer GivePointerAccess(EvaluatedEx* pointer)
		{
			return GivePointerAccess(pointer->Type, &pointer->EvaluatedObject);
		}

		void RemoveAccess(RawEvaluatedObject* pointer)
		{
			for (auto& Item : ActivePointers)
			{
				if (Item.second.pointer == pointer)
				{
					return;
				}
			}
			UCodeLangUnreachable();
		}
		void RemoveAccess(EvaluatedEx* pointer)
		{
			RemoveAccess(&pointer->EvaluatedObject);
		}

		bool PointerWrite(EvalPointer pointer, size_t offset, RawEvaluatedObject& val)
		{
			if (ActivePointers.HasValue(pointer))
			{
				auto& p = ActivePointers.GetValue(pointer);

				void* ptr = p.pointer->Object_AsPointer.get() + offset;



				UCodeLangAssert((p.pointer->ObjectSize - offset) >= val.ObjectSize);

				memcpy(ptr, val.Object_AsPointer.get(), val.ObjectSize);
				return true;
			}
			else
			{
				return false;
			}
		}
		bool PointerRead(EvalPointer pointer, size_t offset, RawEvaluatedObject& out)
		{
			if (ActivePointers.HasValue(pointer))
			{
				return false;
			}
			else
			{
				return false;
			}
		}
	};
	struct EvalFuncData
	{
		UnorderedMap<SymbolID, RawEvaluatedObject> Pars;
		RawEvaluatedObject Ret;

		UnorderedMap<SymbolID, RawEvaluatedObject> Vars;

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
		const MatchAutoPassEnum& Get_AutoPassEnum() const
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
		TypeSymbol  _MatchAssignmentType;
		MatchArmData ArmData;
	};
	struct AwaitData
	{

	};
	struct YieldData
	{

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
	struct RetData
	{
		IRInstruction* JumpIns = nullptr;
	};
	enum class NodeSyb_t
	{
		Any,
		Variable,
		Parameter,
		ClassField,
		Ret,
	};
	struct CompileTimeforNode
	{
		Symbol* SybToLoopOver = nullptr;
		Vector<Symbol*> SybItems;
		Vector<Symbol*> SymOtherItems;
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
		void SetAsFunctorPtrInfo(Symbol* Item)
		{
			FunctorInfo* Info = Item->Get_Info<FunctorInfo>();
			Pars = &Info->Pars;
			Ret = &Info->Ret;
			this->Item = Item;
		}
	};
	struct StrExELav
	{

		String OwnedStr;
		String_view Msg;
		bool IsConstantExpression = false;
	};

	enum class LoadLibMode
	{
		GetTypes,
		FixTypes,
		Done
	};
	struct BuildMatch_ret
	{
		IRBlock::NewConditionalFalseJump_t JumpToUpdateIFMatchTrue;
		IRInstruction* JumpToUpdateEndIndex = 0;
	};
	struct BuildMatch_State
	{
		Vector<BuildMatch_ret> MatchList;
	};
	struct GeneratedGenericSymbolData
	{
		SymbolID ID = SymbolID();
		GenericFuncInfo Info;
		Vector<TypeSymbol> Types;
	};
	struct CaptureErrorContext
	{
		size_t errcount = 0;
	};

	struct DoBinaryOpContext
	{
		const RawEvaluatedObject* Op1 = nullptr;
		const RawEvaluatedObject* Op2 = nullptr;
		TokenType type = TokenType::Null;
		RawEvaluatedObject* OpOut = nullptr;
	};
	//Members
	CompilationErrors* _ErrorsOutput = nullptr;
	CompilationSettings* _Settings = nullptr;
	UClib _Lib;

	PassType _PassType = PassType::Null;

	const Vector<NeverNullPtr<FileNode>>* _Files = nullptr;
	const Vector<NeverNullPtr<UClib>>* _Libs = nullptr;
	const Vector<Path>* _LibsNames = nullptr;
	Vector<FileNode> _LibsFiles;
	SymbolTable _Table;

	UnorderedMap<String, UnorderedMap<const void*, SymbolID>> _SybIdMap;
	uintptr_t _IDIndex = 0;
	ScopeGenerator _ScopeGenerator;
	//Args
	bool _ForceImportArgWasPassed = false;
	bool _ImmutabilityIsForced = false;
	Optional<String> _StartingNameSpace;

	Stack<ClassStackInfo> _ClassStack;


	const FileNode* _LookingAtFile = nullptr;
	UnorderedMap<NeverNullPtr<FileNode_t>, Shared_ptr<FileNodeData>> _FilesData;
	UnorderedMap<SymbolID, BinaryExpressionNode_Data> _BinaryExpressionNode_Datas;
	UnorderedMap<SymbolID, IndexedExpresion_Data> _IndexedExpresion_Datas;
	UnorderedMap<SymbolID, PostFixExpressionNode_Data> _PostFix_Datas;
	UnorderedMap<SymbolID, CompoundExpresion_Data> _Compound_Datas;
	UnorderedMap<SymbolID, ForExpresion_Data> _For_Datas;
	UnorderedMap<SymbolID, CastExpressionNode_Data> _CastDatas;
	UnorderedMap<SymbolID, AssignExpression_Data > _AssignExpressionDatas;
	UnorderedMap<SymbolID, bool> _ValidNodes;
	UnorderedMap<SymbolID, CompileTimeforNode> _ForNodes;
	UnorderedMap<SymbolID, MatchStatementData> _MatchStatementDatas;
	UnorderedMap<SymbolID, MatchExpressionData> _MatchExpressionDatas;
	UnorderedMap<SymbolID, AwaitData>  _AwaitDatas;
	UnorderedMap<SymbolID, YieldData>  _YieldDatas;
	UnorderedMap<SymbolID, UnaryExpression_Data>  _UnaryDatas;

	UnorderedMap<SymbolID, VarableMemberData> _VarableMemberDatas;//Var.$Item
	UnorderedMap<AssemblyNode*, Symbol*> LibGenericSymbolLoad;
	UnorderedMap<SymbolID, TypeSymbol> DelareVarableImplicit;
	UnorderedMap<TypeSymbol*, String> GenericOutputs;
	struct Test
	{
		const CompileTimeIfNode* node = nullptr;
		SymbolContext Context;
		SymbolID _SymID;
	};
	struct Test1
	{
		Vector<String_view> PossibleSymbolNames;
		Test node;
	};
	Vector<Test1> NodeCompileTimeIfs;

	Vector<FuncStackInfo> _FuncStack;


	NullablePtr<Token> _LastLookedAtToken = nullptr;

	Vector<NodeType> _NodeTypeStack;
	Vector<String> _ExtendedErr;
	//
	Stack<TypeSymbol> _LookingForTypes;
	TypeSymbol _LastExpressionType;
	UnorderedMap<SymbolID, Get_FuncInfo> _FuncToSyboID;
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
	Stack<GenericFuncInfo> _Generic_GenericSymbolStack;
	Vector<JumpsData> _IR_Jumps;
	Vector<RetData> _IR_Rets;
	String _LastIRFileName;
	size_t _LastLineNumber = -1;

	UnorderedSet<SymbolID> _ConstantExpressionMap;
	Stack<GetValueMode> _GetExpressionMode;
	Stack<VarableUseData> _Varable;
	Vector<Unique_ptr<EvalFuncData>> _Eval_FuncStackFrames;
	Vector<EvalSharedState> _SharedEvalStates;

	NullablePtr<EvalSharedState> GetSharedEval()
	{
		if (_SharedEvalStates.size())
		{
			return &_SharedEvalStates.front();
		}
		return {};
	}

	//To Fix Types being Loaded out of order.
	Vector<LibLoadTypeSeter> _Lib_TypesToFix;

	UnorderedMap<SymbolID, IRidentifierID> _Symbol_SybToIRMap;

	IRBlockDebugInfo* _Debug_LastLookAtDebugBlock = nullptr;
	Optional<SymbolID> _Type_UnMapTypeSymbol;
	Vector<GeneratedGenericSymbolData> _Generic_GeneratedGenericSymbol;
	Vector<Symbol*> _GeneratedTraitSymbols;
	Vector<NeverNullPtr<Symbol>> _InlineEnums;

	Vector<Unique_ptr<String>> StringsFromLoadLib;

	Vector<Unique_ptr<FileNode>> NodesFromLoadLib;
	Vector<Unique_ptr<Vector<Token>>> TokensFromLoadLib;

	struct FuncGenericIndirectInfo2
	{
		Symbol* Indirect = nullptr;
	};
	Vector<FuncGenericIndirectInfo2> IndirectGenericFunc;
	bool IsExported(SymbolID type);

	//Funcs
	bool IsInUnSafeBlock()
	{
		if (_FuncStack.size())
		{
			auto& func = _FuncStack.front();
			for (auto& Item : func.BlockContexts)
			{
				if (Item.IsUnSafeBlock)
				{
					return true;
				}
			}
			return false;
		}
		return false;
	}
	bool IsAfterYield()
	{
		if (_FuncStack.size())
		{
			return _FuncStack.front().BlockContexts.front().PassedYield;
		}
		return false;
	}
	void SetAfterYeld()
	{
		if (_FuncStack.size())
		{
			_FuncStack.front().BlockContexts.front().PassedYield = true;
		}
	}

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
		R.Scope.ThisScope = ScopeHelper::GetReMoveScope(_Table._Scope.ThisScope);
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

	void Set_SymbolContext(const SymbolContext& Context)
	{
		_LookingAtFile = Context.File;
		_Table._Scope = Context.Scope;
		_Table.Useings = Context.Useings;
		_ClassStack = Context._ClassStack;
	}
	void Set_SymbolContext(SymbolContext&& Context)
	{
		_LookingAtFile = std::move(Context.File);
		_Table._Scope = std::move(Context.Scope);
		_Table.Useings = std::move(Context.Useings);
		_ClassStack = std::move(Context._ClassStack);
	}
	CaptureErrorContext GetErrorCaptureContext();
	bool ErrorCaptureHasErrors(const CaptureErrorContext& capture);


	void Push_ExtendedErr(String Err, const NeverNullPtr<Token> token)
	{
		_ExtendedErr.push_back(Err + ". On line " + std::to_string(token->OnLine));
	}
	void Pop_ExtendedErr()
	{
		_ExtendedErr.pop_back();
	}

	void Lib_BuildLibs(bool DoIR);
	void Lib_BuildLib(const UClib& lib, const Path& LibName, bool DoIR);
	static bool IsWrite(GetValueMode Value)
	{
		return Value == GetValueMode::Write
			|| Value == GetValueMode::WritePointerReassment || Value == GetValueMode::ReadAndWrite;
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

	IRidentifierID IR_Build_ConvertToIRClassIR(const Symbol& Class);
	IRidentifierID IR_Build_ConvertToStaticArray(const Symbol& Class);
	IRidentifierID IR_Build_ConveToIRVariantEnum(const Symbol& Class);



	IRType IR_ConvertToIRType(const ParInfo& Value)
	{
		if (Value.IsOutPar)
		{
			return IRType(IRTypes::pointer);
		}
		return IR_ConvertToIRType(Value.Type);
	}
	IRType IR_ConvertToIRType(const TypeSymbol& Value);
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

	inline bool Symbol_IsDependencies(const ClassInfo* Value) const
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
	inline void Push_ClassDependencie(const ClassInfo* Value)
	{
		_ClassDependencies.push_back(Value);
	}
	inline void Pop_ClassDependencie()
	{
		_ClassDependencies.pop_back();
	}
	inline bool Symbol_IsDependencies(const FuncInfo* Value)
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



	inline void Push_ToNodeScope(const Node& node)
	{
		_NodeTypeStack.push_back(node.Get_Type());
	}
	inline void Pop_NodeScope()
	{
		_NodeTypeStack.pop_back();
	}


	inline FuncStackInfo* Symbol_GetDependencies(const FuncInfo* Value)
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
	inline bool Symbol_IsRetDependencies(const FuncInfo* Value)
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
	void Debug_Add_SetLineNumber(const NeverNullPtr<Token> token, size_t InsInBlock);
	void Debug_Add_SetVarableInfo(const Symbol& Syb, size_t InsInBlock);

	const NeverNullPtr<FileNode> FileDependency_Get_FileUseingSymbol(const NeverNullPtr<Symbol> Syb);
	void FileDependency_AddDependencyToCurrentFile(const NeverNullPtr<Symbol> Syb);
	void FileDependency_AddDependencyToCurrentFile(const NeverNullPtr<FileNode> file);
	void FileDependency_AddDependencyToCurrentFile(const TypeSymbol& type);
	Symbol& Symbol_AddSymbol(SymbolType type, const String& Name, const String& FullName, AccessModifierType Access);
	bool Symbol_IsVarableType(SymbolType type) const;
	void Pass();
	void UpdateIndirectExportFuncs();
	void UpdateIndirectExport();
	void OnFileNode(const FileNode& File);
	void OnClassNode(const ClassNode& node);
	void OnAliasNode(const AliasNode& node);
	void OnUseingNode(const UsingNode& node);

	CapturedUseStatements Generic_GetCurrentUseStatements();
	void Generic_InitGenericalias(const GenericValuesNode& GenericList, bool IsgenericInstantiation, Generic& Out);
	void Generic_GenericAliasFixTypes(const GenericValuesNode& GenericList, bool IsgenericInstantiation, Generic& Out);
	//Because we support function overloading
	//we need a way for symbols FullNames to not Clash
	//OnFuncNode will Mangling those symbol and this will remove it
	String RemoveSymboolFuncOverloadMangling(const String_view fullname);

	void OnFuncNode(const FuncNode& node);
	String GetImplementationFromFunc(String_view filetext, const Token* nametoken, const Token* endtoken);
	void Node_InStatetements(bool Value);
	void Str_FuncGetName(const NeverNullPtr<Token> NameToken, String_view& FuncName, FuncInfo::FuncType& FuncType);

	String Str_GetEnumVariantUnionName(const String& FullName);
	String Str_GetUnrefencedableName(const String& FullName);
	String Str_GetScopedNameAsString(const ScopedNameNode& node);

	void OnStatement(const Node& node2);
	void OnRetStatement(const RetStatementNode& node);
	void OnEnum(const EnumNode& node);
	void OnNamespace(const NamespaceNode& node);
	void OnAttributeNode(const AttributeNode& node, OptionalRef<Vector<Symbol*>> Out = {});
	void OnAttributesNode(const Vector<Unique_ptr<AttributeNode>>& nodes, OptionalRef<Vector<Symbol*>> Out = {});
	void OnDeclareVariablenode(const DeclareVariableNode& node, DeclareStaticVariableNode_t type);

	bool IsEnableAttribute(const Symbol& symbol);
	struct EnableAttributeData
	{
		bool IsEnable = true;
		String ErrorMsg;
	};
	EnableAttributeData GetEnableAttribute(const Symbol& symbol);

	void IR_Build_OnStoreVarable(bool IsStructObjectPassRef, IRInstruction* OnVarable, Symbol* syb, const SymbolID& sybId);
	void IR_Build_AddDestructorToStack(const NeverNullPtr<Symbol> syb, const SymbolID& sybId, IRInstruction* OnVarable);
	void IR_Build_AddDestructorToStack(const TypeSymbol& Type, IRInstruction* OnVarable);
	void Type_DeclareVariableTypeCheck(TypeSymbol& VarType, const TypeSymbol& Ex, const NeverNullPtr<Token> Token);
	void Type_DeclareVarableCheck(TypeSymbol& VarType, const Node* Ex, const NeverNullPtr<Token> Token);
	bool Type_IsTypeExported(TypeSymbol type);

	void OnAssignExpressionNode(const AssignExpressionNode& node);
	void OnIfNode(const IfNode& node);
	void OnWhileNode(const WhileNode& node);
	void OnDoNode(const DoNode& node);
	void OnDeclareStaticVariableNode(const DeclareStaticVariableNode& node);
	void OnDeclareThreadVariableNode(const DeclareThreadVariableNode& node);

	void OnDeclareEvalVariableNode(const DeclareEvalVariableNode& node);
	void FuncRetCheck(const Token& Name, const NeverNullPtr<Symbol> FuncSyb, const FuncInfo* Func);

	void OnForNode(const ForNode& node);

	void IR_Build_UpdateJumpsBreakContiunes(size_t JumpIndex, size_t BoolCode, size_t BreakCode);

	void OnContinueNode(const ContinueNode& node);
	void OnBreakNode(const BreakNode& node);
	void OnLambdaNode(const LambdaNode& node);

	bool IsSymbolLambdaObjectClass(const NeverNullPtr<Symbol> Sym) const;
	bool IsLambdaClassSymFromThisPtr(const NeverNullPtr<Symbol> LambdaClassSym, const NeverNullPtr<Symbol> Sym) const;

	void OnTrait(const TraitNode& node);
	void OnTag(const TagTypeNode& node);
	void OnBitCast(const BitCastExpression& node);
	void OnImportNode(const ImportStatement& node);

	void OnCompileTimeIfNode(const CompileTimeIfNode& node, bool IsInFunc = true);

	void OnCompileTimeforNode(const CompileTimeForNode& node);
	void CompileTimeforNodeEvaluateStatements(const CompileTimeForNode& node);


	TypeSymbol Type_GetUnMapType();
	bool Type_IsUnMapType(const TypeSymbol& Type) const;
	bool Type_IsUnMapType(const Symbol& Type) const;

	void OnCompareTypesNode(const CMPTypesNode& node);
	bool Type_Compare(const TypeSymbol& Type0, const TypeSymbol& Type1, const NeverNullPtr<Token> Value);



	String Str_GetClassWithTraitVTableName(const String& ClassFullName, const String& TraitFullName);
	String Str_GetTraitVTableName(const String& TraitName);
	String Str_GetTraitVStructTableName(const String& TraitName);

	void Symbol_InheritTrait(NeverNullPtr<Symbol> Syb, ClassInfo* ClassInfo, const NeverNullPtr<Symbol> Trait, const NeverNullPtr<Token> ClassNameToken);

	void Symbol_BuildTrait(const NeverNullPtr<Symbol> Syb, ClassInfo* ClassInfo, const NeverNullPtr<Symbol> Trait, const NeverNullPtr<Token> ClassNameToken);


	NeverNullPtr<Symbol> Symbol_MakeNewDropFuncSymbol(ClassInfo* ClassInfo, const TypeSymbol& ClassAsType);
	void IR_Build_FuncDropUsingFields(const ClassInfo* ClassInfo, const IRType& ThisPar);


	void OnStatementsWithSetableRet(const StatementsNode& node, TypeSymbol& RetOut, const NeverNullPtr<Token> Token);

	String Str_GetLambdaFuncTepName(const String& LambdaName)
	{
		return LambdaName + "ptr";
	}

	bool Type_IsStructPassByRef(const NeverNullPtr<Symbol> syb) const;

	bool Symbol_MemberTypeSymbolFromVar(size_t Start, size_t End, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out);
	bool Symbol_MemberTypeSymbolFromVar(size_t Start, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out)
	{
		return Symbol_MemberTypeSymbolFromVar(Start, -1, node, Out);
	}
	bool Symbol_MemberTypeSymbolFromVar(const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& Out)
	{
		return Symbol_MemberTypeSymbolFromVar(0, node, Out);
	}
	bool Symbol_StepGetMemberTypeSymbolFromVar(const ScopedNameNode& node, size_t Index, ScopedName::Operator_t OpType, GetMemberTypeSymbolFromVar_t& Out);



	void IR_Build_MemberDereferencStore(const GetMemberTypeSymbolFromVar_t& In, IRInstruction* Value);
	void IR_Build_Member_Store(const GetMemberTypeSymbolFromVar_t& In, IRInstruction* Value);

	IROperator IR_Build_Member_Store(const GetMemberTypeSymbolFromVar_t& In);
	IROperator IR_Build_Member_DereferencStore(const GetMemberTypeSymbolFromVar_t& In);
	IROperator IR_Build_Member_Store(const GetMemberTypeSymbolFromVar_t& In, const TypeSymbol& Type);

	IRInstruction* IR_Build_Member_GetPointer(const GetMemberTypeSymbolFromVar_t& In);
	IRInstruction* IR_Build_Member_GetValue(const GetMemberTypeSymbolFromVar_t& In);
	IRInstruction* IR_Build_Member_DereferenceValue(const GetMemberTypeSymbolFromVar_t& In);
	IRInstruction* IR_Build_Member_AsPointer(const GetMemberTypeSymbolFromVar_t& In);
	IRInstruction* IR_Build_Member_AsValue(const GetMemberTypeSymbolFromVar_t& In);
	void BuildMember_Access(const GetMemberTypeSymbolFromVar_t& In, IRInstruction*& Output);
	void StepBuildMember_Access(const ScopedName& ITem, TypeSymbol& Last_Type, ScopedName::Operator_t OpType, const GetMemberTypeSymbolFromVar_t& In, IRInstruction*& Output, Symbol*& LastVarSym);
	void BuildMember_Reassignment(const GetMemberTypeSymbolFromVar_t& In, const TypeSymbol& Type, IRInstruction* Value);
	NullablePtr<Symbol> GetTepFuncPtrSyb(const String& TepFuncPtr, const FuncInfo* Finfo);
	String GetTepFuncPtrName(const FuncInfo* SymbolVar);
	String_view GetTepFuncPtrNameAsName(const String_view Str);

	void OnPostfixVariableNode(const PostfixVariableNode& node);
	void OnCompoundStatementNode(const CompoundStatementNode& node);
	void OnExpressionTypeNode(const Node* node, GetValueMode Mode);
	void OnExpressionTypeNode(const ExpressionNodeType& node, GetValueMode Mode)
	{
		OnExpressionTypeNode(node._Value.get(), Mode);
	}
	void OnExpressionNode(const ValueExpressionNode& node);
	void OnMovedNode(const MoveNode* nod);
	void OnNumberliteralNode(const NumberliteralNode* num);
	void OnBoolliteralNode(const BoolliteralNode* num);
	void OnCharliteralNode(const CharliteralNode* num);
	void OnFloatLiteralNode(const FloatliteralNode* num);
	void OnStringLiteral(const StringliteralNode* nod, bool& retflag);
	void OnSizeofNode(const SizeofExpresionNode* nod);
	void OnNewNode(const NewExpresionNode* nod);
	void OnAnonymousObjectConstructor(const AnonymousObjectConstructorNode* nod);
	void OnReadVariable(const ReadVariableNode& nod);
	void OnExpressionNode(const BinaryExpressionNode& node);
	void OnExpressionNode(const CastNode& node);
	void OnExpressionNode(const IndexedExpresionNode& node);
	void OnFuncCallNode(const FuncCallNode& node);
	void OnDropStatementNode(const DropStatementNode& node);
	void OnInvalidNode(const InvalidNode& node);
	void OnvalidNode(const ValidNode& node);
	void OnExpressionNode(const ExtendedScopeExpression& node);
	void OnExpressionNode(const ExtendedFuncExpression& node);
	void OnMatchExpression(const MatchExpression& node);
	void OnExpressionToTypeValueNode(const ExpressionToTypeValueNode& node);
	void OnAwaitExpression(const AwaitExpression& node);
	void OnYieldExpression(const YieldExpression& node);

	void OnAwaitStatement(const AwaitStatement& node);
	void OnYieldStatement(const YieldStatement& node);

	void OnUnsafeStatement(const UnsafeStatementsNode& node);
	void OnUnsafeExpression(const UnsafeExpression& node);
	void OnDeferStatement(const DeferStatementNode& node);
	void OnExpressionNode(const UnaryExpressionNode& node);
	void OnStatements(const StatementsNode& node);
	void OnPanicStatement(const PanicNode& node);
	void OnForTypeNode(const ForTypeNode& node);
	void OnExtenedFuncStatmentNode(const ExtendedFuncStatement& node);


	struct OptionalTypeInfo
	{
		TypeSymbol SomeType;
	};
	enum class OptionalGetValueMode
	{
		Copy,
		Move,
		Pointer,
	};
	Optional<OptionalTypeInfo> IsOptionalType(const TypeSymbol& Type) const;
	IRInstruction* IR_OptionalIsSomeType(const TypeSymbol& Type, IRInstruction* optional);
	IRInstruction* IR_OptionalGetSomeType(const TypeSymbol& Type, IRInstruction* optional, OptionalGetValueMode mode);
	IRInstruction* IR_MakeOptionalWithNone(const TypeSymbol& Type);

	struct ResultTypeInfo
	{
		TypeSymbol SomeType;
		TypeSymbol ErrorType;
	};
	Optional<ResultTypeInfo> IsResultType(const TypeSymbol& Type) const;

	TypeSymbol Type_MakeFutureFromType(const TypeSymbol& BaseType);
	bool Type_IsFuture(const TypeSymbol& Future);
	TypeSymbol Type_GetBaseFromFuture(const TypeSymbol& Future);

	IRInstruction* GetFutureHandle(const TypeSymbol& Future, IRInstruction* IR);
	IRInstruction* MakeFutureFromHandle(const TypeSymbol& Future, IRInstruction* IR);

	bool Type_IsStaticCharArr(const TypeSymbol& Type);

	TypeSymbol ExtendedFuncExpressionGetTypeToStart(const TypeSymbol& ExpressionType, const ExtendedFuncExpression& node);

	void OnTypeToValueNode(const TypeToValueNode& node);
	void OnMatchStatement(const MatchStatement& node);
	void Type_CanMatch(const TypeSymbol& MatchItem, const ExpressionNodeType& MatchValueNode, const ExpressionNodeType& node, MatchArmData& Data);


	bool MatchShouldOutPassEnumValue(const ExpressionNodeType& node);
	void MatchAutoPassEnumValueStart(MatchAutoPassEnum& V, const ExpressionNodeType& matchval, const ValueExpressionNode* Val, const FuncCallNode* Call);
	void MatchAutoPassEnd(MatchAutoPassEnum& V);
	void TryError_AllValuesAreMatched(const NeverNullPtr<Token> Token, const TypeSymbol& MatchItem, const MatchArmData& Data, const Vector<NeverNullPtr<ExpressionNodeType>>& _Arms);


	BuildMatch_ret IR_Build_Match(const TypeSymbol& MatchItem, const ExpressionNodeType& MatchValueNode, IRInstruction* Item, BuildMatch_State& State, MatchArm& Arm, const ExpressionNodeType& ArmEx);
	BuildMatch_ret IR_Build_InvaildMatch(const TypeSymbol& MatchItem, IRInstruction* Item, const BuildMatch_State& State);
	void IR_Build_Match(BuildMatch_ret& Value, const BuildMatch_State& State);
	void IR_Build_MatchState(const BuildMatch_State& State);


	void Assembly_ConvertAttributes(const Vector<Unique_ptr<AttributeNode>>& nodes, Vector<UsedTagValueData>& Out);
	void Assembly_ConvertAttribute(const AttributeNode& nodes, UsedTagValueData& Out);

	struct ImportLibInfo
	{
		UnorderedSet<AssemblyNode*> ClassesToAdd;
	};

	void Assembly_LoadLibSymbols();
	void Assembly_LoadLibSymbols(const UClib& lib, ImportLibInfo& libinfo, LoadLibMode Mode);

	void Assembly_LoadTraitAliases(const String& FullName, const Vector<TraitAlias>& GenericAlias);
	void Assembly_LoadTraitAliases_FixTypes(const Vector<TraitAlias>& GenericAlias);

	bool Assembly_IsExport(ExportType IsNodeExport, ReflectionCustomTypeID id);
	bool Assembly_IsExport(ExportType IsNodeExport, const String& id);
	void Assembly_LoadClassSymbol(const Class_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode);
	void Assembly_LoadEnumSymbol(const Enum_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode);
	void Assembly_LoadAliasSymbol(const Alias_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode);
	void Assembly_LoadTagSymbol(const Tag_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode);
	void Assembly_LoadTraitSymbol(const Trait_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode);
	void Assembly_LoadFuncPtrSymbol(const FuncPtr_Data& Item, const String& FullName, const String& Scope, SystematicAnalysis::LoadLibMode Mode);

	void Assembly_LoadSymbol(const ForType_Data& Item, SystematicAnalysis::LoadLibMode Mode);
	void Assembly_LoadSymbol(const NameSpace_Data& Item, const String& FullName, SystematicAnalysis::LoadLibMode Mode);

	void Assembly_LoadSymbol(const ClassMethod& Item, SystematicAnalysis::LoadLibMode Mode);
	void LoadFuncInfoGetTypes(FuncInfo* Funcinfo, const ClassMethod& Item);
	void LoadFuncInfoFixTypes(FuncInfo* Funcinfo, const ClassMethod& Item);

	void Assembly_LoadType(const ReflectionTypeInfo& Item, TypeSymbol& Out);
	TypeSymbol Assembly_LoadType(const ReflectionTypeInfo& Item);

	auto Output_Type()
	{
		return  _Settings->_Type;
	}
	auto Output_TypeAsLibType()
	{
		return   Output_Type() == OutPutType::Lib ? LibType::Lib : LibType::Dll;
	}

	static String Str_GetFuncAnonymousObjectFullName(const String& FullFuncName);
	struct AddClassExtraInfo
	{
		bool IsgenericInstantiation = false;
	};
	void Assembly_AddClass(const Vector<Unique_ptr<AttributeNode>>& attributes, const NeverNullPtr<Symbol> ClassSyb, Optional<AddClassExtraInfo> Extra = {});
	ReflectionTypeInfo Assembly_ConvertToType(const TypeSymbol& Type);

	void Assembly_AddEnum(const NeverNullPtr<Symbol> ClassSyb);
	void Assembly_AddStaticArray(const NeverNullPtr<Symbol> ClassSyb);

	TypeSymbolID Type_GetTypeID(TypesEnum Type, SymbolID SymbolId);
	TypeSymbol Type_GetStaticArrayType(const TypeSymbol& BaseType, size_t Size);

	bool Type_AreTheSame(const TypeSymbol& TypeA, const TypeSymbol& TypeB);
	bool Type_AreTheSame(const ParInfo& TypeA, const ParInfo& TypeB);
	bool Type_AreTheSameWithOutimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB);
	bool Type_AreTheSameWithOutMoveAndimmutable(const TypeSymbol& TypeA, const TypeSymbol& TypeB);

	BinaryOverLoadWith_t Type_HasBinaryOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB);
	CompoundOverLoadWith_t Type_HasCompoundOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp, const TypeSymbol& TypeB);
	PostFixOverLoadWith_t Type_HasPostfixOverLoadWith(const TypeSymbol& TypeA, TokenType BinaryOp);
	IndexOverLoadWith_t  Type_HasIndexedOverLoadWith(const TypeSymbol& TypeA, const TypeSymbol& TypeB);
	ForOverLoadWith_t Type_HasForOverLoadWith(const TypeSymbol& TypeA);
	UrinaryOverLoadWith_t Type_HasUrinaryOverLoadWith(const TypeSymbol& TypeA, TokenType Op);

	bool Type_IsCopyable(const TypeSymbol& Type);
	bool Type_HasCopyFunc(const TypeSymbol& Type) const;

	TypeSymbol Type_BinaryExpressionShouldRurn(TokenType Op, const TypeSymbol& Ex0Type);


	String ToString(const ParInfo& Type) const
	{
		String R;

		if (Type.IsOutPar)
		{
			R += "out ";
		}
		R += ToString(Type.Type);
		return R;
	}

	String ToString(const TypeSymbol& Type) const;
	String ToString(const TokenType& Type) const
	{
		return TokenStringHelper::ToString(Type);
	}



	void Type_Convert(const TypeNode& V, TypeSymbol& Out, bool allowtraitasself = false);

	NullablePtr<Symbol> Generic_InstantiateOrFindGenericSymbol(const NeverNullPtr<Token> Token, const UseGenericsNode& GenericsVals, const String_view& Name);


	void Type_ConvertAndValidateType(const TypeNode& V, TypeSymbol& Out, NodeSyb_t Syb);
	TypeSymbol Type_ConvertAndValidateType(const TypeNode& V, NodeSyb_t Syb);
	bool Type_ValidateType(const TypeSymbol& V, const NeverNullPtr<Token> Token, NodeSyb_t Syb);



	NullablePtr<Symbol> Symbol_GetSymbol(String_view Name, SymbolType Type);
	NeverNullPtr<Symbol> Symbol_GetSymbol(const FuncInfo* Info)
	{
		return Symbol_GetSymbol(RemoveSymboolFuncOverloadMangling(Info->FullName), SymbolType::Func).value();
	}
	NullablePtr<Symbol> Symbol_GetSymbol(const TypeSymbol& Info)
	{
		if (Info._Type == TypesEnum::CustomType) {
			return Symbol_GetSymbol(Info._CustomTypeSymbol).AsNullable();
		}
		return nullptr;
	}
	NeverNullPtr<Symbol> Symbol_GetSymbol(const SymbolID Info)
	{
		return &_Table.GetSymbol(Info);
	}

	const NullablePtr<Symbol> Symbol_GetSymbol(String_view Name, SymbolType Type) const;
	const NullablePtr<Symbol> Symbol_GetSymbol(const FuncInfo* Info) const
	{
		return Symbol_GetSymbol(Info->FullName, SymbolType::Func);
	}
	const NullablePtr<Symbol> Symbol_GetSymbol(const TypeSymbol& Info) const
	{
		if (Info._Type == TypesEnum::CustomType) {
			return Symbol_GetSymbol(Info._CustomTypeSymbol).AsNullable();
		}
		return nullptr;
	}
	const NeverNullPtr<Symbol> Symbol_GetSymbol(const SymbolID Info) const
	{
		return NeverNullptr(&_Table.GetSymbol(Info));
	}

	bool Context_InSideClass()
	{
		return  _ClassStack.size() && _ClassStack.top()._InStatements == false;
	}
	bool Context_IsInThisFuncCall()
	{
		if (_FuncStack.size()) {
			return Symbol_IsThisFuncCall(_FuncStack.back().Pointer);
		}
		return false;
	}

	Vector<Symbol*>& GetSymbolsWithName(const String& Name)
	{
		return  GetSymbolsWithName((String_view)Name);
	}

	Vector<Symbol*>& GetSymbolsWithName(const String_view& Name, SymbolType Type)
	{
		return GetSymbolsWithName(Name);
	}
	const Vector<const Symbol*>& GetSymbolsWithName(const String_view& Name, SymbolType Type) const
	{
		return GetSymbolsWithName(Name);
	}

	Vector<Symbol*>& GetSymbolsWithName(const String_view& Name);
	const Vector<const Symbol*>& GetSymbolsWithName(const String_view& Name) const;

	const NullablePtr<FuncInfo> Context_GetCuruntFunc() const
	{
		if (_FuncStack.size())
		{
			return _FuncStack.back().Pointer;
		}
		return nullptr;
	}
	bool Symbol_IsThisFuncCall(const NeverNullPtr<FuncInfo> Func) const
	{
		if (auto ObjectType = Func->GetObjectForCall())
		{
			auto objecttypesyb = Symbol_GetSymbol(*ObjectType);
			if (Type_IsUnMapType(*ObjectType))//Generic Test
			{
				return true;
			}

			if (objecttypesyb == nullptr)
			{
				auto sym = _ClassStack.top().Syb;
				//this would only happen in Adding funcions in ForType as Type_alias are resolved immediately
				if (sym->Type == SymbolType::Type_alias)
				{
					if (sym->VarType._Type == ObjectType->_Type)
					{
						return true;
					}
				}
			}


			return _ClassStack.top().Syb == objecttypesyb.value().value();
		}

		return false;
	}


	String GetScopeLabelName(const void* node);

	void Symbol_RedefinitionCheck(const NeverNullPtr<Symbol> Syb, const NeverNullPtr<Token> Value);
	void Symbol_RedefinitionCheck(const String_view FullName, SymbolType Type, const NeverNullPtr<Token> Value);
	void Symbol_RedefinitionCheck(const NeverNullPtr<Symbol> Syb, const FuncInfo* Fvalue, const NeverNullPtr<Token> Value);

	bool Type_CanDoTypeToTrait(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);

	bool Type_IsValid(TypeSymbol& Out);
	bool Type_CanBeImplicitConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type, bool ReassignMode = true, bool isdeclare = false);
	NullablePtr<FuncInfo> Symbol_GetAnImplicitConvertedFunc(const TypeSymbol& MainType, const TypeSymbol& ToType);
	CastOverLoadWith_t  Type_CanBeExplicitlyConverted(const TypeSymbol& TypeToCheck, const TypeSymbol& Type);
	Optional<FuncInfo*> Symbol_GetAnExplicitlyConvertedFunc(const TypeSymbol& TypeToCheck);

	NullablePtr<Symbol> HasCopyContructer(const TypeSymbol& ExType);
	bool HasCopyContructerHasIRFunc(const TypeSymbol& ExType);
	NullablePtr<Symbol> HasMoveContructer(const TypeSymbol& ExType);
	bool HasMoveContructerHasIRFunc(const TypeSymbol& ExType);
	bool IR_Build_ImplicitConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType);
	void IR_Build_ExplicitConversion(IRInstruction* Ex, const TypeSymbol ExType, const TypeSymbol& ToType, const CastExpressionNode_Data& Data);

	bool Type_IsSIntType(const TypeSymbol& TypeToCheck) const;
	bool Type_IsUIntType(const TypeSymbol& TypeToCheck) const;
	bool Type_IsIntType(const TypeSymbol& TypeToCheck) const
	{
		return  Type_IsSIntType(TypeToCheck) || Type_IsUIntType(TypeToCheck);
	}
	bool Type_IsfloatType(const TypeSymbol& TypeToCheck)const;
	bool Type_IsCharType(const TypeSymbol& TypeToCheck) const;
	bool Type_IsPrimitive(const TypeSymbol& TypeToCheck) const;
	bool Type_IsPrimitiveNotIncludingPointers(const TypeSymbol& TypeToCheck) const;
	bool Type_IsStaticArray(const TypeSymbol& TypeToCheck) const;

	//Span or StringSpan
	bool Type_IsReference(const TypeSymbol& TypeToCheck) const;
	bool Type_IsimmutableRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type)const;

	bool Type_IsAddessAndLValuesRulesfollowed(const TypeSymbol& TypeToCheck, const TypeSymbol& Type, bool ReassignMode, bool isdeclare = false) const;


	bool Symbol_HasDestructor(const TypeSymbol& TypeToCheck);
	bool Symbol_HasDestructor(const SymbolID& TypeToCheck);

	void Symbol_Update_ClassSym_ToFixedTypes(NeverNullPtr<Symbol> Sym);
	void Symbol_Update_FuncSym_ToFixedTypes(NeverNullPtr<Symbol> Sym);
	void Symbol_Update_EnumSym_ToFixedTypes(NeverNullPtr<Symbol> Sym);
	void Symbol_Update_TraitSym_ToFixedTypes(NeverNullPtr<Symbol> Sym);
	void Symbol_Update_TagSym_ToFixedTypes(NeverNullPtr<Symbol> Sym);
	void Symbol_Update_AliasSym_ToFixedTypes(NeverNullPtr<Symbol> Sym);
	void Symbol_Update_EvalSym_ToFixedTypes(NeverNullPtr<Symbol> Sym);
	void Symbol_Update_ThreadAndStatic_ToFixedTypes(NeverNullPtr<Symbol> Sym);
	void Symbol_Update_ForType_ToFixedTypes(NeverNullPtr<Symbol> Sym);
	void Symbol_Update_ClassField_ToFixedTypes(NeverNullPtr<Symbol> Sym);

	void Symbol_Update_Sym_ToFixedTypes(NeverNullPtr<Symbol> Sym);

	Optional<size_t> Type_GetSize(const TypeSymbol& Type)
	{
		size_t V;
		if (Type_GetSize(Type, V))
		{
			return Opt<size_t>(V);
		}
		return {};
	}
	bool Type_GetSize(const TypeSymbol& Type, size_t& OutSize);

	Optional<size_t> Type_GetOffset(const ClassInfo& Type, const FieldInfo* Field)
	{
		size_t V;
		if (Type_GetOffset(Type, Field, V))
		{
			return Opt<size_t>(V);
		}
		return {};
	}
	bool Type_GetOffset(const ClassInfo& Type, const FieldInfo* Field, size_t& OutOffset);


	Vector<Symbol*> Type_FindForTypeFuncions(const TypeSymbol& ThisType);
	Vector<Symbol*> Type_FindForTypeFuncions(const TypeSymbol& ThisType, const String& FuncionName);
	Get_FuncInfo Type_GetFunc(const TypeSymbol& Name, const ValueParametersNode& Pars, const NeverNullPtr<Token> token);
	Get_FuncInfo Type_GetFunc(const ScopedNameNode& Name, const ValueParametersNode& Pars, TypeSymbol Ret);

	Optional<Optional<Get_FuncInfo>> Type_FuncinferGenerics(Vector<TypeSymbol>& GenericInput, const Vector<ParInfo>& ValueTypes
		, const UseGenericsNode* Generics
		, Symbol* Item
		, bool _ThisTypeIsNotNull);

	bool CheckForGenericInputIsConstantExpression(const FuncInfo* Info, const Vector<TypeSymbol>& GenericInput);

	void Type_RemoveTypeattributes(TypeSymbol& tep_);


	Get_FuncInfo Symbol_GetEnumVariantFunc(NeverNullPtr<Symbol> EnumSyb, size_t FieldIndex, NeverNullPtr<Symbol> EnumFieldSyb, const ValueParametersNode& Pars, const NeverNullPtr<Token> Token, const Vector<ParInfo>& ValueTypes);
	void Symbol_SetOutExpression(const OutExpression* Ex, const TypeSymbol& TypeToSet);

	NeverNullPtr<Symbol> Symbol_GetSymbolFromExpression(const NeverNullPtr<OutExpression> Ex);

	void Type_SetFuncRetAsLastEx(const Get_FuncInfo& Info);

	bool Type_HasDefaultConstructorFunc(const TypeSymbol& Type) const;

	bool Type_IsCompatible(const IsCompatiblePar& FuncPar, const Vector<ParInfo>& ValueTypes, bool _ThisTypeIsNotNull, const NeverNullPtr<Token> Token);

	const OptionalRef<Vector<TypeSymbol>> GetTypePackFromInputPar(const Vector<ParInfo>& Pars);
	ParInfo GetParInfoResolveTypePack(size_t i, const Vector<ParInfo>& Pars, const OptionalRef<Vector<TypeSymbol>> TypePack);
	size_t GetParCountResolveTypePack(const Vector<ParInfo>& Pars);

	int Type_GetCompatibleScore(const ParInfo& ParFunc, const ParInfo& Value);
	int Type_GetCompatibleScore(const IsCompatiblePar& Func, const Vector<ParInfo>& ValueTypes);

	bool Symbol_AccessCheck(const NeverNullPtr<Symbol> Syb, const  NeverNullPtr<Token> Token, const String_view Scope);
	bool Symbol_AccessCheck(const NeverNullPtr<Symbol> Syb, const  NeverNullPtr<Token> Token)
	{
		return Symbol_AccessCheck(Syb, Token, this->_Table._Scope.ThisScope);
	}


	bool Type_IsStringSpan(const TypeSymbol& type);
	bool Type_IsStringSpan8(const TypeSymbol& type);
	bool Type_IsStringSpan16(const TypeSymbol& type);
	bool Type_IsStringSpan32(const TypeSymbol& type);

	bool Type_IsString(const TypeSymbol& type);
	bool Type_IsString8(const TypeSymbol& type);
	bool Type_IsString16(const TypeSymbol& type);
	bool Type_IsString32(const TypeSymbol& type);

	Optional<TypeSymbol> Type_GetStringSpan(const NeverNullPtr<Token> Token);
	Optional<TypeSymbol> Type_GetStringSpan8(const NeverNullPtr<Token> Token);
	Optional<TypeSymbol> Type_GetStringSpan16(const NeverNullPtr<Token> Token);
	Optional<TypeSymbol> Type_GetStringSpan32(const NeverNullPtr<Token> Token);
	//Generics

	void Generic_GenericFuncInstantiate(const NeverNullPtr<Symbol> Func, const Vector<TypeSymbol>& GenericInput);

	void Push_GenericInfo(const String& NewName, const Vector<TypeSymbol>& GenericInput, const NeverNullPtr<Symbol> Func, const Generic& GenericData);

	String Generic_GetGenericExtendedErrValue(const Generic& Generic, const GenericValuesNode& GenericAsNode, const Vector<TypeSymbol>& GenericInput);
	Optional<SymbolID> Generic_MakeTypePackSymbolIfNeeded(const String& NewName, const Vector<TypeSymbol>& GenericInput, const Generic& Generic);

	String Generic_SymbolGenericFullName(const NeverNullPtr<Symbol> Func, const Vector<TypeSymbol>& Type) const;
	String Generic_SymbolGenericName(const NeverNullPtr<Symbol> Func, const Vector<TypeSymbol>& Type) const;

	void Set_TraitGenericAlias(Vector<TraitGenericAlias>& Out, const Vector<TypeSymbol>& In, const GenericValuesNode& GenericValueNode);
	void Generic_TypeInstantiate(const NeverNullPtr<Symbol> Class, const Vector<TypeSymbol>& Type);
	void Generic_TypeInstantiate_Trait(const NeverNullPtr<Symbol> Trait, const Vector<TypeSymbol>& Type);
	void Pop_AddToGeneratedGenricSymbol(Symbol& addedSymbol, const Vector<TypeSymbol>& Type);
	void Generic_TypeInstantiate_Alias(const NeverNullPtr<Symbol> Alias, const Vector<TypeSymbol>& Type);
	void Generic_TypeInstantiate_Enum(const NeverNullPtr<Symbol> Enum, const Vector<TypeSymbol>& Type);
	void Generic_TypeInstantiate_Tag(const NeverNullPtr<Symbol> Trait, const Vector<TypeSymbol>& Type);
	void Generic_TypeInstantiate_ForType(const NeverNullPtr<Symbol> ForType, const Vector<TypeSymbol>& Type);

	bool TypeHasTrait(const TypeSymbol& Type, SymbolID id);

	EvaluatedEx Eval_MakeEx(const TypeSymbol& Type);
	RawEvaluatedObject Eval_MakeExr(const TypeSymbol& Type);
	void* Eval_Get_Object(const TypeSymbol& Input, const RawEvaluatedObject& Input2);
	void* Eval_Get_Object(const EvaluatedEx& Input);
	const void* Eval_Get_Object(const TypeSymbol& Input, const RawEvaluatedObject& Input2) const;
	const void* Eval_Get_Object(const EvaluatedEx& Input) const;
	template<typename T> T* Eval_Get_ObjectAs(const TypeSymbol& Input, const RawEvaluatedObject& Input2)
	{
#if UCodeLangDebug
		if ((Input.IsAddress() || Input.IsAddressArray()))
		{
			if (sizeof(T) != sizeof(EvalPointer))
			{
				String TepStr = "type miss-mach when EvaluatedObject To Cpp type '" + (String)typeid(T).name() + "' ";
				UCodeLangThrowException(TepStr.c_str());
			}
		}
		else if (Input2.ObjectSize != sizeof(T))
		{
			String TepStr = "type miss-mach when EvaluatedObject To Cpp type '" + (String)typeid(T).name() + "' ";
			UCodeLangThrowException(TepStr.c_str());
		}
#endif // DEBUG
		return (T*)Eval_Get_Object(Input, Input2);
	}
	template<typename T> T* Eval_Get_ObjectAs(const EvaluatedEx& Input)
	{
		return Eval_Get_ObjectAs<T>(Input.Type, Input.EvaluatedObject);
	}

	template<typename T> const T* Eval_Get_ObjectAs(const TypeSymbol& Input, const RawEvaluatedObject& Input2) const
	{
#if UCodeLangDebug
		if ((Input.IsAddress() || Input.IsAddressArray()))
		{
			if (sizeof(T) != sizeof(EvalPointer))
			{
				String TepStr = "type miss-mach when EvaluatedObject To Cpp type '" + (String)typeid(T).name() + "' ";
				UCodeLangThrowException(TepStr.c_str());
			}
		}
		else if (Input2.ObjectSize != sizeof(T))
		{
			String TepStr = "type miss-mach when EvaluatedObject To Cpp type '" + (String)typeid(T).name() + "' ";
			UCodeLangThrowException(TepStr.c_str());
		}
#endif // DEBUG
		return (T*)Eval_Get_Object(Input, Input2);
	}
	template<typename T> const T* Eval_Get_ObjectAs(const EvaluatedEx& Input) const
	{
		return Eval_Get_ObjectAs<T>(Input.Type, Input.EvaluatedObject);
	}

	template<typename T> void Eval_Set_ObjectAs(const TypeSymbol& Input, RawEvaluatedObject& Input2, const T& Value)
	{
#if UCodeLangDebug
		if (Input.IsAddress() || Input.IsAddressArray())
		{
			if (sizeof(EvalPointer) != sizeof(Value))
			{
				String TepStr = "type miss-mach when Cpp type To EvaluatedObject'" + (String)typeid(T).name() + "' ";
				UCodeLangThrowException(TepStr.c_str());
			}
		}
		else if (Input2.ObjectSize != sizeof(T))
		{
			String TepStr = "type miss-mach when Cpp type To EvaluatedObject'" + (String)typeid(T).name() + "' ";
			UCodeLangThrowException(TepStr.c_str());
		}
#endif // DEBUG

		auto Ptr = (T*)Eval_Get_Object(Input, Input2);
		*Ptr = Value;
	}
	template<typename T> void Eval_Set_ObjectAs(EvaluatedEx& Input, const T& Value)
	{
		return Eval_Set_ObjectAs<T>(Input.Type, Input.EvaluatedObject, Value);
	}
	void Eval_Set_ObjectAs(EvaluatedEx& Input, const void* Object, size_t ObjectSize)
	{
		return Eval_Set_ObjectAs(Input.Type, Input.EvaluatedObject, Object, ObjectSize);
	}
	void Eval_Set_ObjectAs(const TypeSymbol& Input, RawEvaluatedObject& Input2, const void* Object, size_t ObjectSize)
	{
#if UCodeLangDebug
		if (Input.IsAddress() || Input.IsAddressArray())
		{
			if (sizeof(EvalPointer) != ObjectSize)
			{
				String TepStr = "type miss-mach when Cpp type To EvaluatedObject'";
				UCodeLangThrowException(TepStr.c_str());
			}
		}
		else if (Input2.ObjectSize != ObjectSize)
		{
			String TepStr = "type miss-mach when Cpp type To EvaluatedObject'";
			UCodeLangThrowException(TepStr.c_str());
		}
#endif // DEBUG




		auto Ptr = (Byte*)Eval_Get_Object(Input, Input2);
		memcpy(Ptr, Object, ObjectSize);
	}

	using TypeInstantiateFunc = void(SystematicAnalysis::*)(const NeverNullPtr<Symbol> Symbol, const Vector<TypeSymbol>& GenericInput);
	NullablePtr<Symbol> Generic_InstantiateOrFindGenericSymbol(const NeverNullPtr<Token> Name, const NeverNullPtr<Symbol> Symbol, const GenericValuesNode& SymbolGenericValues, const Generic& GenericData, const UseGenericsNode& UseNode, TypeInstantiateFunc Instantiate)
	{
		bool hasbadgenericcount = false;
		if (GenericData.IsPack())
		{
			auto mingenericcount = GenericData._Genericlist.size();
			hasbadgenericcount = mingenericcount < UseNode._Values.size();
		}
		else
		{
			hasbadgenericcount = GenericData._Genericlist.size() != UseNode._Values.size();
		}

		if (hasbadgenericcount)
		{
			LogError_CanIncorrectGenericCount(Name, Name->Value._String, UseNode._Values.size(), GenericData._Genericlist.size());
			return nullptr;
		}


		auto GenericInput = std::make_unique<Vector<TypeSymbol>>();//pointer must be unique so it can't be on the stack
		for (size_t i = 0; i < UseNode._Values.size(); i++)
		{
			const auto& Tnode = UseNode._Values[i];
			const auto& GenericInfo = GenericData._Genericlist[i];
			TypeSymbol Type;

			TypeSymbol typehint = TypesEnum::Any;
			if (GenericInfo.BaseOrRule.has_value())
			{
				auto& rule = GenericInfo.BaseOrRule.value();
				if (rule.Is<TypeSymbol>())
				{
					typehint = rule.Get<TypeSymbol>();
				}
			}

			_LookingForTypes.push(typehint);
			Type_ConvertAndValidateType(Tnode, Type, NodeSyb_t::Any);
			_LookingForTypes.pop();

			if (Type_IsUnMapType(Type) || Type.IsNull())
			{
				return nullptr;
			}

			{
				bool InputTypeIsConstantExpression = false;

				auto TypeSyb = Symbol_GetSymbol(Type);
				if (TypeSyb)
				{
					InputTypeIsConstantExpression = TypeSyb.value()->Type == SymbolType::ConstantExpression;
				}

				if (InputTypeIsConstantExpression != GenericInfo.IsConstantExpression())
				{
					const NeverNullPtr<Token> nodeToken = NeverNullptr(Tnode._name._ScopedName.back()._token);
					auto& GenericNo = SymbolGenericValues._Values[i];
					if (InputTypeIsConstantExpression)
					{
						LogError_GenericInputWantsaExpressionNotType(nodeToken, GenericNo.token->Value._String);
					}
					else
					{
						LogError_GenericInputWantsaExpressionNotType(nodeToken, GenericNo.token->Value._String);
					}

					return nullptr;
				}
				else
				{
					if (InputTypeIsConstantExpression)
					{
						if (GenericInfo.BaseOrRule.has_value())
						{
							auto& rule = GenericInfo.BaseOrRule.value();
							if (rule.Is<TypeSymbol>())
							{
								auto ruletype = rule.Get<TypeSymbol>();
								TypeSymbol extype = TypeSyb.value()->VarType;

								bool issame = Type_AreTheSame(ruletype, extype);
								if (!issame)
								{
									const NeverNullPtr<Token> nodeToken = NeverNullptr(Tnode._name._ScopedName.back()._token);
									LogError_Eval_CantCastImplicitTypes(nodeToken, extype, ruletype);
									return nullptr;
								}
							}
						}
					}
				}
			}


			GenericInput->push_back(Type);
		}

		String NewName = Generic_SymbolGenericFullName(Symbol, *GenericInput);
		auto FuncIsMade = Symbol_GetSymbol(NewName, SymbolType::Type_class);
		if (!FuncIsMade.has_value())
		{
			(*this.*Instantiate)(Symbol, *GenericInput);

			_TepFuncs.push_back({ std::move(GenericInput) });
		}

		return Symbol_GetSymbol(NewName, SymbolType::Type);
	}
	NullablePtr<Symbol> Generic_InstantiateOrFindGeneric_Class(const NeverNullPtr<Token> Name, const NeverNullPtr<Symbol> Symbol, const GenericValuesNode& SymbolGenericValues, const Generic& GenericData, const UseGenericsNode& UseNode)
	{
		UCodeLangAssert(Symbol->PassState == PassType::FixedTypes || Symbol->PassState == PassType::BuidCode);
		UCodeLangAssert(Symbol->ValidState == SymbolValidState::valid);

		TypeInstantiateFunc Func = &SystematicAnalysis::Generic_TypeInstantiate;
		return Generic_InstantiateOrFindGenericSymbol(Name, Symbol, SymbolGenericValues, GenericData, UseNode, Func);
	}
	NullablePtr<Symbol> Generic_InstantiateOrFindGeneric_Trait(const NeverNullPtr<Token> Name, const NeverNullPtr<Symbol> Symbol, const GenericValuesNode& SymbolGenericValues, const Generic& GenericData, const UseGenericsNode& UseNode)
	{
		UCodeLangAssert(Symbol->PassState == PassType::FixedTypes || Symbol->PassState == PassType::BuidCode);
		UCodeLangAssert(Symbol->ValidState == SymbolValidState::valid);

		TypeInstantiateFunc Func = &SystematicAnalysis::Generic_TypeInstantiate_Trait;
		return Generic_InstantiateOrFindGenericSymbol(Name, Symbol, SymbolGenericValues, GenericData, UseNode, Func);
	}

	NullablePtr<Symbol> Generic_InstantiateOrFindGeneric_Alias(const NeverNullPtr<Token> Name, const NeverNullPtr<Symbol> Symbol, const GenericValuesNode& SymbolGenericValues, const Generic& GenericData, const UseGenericsNode& UseNode)
	{
		UCodeLangAssert(Symbol->PassState == PassType::FixedTypes || Symbol->PassState == PassType::BuidCode);
		UCodeLangAssert(Symbol->ValidState == SymbolValidState::valid);

		TypeInstantiateFunc Func = &SystematicAnalysis::Generic_TypeInstantiate_Alias;
		return Generic_InstantiateOrFindGenericSymbol(Name, Symbol, SymbolGenericValues, GenericData, UseNode, Func);
	}
	NullablePtr<Symbol> Generic_InstantiateOrFindGeneric_Enum(const NeverNullPtr<Token> Name, const NeverNullPtr<Symbol> Symbol, const GenericValuesNode& SymbolGenericValues, const Generic& GenericData, const UseGenericsNode& UseNode)
	{
		UCodeLangAssert(Symbol->PassState == PassType::FixedTypes || Symbol->PassState == PassType::BuidCode);
		UCodeLangAssert(Symbol->ValidState == SymbolValidState::valid);

		TypeInstantiateFunc Func = &SystematicAnalysis::Generic_TypeInstantiate_Enum;
		return Generic_InstantiateOrFindGenericSymbol(Name, Symbol, SymbolGenericValues, GenericData, UseNode, Func);
	}
	NullablePtr<Symbol> Generic_InstantiateOrFindGeneric_Tag(const NeverNullPtr<Token> Name, const NeverNullPtr<Symbol> Symbol, const GenericValuesNode& SymbolGenericValues, const Generic& GenericData, const UseGenericsNode& UseNode)
	{
		UCodeLangAssert(Symbol->PassState == PassType::FixedTypes || Symbol->PassState == PassType::BuidCode);
		UCodeLangAssert(Symbol->ValidState == SymbolValidState::valid);

		TypeInstantiateFunc Func = &SystematicAnalysis::Generic_TypeInstantiate_Tag;
		return Generic_InstantiateOrFindGenericSymbol(Name, Symbol, SymbolGenericValues, GenericData, UseNode, Func);
	}

	StrExELav Eval_GetStrEVal(const Node* node);
	bool Eavl_ConstantExpressionAbleType(const TypeSymbol& Type);
	bool Eval_EvaluateDefaultConstructor(EvaluatedEx& Out);
	bool Eval_Evaluate(EvaluatedEx& Out, const ValueExpressionNode& node);
	bool Eval_Evaluate(EvaluatedEx& Out, const BinaryExpressionNode& node);
	bool Eval_Evaluate(EvaluatedEx& Out, const CastNode& node);
	bool Eval_Evaluate(EvaluatedEx& Out, const ReadVariableNode& nod);
	bool Eval_Evaluate(EvaluatedEx& Out, const MatchExpression& nod, bool runmatchcheck = true);
	bool Eval_MatchArm(const TypeSymbol& MatchItem, const EvaluatedEx& Item, MatchArm& Arm, const ExpressionNodeType& ArmEx);
	void DoBinaryOpContextWith(TypeSymbol type, const DoBinaryOpContext& context);

	template<typename T>
	static void DoBinaryIntOp(const SystematicAnalysis::DoBinaryOpContext& context)
	{
		T& op1 = *(T*)context.Op1->Object_AsPointer.get();
		T& op2 = *(T*)context.Op2->Object_AsPointer.get();
		T& out = *(T*)context.OpOut->Object_AsPointer.get();
		bool& outequal = *(bool*)context.OpOut->Object_AsPointer.get();

		switch (context.type)
		{
		case TokenType::plus:
			out = op1 + op2;
			break;
		case TokenType::minus:
			out = op1 + op2;
			break;
		case TokenType::star:
			out = op1 * op2;
			break;
		case TokenType::forwardslash:
			out = op1 / op2;
			break;
		case TokenType::modulo:
			out = op1 % op2;
			break;

		case TokenType::equal_Comparison:
			outequal = op1 == op2;
			break;
		case TokenType::Notequal_Comparison:
			outequal = op1 != op2;
			break;
		case TokenType::logical_and:
			outequal = op1 && op2;
			break;
		case TokenType::logical_or:
			outequal = op1 || op2;
			break;
		case TokenType::greater_than_or_equalto:
			outequal = op1 >= op2;
			break;
		case TokenType::less_than_or_equalto:
			outequal = op1 <= op2;
			break;
		case TokenType::greaterthan:
			outequal = op1 > op2;
			break;
		case TokenType::lessthan:
			outequal = op1 < op2;
			break;

		case TokenType::bitwise_LeftShift:
			out = op1 << op2;
			break;
		case TokenType::bitwise_RightShift:
			out = op1 >> op2;
			break;
		case TokenType::bitwise_and:
			out = op1 && op2;
			break;
		case TokenType::bitwise_or:
			out = op1 || op2;
			break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}

	bool Eval_Evaluate_t(EvaluatedEx& Out, const Node* node, GetValueMode Mode);
	bool Eval_Evaluate(EvaluatedEx& Out, const ExpressionNodeType& node, GetValueMode Mode);
	bool Eval_EvaluatePostfixOperator(EvaluatedEx& Out, TokenType Op);
	bool Eval_HasConstantPostfixOperator(const TypeSymbol& Type, TokenType Op);
	bool Eval_CanEvaluateImplicitConversionConstant(const TypeSymbol& Type, const TypeSymbol& ToType);
	bool Eval_EvaluateImplicitConversion(EvaluatedEx& In, const TypeSymbol& ToType, EvaluatedEx& out);
	bool Eval_EvalutateCMPTypesNode(EvaluatedEx& Out, const CMPTypesNode& node);
	bool Eval_EvalutateValidNode(EvaluatedEx& Out, const ValidNode& node);
	bool Eval_EvalutateFunc(EvaluatedEx& Out, const FuncCallNode& node);
	bool Eval_EvalutateFunc(EvaluatedEx& Out, const Get_FuncInfo& Func, const ScopedNameNode& Name, const Vector<EvaluatedEx>& Pars);
	bool Eval_EvalutateFunc(EvaluatedEx& Out, const TypeSymbol& Type, const Get_FuncInfo& Func, const Vector<EvaluatedEx>& ValuePars);
	bool Eval_Evaluate(EvaluatedEx& Out, const ExtendedScopeExpression& node);
	bool Eval_Evaluate(EvaluatedEx& Out, const ExtendedFuncExpression& node);

	EvaluatedEx Eval_Evaluate_GetPointer();

	bool Eval_EvalutateFunc(EvalFuncData& State, const NeverNullPtr<Symbol> Func, const Vector<EvaluatedEx>& Pars);
	bool Eval_EvalutateStatement(EvalFuncData& State, const Node* node);



	bool Eval_EvalutateScopedName(EvaluatedEx& Out, size_t Start, size_t End, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& OtherOut);
	bool Eval_EvalutateScopedName(EvaluatedEx& Out, size_t Start, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& OtherOut)
	{
		return Eval_EvalutateScopedName(Out, Start, -1, node, OtherOut);
	}
	bool Eval_EvalutateScopedName(EvaluatedEx& Out, const ScopedNameNode& node, GetMemberTypeSymbolFromVar_t& OtherOut)
	{
		return Eval_EvalutateScopedName(Out, 0, node, OtherOut);
	}
	bool Eval_EvalutateStepScopedName(EvaluatedEx& Out, const ScopedNameNode& node, size_t Index, ScopedName::Operator_t OpType, GetMemberTypeSymbolFromVar_t& OtherOut);

	bool Eval_CanEvalutateFuncCheck(const Get_FuncInfo& Func);

	bool Eval_Evaluate(EvaluatedEx& Out, const TypeSymbol& MustBeType, const ExpressionNodeType& node);
	Optional<EvaluatedEx> Eval_Evaluate(const TypeSymbol& MustBeType, const ExpressionNodeType& node);
	Optional<EvaluatedEx> Eval_Evaluate(const TypeSymbol& MustBeType, const Node& node)
	{
		return  Eval_Evaluate(MustBeType, *ExpressionNodeType::As(&node));
	}

	void Eval_SetOutExpressionEval(const OutExpression* Ex, const EvaluatedEx& ObjectToSet);

	bool Eval_EvaluateToAnyType(EvaluatedEx& Out, const ExpressionNodeType& node);
	Optional<EvaluatedEx> Eval_EvaluateToAnyType(const ExpressionNodeType& node);
	String ToString(const TypeSymbol& Type, const RawEvaluatedObject& Data) const;
	IRInstruction* IR_RawObjectDataToCString(const RawEvaluatedObject& EvalObject);

	bool EvalStore(EvalFuncData& State, const ExpressionNodeType& Storenode, EvaluatedEx& In);
	
	bool TypesBeingCheckedForEval();
	//IR
	void IR_Build_FuncCall(Get_FuncInfo Func, const ScopedNameNode& Name, const ValueParametersNode& Pars);
	void IR_Build_FuncCall(const TypeSymbol& Type, const Get_FuncInfo& Func, const ValueParametersNode& ValuePars);
	void IR_Build_DestructorCall(const ObjectToDrop& Object);
	void IR_Build_EnumOut(NeverNullPtr<Symbol> EnumSymbol, size_t EnumIndex, const ValueParametersNode& Pars);
	void IR_Build_EnumOut(NeverNullPtr<Symbol> EnumSymbol, IRInstruction* ThisEnum, size_t EnumIndex, const ValueParametersNode& Pars, size_t StartIndex = 0);



	IRInstruction* IR_Load_UIntptr(UAddress Value);
	IRInstruction* IR_Load_SIntptr(SIntNative Value);
	IRInstruction* IR_Build_Add_uIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* IR_Build_Sub_uIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* IR_Build_Add_sIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* IR_Build_Sub_sIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* IR_Build_Mult_uIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* IR_Build_Mult_sIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* Build_Div_uIntPtr(IRInstruction* field, IRInstruction* field2);
	IRInstruction* IR_Build_Div_sIntPtr(IRInstruction* field, IRInstruction* field2);
	void IR_Build_Increment_uIntPtr(IRInstruction* field, UAddress Value);
	void IR_Build_Decrement_uIntPtr(IRInstruction* field, UAddress Value);
	void IR_Build_Increment_sIntPtr(IRInstruction* field, SIntNative Value);
	void IR_Build_Decrement_sIntPtr(IRInstruction* field, SIntNative Value);
	IRInstruction* LoadEvaluatedEx(const RawEvaluatedObject& Value, const TypeSymbol& ValueType);
	//Errors

	void LogError_CantFindCompoundOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type);
	void LogError_CantFindPostfixOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type);
	void LogError_CantFindBinaryOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type, TypeSymbol& Ex1Type);
	void LogError_CantFindUnaryOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type);


	void LogError_ExpressionMustbeAnLocationValueError(const NeverNullPtr<Token> Token, TypeSymbol& Ex0Type);
	void LogError_YouMustReturnSomethingError(const NeverNullPtr<Token> Token);
	void LogError_CantguessVarTypeError(const NeverNullPtr<Token> Token);
	void LogError_CantUseThisKeyWordHereError(const NeverNullPtr<Token> NameToken);
	void LogError_CantCastImplicitTypes(const NeverNullPtr<Token> Token, const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType, bool ReassignMode, bool isdeclare = false);
	void LogError_ReadingFromInvaidVariable(const NeverNullPtr<Token> Token, String_view Str);
	void LogError_CantFindVarError(const NeverNullPtr<Token> Token, String_view Str);
	void LogError_CantFindVarMemberError(const NeverNullPtr<Token> Token, String_view Str, const TypeSymbol& OnType);
	void LogError_CantModifyiMutableError(const NeverNullPtr<Token> Token, String_view Name);
	void LogError_CantCastExplicityTypes(const NeverNullPtr<Token> Token, TypeSymbol& Ex0Type, TypeSymbol& ToTypeAs);
	void LogError_CantFindTypeError(const NeverNullPtr<Token> Token, String_view Name);
	void LogError_CantFindSymbolError(const NeverNullPtr<Token> Token, String_view Name);
	void LogError_TypeDependencyCycle(const NeverNullPtr<Token> Token, const ClassInfo* Value);
	void LogError_CantUseThisHere(const NeverNullPtr<Token> Token);

	void LogError_CanIncorrectParCount(const NeverNullPtr<Token> Token, String_view FuncName, size_t Count, size_t FuncCount);
	void LogError_CanIncorrectGenericCount(const NeverNullPtr<Token> Token, String_view FuncName, size_t Count, size_t FuncCount);

	void LogError_CantFindFuncError(const NeverNullPtr<Token> Token, String_view FuncName,
		const Vector<TypeSymbol>& Generics,
		const Vector<TypeSymbol>& WithTypes,
		const TypeSymbol& RetType);
	void LogError_CantFindFuncError(const NeverNullPtr<Token> Token, String_view FuncName,
		const Vector<TypeSymbol>& Generics,
		const Vector<ParInfo>& WithTypes,
		const TypeSymbol& RetType);

	void LogError_TypeMustBeAnConstantExpressionAble(const NeverNullPtr<Token> Token, const TypeSymbol& Type);
	void LogError_CantFindPostfixOpForTypes_Constant(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type);
	void LogError_Eval_CantDoPostfixOpForTypes(const NeverNullPtr<Token> BinaryOp, TypeSymbol& Ex0Type);
	void LogError_Eval_CantCastImplicitTypes(const NeverNullPtr<Token> Token, const TypeSymbol& Ex1Type, const TypeSymbol& UintptrType);
	void LogError_CantFindNamespace(const NeverNullPtr<Token> Token, const String_view Namespace);
	void LogError_LogFuncMustBe(const NeverNullPtr<Token> Token, const String_view FuncName, TypeSymbol& TypeSybToBe);
	void LogError_SymbolRedefinition(const NeverNullPtr<Token> Token, const NeverNullPtr<Symbol> Symbol);
	void LogError_UseingVarableBeforDeclared(const NeverNullPtr<Token> Token);
	void LogError_BeMoreSpecifiicForRetType(const String_view FuncName, const NeverNullPtr<Token> Token);
	void LogError_CantBeIndexWithType(const NeverNullPtr<Token> Token, const  TypeSymbol& Ex0Type, const  TypeSymbol& IndexType);
	void LogError_CantUseThisInStaticFunction(const NeverNullPtr<Token> Token);
	void LogError_FuncDependencyCycle(const NeverNullPtr<Token> Token, const FuncInfo* Value);

	void LogError_GenericInputWantsaExpressionNotType(const NeverNullPtr<Token> Token, const String_view NameOfPar);
	void LogError_GenericInputWantsaTypeNotExpression(const NeverNullPtr<Token> Token, const String_view NameOfPar);

	void LogError_ExpectedSymbolToBea(const NeverNullPtr<Token> Token, const Symbol& Syb, SymbolType Value);

	void LogError_CanIncorrectStaticArrCount(const NeverNullPtr<Token> Token, const TypeSymbol& Type, size_t Count, size_t FuncCount);
	void LogError_BeMoreSpecifiicWithStaticArrSize(const NeverNullPtr<Token> Token, const TypeSymbol& Type);
	void LogError_BinaryOverloadPars(const Token& Name, const FuncInfo* Func);
	void LogError_IndexOverloadPars(const Token& Name, const FuncInfo* Func);
	void LogError_PostfixOverloadPars(const Token& Name, const FuncInfo* Func);
	void LogError_CantOverLoadOverload(const NeverNullPtr<Token> NameToken);
	void LogError_CantFindMemberOverloadForType(const NeverNullPtr<Token> Item, TokenType Op, const TypeSymbol& Out);
	void LogError_CantUseMoveTypeHere(const NeverNullPtr<Token> Token);
	void LogError_MustMakeEnumLikeafunction(EnumInfo* Einfo, size_t Index, const NeverNullPtr<Token> Token);
	void LogError_DynamicMustBeRrait(const TypeNode& V, const TypeSymbol& Out);
	void LogError_TraitCantBeAlone(const NeverNullPtr<Token> Token);
	void LogError_WantedAType(const TypeNode& V, Symbol* SybV);
	void LogError_OutCanOnlyBeInControlFlow(const NeverNullPtr<Token> Token);
	void LogError_ParamterMustBeAnOutExpression(const NeverNullPtr<Token> Token, const size_t& i);
	void LogError_LogInvalidNodeError(const NeverNullPtr<Token> Token, String_view ErrStr);
	void LogError_EmptyInvalidError(const NeverNullPtr<Token> Token);
	void LogError_LogParPackIsNotLast(const NeverNullPtr<Token> Token);
	void LogError_ParPackTypeIsNotLast(const NeverNullPtr<Token> Token);
	void LogError(ErrorCodes Err, const String& MSG, const NeverNullPtr<Token> Token);
	void LogError(ErrorCodes Err, size_t Line, size_t Pos, const String& MSG);
	void LogError_LogWantedAVariable(const NeverNullPtr<Token>& Item, Symbol* TepSyb);
	void LogError_CantBindTypeItNotTypeInfo(const NeverNullPtr<Token> Token, TypeSymbol& Type);
	void LogError_CantUseTypeVoidHere(const NeverNullPtr<Token> Token);
	void LogError_UseingTypeinfoInNonEvalVarable(const NeverNullPtr<Token> Token);
	void LogError_UseingTypeinfoInEvalFuncPar(const NeverNullPtr<Token> Token);
	void LogError_CantOutputTypeinfo(const NeverNullPtr<Token> Token);
	void LogError_DoesNotHaveForOverload(const NeverNullPtr<Token> Token, TypeSymbol& ExType);
	void LogError_CantgussTypesTheresnoassignment(const NeverNullPtr<Token> Token);
	void LogError_MissingFunctionforTrait(const String_view& FuncName, const FuncInfo* Info, const NeverNullPtr<Symbol> Trait, const NeverNullPtr<Token> ClassNameToken);
	void LogError_CantUseOutInOverloadFunc(const Token& Name);
	void LogError_yieldnotAsync(const NeverNullPtr<Token> token);
	void LogError_UseScopeResolutionAndNotDot(const NeverNullPtr<Symbol>& Sym, const NeverNullPtr<Token>& ItemToken);
	void LogError_UseDotAndNotScopeResolution(const NeverNullPtr<Symbol>& Sym, const NeverNullPtr<Token>& ItemToken);
	void LogError_TypeIsNotCopyable(const NeverNullPtr<Token> Token, const TypeSymbol& Ex0Type);
	void LogError_TypeIsNotExport(const NeverNullPtr<Token> Token, const TypeSymbol Type, NeverNullPtr<Symbol> Symbol);
	void LogError_ExportIsPrivate(const NeverNullPtr<Token> Token, NeverNullPtr<Symbol> Symbol);

	ReadVarErrorCheck_t TryLogError_OnReadVar(String_view VarName, const NeverNullPtr<Token> Token, const NullablePtr<Symbol> Syb);
	void TryLogError_OnWritingVar(NeverNullPtr<Symbol> Symbol, const NeverNullPtr<Token> Token, const String_view Name, GetValueMode mode);

	String ToString(SymbolType Value) const;
	Class_Data* Assembly_GetAssemblyClass(const String& FullName);

	String IR_MangleName(const FuncInfo* Func);
	IRidentifierID IR_GetIRID(const FuncInfo* Func);

	GenericData::Type Generic_TypeToGenericDataType(GenericValueNode::GenericType type);
};
UCodeLangFrontEnd

#endif
