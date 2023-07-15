#include "UAssembly.hpp"
#include "CompilerTypes.hpp"
#include "UCodeLang/LangCore/DataType/BinaryVectorMap.hpp"
#include "UCodeLang/Compliation/Helpers/NameDecoratior.hpp"
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

	auto UCodeLayer = Lib->GetLayer(UCode_CodeLayer_UCodeVM_Name);
	if (UCodeLayer)
	{
		for (const auto& Item2 : UCodeLayer->Get_NameToPtr())
		{
			AddressToName[Item2._Value] = Item2._Key;
		}
	}
	r += "[ClassData]-- \n";
	
	auto& Assembly = Lib->Get_Assembly();
	for (auto& Item : Assembly.Classes)
	{
		switch (Item->Get_Type())
		{
		case ClassType::Class:
		{
			auto& Class = Item->Get_ClassData();

			for (auto Item2 : Class.Attributes.Attributes)
			{
				ToString(Item2,Assembly);
			}
			r += "$" + Item->FullName + ":\n";
			

			r += ".size:" + std::to_string(Class.Size) + "\n";
			
			for (auto Item2 : Class.Fields)
			{
				r += " " + ToString(Item2.Type, Assembly) + " " + Item2.Name + ";//Offset " + std::to_string(Item2.offset) + "\n";
			}
			r += "\n";

			for (auto Item2 : Class.Methods)
			{
				for (auto Item3 : Item2.Attributes.Attributes)
				{
					r += ToString(Item3, Assembly);
				}

				r += " |" + Item2.FullName + "[";
				for (auto& Item3 : Item2.ParsType)
				{
					r += ToString(Item3,Assembly);
				}
				r += "] -> " + ToString(Item2.RetType, Assembly) + ";" ;
				r += "//" + Item2.DecorationName + '\n';
					
			}
			r += "\n\n";
		}
		break; 
		case ClassType::Alias:
		{
			auto& Class = Item->Get_AliasData();
			r += "$" + Item->FullName + " = " + ToString(Class.Type, Assembly) + ";\n\n";
		}
		break;
		case ClassType::Enum:
		{
			auto& Enum = Item->Get_EnumData();
			r += "$" + Item->FullName + " enum[" + ToString(Enum.BaseType,Assembly) + "]:\n";
			for (auto& Item : Enum.Values)
			{
				r += " " + Item.Name;
				if (Item.EnumVariantType.has_value())
				{
					auto& Value = Item.EnumVariantType.value();
					r += "[";
					r += ToString(Value, Assembly);
					r += "]";
				}

				r += " = " + ToString(Enum.BaseType, Item._Data, Assembly,Lib->BitSize);

				if (&Item != &Enum.Values.back())
				{
					r += ",\n";
				}
			}
			if (Enum.EnumVariantUnion)
			{
				auto Ptr = Assembly.Find_Node(Enum.EnumVariantUnion.value());
				if (Ptr)
				{
					r += "\n//Union = " + Ptr->FullName;
				}
			}

			r += "\n\n";
		}
		break;
		default:
			break;
		} 
	}


	if (UCodeLayer) 
	{
		r += "\n[Instructions]-- \n";
		auto& Insts = UCodeLayer->Get_Instructions();
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
					OpValueToString(MapData->Op_0, Item.Value0, AddressToName, r, Lib);
				}
				if (MapData->Op_1 != OpCodeType::NoOpCode)
				{
					r += ",";
					OpValueToString(MapData->Op_1, Item.Value1, AddressToName, r, Lib);
				}

			}
			else
			{
				r += "Ins " + std::to_string((uintptr_t)Item.OpCode) + ":" + std::to_string((uintptr_t)Item.Value0.AsPtr) + ","
					+ std::to_string((uintptr_t)Item.Value1.AsPtr);
			}

			r += '\n';
		}
	}

    return r;
}
String UAssembly::ToString(const ReflectionTypeInfo& Value, const ClassAssembly& Assembly)
{
	String r;
	if (Value.Isimmutable())
	{
		r = "umut ";
	}

	if (Value._MoveData ==ReflectionMoveData::Moved)
	{
		r += "moved ";
	}

	if (Value._IsDynamic)
	{
		r += "dynamic<";
	}

	switch (Value._Type)
	{
	case ReflectionTypes::Bool:r += "bool";break;
	case ReflectionTypes::Char:r += "char"; break;

	case ReflectionTypes::sInt8:r += "sint8"; break;
	case ReflectionTypes::uInt8:r += "uint8"; break;

	case ReflectionTypes::sInt16:r += "sint16"; break;
	case ReflectionTypes::uInt16:r += "uint16"; break;

	case ReflectionTypes::sInt32:r += "sint32"; break;
	case ReflectionTypes::uInt32:r += "uint32"; break;

	case ReflectionTypes::sInt64:r += "sint64"; break;
	case ReflectionTypes::uInt64:r += "uint64"; break;

	case ReflectionTypes::uIntPtr:r += "uIntPtr"; break;
	case ReflectionTypes::sIntPtr:r += "sIntPtr"; break;


	case ReflectionTypes::float32:r += "float32"; break;
	case ReflectionTypes::float64:r += "float64"; break;

	
	case ReflectionTypes::Void:
		r += "void";	break;
	case  ReflectionTypes::Any:
		r += "[any]";
		break;
	case ReflectionTypes::Null:
	default:	
		r += "[badtype]";
		break;
	case ReflectionTypes::CustomType:
	{
		auto Ptr =Assembly.Find_Node(Value._CustomTypeID);
		if (Ptr)
		{
			r += Ptr->FullName;
		}
		else
		{
			r += "[?]";
		}
	}
		break;
	}

	if (Value._IsDynamic)
	{
		r += ">";
	}

	if (Value.IsAddress())
	{
		r += "&";
	}
	if (Value.IsAddressArray())
	{
		r += "[&]";
	}

	return r;
}
String UAssembly::ToString(const ReflectionTypeInfo& Value, const ReflectionRawData& Data, const ClassAssembly& Assembly, UClib::NTypeSize PtrSize)
{
	String r;
	switch (Value._Type)
	{
	case ReflectionTypes::Bool:
		r += *Data.Get_DataAs<bool>() ? "true" : "false"; 
		break;
	case ReflectionTypes::Char:
		r += *Data.Get_DataAs<char>();
		break;
	case ReflectionTypes::sInt8:
		r += std::to_string(*Data.Get_DataAs<Int8>());
		break;
	case ReflectionTypes::uInt8:
		r += std::to_string(*Data.Get_DataAs<UInt8>());
		break;
	case ReflectionTypes::sInt16:
		r += std::to_string(*Data.Get_DataAs<Int16>());
		break;
	case ReflectionTypes::uInt16:
		r += std::to_string(*Data.Get_DataAs<UInt16>());
		break;
	case ReflectionTypes::sInt32:
		r += std::to_string(*Data.Get_DataAs<Int32>());
		break;
	case ReflectionTypes::uInt32:
		r += std::to_string(*Data.Get_DataAs<UInt32>());
		break;
	case ReflectionTypes::sInt64:
		r += std::to_string(*Data.Get_DataAs<Int64>());
		break;
	case ReflectionTypes::uInt64:
		r += std::to_string(*Data.Get_DataAs<UInt64>());
		break;
		r += "uint64"; break;

	case ReflectionTypes::uIntPtr:
		r += PtrSize == UClib::NTypeSize::int32 ? std::to_string(*Data.Get_DataAs<UInt32>()) : std::to_string(*Data.Get_DataAs<UInt64>());
		break;
	case ReflectionTypes::sIntPtr:
		r += PtrSize == UClib::NTypeSize::int32 ? std::to_string(*Data.Get_DataAs<Int32>()) : std::to_string(*Data.Get_DataAs<Int64>());
		break;


	case ReflectionTypes::float32:
		r += std::to_string(*Data.Get_DataAs<float32>());
		break;
	case ReflectionTypes::float64:
		r += std::to_string(*Data.Get_DataAs<float64>());
		break;
	default:
		break;
	}
	return r;
}
String UAssembly::ToString(const UsedTagValueData& Value, const ClassAssembly& Assembly)
{
	String R;

	R += "[";

	auto node = Assembly.Find_Node(Value.TypeID);
	if (node)
	{
		R += node->FullName;
	}
	else
	{
		R += "?";
	}

	R += "]";

	return R;
}
String UAssembly::ToString(const ClassMethod::Par& Value, const ClassAssembly& Assembly)
{
	String R;
	if (Value.IsOutPar)
	{
		R += "out ";
	}
	R += ToString(Value.Type, Assembly);
	return R;
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
	}	
	break;
	case OpCodeType::RegPtrAndRegOut:
	{
		const RegisterID* ReV = &In.AsRegister;
		RegisterID Ptr = ReV[0];
		RegisterID RegOut = ReV[1];
		out += "[Ptr:" + GetRegisterToString(Ptr) + "]," + "[Target:" + GetRegisterToString(RegOut) + "]";
	}
		
		break;
	default:
		break;
	}
}
size_t UAssembly::BuildHashForSub(const Instruction* Pointer, size_t BufferSize)
{
	String_view Item((char*)Pointer, BufferSize * sizeof(Instruction));

	return std::hash<String_view>()(Item);
}
UAssemblyEnd
