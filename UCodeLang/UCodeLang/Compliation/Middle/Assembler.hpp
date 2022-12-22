#pragma once
#include "../../LangCore.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../Helpers/InstructionBuilder.hpp"
#include "../CompliationSettings.hpp"
#include "../Front/SemanticAnalysis.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"
UCodeLangStart


using symbolType_t = UInt8;
enum class symbolType : symbolType_t
{
	Null,
	Class,
	Func,
	Var,

	Parameter,
	Parameter_Input,//A parameter but its not on the stack its in the input reg
	EnumValue,
	ThisVar,
	This,
	StaticVar,
	ClassValue,

};

struct TypeData
{
	String Name;
	UAddress Size = 1;

	static void SetToUInt8(TypeData& Out)
	{
		Out.Name = Uint8TypeName;
		Out.Size = sizeof(UInt8);
	}
};
struct symbolInfo
{
	String_view FilePath;
	size_t Line = 0;
	size_t Pos = 0;
	String_view Name;
};
struct func_symbol
{
	symbolInfo Data;
	TypeData RetType;
};
struct Var_symbol
{
	enum class LocationType : UInt8
	{
		Null,
		Stack,
	};
	symbolInfo Data;
	TypeData Type;

	LocationType Location = LocationType::Null;
	UAddress Address = 0;
};

class symbol
{
public:
	
	symbolType _Type;
	//union
	//{
	
		func_symbol Func;
		Var_symbol Var;
	//};
	symbol(symbolType Type)
	{
		_Type = Type;
		
	}
	
	~symbol()
	{

	}
	inline void SetInfo(symbolInfo& Info)
	{
		switch (_Type)
		{
		case UCodeLang::symbolType::Null:
			break;
		case UCodeLang::symbolType::Func:
			Func.Data = Info;
			break;
		case UCodeLang::symbolType::Var:
			Var.Data = Info;
			break;
		default:
			break;
		}
	}
	inline symbolInfo& GetInfo()
	{
		switch (_Type)
		{
		case UCodeLang::symbolType::Null:
			break;
		case UCodeLang::symbolType::Func:
			return Func.Data;
		case UCodeLang::symbolType::Var:
			return Var.Data;
		default:
			break;
		}
	}

};

class SymbolManger
{
public:
	ScopeHelper Scope;
	Vector<symbol*> Symbols;
	Vector<String_view> Use_;

	UCodeLangForceinline void AddUseing(const String_view& Name)
	{
		Use_.push_back(Name);
	}

	symbol* FindSymbol(const String_view& Name)
	{
		symbol* r = FindSymbol(Name, Scope.ThisScope);

		if (r == nullptr)
		{
			for (auto Item : Use_)
			{
				r = FindSymbol(Name, Scope.ThisScope);
				if (r != nullptr){return r;}
			}
		}
		return r;
	}

	symbol* FindSymbol(const String_view& Name, const String_view& Scope)
	{
		String TepScope = Scope.data();

		while (true)
		{
			String FullName = TepScope + ScopeHelper::_ScopeSep + Name.data();
			for (auto& item : Symbols)
			{
				auto& Info = item->GetInfo();
				if (Info.Name == FullName)
				{
					return item;
				}
			}

			if (TepScope == ScopeHelper::_globalScope) { break; }
			ScopeHelper::ReMoveScope(TepScope);
		}
		return nullptr;
	}
	//
	symbol& AddSymbol(const String_view Name,symbolType Type, symbolInfo& FileInfo)
	{
		Symbols.push_back(new symbol(Type));
		auto& V = Symbols.back();

		FileInfo.Name = Name;
		V->SetInfo(FileInfo);
		return *V;
	};
	func_symbol& AddFunc(const String_view Name, symbolInfo& FileInfo)
	{
		return AddSymbol(Name,symbolType::Func, FileInfo).Func;
	};
	Var_symbol& AddVar(const String_view Name, symbolInfo& FileInfo)
	{
		return AddSymbol(Name, symbolType::Var, FileInfo).Var;
	};
	~SymbolManger()
	{
		for (auto Item : Symbols)
		{
			delete Item;
		}
		Symbols.clear();
	}
	void clear()
	{
		for (auto Item : Symbols)
		{
			delete Item;
		}
		Scope.ThisScope = ScopeHelper::_globalScope;
		Symbols.clear();
	}
};


struct RegisterManger
{
	enum class InUseState : UInt8
	{
		NotinUse,
		HasSymbol,
		HasData,
	};
	struct RegistersInUseData
	{
		InUseState InUse;
		Var_symbol* WhosInhere;//if this is null when Isuse then its for math.
		RegistersInUseData() :InUse(InUseState::NotinUse), WhosInhere(nullptr)
		{

		}

	};

	static constexpr size_t RegisterMaxIndex = (RegisterID_t)RegisterID::EndRegister - (RegisterID_t)RegisterID::StartRegister;
	static constexpr size_t RegisterCount = 1 + RegisterMaxIndex;

	RegistersInUseData RegistersInUse[RegisterCount];

