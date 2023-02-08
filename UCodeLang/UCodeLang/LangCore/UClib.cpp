#include "UClib.hpp"
#include <fstream>
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
		Output.WriteBytes(Lib->_StaticBytes.data(), Lib->_StaticBytes.size());
	}

	{// Instructions
		Output.WriteBytes((const Byte*)Lib->_Instructions.data(), Lib->_Instructions.size() * sizeof(Instruction));
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
		Output.WriteBytes(Lib->_DebugBytes.data(), Lib->_DebugBytes.size());
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
			ToBytes(Output,Item->_Alias.Type);
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
					ToBytes(Output, Item2.Type);
					Output.WriteType((Size_tAsBits)Item2.offset);
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
					Output.WriteType((EnumValues::State_t)Item2._State);
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
	BitReader reader(Data.Bytes,Data.Size);
	

	

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
				UAddress V2 = 0;
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
		union
		{
			Size_tAsBits bits = 0;
			size_t bits_Size;
		};
		reader.ReadType(bits, bits);
		bits_Size = bits;
		
		for (size_t i = 0; i < bits_Size; i++)
		{
			auto& Item = Assembly.AddClass("","");

			reader.ReadType(Item.Name, Item.Name);
			reader.ReadType(Item.FullName, Item.FullName);
			reader.ReadType(*(ClassType_t*)&Item.Type, *(ClassType_t*)&Item.Type);

			switch (Item.Type)
			{
			case ClassType::Alias:
			{
				FromBytes(reader, Item._Alias.Type);
			}
			break;
			case ClassType::Class:
			{
				Size_tAsBits _Classbits = 0;
				reader.ReadType(_Classbits, _Classbits);
				Item._Class.Size =bits;

				union
				{
					Size_tAsBits  Attributes_Sizebits = 0;
					size_t Attributes_Size;
				};
				reader.ReadType(Attributes_Sizebits, Attributes_Sizebits);
				Attributes_Size = Attributes_Sizebits;

				Item._Class.Attributes.resize(Attributes_Size);
				for (size_t i2 = 0; i2 < Attributes_Size; i2++)
				{
					auto& Item2 = Item._Class.Attributes[i2];
					FromBytes(reader,Item2);
				}


				union
				{
					Size_tAsBits  Feld_Sizebits = 0;
					size_t Feld_Size;
				};
				reader.ReadType(Feld_Sizebits, Feld_Sizebits);
				Feld_Size = Feld_Sizebits;

				Item._Class.Fields.resize(Feld_Size);
				for (size_t i2 = 0; i2 < Feld_Size; i2++)
				{
					auto& Item2 = Item._Class.Fields[i2];
					reader.ReadType(Item2.Name, Item2.Name);
					FromBytes(reader, Item2.Type);
					reader.ReadType(Item2.offset, Item2.offset);
				}

				union
				{
					Size_tAsBits  Methods_Sizebits = 0;
					size_t Methods_Size;
				};
				reader.ReadType(Methods_Sizebits, Methods_Sizebits);
				Methods_Size = Methods_Sizebits;

				Item._Class.Methods.resize(Methods_Size);
				for (size_t i2 = 0; i2 < Methods_Size; i2++)
				{
					auto& Item2 = Item._Class.Methods[i2];
					FromBytes(reader, Item2);
				}

			}
				break;
			case ClassType::Enum:
			{
				reader.ReadType(*(EnumSizez_t*)&Item._Enum.Size, *(EnumSizez_t*)&Item._Enum.Size);

				union
				{
					Size_tAsBits  Sizebits = 0;
					size_t Size;

				};
				reader.ReadType(Sizebits, Sizebits);
				Size = Sizebits;

				Item._Enum.Values.resize(Size);
				for (size_t i2 = 0; i2 < Size; i2++)
				{
					auto& Item2 = Item._Enum.Values[i2];
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