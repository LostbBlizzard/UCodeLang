#include "RunTimeLib.hpp"
#include "../Compliation/Helpers/InstructionBuilder.hpp"
UCodeLangStart

#if UCodeLang_32BitSytem
#define OtherV AsUInt64
#define NativeFunc(FuncName,Pars) FuncName##32 Pars
#else
#define OtherV AsUInt32
#define NativeFunc(FuncName,Pars) FuncName##64 Pars
#endif // UCodeLang_32BitSytem





void RunTimeLib::Init(UClib* Lib)
{
	_Lib = Lib;
	_Instruction.clear();
	auto ThisE = BitConverter::_CPUEndian;
	
	#if UCodeLang_32BitSytem
	int TestNative = 5;
	#endif // UCodeLang_64BitSytem

	for (const auto& Item : _Lib->Get_Instructions())//Make set native
	{
		Instruction NewInst = Item;
		if (Item.OpCode >= InstructionSet::NativeSetMin && Item.OpCode < InstructionSet::NativeSetMax) {

			switch (Item.OpCode)
			{
			case InstructionSet::StoreNative:
				NativeFunc(InstructionBuilder::Store,(NewInst, Item.Value0.AsRegister, (UIntNative)Item.Value1. OtherV));
				break;
			case InstructionSet::StoreFromPtrToRegNative:
				NativeFunc(InstructionBuilder::StoreFromPtrToReg,(NewInst, Item.Value0.AsRegister, Item.Value1.AsRegister));
				break;
			case InstructionSet::StoreRegToPtrNative:
				NativeFunc(InstructionBuilder::StoreRegToPtr,(NewInst, Item.Value0.AsRegister, Item.Value1.AsRegister));
				break;
			case InstructionSet::PushNative:
				NativeFunc(InstructionBuilder::Push,(NewInst, Item.Value0.AsRegister));
				break;
			case InstructionSet::PopNative:
				NativeFunc(InstructionBuilder::Pop,(NewInst, Item.Value0.AsRegister));
				break;
			case InstructionSet::StoreRegToRegNative:
				NativeFunc(InstructionBuilder::StoreRegToReg,(NewInst, Item.Value0.AsRegister, Item.Value1.AsRegister));
				break;
			case InstructionSet::AddNative:
				NativeFunc(InstructionBuilder::Add,(NewInst, Item.Value0.AsRegister, Item.Value1.AsRegister));
				break;
			case InstructionSet::SubNative:
				NativeFunc(InstructionBuilder::Add,(NewInst, Item.Value0.AsRegister, Item.Value1.AsRegister));
				break;
			default:
				#if CompliationTypeSafety
				throw std::exception("unknown instruction");
				#endif // DEBUG
				break;
			}
		}
		_Instruction.push_back(NewInst);
	}
}
void RunTimeLib::UnLoad()
{
	_Lib = nullptr;
	_Instruction.clear();
}
UCodeLangEnd


