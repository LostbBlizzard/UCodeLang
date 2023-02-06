#include "UClib.hpp"
#include <fstream>
#include "UCodeLang/LangCore/BitMaker.hpp"
UCodeLangStart

UClib::UClib() : LibEndianess(BitConverter::InputOutEndian)
{
}

UClib::~UClib()
{

}
UClib::UClib(UClib& GetFrom)
{
	throw std::exception("");
}
#define UpdateNewPtr() NewPtr = (void*)((size_t)NewBits.Bytes.get() + (size_t)BitPos);
#define UpdateReadPtr() NewPtr = (void*)((size_t)Data.Bytes + (size_t)Indexoffset);


#define PushString(Var) StringSize = Var .size(); \
			BitConverter::MoveBytes((Size_tAsBits)StringSize, NewBits.Bytes.get(), BitPos); \
			BitPos += sizeof(Size_tAsBits); \
\
			UpdateNewPtr();\
\
			memcpy(NewPtr,Var .c_str(), StringSize);\
\
			BitPos += StringSize;\
			UpdateNewPtr();\

#define PullString(OutVar) StringSize = (size_t)BitConverter::BytesToInt(Data.Bytes, Indexoffset); \
Indexoffset += sizeof(Size_tAsBits); \
\
OutVar .clear(); \
for (size_t i2 = 0; i2 < StringSize; i2++) \
{ \
	OutVar .push_back((char)Data.Bytes[Indexoffset]); \
	Indexoffset++; \
} \

#define PullChar(Var) \
Var = BitConverter::BytesToChar(Data.Bytes, Indexoffset); \
Indexoffset += sizeof(char);\

#define PullCharEnum(Var,Type) \
Var = (Type)BitConverter::BytesToChar(Data.Bytes, Indexoffset); \
Indexoffset += sizeof(Type); \

#define PullSize_t(Var) \
Var = (size_t)BitConverter::BytesToInt(Data.Bytes, Indexoffset); \
Indexoffset += sizeof(Size_tAsBits); \

#define PullUInt64(Var) \
Var = (UInt64)BitConverter::BytesToInt64(Data.Bytes, Indexoffset); \
Indexoffset += sizeof(UInt64); \

#define PullUInt64AsE(Var,Type) \
Var = (Type)BitConverter::BytesToInt64(Data.Bytes, Indexoffset); \
Indexoffset += sizeof(UInt64); \

using Size_tAsBits = int;


const unsigned char UClibSignature[]= "Lost_blizzard_Ulib";
constexpr size_t UClibSignature_Size = sizeof(UClibSignature);

