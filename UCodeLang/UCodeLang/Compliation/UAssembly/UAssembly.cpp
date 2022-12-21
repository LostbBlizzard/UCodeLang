#include "UAssembly.hpp"
#include "CompilerTypes.hpp"
#define StackName "Stack"
#define StackName_ "[" + StackName + "]"

UAssemblyStart

void UAssembly::Assemble(const String_view& Text, UCodeLang::UClib* Out)
{
	Lexer Lex; 
	Parser Parse; 
	Lex.Set_ErrorsOutput(_ErrorsOutput);
	Lex.Set_Settings(_Settings);
	Parse.Set_ErrorsOutput(_ErrorsOutput);
	Parse.Set_Settings(_Settings);
	
	
	Lex.Lex(Text);
	Parse.Parse(Lex.Get_Output(), Out);
}
String UAssembly::ToString(const UCodeLang::UClib* Lib)
{
	auto& InsMapData = Get_InsToInsMapValue();
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
		
		if (InsMapData.count(Item.OpCode))
		{
			auto& MapData = InsMapData[Item.OpCode];
			r += MapData->InsName.data();
			r += " ";

			if (MapData->Op_0 != OpCodeType::NoOpCode)
			{
				OpValueToString(MapData->Op_0,Item.Value0, r);
			}
			if (MapData->Op_1 != OpCodeType::NoOpCode)
			{
				r += ",";
				OpValueToString(MapData->Op_1,Item.Value1, r);
			}
			
		}
		else
		{
			r += "Ins " + std::to_string((uintptr_t)Item.OpCode) + ":" + std::to_string((uintptr_t)Item.Value0.AsPtr) + ","
				+ std::to_string((uintptr_t)Item.Value1.AsPtr);
		}
		
		r += '\n';
	}

    return r;
}
void UAssembly::OpValueToString(OpCodeType OpType,const AnyInt64& In, String& out)
{
	switch (OpType)
	{
	case UCodeLang::UAssembly::OpCodeType::NoOpCode:
		break;
	case UCodeLang::UAssembly::OpCodeType::AnyInt8:
		out += std::to_string((UInt64)In.AsUInt8) + "|" + std::to_string((Int64)In.AsInt8);
		break;
	case UCodeLang::UAssembly::OpCodeType::AnyInt16:
		out += std::to_string((UInt64)In.AsUInt16) + "|" + std::to_string((Int64)In.AsInt16);
		break;
	case UCodeLang::UAssembly::OpCodeType::AnyInt32:
		out += std::to_string(In.AsUInt32) + "|" + std::to_string(In.AsInt32);
		break;
	case UCodeLang::UAssembly::OpCodeType::AnyInt64:
		out += std::to_string(In.AsUInt64) + "|" + std::to_string(In.AsInt64);
		break;
	case UCodeLang::UAssembly::OpCodeType::Register:
		out += GetRegisterToString(In.AsRegister);
		break;
	default:
		break;
	}
}
UAssemblyEnd
