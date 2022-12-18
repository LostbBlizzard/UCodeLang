#pragma once
#pragma once
#include "../../LangCore.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../Helpers/InstructionBuilder.hpp"
#include "../CompliationSettings.hpp"
#include "../Front/SemanticAnalysis.hpp"

UCodeLangStart

class Amd64Assembler
{
public:
	Amd64Assembler(){}
	~Amd64Assembler(){}
	void Reset();
	bool BuildFunc(const Vector<Instruction>& Data, size_t Offset,Vector<UInt8>& Output);

private:
};

UCodeLangEnd

