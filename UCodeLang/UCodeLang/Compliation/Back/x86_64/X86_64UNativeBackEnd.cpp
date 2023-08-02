#include "X86_64UNativeBackEnd.hpp"
UCodeLangStart
void X86_64UNativeBackEnd::Reset()
{
	this->~X86_64UNativeBackEnd();
	new (this) X86_64UNativeBackEnd;
}

void X86_64UNativeBackEnd::Build(const IRBuilder* Input)
{
	_Input = Input;
	_Output = &Get_Output();


	if (Get_Settings()._Type == OutPutType::IRAndSymbols)
	{
		auto V = _Output->AddLayer(UCode_CodeLayer_IR_Name);
		auto V2 = CodeLayer::JustData();
		V2._Data = Input->ToBytes().MoveToVector();

		V->_Data = std::move(V2);
	}
	else
	{
		auto V = _Output->AddLayer(UCode_CodeLayer_X86_UCodeVM_Name);
		V->_Data = CodeLayer::MachineCode();
		_OutLayer = V->_Data.Get_If<CodeLayer::MachineCode>();

		
	}
}
UCodeLangEnd