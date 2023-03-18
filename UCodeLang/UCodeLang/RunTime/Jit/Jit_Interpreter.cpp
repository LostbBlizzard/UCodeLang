#include "Jit_Interpreter.hpp"
UCodeLangStart



Interpreter::Return_t Jit_Interpreter::ThisCall(UAddress This, const String& FunctionName)
{
	PushParameter(This);
	return Call(FunctionName);
}

Interpreter::Return_t Jit_Interpreter::ThisCall(UAddress This, UAddress address)
{
	PushParameter(This);
	return Call(address);
}

void Jit_Interpreter::PushParameter(const void* Value, size_t ValueSize)
{
	#if UCodeLang_KeepJitInterpreterFallback
	_Interpreter.PushParameter(Value, ValueSize);
	#else
	throw std::exception("not added");
	#endif
}

bool Jit_Interpreter::CheckIfFunctionExist(const String& FunctionName)
{
	#if UCodeLang_KeepJitInterpreterFallback
	return _Interpreter.CheckIfFunctionExist(FunctionName);
	#else
	throw std::exception("not added");
	#endif
}

void Jit_Interpreter::Get_Return(void* Output, size_t OutputSize)
{
	#if UCodeLang_KeepJitInterpreterFallback
	return _Interpreter.Get_Return(Output, OutputSize);
	#else
	throw std::exception("not added");
	#endif
}

Interpreter::Return_t Jit_Interpreter::Call(const String& FunctionName)
{
	auto address = Get_State()->FindAddress(FunctionName);
	if (!address.has_value())
	{
		return Interpreter::Return_t(Interpreter::RetState::Error_Function_doesnt_exist);
	}

	return Call(address.value());
}

Interpreter::Return_t Jit_Interpreter::Call(UAddress address)
{
	if (!UFuncToCPPFunc.count(address)){UFuncToCPPFunc[address] = {};}
	
	
	auto& Item = UFuncToCPPFunc[address];
	
	if (Item.Type == JitFuncType::Null)
	{
		auto& LibManger = Get_State()->Get_Libs();
		auto& Insts = LibManger.GetInstructions();
		TepOutBuffer.clear();

		Instruction& NextIns = Insts[address];
		if (NextIns.OpCode == InstructionSet::Call_Code)
		{
			Item.Type = JitFuncType::CPPCall;
			Item.Func = Get_State()->Get_Libs().Get_ExFunc(NextIns.Value0.AsAddress);
		}
		else if (_Assembler.BuildFunc(Insts, address, TepOutBuffer))
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

					_Assembler.SubCall(SomeV.Func, Item.CPPoffset, TepOutBuffer);
				}
				else
				{
					_Assembler.SubCall(OnUAddressCall, Item.CPPoffset, TepOutBuffer);
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

	#if UCodeLang_KeepJitInterpreterFallback
	if (Item.Type == JitFuncType::UCodeCall)
	{
		return _Interpreter.Call(Item.UCodeFunc);
	}
	else
	{
		//return { Interpreter::RetState::Success ,Interpreter::Register(Call_CPPFunc(Item.Func))};
		return { Interpreter::RetState::Success ,_Interpreter.Get_OutRegister() };
	}
	#else
	throw std::exception("not added");
	#endif
}
CPPCallRet Jit_Interpreter::OnUAddressCall(CPPCallParsNone)
{
	//save all pars on stack and build code
	//
	//UAddress V = Cpp.GetParameters<UAddress>();
	//Call(V);

	throw std::exception("not added");
	SetCPPRetNone
}
CPPCallRet Jit_Interpreter::Call_CPPFunc(JitFunc ToCall)
{
	#if UCodeLang_KeepJitInterpreterFallback

	{
		//push pars

	}


	ToCall(InterpreterCPPinterface(&_Interpreter));
	//_Interpreter.Get_OutRegister().Value = r;
	
	SetCPPRetNone
	
#else
	throw std::exception("not added");
	#endif
}

UCodeLangEnd