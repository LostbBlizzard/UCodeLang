#include "RunTimeLib.hpp"
#include "../Compliation/Helpers/InstructionBuilder.hpp"
UCodeLangStart

void RunTimeLib::Init(UClib* Lib)
{
	_Lib = Lib;
	_Instruction.clear();
	auto ThisE = BitConverter::Get_CPU_Endian();


	for (const auto& Item : _Lib->Get_Instructions())//Make set native
	{
		Instruction NewInst = Item;
		if (Item.OpCode >= InstructionSet::NativeSetMin && Item.OpCode < InstructionSet::NativeSetMax) {

			switch (Item.OpCode)
			{

			#if UCodeLang_64BitSytem
			case InstructionSet::Store8RegOnStack:
				InstructionBuilder::Store8RegOnStack(NewInst, Item.Value0_RegisterID, (UIntNative)Item.Value1_AsUInt32);
				break;
			case InstructionSet::Get8FromStack:
				InstructionBuilder::Get8FromStack(NewInst,(UIntNative)Item.Value1_AsUInt32,Item.Value0_RegisterID);
				break;

			case InstructionSet::StoreNativeU:
				InstructionBuilder::Store64(NewInst, Item.Value0_RegisterID, (UIntNative)Item.Value1_AsUInt32);
				break;
			case InstructionSet::StoreNativeS:
				InstructionBuilder::Store64(NewInst, Item.Value0_RegisterID, (SIntNative)Item.Value1_AsInt32);
				break;
			case InstructionSet::StoreNativeFromPtrToReg:
				InstructionBuilder::Store64FromPtrToReg(NewInst, Item.Value0_RegisterID, Item.Value1_RegisterID);
				break;
			case InstructionSet::StoreNativeRegToPtr:
				InstructionBuilder::Store64RegToPtr(NewInst, Item.Value0_RegisterID, Item.Value1_RegisterID);
				break;
			case InstructionSet::PushNative:
				InstructionBuilder::PushNative(NewInst, Item.Value0_RegisterID);
				break;
			case InstructionSet::PopNative:
				InstructionBuilder::PopNative(NewInst, Item.Value0_RegisterID);
				break;
			case InstructionSet::StoreNativeRegToReg:
				InstructionBuilder::Store64RegToReg(NewInst, Item.Value0_RegisterID, Item.Value1_RegisterID);
				break;
			case InstructionSet::AddUNative:
				InstructionBuilder::Add64U(NewInst, Item.Value0_RegisterID, Item.Value1_RegisterID);
				break;
			case InstructionSet::AddSNative:
				InstructionBuilder::Add64S(NewInst, Item.Value0_RegisterID, Item.Value1_RegisterID);
				break;
			#endif // UCodeLang_64BitSytem
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