	UCodeLangForceinline void SetRegisterUse(RegisterID id, bool V)
	{
		RegistersInUse[(RegisterID_t)id].InUse = V ? InUseState::HasData : InUseState::NotinUse;
	}
	void SetRegisterUse(RegisterID id, Var_symbol* Symbol)
	{
		auto& V = RegistersInUse[(RegisterID_t)id];
		V.InUse = InUseState::HasSymbol;
		V.WhosInhere = Symbol;
	}

	UCodeLangForceinline void RegisterLock(RegisterID id) { SetRegisterUse(id, true); }
	UCodeLangForceinline void RegisterUnLock(RegisterID id) { SetRegisterUse(id, false); }
	RegisterID GetFreeRegister()
	{
		for (size_t i = 0; i < RegisterCount; i++)
		{
			auto& item = RegistersInUse[(RegisterID_t)i];
			bool V = item.InUse == InUseState::NotinUse;
			if (V)
			{
				item.WhosInhere = nullptr;
				return (RegisterID)i;
			}
		}
		return RegisterID::NullRegister;
	}
	RegisterID GetFreeRegisterAndLock()
	{
		for (size_t i = 0; i < RegisterCount; i++)
		{
			auto& item = RegistersInUse[(RegisterID_t)i];
			bool V = item.InUse == InUseState::NotinUse;
			if (V)
			{
				auto V = (RegisterID)i;
				SetRegisterUse(V, true);
				item.WhosInhere = nullptr;
				return (RegisterID)V;
			}
		}
		return RegisterID::NullRegister;
	}
	bool IsRegisterInUse(RegisterID id)
	{
		return RegistersInUse[(RegisterID_t)id].InUse != InUseState::NotinUse;
	}
	RegistersInUseData& GetRegisterInUseData(RegisterID id)
	{
		return RegistersInUse[(RegisterID_t)id];
	}
	void ResetRegistersData()
	{
		for (size_t i = 0; i < RegisterMaxIndex; i++)
		{
			RegistersInUse[i] = RegistersInUseData();
		}
	}

};

class Assembler
{
public:
	void Assemble(UClib* Output,UClib* Data);
	void LinkIns();
	void Reset();

