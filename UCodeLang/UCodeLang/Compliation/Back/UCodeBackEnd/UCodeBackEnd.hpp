#pragma once
#include "../BackEndInterface.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
#include "UCodeLang/Compliation/Middle/SystematicAnalysis.hpp"
UCodeLangStart

class UCodeBackEndObject
{
public:
	UCodeBackEndObject();
	~UCodeBackEndObject();


	void Build(BackEndInput& Input);

	void BuildAsmNode(const AsmBlockNode& node);
	void BuildRetNode(const RetStatementNode& node);
	void BuildExpression(const ExpressionNodeType& node);

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

