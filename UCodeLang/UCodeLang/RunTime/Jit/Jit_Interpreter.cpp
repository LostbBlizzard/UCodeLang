#include "Jit_Interpreter.hpp"
UCodeLangStart


void Test(Jit_Interpreter::CPPInput input)
{
	

	input.Set_Return(5);
}

Interpreter::Return_t Jit_Interpreter::ThisCall(PtrType This, const String& FunctionName, parameters Pars)
{
	return Interpreter::Return_t();
}

Interpreter::Return_t Jit_Interpreter::ThisCall(PtrType This, UAddress address, parameters Pars)
{
	return Interpreter::Return_t();
}

Interpreter::Return_t Jit_Interpreter::Call(const String& FunctionName, parameters Pars)
{
	auto address = Get_State()->FindAddress(FunctionName);
	if (address == NullAddress)
	{
		return Interpreter::Return_t(Interpreter::RetState::Error_Function_doesnt_exist);
	}

	return Call(address, Pars);
}

Interpreter::Return_t Jit_Interpreter::Call(UAddress address, parameters Pars)
{
	if (!UFuncToCPPFunc.count(address)){UFuncToCPPFunc[address] = {};}
	
	
	auto& Item = UFuncToCPPFunc[address];

	if (Item.Type == JitFuncType::Null)
	{
		auto& LibManger = Get_State()->Get_Libs();
		auto& Insts = LibManger.GetInstructions();
		TepOutBuffer.clear();
		if (_Assembler.BuildFunc(Insts, address, TepOutBuffer))
		{
			size_t InsSize = TepOutBuffer.size();
			ExBuffer.SetToReadWriteMode();

			intptr_t Ptr = (intptr_t)ExBuffer.Data + (intptr_t)Insoffset;

			Insoffset += TepOutBuffer.size();
			Item.Type = JitFuncType::CPPCall;
			Item.Func = (JitFunc)Ptr;

			for (auto Item : _Assembler.NullCalls)
			{
				Instruction& Ins = Insts[Item.UCodeAddress];
				UAddress CodeFuncAddress = Ins.Value0.AsAddress + 1;
				if (UFuncToCPPFunc.count(CodeFuncAddress))
				{
					auto& SomeV = UFuncToCPPFunc.at(CodeFuncAddress);
				
					_Assembler.SubCall((Amd64Assembler::VoidFunc)SomeV.Func, Item.CPPoffset, TepOutBuffer);
				}
				else
				{
					_Assembler.SubCall((Amd64Assembler::VoidFunc)OnUAddressCall, Item.CPPoffset, TepOutBuffer);
				}
			}
			
			
			
			memcpy((void*)Ptr, &TepOutBuffer[0], InsSize);

			ExBuffer.SetToExecuteMode();

			for (auto& Item : Insts)
			{
				if (Item.OpCode == InstructionSet::Call && Item.Value0.AsAddress == address)
				{
					Item.OpCode = InstructionSet::CPPCall;
					Item.Value0.AsPtr = (void*)Ptr;
				}
			}
		}
		else
		{
			Item.Type = JitFuncType::UCodeCall;
			Item.UCodeFunc = address;
		}
	}
	InterpreterCPPinterface Inter = &_Interpreter;
	Test(Inter);

	if (Item.Type == JitFuncType::UCodeCall)
	{
		return _Interpreter.Call(Item.UCodeFunc, Pars);
	}
	else
	{
		return { Interpreter::RetState::Success ,Interpreter::Register(Call_CPPFunc(Item.Func, Pars))};
	}
}

UCodeLangEnd