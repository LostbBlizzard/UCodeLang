#pragma once
#include "../../LangCore.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../Helpers/InstructionBuilder.hpp"
#include "../CompliationSettings.hpp"
#include "../Front/SemanticAnalysis.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
UCodeLangStart

class Assembler
{
public:
	void Assemble(UClib* Output,UClib* Data);
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
	
	UClib* _Input = nullptr;
	UClib* _OutPut = nullptr;
	bool _LinkSuccess = false;
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	size_t Index =0;
	SemanticAnalysisData Symbols;
	ScopeHelper Scope;
	RegisterID OutValue =RegisterID::NullRegister;
	UCodeLang::Instruction _Ins;
	UIntNative funcStackSize = 0;
	UIntNative StaticStackSize = 0;
	UIntNative ParameterSize = 0;
	RegistersInUseData RegistersInUse[RegisterCount];
	unordered_map<String_view, UAddress> _Strings;
	size_t Debugoffset = 0;
	
	Vector<ClassData*> Tep;
	UAssembly::UAssembly _Assembly;


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

	
	

	void UpdateAllTypes()
	{
		for (auto Item : _Input->Get_Assembly().Classes)
		{
			UpdateTypeData(Item);
		}
	}
	inline ClassData* GetType(const String_view Name)
	{
		auto V = _Input->Get_Assembly().Find_Class(Name, ScopeHelper::_globalScope);
		if (V)
		{
			UpdateTypeData(V);
		}
		return V;
	}

	void UpdateTypeData(ClassData* Data);

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

	//generates code that puts a Symbol in a register
	void GetSymbolInRegister(symbol* Symbol, RegisterID id);
	void ForceSymbolInRegister(symbol* Symbol, RegisterID id);

	

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
			GetSymbolInRegister(Symbol,V);
			RegisterLock(V);
		}
		return V;
	}

	inline void ReSetIns()
	{
		_Ins = Instruction();
	}

	symbol* FindSymbol(const String_view& Name)
	{
		return Symbols.FindSymbol(Name,Scope.ThisScope);
	}
	symbol* FindVarSymbol(const String_view& Name)
	{
		return FindSymbol(Name);
	}
	symbol* FindFuncSymbol(const String_view& Name)
	{
		return FindSymbol(Name);
	}
	symbol* FindClassSymbol(const String_view& Name)
	{
		return FindSymbol(Name);
	}
	String_view Get_StringFromDebug(UAddress I)
	{
		auto Ptr = &_Input->Get_DebugByte()[(UAddress)Debugoffset + I];
		return String_view((const char*)Ptr);
	}
	//
	symbol& AddSymbol(const String_view Name,size_t Line,size_t pos)
	{
		Symbols.Symbols.push_back(symbol(Scope.ThisScope + Name.data()));
		auto& V = Symbols.Symbols.back();
		V.Line = Line;
		V.Pos = pos;
		return V;
	};
	symbol& AddVarSymbol(const String_view Name, size_t Line, size_t pos)
	{
		auto& V = AddSymbol(Name,Line,pos);
		V.Type = symbolType::Var;
		return V;
	};
	symbol& AddFuncSymbol(const String_view Name, size_t Line, size_t pos)
	{
		auto& V = AddSymbol(Name, Line, pos);
		V.Type = symbolType::Func;
		return V;
	};
	//
	UAddress Get_StaticString(const String_view& Value)
	{
		return _OutPut->AddStaticBytes(Value);
	}
	UAddress Get_DegugString(const String_view& Value)
	{
		return _OutPut->AddDebugBytes(Value);
	}
	//helpers
	void BuildDeclareFunc(Intermediate_Instruction& Ins);
	void DeclareVar(Intermediate_Instruction& Ins);
	void DeclareStaticVar(Intermediate_Instruction& Ins);
	void DeclareThisVar(Intermediate_Instruction& Ins);
	void DeclareThis(Intermediate_Instruction& Ins);
	void DeclareParameter(Intermediate_Instruction& Ins);
	void BuildAsm(Intermediate_Instruction& Ins);
	void StoreVar(Intermediate_Instruction& Ins);
	RegisterID GetVar(Intermediate_Instruction& Ins);
	RegisterID DeclareExpression();
	RegisterID DeclareBinaryExpression(Intermediate_Instruction& Ins);
	RegisterID UnaryExpression(Intermediate_Instruction& Ins);
	void BuildStore8RegToReg(RegisterID id, RegisterID out);
	void BuildStore64RegToReg(RegisterID id, RegisterID out);
	void BuildRawStringStaticOffset(RegisterID Register,const String_view& Value);
};
UCodeLangEnd
