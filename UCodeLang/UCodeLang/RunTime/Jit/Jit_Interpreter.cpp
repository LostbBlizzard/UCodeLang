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
int Func(int A,int B)
{
	return Other();
}

//template
CPPCallRet TempFunc(InterpreterCPPinterface& Input)
{
	auto A = Input.GetParameter<int>();
	auto B = Input.GetParameter<int>();
	Input.Set_Return(Func(A,B));
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

void Jit_Interpreter::TryBuildAllFuncs()
{
	for (auto& Item : Get_State()->Get_Libs().Get_NameToAddress())
	{
		auto address = Item._Value;
		if (!UFuncToCPPFunc.count(address)) { UFuncToCPPFunc[address] = {}; }
		auto& Item = UFuncToCPPFunc[address];
		BuildCheck(Item, address);
	}

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

	//return _Interpreter.Call(address);//remove this when jit-Interpreter works


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

		_Interpreter.FlushParametersIntoCPU();
		
		{//here the magic happens and were your going to spend debug for hours.
			Item.Func(InterpreterCPPinterface(&_Interpreter));

			//using Func = int(*)(int V);
			//int V = 0;
			//V = ((Func)Item.NativeFunc)(5);
		}
		_ThisState.StackFrames.pop();

		return { Interpreter::RetState::Success, _Interpreter.Get_OutRegister()};
	}
	#else
	throw std::exception("not added");
	#endif

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

template<typename T>
String ToHex(T Value)
{
	String R;
	for (size_t i = 0; i < sizeof(T); i++)
	{
		R += byte_to_hex(((Byte*)&Value)[i]);
	}
	return R;
}

template<typename T>
String ToHexR(T Value,bool keep0Byte =false)
{
	String R;
	for (int i = sizeof(T) - 1; i >= 0; i--)
	{
		if (keep0Byte) {
			if (((Byte*)&Value)[i] == 0) { continue; }
		}

		R += byte_to_hex(((Byte*)&Value)[i]);
	}
	return R;
}

bool IsNumber(char Value)
{
	const char Num[] = "1234567890";
	for (size_t i = 0; i < sizeof(Num); i++)
	{
		if (Num[i] == Value)
		{
			return true;
		}
	}

	return false;
}
bool IsHex(char Value)
{
	const char Num[] = "1234567890ABCDEF";
	for (size_t i = 0; i < sizeof(Num); i++)
	{
		if (Num[i] == Value)
		{
			return true;
		}
	}

	return false;
}

template<typename T>
bool CMP(T Value, const String& Hex)
{
	return (("0x" + ToHexR(Value, true)) == Hex) || (("0x" + ToHexR(Value, false)) == Hex);
}
Optional<String> Jit_Interpreter::GetNameForHex(const String& Hex)
{
	Optional<String> R;
#if HasSupportforJit 
	if (CMP(_Assembler.InterpreterCPPinterface_Set_ReturnPtr, Hex))
	{
		R = String("InterpreterCPPinterface::Set_ReturnPtr");
	}
	else 
	{
		if (CMP(_Assembler.BuildAddressPtr, Hex))
		{
			R = String("Jit_Interpreter::OnUAddressCall");
		}
		else if (CMP(_Assembler.InterpreterCPPinterface_Get_Par, Hex))
		{
			R = String("InterpreterCPPinterface::GetParameter_jit");
		}
		else
		{
			for (auto& Item : UFuncToCPPFunc)
			{
				if (CMP(Item._Value.UCodeFunc,Hex))
				{
					String Str;
					Str = "cpp-call:" + std::to_string(Item._Key);

					Str += '-' + Get_State()->GetName(Item._Key);
					return Str;
				}
				else
				{
					if (CMP(Item._Value.NativeFunc, Hex))
					{
						String Str;
						Str = "native:" + std::to_string(Item._Key);

						Str += '-' + Get_State()->GetName(Item._Key);
						return Str;
					}
				}
			}

			for (auto& Item : _Assembler.FuncsPlaceHolder)
			{
				void* Pointer = (void*)((uintptr_t)this->ExBuffer.Data + Item._Value.Offset);
				if (CMP(Pointer, Hex))
				{
					String Str;
					Str = "PlaceHolder-" + std::to_string(Item._Key);

					Str += '-' + Get_State()->GetName(Item._Key);
					return Str;
				}
			}

			for (auto& Item : Get_State()->Get_Libs().Get_CPPCalls())
			{
				void* Pointer = Item._Value.NativeCall;
				if (CMP(Pointer, Hex))
				{
					String Str;
					Str = "CPP-Native-" + Item._Key;
					return Str;
				}
				else
				{
					void* Pointer = Item._Value.InterpreterCall;
					if (CMP(Pointer, Hex))
					{
						String Str;
						Str = "CPP-Interpreter-" + Item._Key;
						return Str;
					}
				}

			}
		}
	}
	return R;
#endif
	return {};
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
			_Assembler.BuildAddressPtr = &Jit_Interpreter::OnUAddressCall;

			_Assembler.InterpreterCPPinterface_Set_ReturnPtr = (NativeJitAssembler::InterpreterCPPinterface_SetRet)&InterpreterCPPinterface::Set_Return_jit;
			_Assembler.InterpreterCPPinterface_Get_Par = (NativeJitAssembler::InterpreterCPPinterface_GetParm)&InterpreterCPPinterface::GetParameter_jit;

			bool shouldJit = ShouldJit(address, Insts);


			if (shouldJit && _Assembler.Func && _Assembler.BuildFunc(Insts, address, TepOutBuffer))
			{
				size_t InsSize = TepOutBuffer.size();
				ExBuffer.SetToReadWriteMode();

				intptr_t Ptr = (intptr_t)ExBuffer.Data + (intptr_t)Insoffset;

				Insoffset += TepOutBuffer.size();
				Item.Type = JitFuncType::CPPCall;
				Item.Func = (JitFunc)(Ptr + _Assembler.Out_CppCallOffset);
				Item.NativeFunc = (void*)(Ptr + _Assembler.Out_NativeCallOffset);

				memcpy((void*)Ptr, TepOutBuffer.data(), InsSize);

				{
					for (auto& Item : _Assembler.LinkingData)
					{
						if (UFuncToCPPFunc.count(Item.OnUAddress))
						{
							auto& SomeV = UFuncToCPPFunc.at(Item.OnUAddress);

							if (SomeV.Type == JitFuncType::CPPCall)
							{
								intptr_t val = (intptr_t)SomeV.NativeFunc;
								_Assembler.SubCall((JitInfo::FuncType)(val - 5), Item.CPPOffset, ExBuffer.Data);
							}
						}
						else
						{
							//_Assembler.SubCall((JitInfo::FuncType)OnUAddressCall,Item.CPPOffset, ExBuffer.Data);
						}
					}
				}

				LogASM();

				ExBuffer.SetToExecuteMode();

				/*
				for (auto& Item : Insts)
				{
					if (Item.OpCode == InstructionSet::Call && Item.Value0.AsAddress == address)
					{
						Item.OpCode = InstructionSet::CPPCall;
						Item.Value0.AsPtr = (void*)Ptr;
					}
				}
				*/

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



String Jit_Interpreter::GetJitState()
{
	std::stringstream r;
	void* InsData = ExBuffer.Data;
	size_t InsSize = Insoffset;
#if UCodeLang_CPUIs_x86_64 || UCodeLang_CPUIs_x86

#if UCodeLang_CPUIs_x86
	auto MachineMode = ZYDIS_MACHINE_MODE_LONG_COMPAT_32;
#else
	auto MachineMode = ZYDIS_MACHINE_MODE_LONG_64;
#endif
	ZyanU64 runtime_address = (ZyanU64)InsData;
	//runtime_address = 0;
	// Loop over the instructions in our buffer.
	ZyanUSize offset = 0;
	ZydisDisassembledInstruction instruction;


	const size_t MaxInsSize = 8;
	while (ZYAN_SUCCESS(ZydisDisassembleIntel(
		/* machine_mode:    */ MachineMode,
		/* runtime_address: */ runtime_address,
		/* buffer:          */ (void*)((uintptr_t)InsData + offset),
		/* length:          */ Insoffset - offset,
		/* instruction:     */ &instruction
	)))
	{

		String InsStr = instruction.text;


		{
			int State = 0;
			size_t StrIndex = 0;
			String_view V;

			String NewIns = "";

			for (size_t i = 0; i < InsStr.size(); i++)
			{
				auto Char = InsStr[i];
				if (Char == '0')
				{
					State = 1;
				}
				else
					if (Char == 'x')
					{
						if (State == 1)
						{
							String B = "0x";

							for (size_t i2 = i + 1; i2 < InsStr.size(); i2++)
							{
								if (IsHex(InsStr[i2]))
								{
									B += InsStr[i2];
									i = i2;
								}
								else
								{
									i = i2;
									break;
								}
							}

							int a = 0;
							auto NewName = GetNameForHex(B);

							NewIns += NewName.value_or(B);
							State = 0;
						}
						else
						{
							State = 0;
							NewIns += Char;
						}
					}
					else
					{
						NewIns += Char;
					}
			}
			InsStr = NewIns;
		}
		void* Pointer = (void*)((uintptr_t)InsData + offset);
		for (auto& Item : UFuncToCPPFunc)
		{
			if (Item._Value.Type != JitFuncType::CPPCall) { continue; }
			if ((ZyanU64)Item._Value.Func == (ZyanU64)Pointer)
			{
				r << "\n";
				r << "CPPCall-" + std::to_string(Item._Key) + "-";

				r << Get_State()->GetName(Item._Key);

				r << ":";
				r << '\n';
			}
			else if ((ZyanU64)Item._Value.NativeFunc == (ZyanU64)Pointer)
			{
				r << "\n";
				r << "Native-" << Item._Key << '-';

				r << Get_State()->GetName(Item._Key);

				r << ":";
				r << std::endl;
			}
		}

		for (auto& Item : _Assembler.FuncsPlaceHolder)
		{
			void* Pointer2 = (void*)((uintptr_t)this->ExBuffer.Data + Item._Value.Offset);
			if (Pointer == Pointer2)
			{
				r << "\n";
				r << "PlaceHolder-" + std::to_string(Item._Key);

				r << '-' + Get_State()->GetName(Item._Key);
				r << ":";
				r << std::endl;
			}
		}

		r << " ";
		r << ToHexR(runtime_address);
		r << "/";
		r << std::to_string(offset);
		r << ":";

		for (size_t i = 0; i < MaxInsSize; i++)
		{
			if (i > instruction.info.length)
			{
				r << "   ";
			}
			else
			{
				Byte* byte = &((Byte*)((uintptr_t)InsData + offset))[i];
				r << byte_to_hex(*byte) << " ";
			}
		}
		r << " " << InsStr;



		r << '\n';



		offset += instruction.info.length;
		runtime_address += instruction.info.length;
	}
#endif
	r << "/end" << std::endl;
	int DebugYourBreakPointHere = 0;

	return r.str();
}

void Jit_Interpreter::LogASM()
{
#ifdef UCodeLangDebug
	std::cout << GetJitState() << std::endl;
#endif
	
}
bool Jit_Interpreter::ShouldJit(UAddress address, const Vector<Instruction>& Insts)
{
	bool ShouldJit = false;
	if (AlwaysJit == true)
	{
		ShouldJit = true;
	}
	else
	{
		size_t FuncSize = 0;
		for (size_t i = address; i < Insts.size(); i++)
		{
			const Instruction& NextIns = Insts[i];
			if (NextIns.OpCode == InstructionSet::Return)
			{
				FuncSize = i;
			}
		}


		if (FuncSize < 12)
		{
			//Func too small to jit compile

		}
		else
		{
			ShouldJit = true;
		}

	}
	return ShouldJit;
}
void Jit_Interpreter::OnUAddressCall(UAddress addresstojit)
{
#if HasSupportforJit 
	auto State = _ThisState._This->Get_State();
	auto& LibManger = State->Get_Libs();

	if (!_ThisState._This->UFuncToCPPFunc.count(addresstojit)) { _ThisState._This->UFuncToCPPFunc[addresstojit] = {}; }
	auto& Item = _ThisState._This->UFuncToCPPFunc[addresstojit];

	bool shouldJit = _ThisState._This->ShouldJit(addresstojit,LibManger.GetInstructions());

	if (shouldJit)
	{
		#ifdef UCodeLangDebug
		{
			std::cout << " building because a funcion needed it " << std::to_string(addresstojit) << std::endl;
		}
		#endif
		_ThisState._This->BuildCheck(Item, addresstojit);
		return;
	}
	else
	{
		//fall back on Interpreter.
		throw std::exception("path not added");
	}
#endif	
}

UCodeLangEnd