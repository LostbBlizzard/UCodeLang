#pragma once
#include "UCodeLang/LangCore/Instruction.hpp"
UCodeLangStart


class InsHelper
{
public:
	static bool ReadReg(Instruction& Op, RegisterID A);
	static bool WriteReg(Instruction& Op, RegisterID A);
	static bool ReadPtrReg(Instruction& Op, RegisterID A);
	static bool WritePtrReg(Instruction& Op, RegisterID A);

	static void SetWriteReg(Instruction& Op, RegisterID A);
};

UCodeLangEnd