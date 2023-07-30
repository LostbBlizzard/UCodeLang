#include "NativeInterpreter.hpp"
UCodeLangStart
bool NativeInterpreter::CheckIfFunctionExist(const String& FunctionName)
{
	//auto address = _State->FindAddress(FunctionName);
	//return address.has_value();
	return false;
}


NativeInterpreter::Return_t NativeInterpreter::Call(const String& FunctionName)
{
	return {};
}
NativeInterpreter::Return_t NativeInterpreter::Call(UAddress address)
{
	return {};
}
void NativeInterpreter::Get_Return(void* Output, size_t OutputSize)
{

}
UCodeLangEnd