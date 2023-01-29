#pragma once
#include "../BackEndInterface.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
#include "UCodeLang/Compliation/Middle/SystematicAnalysis.hpp"
#include "RegistersManager.hpp"
UCodeLangStart

class UCodeBackEndObject
{
	
public:
	UCodeBackEndObject();
	~UCodeBackEndObject();


	void Build(BackEndInput& Input);
	

	UAssembly::UAssembly UAssembly;
	CompliationErrors* ErrorOutput = nullptr;
	SystematicAnalysis* Analysis = nullptr;
	UCodeLangForceinline UClib& Getliboutput()
	{
		return Analysis->Get_Output();
	}
	UCodeLangForceinline void ResetIns()
	{
		_Ins = Instruction();
	}
private:
	Instruction _Ins;
	BackEndInput* _BackInput = nullptr;
	size_t _Index = 0;
	RegistersManager _Registers;
	RegisterID ParameterRegisterValue = RegisterID::StartParameterRegister;
	RegisterID CallParameterRegisterValue = RegisterID::StartParameterRegister;
	UAddress StackSize = 0;
	static constexpr size_t RegisterSize = sizeof(AnyInt64);

	void BuildFunc();
	void OnAsPointer(UCodeLang::RegisterID& R, UCodeLang::IRCode& IR);
	void StoreResultIR(UCodeLang::IRCode& IR, UCodeLang::RegisterID R);
	void OnReadVarOperand(UCodeLang::RegisterID& R, UCodeLang::IRCode& IR, UCodeLang::UClib& ULib);
	void Link();

	void SetSybToRegister(RegisterID R,IRCode& IR);
	void SetIRToRegister(RegisterID R, IRField IR);

	RegisterID GetOperandInAnyRegister(const IROperand& operand);
	void GetOperandInRegister(const IROperand& operand, RegisterID id);
	void StoreVar(const IRCode& IR, const RegisterID R);

	enum class BuildData_t :UInt8
	{
		Null,
		ThisObjectWithOffset,
		ParameterInRegister,
	};
	struct BuildData
	{
		UAddress offset = NullAddress;
		UAddress DataSize = NullAddress;
		BuildData_t Type = BuildData_t::Null;
	};

	Unordered_map<SymbolID, BuildData> SymbolToData;
	
	struct CallInfo
	{
		size_t CallIns=0;
		SymbolID ID =0;
	};
	Vector<CallInfo> _InsCalls;
	struct DeclareCall
	{
		size_t FuncAddress = 0;
	};
	Unordered_map<SymbolID, DeclareCall> DeclareCalls;
	Unordered_map<IRField, UAddress> IRCodeIndexToUAddressIndexs;

	struct JumpInsInfo
	{
		UAddress InsAddress = 0;
	};
	Vector<JumpInsInfo> JumpCallsToUpdate;
};
class UCodeBackEnd
{
	//You can Reuse some parts of the default backend if needed.
public:
	static void Bind(BackEndInterface& Obj);
	static const BackEndInterface* Get();

	//Just Copy and paste for your backend
	using BackEnd = UCodeBackEndObject;
	static auto Gen() { return new BackEnd(); }
	static auto Delete(BackEnd* Obj) { delete Obj; }
	static auto Set_ErrorsOutput(BackEnd* Obj, CompliationErrors* Errors) { ((BackEnd*)Obj)->ErrorOutput = Errors; }
	static auto Set_Analysis(BackEnd* Obj, SystematicAnalysis* Analysis){ ((BackEnd*)Obj)->Analysis = Analysis; }
	static auto Build(BackEnd* Obj, BackEndInput& Input) { ((BackEnd*)Obj)->Build(Input); }
};
UCodeLangEnd

