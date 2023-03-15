#include "RunTimeLib.hpp"
#include "../Compliation/Helpers/InstructionBuilder.hpp"
UCodeLangStart







void RunTimeLib::Init(UClib* Lib)
{
	_Lib = Lib;
	_Instruction.clear();
	_Instruction = _Lib->Get_Instructions();
}
void RunTimeLib::UnLoad()
{
	_Lib = nullptr;
	_Instruction.clear();
}
UCodeLangEnd


