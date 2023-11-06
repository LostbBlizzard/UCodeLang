#include "PE_File.hpp"
#include <fstream>
#include <filesystem>
UCodeLangStart



void PEFile::ToBytes(BitMaker& bit) const
{

}
bool PEFile::FromBytes(BitReader& bit)
{
	return false;
}

bool PEFile::FromFile(const Path& path, PEFile& file)
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
bool PEFile::ToFile(const PEFile& file, const Path& path)
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

bool PEFile::FromBytes(PEFile& file, const BytesView Bytes)
{
	BitReader bit;
	bit.SetBytes(Bytes.Data(), Bytes.Size());
	return file.FromBytes(bit);
}
BytesPtr PEFile::ToBytes(const PEFile& file)
{
	BitMaker bit;
	file.ToBytes(bit);
	return bit.AsBytePtrAndMove();
}

UCodeLangEnd

