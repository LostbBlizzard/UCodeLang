#include "UAssembly.hpp"
#include "CompilerTypes.hpp"
#include "UCodeLang/LangCore/DataType/BinaryVectorMap.hpp"

#define StackName "Stack"
#define StackName_ "[" + StackName + "]"

UAssemblyStart

void UAssembly::Assemble(const String_view& Text, UClib* Out)
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
String UAssembly::ToString(const UClib* Lib)
{
	auto& InsMapData = Get_InsToInsMapValue();
    String r;
	BinaryVectorMap<UAddress, String> AddressToName;
	for (const auto& Item2 : Lib->Get_NameToPtr())
	{
		AddressToName[Item2._Value] = Item2._Key;
	}
	r += "[ClassData]-- \n";
	
	auto& Assembly = Lib->Get_Assembly();
	for (auto& Item : Assembly.Classes)
	{
		switch (Item->Type)
		{
		case ClassType::Class:
		{
			auto& Class = Item->_Class;

			for (auto Item2 : Class.Attributes)
			{
				r += Item2.Name;
			}
			r += "$" + Item->FullName + ":\n";
			

			r += ".size:" + std::to_string(Class.Size) + "\n";
			
			for (auto Item2 : Class.Fields)
			{
				r += " " + Item2.Type.FullNameType + " " + Item2.Name + ";//Offset " + std::to_string(Item2.offset) + "\n";
			}
			r += "\n";

			for (auto Item2 : Class.Methods)
			{
				r += " |" + Item2.FullName + "[";
				for (auto& Item3 : Item2.ParsType)
				{
					r += ToString(Item3);
				}
				r += "] -> " + ToString(Item2.RetType) + ";" ;
				r += "//" + Item2.DecorationName + '\n';
					
			}
			r += "\n\n";
		}
		break; 
		case ClassType::Alias:
		{
			auto& Class = Item->_Alias;
			r += "$" + Item->FullName + " = " + ToString(Class.Type) + ";\n";
		}
		break;
		default:
			break;
		} 
	}


	r += "\n[Instructions]-- \n";
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
			r += (String)MapData->InsName;
			r += " ";

			if (MapData->Op_0 != OpCodeType::NoOpCode)
			{
				OpValueToString(MapData->Op_0,Item.Value0, AddressToName, r,Lib);
			}
			if (MapData->Op_1 != OpCodeType::NoOpCode)
			{
				r += ",";
				OpValueToString(MapData->Op_1,Item.Value1, AddressToName, r, Lib);
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
String UAssembly::ToString(const ReflectionTypeInfo& Value)
{
	String r;

	r += Value.FullNameType;
	return r;
}
void UAssembly::OpValueToString(OpCodeType OpType,const AnyInt64& In,const BinaryVectorMap<UAddress, String>& AddressToName, String& out, const UClib* Lib)
{

	switch (OpType)
	{
	case OpCodeType::NoOpCode:
		break;
	case OpCodeType::AnyInt8:
	{
		String tepS = std::to_string((UInt64)In.AsUInt8);
		String teps2 = std::to_string((Int64)In.AsInt8);
		out += (tepS == teps2) ? tepS : tepS + "|" + teps2;
	}	
	break;
	case OpCodeType::AnyInt16:
	{
		String tepS = std::to_string((UInt64)In.AsUInt16);
		String teps2 = std::to_string((Int64)In.AsInt16);
		out += (tepS == teps2) ? tepS : tepS + "|" + teps2;
	}
	break;
	case OpCodeType::AnyInt32:
	{
		String tepS = std::to_string((UInt64)In.AsUInt32);
		String teps2 = std::to_string((Int64)In.AsInt32);
		out += (tepS == teps2) ? tepS : tepS + "|" + teps2;
	}	
	break;
	case OpCodeType::AnyInt64:
	{
		String tepS = std::to_string((UInt64)In.AsUInt64);
		String teps2 = std::to_string((Int64)In.AsInt64);
		out += (tepS == teps2) ? tepS : tepS + "|" + teps2;
	}	
	break;

	case OpCodeType::Anyfloat32:
		out += std::to_string(In.Asfloat32);
		break;
	case OpCodeType::Anyfloat64:
		out += std::to_string(In.Asfloat64);
		break;

	case OpCodeType::Register:
		out += GetRegisterToString(In.AsRegister);
		break;
	case OpCodeType::UIntPtr:
		out += std::to_string(In.AsUInt64);
		break;
		
	case OpCodeType::StaticCString:
		out += "\"" + (String)(const char*)&Lib->Get_StaticBytes()[In.AsUIntNative] + "\"";
		break;

	case OpCodeType::InsAddress:
	{
		auto NewAddress = In.AsAddress + 1;
		if (AddressToName.count(NewAddress))
		{
			out += "{" + AddressToName.at(NewAddress) + "}";
		}
		else
		{
			out += "{" + std::to_string(In.AsUInt64) + "}";
		}
	}	break;
	default:
		break;
	}
}
UAssemblyEnd
