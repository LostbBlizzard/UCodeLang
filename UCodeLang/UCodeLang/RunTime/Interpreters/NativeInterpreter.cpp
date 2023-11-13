#include "NativeInterpreter.hpp"
UCodeLangStart


using NativeCallType = void(UCodeLangAPI*)(NativeInterpreter* Input);

bool NativeInterpreter::CheckIfFunctionExist(const String& FunctionName)
{
	//auto address = _State->FindAddress(FunctionName);
	//return address.has_value();
	return false;
}


NativeInterpreter::Return_t NativeInterpreter::Call(const String& FunctionName)
{
	auto address = _State->FindAddress(FunctionName);
	if (!address.has_value())
	{
		return Return_t(RetState::Error_Function_doesnt_exist);
	}

	return Call(address.value());
}
NativeInterpreter::Return_t NativeInterpreter::Call(UAddress address)
{
	const void* Pointer = Get_State()->GetNativeCodeEx(address);
	auto func = (NativeCallType)Pointer;

	func(this);

	return {Interpreter::RetState::Success};
}
void NativeInterpreter::Get_Return(void* Output, size_t OutputSize)
{

}
UCodeLangEnd