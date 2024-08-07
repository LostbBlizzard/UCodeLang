#include "RunTimeLib.hpp"
#include "../Compilation/Helpers/InstructionBuilder.hpp"
UCodeLangStart







void RunTimeLib::Init(UClib* Lib)
{
	_Lib = Lib;
	_Instruction.clear();
	_NameToCppCall.clear();

	auto UCodeLayer = Lib->GetLayer(UCode_CodeLayer_UCodeVM_Name);
	if (UCodeLayer)
	{
		auto& Info = UCodeLayer->_Data.Get<CodeLayer::UCodeByteCode>();
		_Instruction = Info.Get_Instructions();
	}
}
void RunTimeLib::UnLoad()
{
	_Lib = nullptr;
	_Instruction.clear();
	_NameToCppCall.clear();
}
UCodeLangEnd


