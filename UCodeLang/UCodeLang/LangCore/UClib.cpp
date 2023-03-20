#include "UClib.hpp"
#include <fstream>
UCodeLangStart

UClib::UClib() : LibEndianess(BitConverter::InputOutEndian)
{
}

UClib::~UClib()
{

}
using Size_tAsBits = BitMaker::SizeAsBits;


const unsigned char UClibSignature[]= "Lost_blizzard_Ulib";
constexpr size_t UClibSignature_Size = sizeof(UClibSignature);

BytesPtr UClib::ToRawBytes(const UClib* Lib)
{
	BitMaker Output;



	//Ulib signature
	{
		Output.WriteType((Size_tAsBits)UClibSignature_Size);
		Output.WriteBytes(UClibSignature, UClibSignature_Size);

		Output.WriteType((InstructionSet_t)InstructionSet::MAXVALUE);

		Output.WriteType((InstructionSet_t)Intermediate_Set::MAXVALUE);
	}

	Output.WriteType((NTypeSize_t)Lib->BitSize);

	Output.WriteType((LibType_t)Lib->_LibType);

	Output.WriteType((Endian_t)Lib->LibEndianess);

	{//StaticBytes
		Output.WriteType((Size_tAsBits)Lib->_StaticBytes.size());
		Output.WriteBytes(Lib->_StaticBytes.data(), Lib->_StaticBytes.size());
	}

	{// Instructions
		Output.WriteType((Size_tAsBits)Lib->_Instructions.size());
		Output.WriteBytes((const Byte*)Lib->_Instructions.data(), Lib->_Instructions.size() * sizeof(Instruction));
	}


	{// Code
		Output.WriteType((Size_tAsBits)Lib->_Code.size());
		Output.WriteBytes((const Byte*)Lib->_Code.data(), Lib->_Code.size());
	}

	{// _NameToPtr
	
		Output.WriteType((Size_tAsBits)Lib->_NameToPtr.size());
		for (auto& Item : Lib->_NameToPtr)
		{
			Output.WriteType(Item._Key);
			Output.WriteType((Size_tAsBits)Item._Value);
		}
	}

	{//Debug Bytes
		Output.WriteType((Size_tAsBits)Lib->_DebugBytes.size());
		Output.WriteBytes(Lib->_DebugBytes.data(), Lib->_DebugBytes.size());
	}
	
	//ClassAssembly
	{
		auto& Assembly = Lib->_Assembly;
		ToBytes(Output, Assembly);
	}

	BytesPtr V;
	V.Bytes.reset(new Byte[Output.size()]);
	std::memcpy(V.Bytes.get(), Output.data(), Output.size());
	V.Size = Output.size();
	return V;
}
void UClib::ToBytes(BitMaker& Output, const ClassAssembly& Assembly)
{
	Output.WriteType((Size_tAsBits)Assembly.Classes.size());


	for (auto& Item : Assembly.Classes)
	{
		Output.WriteType(Item->Name);
		Output.WriteType(Item->FullName);

		Output.WriteType((ClassType_t)Item->Type);

		switch (Item->Type)
		{
		case ClassType::Alias:
		{
			auto& Alias = Item->_Alias;
			ToBytes(Output, Alias);
		}
		break;
		case ClassType::Class:
		{
			auto& ClassData = Item->_Class;
			ToBytes(Output, ClassData);
		}
		break;
		case ClassType::Enum:
		{
			auto& EnumData = Item->_Enum;

			ToBytes(Output, EnumData);
		}
		break;
		default:
			break;
		}
	}
}
void UClib::ToBytes(BitMaker& Output, const ClassData::Enum_Data& EnumData)
{
	Output.WriteType(*(EnumSizez_t*)&EnumData.Size);

	Output.WriteType((Size_tAsBits)EnumData.Values.size());

	for (auto& Item2 : EnumData.Values)
	{
		Output.WriteType(Item2.Name);
		Output.WriteType((EnumValues::State_t)Item2._State);
		Output.WriteType((Size_tAsBits)Item2.Value);
	}
}
void UClib::ToBytes(BitMaker& Output, const ClassData::Alias_Data& Alias)
{
	ToBytes(Output, Alias.Type);
}
void UClib::ToBytes(BitMaker& Output, const ClassData::Class_Data& ClassData)
{
	Output.WriteType((Size_tAsBits)ClassData.Size);

	ToBytes(Output, ClassData);

	Output.WriteType((Size_tAsBits)ClassData.Fields.size());
	for (auto& Item2 : ClassData.Fields)
	{
		ToBytes(Output, Item2);
	}

	Output.WriteType((Size_tAsBits)ClassData.Methods.size());
	for (auto& Item2 : ClassData.Methods)
	{
		ToBytes(Output, Item2);
	}
}
void UClib::ToBytes(UCodeLang::BitMaker& Output, const Vector<AttributeData>& Attributes)
{
	Output.WriteType((Size_tAsBits)Attributes.size());
	for (auto& Item2 : Attributes)
	{
		ToBytes(Output, Item2);
	}
}
void UClib::ToBytes(BitMaker& Output, const ClassField& Item2)
{
	Output.WriteType(Item2.Name);
	ToBytes(Output, Item2.Type);
	Output.WriteType((Size_tAsBits)Item2.offset);
}
void UClib::ToBytes(BitMaker& Output, const AttributeData& Data)
{
	Output.WriteType(Data.Name);
}
void UClib::ToBytes(BitMaker& Output, const ClassMethod& Data)
{
	Output.WriteType(Data.FullName);
	Output.WriteType(Data.DecorationName);

	ToBytes(Output, Data.RetType);
	Output.WriteType((Size_tAsBits)Data.ParsType.size());
	for (auto& Item : Data.ParsType)
	{
		ToBytes(Output,Item);
	}

	ToBytes(Output, Data.Attributes);
}
void UClib::ToBytes(BitMaker& Output, const ReflectionTypeInfo& Data)
{
	Output.WriteType(Data.FullNameType);
}
bool UClib::FromBytes(UClib* Lib, const BytesView& Data)
{
	BitReader reader(Data.Bytes,Data.Size);
	

	
	//Signature
	{
		union 
		{
			Size_tAsBits bits_Size_tAsBits= 0;
			size_t bits;
		};

		reader.ReadType(bits_Size_tAsBits, bits_Size_tAsBits);
		
		bits = bits_Size_tAsBits;

		if (bits != UClibSignature_Size) { return false; }

		for (size_t i = 0; i < UClibSignature_Size; i++)
		{
			char Bit = reader.GetByteWith_offset(i);
			if (Bit != UClibSignature[i]) { return false; }
		}
		reader.Increment_offset(UClibSignature_Size);
	
		
		InstructionSet Value= InstructionSet::DoNothing;
		reader.ReadType(*(InstructionSet_t*)&Value, *(InstructionSet_t*)&Value);
		if (Value != InstructionSet::MAXVALUE)
		{
			return false;
		}

		auto Value2 = Intermediate_Set::Null;
		reader.ReadType(*(InstructionSet_t*)&Value2, *(InstructionSet_t*)&Value2);
		if (Value2 != Intermediate_Set::MAXVALUE)
		{
			return false;
		}
	}

	reader.ReadType(*(NTypeSize_t*)&Lib->BitSize, *(NTypeSize_t*)&Lib->BitSize);

	reader.ReadType(*(LibType_t*)&Lib->_LibType, *(NTypeSize_t*)&Lib->_LibType);

	reader.ReadType(*(Endian_t*)&Lib->LibEndianess, *(Endian_t*)&Lib->LibEndianess);
	
	auto Old = BitConverter::InputOutEndian;
	BitConverter::InputOutEndian = Lib->LibEndianess;

	{//StaticBytes

		union
		{
			Size_tAsBits bits = 0;
			size_t bits_Size;
		};

		reader.ReadType(bits, bits);
		bits_Size = bits;

		Lib->_StaticBytes.resize(bits_Size);


		memcpy(Lib->_StaticBytes.data(), &reader.GetByteWith_offset(0), bits_Size);

		reader.Increment_offset(bits_Size);
	}

	{// Instructions

		union
		{
			Size_tAsBits bits = 0;
			size_t bits_Size;
		};

		reader.ReadType(bits, bits);
		bits_Size = bits;

		Lib->_Instructions.resize(bits_Size);


		memcpy(Lib->_Instructions.data(), &reader.GetByteWith_offset(0), bits_Size * sizeof(Instruction));

		reader.Increment_offset(bits_Size * sizeof(Instruction));
	}

	{// Code

		union
		{
			Size_tAsBits bits = 0;
			size_t bits_Size;
		};

		reader.ReadType(bits, bits);
		bits_Size = bits;

		Lib->_Code.resize(bits_Size);


		memcpy(Lib->_Code.data(), &reader.GetByteWith_offset(0), bits_Size);

		reader.Increment_offset(bits_Size);
	}

	{// _NameToPtr
		union
		{
			Size_tAsBits bits = 0;
			size_t bits_Size;
		};

		reader.ReadType(bits, bits);
		bits_Size = bits;

		Lib->_NameToPtr.clear();
		Lib->_NameToPtr.reserve(bits_Size);

		for (size_t i = 0; i < bits_Size; i++)
		{
			String V1;
			union
			{
				Size_tAsBits V2 = 0;
				size_t V2bits_Size;
			};
			reader.ReadType(V1, V1);

			reader.ReadType(V2, V2);
			V2bits_Size = V2;

			Lib->_NameToPtr[V1] = V2;
		}
	}

	{//DebugBytes

		union
		{
			Size_tAsBits bits = 0;
			size_t bits_Size;
		};

		reader.ReadType(bits, bits);
		bits_Size = bits;

		Lib->_DebugBytes.resize(bits_Size);



		memcpy(Lib->_DebugBytes.data(), &reader.GetByteWith_offset(0), bits_Size);

		reader.Increment_offset(bits_Size);
	}
	//ClassAssembly
	{
		auto& Assembly = Lib->Get_Assembly();
		FromBytes(reader, Assembly);
	}

	BitConverter::InputOutEndian = Old;
	return true;
}
void UClib::FromBytes(BitReader& reader, ClassAssembly& Assembly)
{
	union
	{
		Size_tAsBits bits = 0;
		size_t bits_Size;
	};
	reader.ReadType(bits, bits);
	bits_Size = bits;

	for (size_t i = 0; i < bits_Size; i++)
	{
		auto& Item = Assembly.AddClass("", "");

		reader.ReadType(Item.Name, Item.Name);
		reader.ReadType(Item.FullName, Item.FullName);
		reader.ReadType(*(ClassType_t*)&Item.Type, *(ClassType_t*)&Item.Type);

		switch (Item.Type)
		{
		case ClassType::Alias:
		{
			auto& Alias = Item._Alias;
			FromBytes(reader, Alias);
		}
		break;
		case ClassType::Class:
		{
			auto& Class = Item._Class;
			FromBytes(reader, Class);
		}
		break;
		case ClassType::Enum:
		{
			auto& Enum = Item._Enum;
			FromBytes(reader, Enum);
		}
		break;
		default:
			break;
		}

	}
}
void UClib::FromBytes(BitReader& reader, ClassData::Enum_Data& Enum)
{
	reader.ReadType(*(EnumSizez_t*)&Enum.Size, *(EnumSizez_t*)&Enum.Size);

	union
	{
		Size_tAsBits  Sizebits = 0;
		size_t Size;

	};
	reader.ReadType(Sizebits, Sizebits);
	Size = Sizebits;

	Enum.Values.resize(Size);
	for (size_t i2 = 0; i2 < Size; i2++)
	{
		auto& Item2 = Enum.Values[i2];
		reader.ReadType(Item2.Name, Item2.Name);
		reader.ReadType(*(EnumValues::State_t*)&Item2._State, *(EnumValues::State_t*)&Item2._State);

		union
		{
			Size_tAsBits  Sizebits = 0;
			size_t Size;
		};
		reader.ReadType(Sizebits, Sizebits);
		Size = Sizebits;
		Item2.Value = Size;
	}
}
void UClib::FromBytes(BitReader& reader, ClassData::Class_Data& Class)
{

	Size_tAsBits _Classbits = 0;
	reader.ReadType(_Classbits, _Classbits);
	Class.Size = _Classbits;

	FromBytes(reader, Class);


	union
	{
		Size_tAsBits  Feld_Sizebits = 0;
		size_t Feld_Size;
	};
	reader.ReadType(Feld_Sizebits, Feld_Sizebits);
	Feld_Size = Feld_Sizebits;

	Class.Fields.resize(Feld_Size);
	for (size_t i2 = 0; i2 < Feld_Size; i2++)
	{
		auto& Item2 = Class.Fields[i2];
		FromBytes(reader, Item2);
	}

	union
	{
		Size_tAsBits  Methods_Sizebits = 0;
		size_t Methods_Size;
	};
	reader.ReadType(Methods_Sizebits, Methods_Sizebits);
	Methods_Size = Methods_Sizebits;

	Class.Methods.resize(Methods_Size);
	for (size_t i2 = 0; i2 < Methods_Size; i2++)
	{
		auto& Item2 = Class.Methods[i2];
		FromBytes(reader, Item2);
	}
}
void UClib::FromBytes(BitReader& reader,Vector<AttributeData>& Attributes)
{
	union
	{
		Size_tAsBits  Attributes_Sizebits = 0;
		size_t Attributes_Size;
	};
	reader.ReadType(Attributes_Sizebits, Attributes_Sizebits);
	Attributes_Size = Attributes_Sizebits;

	Attributes.resize(Attributes_Size);
	for (size_t i2 = 0; i2 < Attributes_Size; i2++)
	{
		auto& Item2 = Attributes[i2];
		FromBytes(reader, Item2);
	}
}
void UClib::FromBytes(BitReader& reader, ClassField& Item2)
{
	reader.ReadType(Item2.Name, Item2.Name);
	FromBytes(reader, Item2.Type);

	Size_tAsBits offset;
	reader.ReadType(offset, offset);
	Item2.offset = offset;
}
void UClib::FromBytes(BitReader& reader, ClassData::Alias_Data& Alias)
{
	FromBytes(reader, Alias.Type);
}
void UClib::FromBytes(BitReader& Input, AttributeData& Data)
{
	Input.ReadType(Data.Name, Data.Name);
}
void UClib::FromBytes(BitReader& Input, ClassMethod& Data)
{
	Input.ReadType(Data.FullName, Data.FullName);
	Input.ReadType(Data.DecorationName, Data.DecorationName);



	FromBytes(Input, Data.RetType);

	{
		union
		{
			Size_tAsBits bits_Size_tAsBits = 0;
			size_t bits;
		};

		Input.ReadType(bits_Size_tAsBits, bits_Size_tAsBits);

		bits = bits_Size_tAsBits;
		Data.ParsType.resize(bits);

		for (size_t i = 0; i < bits; i++)
		{
			FromBytes(Input,Data.ParsType[i]);
		}
	}
	FromBytes(Input, Data.Attributes);
}
void UClib::FromBytes(BitReader& Input, ReflectionTypeInfo& Data)
{
	Input.ReadType(Data.FullNameType, Data.FullNameType);
}
bool UClib::ToFile(const UClib* Lib, const Path& path)
{
	std::ofstream File(path, std::ios::binary);
	if (File.is_open())
	{

		BytesPtr Bits = ToRawBytes(Lib);

		File.write((const char*)Bits.Bytes.get(), Bits.Size);


		File.close();
		return true;
	}
	else
	{
		return false;
	}
}
bool UClib::FromFile(UClib* Lib, const Path& path)
{
	std::ifstream File(path, std::ios::binary);
	if (File.is_open())
	{
		BytesPtr Bits;
		File.seekg(0, File.end);
		Bits.Size = File.tellg();
		File.seekg(0, File.beg);
		Bits.Bytes =std::make_unique<Byte[]>(Bits.Size);

		File.read((char*)Bits.Bytes.get(), Bits.Size);
		File.close();
		auto V = FromBytes(Lib, { Bits.Bytes.get(),Bits.Size });
		
		return V;
	}
	else
	{
		return false;
	}
	
}
UCodeLangEnd