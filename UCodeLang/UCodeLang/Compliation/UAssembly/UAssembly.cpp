#include "UAssembly.hpp"
#define StackName "Stack"
#define StackName_ "[" + StackName + "]"

UCodeLangStart

void UAssembly::Build(const String& Text, UCodeLang::UClib* Out)
{
}
String UAssembly::ToString(const UCodeLang::UClib* Lib)
{
    String r;
	unordered_map<UAddress, String> AddressToName;
	for (const auto& Item2 : Lib->Get_NameToPtr())
	{
		AddressToName[Item2.second] = Item2.first;
	}

	r += "[Instructions]-- \n";
	auto& Insts = Lib->Get_Instructions();
	for (size_t i = 0; i < Insts.size(); i++)
	{
		auto& Item = Insts[i];
		UAddress address = (UAddress)i;
		if (AddressToName.count(address))
		{
			String Name = AddressToName[address];
			r += "---" + Name + ": \n";
		}
		

		r += "   " + std::to_string(i) + " :";
		switch (Item.OpCode)
		{
			
		case InstructionSet::Exit:
			r += "Exit";
			break;
		case InstructionSet::DoNothing:
			r += "NoOp";
			break;
		case InstructionSet::Return:
			r += "Ret";
			break;
		case InstructionSet::Call:
		{
			r += "Call ";
			UAddress ExtPos = Item.Value0_AsAddress;
			if (AddressToName.count(ExtPos))
			{
				String funcName = AddressToName[ExtPos];
				r += "{" + funcName + "}";
			}
			else
			{
				r += GetInsName(Lib, Item.Value0_AsUIntNative);
			}
		}break;
		case InstructionSet::Jump:
			r += "Jump " + GetInsName(Lib,Item.Value0_AsUIntNative);
			break;
		case InstructionSet::Jumpif:
			r += "Jumpif " + GetInsName(Lib, Item.Value0_AsUIntNative);
			break;
		case InstructionSet::Push8:
			r += "Push8 " + GetRegisterToString(Item.Value0_AsUIntNative);
			break;
		case InstructionSet::Pop8:
			r += "Pop8 " + GetRegisterToString(Item.Value0_AsUIntNative);
			break;
		case InstructionSet::Store8:
			r += "Store8 " + std::to_string(Item.Value1_AsUInt8) +
				  "," + GetRegisterToString(Item.Value0_AsUIntNative);
			break;
		case InstructionSet::Get8FromStackSub:
			r += "Store8 " + (String)"[" + StackName + " - " + std::to_string(Item.Value0_AsUIntNative) + "]"
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::Get8FromStack:
			r += "Store8 " + (String)"[" + StackName + " + " + std::to_string(Item.Value0_AsUIntNative) + "]"
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::Store8RegOnStack:
			r += "Store8 " + GetRegisterToString(Item.Value0_AsUIntNative) +
				+ "," + (String)"[" + StackName + " + " + std::to_string(Item.Value1_AsUIntNative) + "]";
			break;
		case InstructionSet::Store8FromPtrToReg:
			r += "Store8 [Ptr -> Reg] " + GetRegisterToString(Item.Value1_AsUIntNative) +
				+"," + GetRegisterToString(Item.Value0_AsUIntNative);
			break;
		case InstructionSet::Store8RegToPtr:
			r += "Store8 [Reg -> Ptr] " + GetRegisterToString(Item.Value0_AsUIntNative) +
				+","  + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::MultU8:
			r += "Mult[U]" + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::MultS8:
			r += "Mult8[S] " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::AddS8:
			r += "Add8[S] " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::AddU8:
			r += "Add8[U] " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::SubS8:
			r += "Sub8[S] " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::SubU8:
			r += "Sub8[U] " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::DivS8:
			r += "Div8[S] " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::DivU8:
			r += "Div8[U] " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::LogicalAnd8:
			r += "And8[L] " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::Logicalor8:
			r += "Or8[L] " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::LogicalNot8:
			r += "Not[L]" + GetRegisterToString(Item.Value0_AsUIntNative);
			break;
		case InstructionSet::equalto8:
			r += "equalto8 " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::notequalto8:
			r += "notequalto8 " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::greaterthan8:
			r += "greaterthan8 " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::lessthan8:
			r += "lessthan8 " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::equal_lessthan8:
			r += "less than or equal 8" + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::equal_greaterthan8:
			r += "greater than or equal 8" + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::bitwiseAnd8:
			r += "bitwiseAnd8 " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::bitwiseOr:
			r += "bitwiseor8 " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::bitwiseLeftShift8:
			r += "bitwiseLeftShift8 " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::bitwiseRightShift8:
			r += "bitwiseRightShift8 " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::bitwiseXor8:
			r += "bitwiseRightShift8 " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::bitwise_Not8:
			r += "bitwiseRightShift8 " + GetRegisterToString(Item.Value0_AsUIntNative);
			break;
		case InstructionSet::Log:
			r += "Log " + GetRegisterToString(Item.Value0_AsUIntNative);
			break;
		case InstructionSet::StoreNativeU:
			r += "Store64[U] " + GetRegisterToString(Item.Value0_AsUIntNative) +
				"," + std::to_string(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::AddUNative:
			r += "Add64[U] " + GetRegisterToString(Item.Value0_AsUIntNative)
				+ "," + GetRegisterToString(Item.Value1_AsUIntNative);
			break;
		case InstructionSet::GetPointerOfStaticMem:
			r += "StaticMem[U] " + GetRegisterToString(Item.Value0_AsUIntNative);
			break;
		case InstructionSet::Push64:
			r += "Push64 " + GetRegisterToString(Item.Value0_AsUIntNative);
			break;
		case InstructionSet::Pop64:
			r += "Pop64 " + GetRegisterToString(Item.Value0_AsUIntNative);
			break;
		case InstructionSet::IncrementStackPointer:
			r += "IncStack " + GetRegisterToString(Item.Value0_AsUIntNative);
			break;
		case InstructionSet::DecrementStackPointer:
			r += "DecStack " + GetRegisterToString(Item.Value0_AsUIntNative);
			break;
		default:
			r += "[???]";
			break;
			
		}
		r += '\n';
	}

    return r;
}
UCodeLangEnd