	Assembler(){}
	~Assembler(){}
	UCodeLangForceinline UClib* Get_Output() { return _OutPut; }
	UCodeLangForceinline bool Get_Success() { return _LinkSuccess; }
	
	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V){_ErrorsOutput = V;}
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) { _Settings = V; }
private:
	struct Intermediate_Instruction
	{
		union
		{
			Intermediate_Set OpCode;
			InstructionSet_t OpCode_AsInt;
		};
		AnyInt64 Value0;
		AnyInt64 Value1;
	};
	
	
	
	UClib* _Input = nullptr;
	UClib* _OutPut = nullptr;
	bool _LinkSuccess = false;
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	
	SymbolManger Symbols;
	symbolInfo ThisSymbolInfo;

	std::stack<ClassData*> OnClass;

	UIntNative funcStackSize = 0;
	UIntNative StaticStackSize = 0;
	UIntNative ParameterSize = 0;
	unordered_map<String_view, UAddress> _Strings;
	
	
	

	UAssembly::UAssembly _Assembly;
	size_t Index =0;
	size_t Debugoffset = 0;
	
	UCodeLangForceinline Intermediate_Instruction* Get_Ins()
	{
		if (Index < _Input->Get_Instructions().size())
		{
			return (Intermediate_Instruction*)&_Input->Get_Instructions()[Index];
		}
		else
		{
			return nullptr;
		}
	}
	UCodeLangForceinline void NextIns()
	{
		Index++;
	}
	

	void BuildBuffer();
	void BuildTypes();
	void BuildCode();
	void BuildClassType(Intermediate_Instruction& Inter);
	void BuildClass(Intermediate_Instruction& Inter);
	void BuildDeclareFunc(Intermediate_Instruction& Inter);
	void SetFilePos(Intermediate_Instruction& Inter);
	


	String_view Get_StringFromDebug(UAddress I)
	{
		auto Ptr = &_Input->Get_DebugByte()[(UAddress)Debugoffset + I];
		return String_view((const char*)Ptr);
	}
	inline bool IsExpression(Intermediate_Set Op)
	{
		switch (Op)
		{
		case UCodeLang::Intermediate_Set::DeclareExpression:return true;
		case UCodeLang::Intermediate_Set::DeclareBinaryExpression:return true;
		case UCodeLang::Intermediate_Set::UnaryExpression:return true;
		default:return true;
		}
	}

	//
	Vector<String> AddVarTep;
	UCodeLangForceinline Var_symbol& AddVar(const String_view Name)
	{
		AddVarTep.push_back({});
		String& NewName = AddVarTep.back();
		NewName = Symbols.Scope.ThisScope + ScopeHelper::_ScopeSep + (String)Name;

		return Symbols.AddVar(NewName, ThisSymbolInfo);
	}
	UCodeLangForceinline func_symbol& AddFunc(const String_view Name)
	{
		AddVarTep.push_back({});
		String& NewName = AddVarTep.back();
		NewName = Symbols.Scope.ThisScope + ScopeHelper::_ScopeSep + (String)Name;

		return Symbols.AddFunc(NewName, ThisSymbolInfo);
	}
	UCodeLangForceinline auto FindSymbol(const String_view Name)
	{
		return Symbols.FindSymbol(Name);
	}
	
	TypeData* FuncType =nullptr;
	TypeData* Exlooking =nullptr;

	//Building Stuff
	UCodeLang::Instruction _Ins;
	RegisterManger _RegisterState;
	UAddress Build_StaticString(const String_view& Value)
	{
		return _OutPut->AddStaticBytes(Value);
	}
	UAddress Build_DegugString(const String_view& Value)
	{
		return _OutPut->AddDebugBytes(Value);
	}
	UCodeLangForceinline void ReSetIns()
	{
		_Ins = Instruction();
	}
	UCodeLangForceinline UAddress PushIns()
	{
		return _OutPut->Add_Instruction(_Ins);
	}

	void BuildAsm(Intermediate_Instruction& Ins);

	struct DeclareExpression_ret_t
	{
		RegisterID Value;
		TypeData ExpressionType;
	};
	DeclareExpression_ret_t DeclareExpressionType(Intermediate_Instruction& Ins);
	DeclareExpression_ret_t DeclareExpression(Intermediate_Instruction& Ins);
	DeclareExpression_ret_t DeclareBinaryExpression(Intermediate_Instruction& Ins);
	void BuildRet(Intermediate_Instruction& Ins);
	void BuildDeclareVar(Intermediate_Instruction& Ins);

	void GetType(TypeData& Out);
	void BuildSetVar(Var_symbol* Sym, RegisterID Register);
	void BuildRegToReg8(RegisterID A, RegisterID B);
	void BuildRegToReg16(RegisterID A, RegisterID B);
	void BuildRegToReg32(RegisterID A, RegisterID B);
	void BuildRegToReg64(RegisterID A, RegisterID B);
	void BuildRegToRegNative(RegisterID A, RegisterID B);
	//generates code that puts a Symbol in a register
	RegisterID GetSymbolInFreeRegister(Var_symbol* Symbol)
	{
		auto V = GetSymbolInRegisterAndLock(Symbol);
		_RegisterState.RegisterUnLock(V);
		return V;
	}
	RegisterID GetSymbolInRegisterAndLock(Var_symbol* Symbol)
	{
		for (size_t i = 0; i < RegisterManger::RegisterCount; i++)
		{
			auto RID = (RegisterID)i;
			auto& R = _RegisterState.GetRegisterInUseData(RID);

			if (R.WhosInhere == Symbol)
			{
				_RegisterState.RegisterLock(RID);
				return RID;
			}
		}


		auto V = _RegisterState.GetFreeRegister();
		if (V != RegisterID::NullRegister)
		{
			GetSymbolInRegister(Symbol, V);
			_RegisterState.RegisterLock(V);
		}
		return V;
	}
	void GetSymbolInRegister(Var_symbol* Symbol, RegisterID id);


	
	
	enum class MathType : UInt8
	{
		Null,
		Add,
		Sub,
	};
	struct MathData
	{
		MathType Type = MathType::Null;
		BitSizeType DataSize = BitSizeType::Null;
	};
	MathType From(Intermediate_Set Op)
	{
		switch (Op)
		{
		case Intermediate_Set::Binary_plus:return MathType::Add;
		case Intermediate_Set::Binary_minus:return MathType::Sub;
		default:return MathType::Null;
		}
	}
	RegisterID BuildStore8(UInt8 Num);
	RegisterID BuildStoreNative(UIntNative Num);

	
	RegisterID DoMath_GetOffset(RegisterID Pointer, UIntNative offset)
	{
		constexpr auto NewV = RegisterID::MathOuPutRegister;
		RegisterID R;
		if (!_RegisterState.IsRegisterInUse(NewV))
		{
			R = NewV;
			DoMath_GetOffset(Pointer, offset, R);
		}
		else
		{
			R = _RegisterState.GetFreeRegister();
			_RegisterState.RegisterLock(R);
			DoMath_GetOffset(Pointer, offset, R);
			_RegisterState.RegisterUnLock(R);
		}
		return R;

	}
	void DoMath_GetOffset(RegisterID Pointer, UIntNative offset, RegisterID Out)
	{
		MathData Data;
		Data.DataSize = BitSizeType::BitNative;
		Data.Type = MathType::Add;
		DoMath(Pointer,BuildStoreNative(offset), Data,Out);
	}
	void DoMath(RegisterID A, RegisterID B, MathData Type, RegisterID Out);
	RegisterID DoMath(RegisterID A, RegisterID B, MathData Type)
	{
		constexpr auto NewV = RegisterID::OuPutRegister;
		RegisterID R;
		if (!_RegisterState.IsRegisterInUse(NewV))
		{
			R = NewV;
			DoMath(A, B, Type,R);
		}
		else
		{
			R = _RegisterState.GetFreeRegister();
			_RegisterState.RegisterLock(R);
			DoMath(A, B, Type,R);
			_RegisterState.RegisterUnLock(R);
		}
		return R;
	}
};
UCodeLangEnd
