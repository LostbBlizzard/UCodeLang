#ifndef UCodeLangNoCompiler
#include "ELF_File.hpp"
#include <fstream>
UCodeLangStart


void ELFFile::ToBytes(BitMaker& bit) const
{

	//ELF  magic number.
	{
		bit.WriteType((Byte)0x7F);

		bit.WriteType((Byte)0x45);

		bit.WriteType((Byte)0x4c);

		bit.WriteType((Byte)0x46);
	}

	bit.WriteType((Byte)format);
	bit.WriteType((Byte)data);
	bit.WriteType((Byte)version);
	bit.WriteType((Byte)Abi);

	bit.WriteType((Byte)0);//e_ident[EI_ABIVERSION]


	bit.WriteType((Byte)0);//Reserved padding bytes. Currently unused. Should be filled with zeros and ignored when read.
	bit.WriteType((Byte)0);
	bit.WriteType((Byte)0);
	bit.WriteType((Byte)0);
	bit.WriteType((Byte)0);
	bit.WriteType((Byte)0);
	bit.WriteType((Byte)0);


	bit.WriteType((UInt16)type);

	bit.WriteType((UInt16)machine);


	bit.WriteType((UInt32)1);//e_version

	if (format == Classformat::bit32)
	{
		bit.WriteType((UInt64)entry);

		bit.WriteType((UInt64)ProgramheaderTableoffset);

		bit.WriteType((UInt64)SectionheaderTableoffset);
	}
	else
	{
		bit.WriteType((UInt32)entry);

		bit.WriteType((UInt32)ProgramheaderTableoffset);

		bit.WriteType((UInt32)SectionheaderTableoffset);
	}

}
bool ELFFile::FromBytes(BitReader& bit)
{

	{
		Byte V = 0;
		bit.ReadType(V);

		if (V != 0x7F)
		{
			return false;
		}

		bit.ReadType(V);

		if (V != 0x45)
		{
			return false;
		}

		bit.ReadType(V);

		if (V != 0x4c)
		{
			return false;
		}

		bit.ReadType(V);

		if (V != 0x46)
		{
			return false;
		}
	}



	return true;
}
bool ELFFile::FromFile(const Path& path, ELFFile& file)
{
	std::ifstream File(path, std::ios::binary);
	if (File.is_open())
	{
		BytesPtr Bits;
		File.seekg(0, File.end);
		Bits.Resize(File.tellg());
		File.seekg(0, File.beg);

		File.read((char*)Bits.Data(), Bits.Size());
		File.close();
		auto V = FromBytes(file, Bits.AsSpan());

		return V;
	}
	else
	{
		return false;
	}
}
bool ELFFile::ToFile(const ELFFile& file, const Path& path)
{
	std::ofstream File(path, std::ios::binary);
	if (File.is_open())
	{

		BytesPtr Bits = ToBytes(file);

		File.write((const char*)Bits.Data(), Bits.Size());


		File.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool ELFFile::FromBytes(ELFFile& file, const BytesView Bytes)
{
	BitReader bit;
	bit.SetBytes(Bytes.Data(), Bytes.Size());
	return file.FromBytes(bit);
}
BytesPtr ELFFile::ToBytes(const ELFFile& file)
{
	BitMaker bit;
	file.ToBytes(bit);
	return bit.AsBytePtrAndMove();
}
UCodeLangEnd

#endif