#pragma once
#include "../BackEndInterface.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
#include "RegistersManager.hpp"
UCodeLangStart

class UCodeBackEndObject : BackEndObject
{
	
public:
	UCodeBackEndObject();
	~UCodeBackEndObject() override;

	void Reset() override;
	void Build(const IRBuilder* Input) override;
	
	static BackEndObject* MakeObject(){return new UCodeBackEndObject();}
private:
	using UObjectSize = UInt64;
	UCodeLangForceinline void ResetIns()
	{
		_Ins = Instruction();
	}

	Instruction _Ins;
	const IRBuilder* _BackInput = nullptr;
	size_t _Index = 0;
	RegistersManager _Registers;
	RegisterID ParameterRegisterValue = RegisterID::StartParameterRegister;
	RegisterID CallParameterRegisterValue = RegisterID::StartParameterRegister;
	UObjectSize StackSize = 0;
	static constexpr size_t RegisterSize = sizeof(AnyInt64);

	void BuildFunc();
	void BuildOperandA(const UCodeLang::IRCode& IR, UCodeLang::RegisterID& R, UCodeLang::UClib& ULib);
	void OnAsPointer(UCodeLang::RegisterID& R, const UCodeLang::IRCode& IR);
	void StoreResultIR(const IRCode& IR, UCodeLang::RegisterID R);
	void OnReadVarOperand(UCodeLang::RegisterID& R, const IRCode& IR, UCodeLang::UClib& ULib);
	void Link();

	void SetSybToRegister(RegisterID R,const IRCode& IR);
	void SetIRToRegister(RegisterID R, IRField IR);

	RegisterID GetOperandInAnyRegister(const IRCode& IR, const IROperand& operand);
	void GetOperandInRegister(const IRCode& IR, const IROperand& operand, RegisterID id);
	void StoreVar(const IRCode& IR, const RegisterID R);

	enum class BuildData_t :UInt8
	{
		Null,
		StackVarable,
		ThisObjectWithOffset,
		ParameterInRegister,
	};
	struct BuildData
	{
		UObjectSize offset = NullAddress;
		UObjectSize DataSize = NullAddress;
		BuildData_t Type = BuildData_t::Null;
	};

	Unordered_map<SymbolID, BuildData> SymbolToData;
	
	struct CallInfo
	{
		UObjectSize CallIns=0;
		SymbolID ID =0;
	};
	Vector<CallInfo> _InsCalls;
	struct DeclareCall
	{
		UObjectSize FuncAddress = 0;
	};
	Unordered_map<SymbolID, DeclareCall> DeclareCalls;
	Vector<UObjectSize> IRCodeIndexToUAddressIndexs;

	struct JumpInsInfo
	{
		UObjectSize InsAddress = 0;
		IRField IRField = 0;
	};
	Vector<JumpInsInfo> JumpCallsToUpdate;
};
UCodeLangEnd

