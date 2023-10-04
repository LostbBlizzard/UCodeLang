#include "WasmFile.hpp"
#include <fstream>
UCodeLangStart

bool WasmFile::FromFile(const Path& path, WasmFile& file)
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
bool WasmFile::ToFile(const WasmFile& file, const Path& path)
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

constexpr char WasmFileMagic[5] = "\0asm";
UInt32 WasmFileMagicAsInt = *(UInt32*)&WasmFileMagic;


void WasmFile::ReadLEB128(BitReader& bit,WasmFile::varU32& out)
{
	varU32 result = 0;
	varU32 shift = 0;
	while (true) {
		Byte byte = 0;
		bit.ReadType(byte);
		result |= (byte & 0x7f) << shift;
		if ((byte & 0x80) == 0) {
			break;
		}
		shift += 7;
	}
	out = result;
}
void WasmFile::WriteLEB128(BitMaker& bit,WasmFile::varU32 value)
{
	do {
		Byte byte = value & 0x7f;
		value >>= 7;
		if (value != 0) {
			byte |= 0x80;
		}
		bit.WriteType(byte);
	} while (value != 0);
}

bool WasmFile::FromBytes(WasmFile& file, const BytesView Bytes)
{
	BitReader bit;
	const auto oldEndian = BitConverter::InputOutEndian;
	BitConverter::InputOutEndian = Endian::little;
	UCodeLangDefer(BitConverter::InputOutEndian = oldEndian);

	bit.SetBytes(Bytes.Data(), Bytes.Size());


	UInt32 magic;
	bit.ReadType(magic);
	if (magic == WasmFileMagicAsInt)
	{
		bit.ReadType(file.Version);
		
		
		while (bit.Get_offset() != Bytes.Size())
		{
			file.section.push_back({});
			file.section.back().FromBytes(bit);
		}

		return true;
	}

	return false;
}
BytesPtr WasmFile::ToBytes(const WasmFile& file)
{

	BitMaker bit;	
	const auto oldEndian = BitConverter::InputOutEndian;
	BitConverter::InputOutEndian = Endian::little;
	UCodeLangDefer(BitConverter::InputOutEndian = oldEndian);

	bit.WriteType(WasmFileMagicAsInt);
	bit.WriteType(file.Version);

	//WriteLEB128(bit, (VectorLength)file.section.size());
	for (size_t i = 0; i < file.section.size(); i++)
	{
		file.section[i].ToBytes(bit);
	}
	

	return bit.AsBytePtrAndMove();
}
String WasmFile::ToWat() const
{
	String r;

	r += "(module\n ";

	for (auto& Item : section)
	{
		if (auto Val = Item.Type.Get_If<TypeSection>())
		{
			for (auto& Item : Val->Types)
			{
				r += ToWat(Item);
				r += "\n ";
			}
		}
		else
		{
			//UCodeLangUnreachable();
		}
	}

	r += " )";

	return r;
}
String WasmFile::ToWat(const ValType& Item) const
{
	switch (Item)
	{
	case ValType::i32:
		return "i32";
	case ValType::i64:
		return "i64";

	case ValType::f32:
		return "f64";

	case ValType::f64:
		return "i64";
	default:
		UCodeLangUnreachable();
		break;
	}
}
String WasmFile::ToWat(const FuncType& Item) const
{
	String r;
	r += "(type";
	r += " ";
	r += "t0 ";

	r += "(func ";


	if (Item.Params.size()) 
	{
		r += "(param ";
		for (auto& item : Item.Params)
		{
			r += ToWat(item);
			r += " ";
		}
		r += ") ";
	}

	if (Item.Results.size())
	{
		r += "(result ";
		for (auto& item : Item.Results)
		{
			r += ToWat(item);
			r += " ";
		}
	}

	r += "))";
	return r;
}
UCodeLangEnd