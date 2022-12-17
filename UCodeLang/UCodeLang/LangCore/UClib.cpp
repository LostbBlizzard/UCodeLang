#include "UClib.hpp"
#include <fstream>
UCodeLangStart

UClib::UClib() : LibEndianess(BitConverter::Get_CPU_Endian())
{
}

UClib::~UClib()
{

}
#define UpdateNewPtr() NewPtr = (void*)((size_t)NewBits.Bytes + (size_t)BitPos);
#define UpdateReadPtr() NewPtr = (void*)((size_t)Data.Bytes + (size_t)Indexoffset);


#define PushString(Var) StringSize = Var .size(); \
			BitConverter::MoveBytes((Size_tAsBits)StringSize, NewBits.Bytes, BitPos); \
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

UClib::LibRawBytes UClib::ToRawBytes(const UClib* Lib)
{
	LibRawBytes NewBits;
	

	size_t StaticBytesSize = Lib->_StaticBytes.size() * sizeof(unsigned char);
	
	size_t InstructionsBytesSize = Lib->_Instructions.size() * sizeof(Instruction);

	size_t NameToPtrBytesSize =0;
	for (auto& Item : Lib->_NameToPtr)
	{
		NameToPtrBytesSize += sizeof(Size_tAsBits) + Item.first.size();
		NameToPtrBytesSize += sizeof((Size_tAsBits)Item.second);
	}
	size_t ClassAssemblySize = sizeof(Size_tAsBits);//size
	for (auto& Item : Lib->_Assembly.Classes)
	{
		ClassAssemblySize += sizeof(Size_tAsBits) + Item->Name.size();
		ClassAssemblySize += sizeof(Size_tAsBits) + Item->FullName.size();
		ClassAssemblySize += (UInt8)Item->Type;
		switch (Item->Type)
		{
		case ClassType::Alias:
		{
			auto& Data = Item->_Alias;
			ClassAssemblySize += sizeof(Size_tAsBits) + Data.StringValue.size();
		}break;
		case ClassType::Class:
		{
			auto& Data = Item->_Class;
			ClassAssemblySize += sizeof(Size_tAsBits);// Size

			ClassAssemblySize += sizeof(Size_tAsBits);
			for (auto& A : Data.Attributes) 
			{
				ClassAssemblySize += sizeof(Size_tAsBits) + A.Name.size();
			}

			ClassAssemblySize += sizeof(Size_tAsBits);
			for (auto& A : Data.Fields)
			{
				ClassAssemblySize += sizeof(Size_tAsBits) + A.Name.size();
				ClassAssemblySize += sizeof(Size_tAsBits) + A.FullNameType.size();
				ClassAssemblySize += sizeof(Size_tAsBits);//offset
			}

			ClassAssemblySize += sizeof(Size_tAsBits);
			for (auto& A : Data.Methods)
			{
				ClassAssemblySize += sizeof(Size_tAsBits) + A.FullName.size();
			}
		
		}break;
		case ClassType::Enum:
		{
			auto& Data = Item->_Enum;
			ClassAssemblySize += sizeof(Data.Size);

			ClassAssemblySize += sizeof(Size_tAsBits);
			for (auto& Item : Data.Values)
			{
				ClassAssemblySize += sizeof(Item._State);
				ClassAssemblySize += sizeof(Size_tAsBits) + Item.Name.size();
				ClassAssemblySize += sizeof(Item.Value);
			}
		}
			break;
		default:
			break;
		}
	}

	size_t StaticDebugBytesSize = Lib->_DebugBytes.size() * sizeof(unsigned char);
	
	size_t ByteSize =
		sizeof(Size_tAsBits) + UClibSignature_Size
		+ sizeof(UInt8) +
		+ sizeof(UInt8) +
		+ sizeof(UInt8) +

		+ sizeof(BitSize) + sizeof(LibEndianess)
		+ sizeof(Size_tAsBits) + StaticBytesSize +
		+ sizeof(Size_tAsBits) + InstructionsBytesSize
		+ sizeof(Size_tAsBits) + NameToPtrBytesSize

		+ sizeof(Size_tAsBits) + StaticDebugBytesSize
		+ ClassAssemblySize;


	NewBits.Bytes = (unsigned char*)malloc(ByteSize);
	NewBits.Size = ByteSize;

	size_t BitPos = 0;
	void* NewPtr = nullptr;
	UpdateNewPtr();

	//Ulib signature
	{
		BitConverter::MoveBytes((Size_tAsBits)UClibSignature_Size, NewBits.Bytes, BitPos);
		BitPos += sizeof(Size_tAsBits);

		UpdateNewPtr();
		memcpy(NewPtr, &UClibSignature, UClibSignature_Size);
		BitPos += UClibSignature_Size;


		if ((size_t)InstructionSet::MAXVALUE > (size_t)UInt8_MaxSize)
		{
			throw std::exception("");
		}

		BitConverter::MoveBytes((UInt8)InstructionSet::MAXVALUE, NewBits.Bytes, BitPos);
		BitPos += sizeof(UInt8);

		BitConverter::MoveBytes((UInt8)Intermediate_Set::MAXVALUE, NewBits.Bytes, BitPos);
		BitPos += sizeof(UInt8);
	}

	BitConverter::MoveBytes((NTypeSize_t)Lib->BitSize, NewBits.Bytes, BitPos);
	BitPos += sizeof(NTypeSize_t);
	
	BitConverter::MoveBytes((LibType_t)Lib->_LibType, NewBits.Bytes, BitPos);
	BitPos += sizeof(LibType_t);

	BitConverter::MoveBytes((Endian_t)Lib->LibEndianess, NewBits.Bytes, BitPos);
	BitPos += sizeof(Lib->LibEndianess);

	
	

	{//StaticBytes
		auto StaticBits = Lib->_StaticBytes.size();
		BitConverter::MoveBytes((Size_tAsBits)StaticBits, NewBits.Bytes, BitPos);
		BitPos += sizeof(Size_tAsBits);

		UpdateNewPtr();
		if (StaticBits > 0) {
			memcpy(NewPtr, &Lib->_StaticBytes[0], StaticBits);
		}
		BitPos += StaticBits;
	}

	{// Instructions

		auto SizeBits = Lib->_Instructions.size();
		BitConverter::MoveBytes((Size_tAsBits)SizeBits, NewBits.Bytes, BitPos);
		BitPos += sizeof(Size_tAsBits);
		for (size_t i = 0; i < SizeBits; i++)
		{
			const auto& Item = Lib->_Instructions[i];

			BitConverter::MoveBytes((InstructionSet_t)Item.OpCode, NewBits.Bytes, BitPos);
			BitPos += sizeof(InstructionSet_t);

			BitConverter::MoveBytes(Item.Value0.AsUInt64, NewBits.Bytes, BitPos);
			BitPos += sizeof(UInt64);

			BitConverter::MoveBytes(Item.Value1.AsUInt64, NewBits.Bytes, BitPos);
			BitPos += sizeof(UInt64);
		}
	}


	{// _NameToPtr
		BitConverter::MoveBytes((Size_tAsBits)Lib->_NameToPtr.size(), NewBits.Bytes, BitPos);
		BitPos += sizeof(Size_tAsBits);

		for (auto& Item : Lib->_NameToPtr)
		{
			size_t StringSize;
			PushString(Item.first);
			//

			BitConverter::MoveBytes(Item.second, NewBits.Bytes, BitPos);
			BitPos += sizeof(Size_tAsBits);
		}
	}

	{//Debug Bytes
		auto StaticBits = Lib->_DebugBytes.size();
		BitConverter::MoveBytes((Size_tAsBits)StaticBits, NewBits.Bytes, BitPos);
		BitPos += sizeof(Size_tAsBits);

		UpdateNewPtr();
		if (StaticBits > 0) {
			memcpy(NewPtr, &Lib->_DebugBytes[0], StaticBits);
		}
		BitPos += StaticBits;
	}
	
	//ClassAssembly
	{
		auto StaticBits = Lib->_Assembly.Classes.size();
		BitConverter::MoveBytes((Size_tAsBits)StaticBits, NewBits.Bytes, BitPos);
		BitPos += sizeof(Size_tAsBits);

		for (auto& Item : Lib->_Assembly.Classes)
		{
			size_t StringSize;
			PushString(Item->Name);
			PushString(Item->FullName);

			BitConverter::MoveBytes((EnumSizez_t)Item->Type, NewBits.Bytes, BitPos);
			BitPos += sizeof(EnumSizez_t);
			switch (Item->Type)
			{
			case ClassType::Alias:
				PushString(Item->_Alias.StringValue);
				break;
			case ClassType::Class:
			{
				auto& ClassData = Item->_Class;
				BitConverter::MoveBytes((Size_tAsBits)ClassData.Size, NewBits.Bytes, BitPos);BitPos += sizeof(Size_tAsBits);

				BitConverter::MoveBytes((Size_tAsBits)ClassData.Attributes.size(), NewBits.Bytes, BitPos);BitPos += sizeof(Size_tAsBits);
				for (auto& Item2 : ClassData.Attributes)
				{
					PushString(Item2.Name);
				}

				BitConverter::MoveBytes((Size_tAsBits)ClassData.Fields.size(), NewBits.Bytes, BitPos); BitPos += sizeof(Size_tAsBits);
				for (auto& Item2 : ClassData.Fields)
				{
					PushString(Item2.Name);
					PushString(Item2.FullNameType);
					
					BitConverter::MoveBytes((Size_tAsBits)Item2.offset, NewBits.Bytes, BitPos); BitPos += sizeof(Size_tAsBits);
				}

				BitConverter::MoveBytes((Size_tAsBits)ClassData.Methods.size(), NewBits.Bytes, BitPos); BitPos += sizeof(Size_tAsBits);

				for (auto& Item2 : ClassData.Methods)
				{
					PushString(Item2.FullName);
				}
			}
				break;
			case ClassType::Enum:
			{
				BitConverter::MoveBytes((EnumSizez_t)Item->_Enum.Size, NewBits.Bytes, BitPos);
				BitPos += sizeof(EnumSizez_t);

				auto StaticBits2 = Item->_Enum.Values.size();
				BitConverter::MoveBytes((Size_tAsBits)StaticBits2, NewBits.Bytes, BitPos);
				BitPos += sizeof(Size_tAsBits);
				for (auto& Item2 : Item->_Enum.Values)
				{
					PushString(Item2.Name);

					BitConverter::MoveBytes((UInt8)Item2._State, NewBits.Bytes, BitPos);
					BitPos += sizeof(UInt8);

					BitConverter::MoveBytes((Size_tAsBits)Item2.Value, NewBits.Bytes, BitPos);
					BitPos += sizeof(Size_tAsBits);
				}
			}
			break;
			default:
				break;
			}
		}
	}
	return NewBits;
}
bool UClib::FromBytes(UClib* Lib, const LibRawBytes& Data)
{
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
		
		if ((size_t)Intermediate_Set::MAXVALUE > (size_t)UInt8_MaxSize)
		{
			throw std::exception("");
		}
		
		auto Value = (InstructionSet)BitConverter::BytesToChar(Data.Bytes, Indexoffset);
		Indexoffset += sizeof(UInt8);
		if (Value != InstructionSet::MAXVALUE)
		{
			return false;
		}

		auto Value2 = (UInt8)BitConverter::BytesToChar(Data.Bytes, Indexoffset);
		Indexoffset += sizeof(UInt8);
		if (Value2 != (UInt8)Intermediate_Set::MAXVALUE)
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

	return true;
}
bool UClib::ToFile(const UClib* Lib, const String& Path)
{
	std::ofstream File(Path, std::ios::binary);
	if (File.is_open())
	{

		LibRawBytes Bits = ToRawBytes(Lib);

		File.write((const char*)Bits.Bytes, Bits.Size);


		Free(Bits);

		File.close();
		return true;
	}
	else
	{
		return false;
	}
}
bool UClib::FromFile(UClib* Lib, const String& Path)
{
	std::ifstream File(Path, std::ios::binary);
	if (File.is_open())
	{
		LibRawBytes Bits;
		File.seekg(0, File.end);
		Bits.Size = File.tellg();
		File.seekg(0, File.beg);
		Bits.Bytes = (unsigned char*)malloc(Bits.Size);

		File.read((char*)Bits.Bytes, Bits.Size);
		File.close();
		auto V = FromBytes(Lib, Bits);
		Free(Bits);

		return V;
	}
	else
	{
		return false;
	}
	
}
UCodeLangEnd