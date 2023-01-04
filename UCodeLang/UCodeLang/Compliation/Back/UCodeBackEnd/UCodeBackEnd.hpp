#pragma once
#include "../BackEndInterface.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
UCodeLangStart

class UCodeBackEndObject
{
public:
	UCodeBackEndObject();
	~UCodeBackEndObject();

	void BuildStatements(const Vector<Node*>& nodes);

	void BuildAsmNode(const AsmBlockNode& node);

	UAssembly::UAssembly UAssembly;
	CompliationErrors* ErrorOutput = nullptr;
	SystematicAnalysis* Analysis = nullptr;
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
	static auto BuildStatements(BackEnd* Obj, const Vector<Node*>& nodes){((BackEnd*)Obj)->BuildStatements(nodes);}
	static auto Set_ErrorsOutput(BackEnd* Obj, CompliationErrors* Errors) { ((BackEnd*)Obj)->ErrorOutput = Errors; }
	static auto Set_Analysis(BackEnd* Obj, SystematicAnalysis* Analysis){ ((BackEnd*)Obj)->Analysis = Analysis; }
};
UCodeLangEnd

