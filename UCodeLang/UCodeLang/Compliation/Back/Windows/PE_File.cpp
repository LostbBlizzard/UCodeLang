#include "PE_File.hpp"
#include <fstream>
#include <filesystem>
UCodeLangStart
PE_File::PE_File()
{

}


PE_File::~PE_File()
{

}

BytesPtr PE_File::ToRawBytes(const PE_File* In)
{
	auto OldOutEndian = BitConverter::InputOutEndian;
	BitConverter::InputOutEndian = Endian::little;
	
		BitMaker bits;
		bits.WriteType('M');
		bits.WriteType('Z');
	
	
	
	BitConverter::InputOutEndian = OldOutEndian;


		return {};
}


bool PE_File::FromBytes(BytesView Bits, PE_File* Out)
{
	auto OldOutEndian = BitConverter::InputOutEndian;
	BitConverter::InputOutEndian = Endian::little;
	{
		BitReader bits;
		char signature[2] = { 'M', 'Z' };
		bits.ReadType(signature[0],'\0');
		bits.ReadType(signature[1],'\0');
		if (!(signature[0] == 'M' && signature[1] == 'Z'))
		{
			return false;
		}
		bits.Increment_offset(sizeof(EXECUTABLE_DOS_HEADER) - sizeof(signature));

	}
	BitConverter::InputOutEndian = OldOutEndian;

	return true;
}

bool PE_File::ToFile(const PE_File* Lib, const Path& path)
{
	std::ofstream File(path, std::ios::binary);
	if (File.is_open())
	{

		BytesPtr Bits = ToRawBytes(Lib);

		File.write((const char*)Bits.Data(), Bits.Size());


		File.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool PE_File::FromFile(PE_File* Lib, const Path& path)
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
		auto V = FromBytes(Bits.AsSpan(), Lib);

		return V;
	}
	else
	{
		return false;
	}
}
UCodeLangEnd

