#include "Jit_Interpreter.hpp"
#include "Zydis/Zydis.h"
#include <inttypes.h>
#include <stdio.h>
#include <sstream>
UCodeLangStart

int Other()
{
	return 0;
}
int Func()
{
	return Other();
}

//template
CPPCallRet TempFunc(InterpreterCPPinterface& Input)
{
	auto R = Func();
	Input.Set_Return(&R,sizeof(R));
}

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

struct JitRuningState
{
	Jit_Interpreter* _This = nullptr;
	Stack<UAddress> StackFrames;
};

thread_local JitRuningState _ThisState = {};

Interpreter::Return_t Jit_Interpreter::Call(UAddress address)
{
	#if HasNoSupportforJit
	return _Interpreter.Call(address);
	#else
	if (!UFuncToCPPFunc.count(address)){UFuncToCPPFunc[address] = {};}
	auto& Item = UFuncToCPPFunc[address];
	
	TempFunc(InterpreterCPPinterface(&_Interpreter));

	BuildCheck(Item, address);

	#if UCodeLang_KeepJitInterpreterFallback
	if (Item.Type == JitFuncType::UCodeCall)
	{
		return _Interpreter.Call(Item.UCodeFunc);
	}
	else
	{
		_ThisState._This = this;
		_ThisState.StackFrames.push(Item.UCodeFunc);
		{
			Item.Func(InterpreterCPPinterface(&_Interpreter));
		}
		_ThisState.StackFrames.pop();

		return { Interpreter::RetState::Success ,_Interpreter.Get_OutRegister() };
	}
	#else
	throw std::exception("not added");
	#endif

	#endif
}



void Jit_Interpreter::BuildCheck(UCodeLang::Jit_Interpreter::JitFuncData& Item, const UCodeLang::UAddress& address)
{
#if HasSupportforJit 
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
		else
		{
			_Assembler.State = Get_State();
			_Assembler.Func = Get_State()->GetMethod(address);
		

			if (_Assembler.Func && _Assembler.BuildFunc(Insts, address, TepOutBuffer))
			{
				size_t InsSize = TepOutBuffer.size();
				ExBuffer.SetToReadWriteMode();

				intptr_t Ptr = (intptr_t)ExBuffer.Data + (intptr_t)Insoffset;

				Insoffset += TepOutBuffer.size();
				Item.Type = JitFuncType::CPPCall;
				Item.Func = (JitFunc)Ptr;
				Item.NativeFunc = (void*)(Ptr + _Assembler.Out_NativeCallOffset);

				for (auto& Item : _Assembler.NullCalls)
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
						_Assembler.SubCall((JitInfo::FuncType)OnUAddressCall, Item.CPPoffset, TepOutBuffer);
					}
				}




				memcpy((void*)Ptr, TepOutBuffer.data(), InsSize);

				LogASM();

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
	}
#endif
}


String byte_to_hex(Byte i)
{
	static const char characrers[] = "0123456789ABCDEF";

	String R(2,0);
	
	auto Buff = R.data();

	*Buff++ = characrers[i >> 4];
	*Buff++ = characrers[i & 0x0f];

	return R;
}

void Jit_Interpreter::LogASM()
{
#if HasSupportforJit
	void* InsData = ExBuffer.Data;
	size_t InsSize = Insoffset;
#if UCodeLang_CPUIs_x86_64 || UCodeLang_CPUIs_x86
	ZyanU64 runtime_address = (ZyanU64)InsData;

	// Loop over the instructions in our buffer.
	ZyanUSize offset = 0;
	ZydisDisassembledInstruction instruction;


	const size_t MaxInsSize = 8;
	while (ZYAN_SUCCESS(ZydisDisassembleIntel(
		/* machine_mode:    */ ZYDIS_MACHINE_MODE_LONG_64,
		/* runtime_address: */ runtime_address,
		/* buffer:          */ (void*)((uintptr_t)InsData + offset),
		/* length:          */ Insoffset - offset,
		/* instruction:     */ &instruction
	)))
	{
		printf("%016" PRIX64 " ", runtime_address);
		
		std::cout << ":";

		for (size_t i = 0; i < MaxInsSize; i++)
		{
			if (i > instruction.info.length)
			{
				std::cout << "   ";
			}
			else 
			{
				Byte* byte = &((Byte*)((uintptr_t)InsData + offset))[i];
				std::cout << byte_to_hex(*byte) << " ";
			}
		}
		String InsStr = instruction.text;

		std::cout << std::dec << InsStr;
		
		for (auto& Item : UFuncToCPPFunc)
		{
			if (Item._Value.UCodeFunc == runtime_address)
			{
				std::cout << "//CPPCall:" << Item._Key;

				auto V = Get_State()->GetMethod(Item._Key);
				if (V) {
					std::cout << "/" << V->DecorationName;
				}
			}
			else if ((ZyanU64)Item._Value.NativeFunc == runtime_address)
			{
				std::cout << "//Native:" << Item._Key;

				auto V = Get_State()->GetMethod(Item._Key);
				if (V) {
					std::cout << "/" << V->DecorationName;
				}
			}
		}
		
		std::cout << '\n';

		

		offset += instruction.info.length;
		runtime_address += instruction.info.length;
	}
#endif


#endif
}
UInt64 Jit_Interpreter::OnUAddressCall()
{
#if HasSupportforJit
	//save all pars on stack and build code

	UAddress TryingToCall = _ThisState._This->_Assembler.OnUAddressPar;
	UAddress address = _ThisState.StackFrames.top();
	auto& Item = _ThisState._This->UFuncToCPPFunc[TryingToCall];
	_ThisState._This->BuildCheck(Item, address);



	#if UCodeLang_KeepJitInterpreterFallback
	if (Item.Type == JitFuncType::UCodeCall)
	{
		_ThisState._This->_Interpreter.Call(Item.UCodeFunc);
		return _ThisState._This->_Interpreter.Get_OutRegister().Value.AsUInt64;
	}
	else
	{
		_ThisState.StackFrames.push(Item.UCodeFunc);
		{
			using Func = UInt64(*)();
			auto V = (*(Func*)&Item.Func)();
			
			_ThisState.StackFrames.pop();
			return V;
		}
	}
	#else
	throw std::exception("not added");
	#endif
	
	//UAddress V = Cpp.GetParameters<UAddress>();
	//Call(V);

	throw std::exception("not added");

#endif
	return 0;
}
CPPCallRet Jit_Interpreter::Call_CPPFunc(JitFunc ToCall)
{
	#if UCodeLang_KeepJitInterpreterFallback

	{
		//push pars

	}


	ToCall(InterpreterCPPinterface(&_Interpreter));
	//_Interpreter.Get_OutRegister().Value = r;
	
	
	
#else
	throw std::exception("not added");
	#endif
}

UCodeLangEnd