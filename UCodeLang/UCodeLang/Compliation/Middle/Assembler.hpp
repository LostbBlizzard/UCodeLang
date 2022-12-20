#pragma once
#include "../../LangCore.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../Helpers/InstructionBuilder.hpp"
#include "../CompliationSettings.hpp"
#include "../Front/SemanticAnalysis.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
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
};
struct symbolInfo
{
	String_view File;
	size_t Line = 0;
	size_t Pos = 0;
	String_view Name;
};
struct func_symbol
{
	symbolInfo Data;
};

class symbol
{
public:
	
	symbolType Type;
	union 
	{
		symbolInfo Data;
		func_symbol Func;
	};
};

class SymbolManger
{
public:
	ScopeHelper Scope;
	Vector<symbol> Symbols;
	Vector<String_view> Use_;

	UCodeLangForceinline void AddUseing(const String_view& Name)
	{
		Use_.push_back(Name);
	}

	symbol* FindSymbol(const String_view& Name, const String_view& Scope = ScopeHelper::_globalScope)
	{
		String TepScope = Scope.data();

		while (true)
		{
			String FullName = TepScope + Name.data();
			for (auto& item : Symbols)
			{
				if (item.Data.Name == FullName)
				{
					return &item;
				}
			}

			if (TepScope == ScopeHelper::_globalScope) { break; }
			ScopeHelper::ReMoveScope(TepScope);
		}
		return nullptr;
	}
	//
	symbol& AddSymbol(const String_view Name, symbolInfo& FileInfo)
	{
		Symbols.push_back({});
		auto& V = Symbols.back();
		V.Data = FileInfo;
		return V;
	};
	void clear()
	{
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
		HasNumber,
	};
	struct RegistersInUseData
	{
		InUseState InUse;
		symbol* WhosInhere;//if this is null when Isuse then its for math.
		RegistersInUseData() :InUse(InUseState::NotinUse), WhosInhere(nullptr)
		{

		}

	};

	static constexpr size_t RegisterMaxIndex = (RegisterID_t)RegisterID::EndRegister - (RegisterID_t)RegisterID::StartRegister;
	static constexpr size_t RegisterCount = 1 + RegisterMaxIndex;

	RegistersInUseData RegistersInUse[RegisterCount];

	void SetRegisterUse(RegisterID id, bool V)
	{
		RegistersInUse[(RegisterID_t)id].InUse = V ? InUseState::HasSymbol : InUseState::NotinUse;
	}
	void SetRegisterUse(RegisterID id, symbol* Symbol)
	{
		auto& V = RegistersInUse[(RegisterID_t)id];
		V.InUse = InUseState::HasSymbol;
		V.WhosInhere = Symbol;
	}

	inline void RegisterLock(RegisterID id) { SetRegisterUse(id, true); }
	inline void RegisterUnLock(RegisterID id) { SetRegisterUse(id, false); }
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
		return RegistersInUse[(RegisterID_t)id].InUse == InUseState::NotinUse;
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
	RegisterID GetSymbolInRegisterAndLock(symbol* Symbol)
	{


		for (size_t i = 0; i < RegisterCount; i++)
		{
			auto RID = (RegisterID)i;
			auto& R = GetRegisterInUseData(RID);

			if (R.WhosInhere == Symbol)
			{
				RegisterLock(RID);
				return RID;
			}
		}


		auto V = GetFreeRegister();
		if (V != RegisterID::NullRegister)
		{
			GetSymbolInRegister(Symbol, V);
			RegisterLock(V);
		}
		return V;
	}
	//generates code that puts a Symbol in a register
	void GetSymbolInRegister(symbol* Symbol, RegisterID id);
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
	UCodeLangForceinline UAddress PushIns()
	{
		return _OutPut->Add_Instruction(_Ins);
	}

	void BuildBuffer();
	void BuildTypes();
	void BuildCode();
	
	String_view Get_StringFromDebug(UAddress I)
	{
		auto Ptr = &_Input->Get_DebugByte()[(UAddress)Debugoffset + I];
		return String_view((const char*)Ptr);
	}
	
	//Building Stuff
	RegisterID OutValue =RegisterID::NullRegister;
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
	inline void ReSetIns()
	{
		_Ins = Instruction();
	}
	void BuildAsm(Intermediate_Instruction& Ins);
};
UCodeLangEnd
