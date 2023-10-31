#include "MachO.hpp"
#include <fstream>
UCodeLangStart


void MachO::ToBytes(BitMaker& bit) const
{

}
bool MachO::FromBytes(BitReader& bit)
{
	return false;
}

bool MachO::FromFile(const Path& path, MachO& file)
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
bool MachO::ToFile(const MachO& file, const Path& path)
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

bool MachO::FromBytes(MachO& file, const BytesView Bytes)
{
	BitReader bit;
	bit.SetBytes(Bytes.Data(), Bytes.Size());
	return file.FromBytes(bit);
}
BytesPtr MachO::ToBytes(const MachO& file)
{
	BitMaker bit;
	file.ToBytes(bit);
	return bit.AsBytePtrAndMove();
}

UCodeLangEnd