BytesPtr UClib::ToRawBytes(const UClib* Lib)
{
	BitMaker Output;



	//Ulib signature
	{
		Output.WriteBytes(UClibSignature, UClibSignature_Size);

		Output.WriteType((InstructionSet_t)InstructionSet::MAXVALUE);

		Output.WriteType((InstructionSet_t)Intermediate_Set::MAXVALUE);
	}

	Output.WriteType((NTypeSize_t)Lib->BitSize);

	Output.WriteType((LibType_t)Lib->_LibType);

	Output.WriteType((Endian_t)Lib->LibEndianess);

	{//StaticBytes
		Output.WriteBytes(&Lib->_StaticBytes[0], Lib->_StaticBytes.size());
	}

	{// Instructions
		Output.WriteBytes((const BitMaker::Byte*)&Lib->_Instructions[0], Lib->_Instructions.size() * sizeof(Instruction));
	}


	{// _NameToPtr
	
		Output.WriteType((Size_tAsBits)Lib->_NameToPtr.size());
		for (auto& Item : Lib->_NameToPtr)
		{
			Output.WriteType(Item.first);
			Output.WriteType((Size_tAsBits)Item.second);
		}
	}

	{//Debug Bytes
		Output.WriteBytes(&Lib->_DebugBytes[0], Lib->_DebugBytes.size());
	}
	
	//ClassAssembly
	{

		Output.WriteType((Size_tAsBits)Lib->_Assembly.Classes.size());


		for (auto& Item : Lib->_Assembly.Classes)
		{
			Output.WriteType(Item->Name);
			Output.WriteType(Item->FullName);
			
			Output.WriteType((ClassType_t)Item->Type);

			switch (Item->Type)
			{
			case ClassType::Alias:
			Output.WriteType(Item->_Alias.StringValue);
			break;
			case ClassType::Class:
			{
				auto& ClassData = Item->_Class;
				Output.WriteType((Size_tAsBits)ClassData.Size);
				
				
				for (auto& Item2 : ClassData.Attributes)
				{
					ToBytes(Output, Item2);
				}

				Output.WriteType((Size_tAsBits)ClassData.Fields.size());
				for (auto& Item2 : ClassData.Fields)
				{
					Output.WriteType(Item2.Name);
					Output.WriteType(Item2.FullNameType);
				}

				Output.WriteType((Size_tAsBits)ClassData.Methods.size());
				for (auto& Item2 : ClassData.Methods)
				{
					ToBytes(Output, Item2);
				}
			}
				break;
			case ClassType::Enum:
			{
				Output.WriteType((EnumSizez_t)Item->_Enum.Size);
				
				Output.WriteType((Size_tAsBits)Item->_Enum.Values.size());

				for (auto& Item2 : Item->_Enum.Values)
				{
					Output.WriteType(Item2.Name);
					Output.WriteType(Item2._State);
					Output.WriteType((Size_tAsBits)Item2.Value);
				}
			}
			break;
			default:
				break;
			}
		}
	}

	BytesPtr V;
	V.Bytes.reset(new Byte[Output.Size()]);
	std::memcpy(V.Bytes.get(), &Output.Get_Bytes()[0], Output.Size());
	V.Size = Output.Size();
	return V;
}
bool UClib::FromBytes(UClib* Lib, const BytesView& Data)
{
	auto Old = BitConverter::InputOutEndian;
	BitConverter::InputOutEndian = Lib->LibEndianess;

	size_t Indexoffset = 0;
	void* NewPtr = nullptr;
	UpdateReadPtr();
	//Ulib signature
	{
		size_t bits;
		PullSize_t(bits);

		if (bits != UClibSignature_Size) { return false; }

		for (size_t i = 0; i < UClibSignature_Size; i++)
		{
			char Bit = Data.Bytes[Indexoffset];
			if (Bit != UClibSignature[i]) { return false; }
			Indexoffset++;
		}
		
	
		
		auto Value = (InstructionSet)BitConverter::BytesToUInt64(Data.Bytes, Indexoffset);
		Indexoffset += sizeof(InstructionSet);
		if (Value != InstructionSet::MAXVALUE)
		{
			return false;
		}

		auto Value2 = (Intermediate_Set)BitConverter::BytesToUInt64(Data.Bytes, Indexoffset);
		Indexoffset += sizeof(InstructionSet);
		if (Value2 != Intermediate_Set::MAXVALUE)
		{
			return false;
		}
	}

	PullCharEnum(Lib->BitSize, NTypeSize);

	PullCharEnum(Lib->_LibType, LibType);

	PullCharEnum(Lib->LibEndianess, Endian);
	
	

	{//StaticBytes
		size_t bits;
		PullSize_t(bits);

		Lib->_StaticBytes.resize(bits);

		UpdateReadPtr();

		if (bits != 0) {
			memcpy(&Lib->_StaticBytes[0], NewPtr, bits);
		}

		Indexoffset += bits;
	}

	{// Instructions

		size_t bits;
		PullSize_t(bits);

		Lib->_Instructions.resize(bits);
		for (size_t i = 0; i < bits; i++)
		{
			auto& Item = Lib->_Instructions[i];

			PullUInt64AsE(Item.OpCode, InstructionSet);

			PullUInt64(Item.Value0.AsUInt64);
			PullUInt64(Item.Value1.AsUInt64);
		
		}

	}

	{// _NameToPtr
		size_t bits;
		PullSize_t(bits);

		Lib->_NameToPtr.clear();
		Lib->_NameToPtr.reserve(bits);

		for (size_t i = 0; i < bits; i++)
		{
			size_t StringSize;
			String V1;
			UAddress V2;

			PullString(V1);


			V2 = (UAddress)BitConverter::BytesToInt(Data.Bytes, Indexoffset);
			Indexoffset += sizeof(Size_tAsBits);

			Lib->_NameToPtr[V1] = V2;
		}
	}

	{//DebugBytes
		size_t bits;
		PullSize_t(bits);
		Lib->_DebugBytes.resize(bits);

		UpdateReadPtr();

		if (bits != 0) {
			memcpy(&Lib->_DebugBytes[0], NewPtr, bits);
		}

		Indexoffset += bits;
	}
	//ClassAssembly
	{
		auto& Assembly = Lib->Get_Assembly();
		size_t StringSize;
		String TepString;

		size_t bits;
		PullSize_t(bits);

		for (size_t i = 0; i < bits; i++)
		{
			auto& Item = Assembly.AddClass("","");

			PullString(TepString); Item.Name = TepString;
			PullString(TepString); Item.FullName = TepString;

			PullCharEnum(Item.Type, ClassType);
			switch (Item.Type)
			{
			case ClassType::Alias:
			{
				PullString(TepString); Item._Alias.StringValue = TepString;
			}
				break;
			case ClassType::Class:
			{
				PullSize_t(Item._Class.Size);

				size_t Attributes_Size;
				PullSize_t(Attributes_Size);

				Item._Class.Attributes.resize(Attributes_Size);
				for (size_t i2 = 0; i2 < Attributes_Size; i2++)
				{
					auto& Item2 = Item._Class.Attributes[i2];
					
					PullString(TepString); Item2.Name = TepString;
				}

				size_t Feld_Size;
				PullSize_t(Feld_Size);

				Item._Class.Fields.resize(Feld_Size);
				for (size_t i2 = 0; i2 < Feld_Size; i2++)
				{
					auto& Item2 = Item._Class.Fields[i2];

					PullString(TepString); Item2.Name = TepString;
					PullString(TepString); Item2.FullNameType = TepString;
					PullSize_t(Item2.offset);
				}

				size_t Methods_Size;
				PullSize_t(Methods_Size);
				Item._Class.Methods.resize(Methods_Size);
				for (size_t i2 = 0; i2 < Methods_Size; i2++)
				{
					auto& Item2 = Item._Class.Methods[i2];
					PullString(TepString); Item2.FullName = TepString;
				}

			}
				break;
			case ClassType::Enum:
			{
				PullCharEnum(Item._Enum.Size, EnumSizez);
				size_t Size;
				PullSize_t(Size);

				Item._Enum.Values.resize(Size);
				for (size_t i2 = 0; i2 < Size; i2++)
				{
					auto& Item2 = Item._Enum.Values[i2];
					PullString(TepString);  Item2.Name = TepString;
					PullCharEnum(Item2._State, EnumValues::State);
					PullSize_t(Item2.Value);
				}
			}
				break;
			default:
				break;
			}
			
		}
	}

	BitConverter::InputOutEndian = Old;
	return true;
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