#include "Interpreter.hpp"

#include <UCodeLang/RunTime/NativeWappers.hpp>


UCodeLangStart

void Interpreter::InterpreterSysCall(InstructionSysCall SysCall, RegisterID ParReg)
{
	switch (SysCall)
	{

		//Cout and Cin
	case InstructionSysCall::Cout_CString:
		Log((const char*)Get_Register(ParReg).Value.AsPtr);
		break;
	case InstructionSysCall::Cout_Char:
		Log((const char*)&Get_Register(ParReg).Value.AsUInt8, 1);
		break;
	case InstructionSysCall::Cout_Buffer:
		Log((const char*)Get_Register(ParReg).Value.AsPtr, Get_InRegister().Value.AsUIntNative);
		break;
	case InstructionSysCall::Cout_ReadChar:
		Get_Register(ParReg).Value = this->Get_State()->ReadChar();
		break;


	case InstructionSysCall::FileP_Open:
		Get_Register(RegisterID::OuPutRegister).Value = UFileHandle::Open2((const UFileHandle::PathChar*)Get_Register(ParReg).Value.AsPtr, Get_InRegister().Value.AsUIntNative);
		break;
	case InstructionSysCall::File_Open:
		Get_Register(RegisterID::OuPutRegister).Value = UFileHandle::Open1((const char*)Get_Register(ParReg).Value.AsPtr, Get_InRegister().Value.AsUIntNative);
		break;
	case InstructionSysCall::File_Close:
		UFileHandle::Close(Get_Register(ParReg).Value.AsPtr);
		break;
	default:
		throw std::exception("bad SysCall");
		break;
	}
}

UCodeLangEnd