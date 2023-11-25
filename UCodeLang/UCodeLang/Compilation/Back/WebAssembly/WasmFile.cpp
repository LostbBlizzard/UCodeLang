
#ifndef UCodeLangNoCompiler
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

	const CodeSection* CodeVal = nullptr;
	for (auto& Item : section)
	{
		if (auto Val = Item.Type.Get_If<CodeSection>())
		{
			CodeVal = Val;
		}
	}
	const ExportSection* ExportVal = nullptr;
	for (auto& Item : section)
	{
		if (auto Val = Item.Type.Get_If<ExportSection>())
		{
			ExportVal= Val;
		}
	}

	for (auto& Item : section)
	{
		if (auto Val = Item.Type.Get_If<TypeSection>())
		{
			for (size_t i = 0; i < Val->Types.size(); i++)
			{
				auto& Item = Val->Types[i];
				auto& Code = CodeVal->code[i];
				auto& Export = ExportVal->Exports[i];

				r += ToWat(Item);

				r += "(export " + Export.Name + ")";

				r += "\n";
				r += ToWat(Code);
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
String WasmFile::ToWat(const Code& Item) const
{
	String r;
	for (auto& Item : Item.Ins)
	{
		r += "  ";
		r += ToWat(Item);
		r += '\n';
	}
	return r;
}
String WasmFile::ToWat(const Expr& Item) const
{
	String r;
	switch (Item.InsType)
	{
	case Expr::Ins::end:
		r += "end";
		break;
	case Expr::Ins::i32const:
		r += "i32.const ";
		r += std::to_string(Item.Const.AsInt32);
		break;
	case Expr::Ins::i64const:
		r += "i64.const ";
		r += std::to_string(Item.Const.AsInt64);
		break;
	case Expr::Ins::f32const:
		r += "f32.const ";
		r += std::to_string(Item.Const.Asfloat32);
		break;
	case Expr::Ins::f64const:
		r += "f64.const ";
		r += std::to_string(Item.Const.Asfloat64);
		break;
	case Expr::Ins::Return:
		r += "return";
		break;
	case Expr::Ins::i32load:
		r += "i32.load ";
		break;
	case Expr::Ins::i32store:
		r += "i32.store ";
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return r;
}

void WasmFile::Expr::ToBytes(BitMaker& bit) const
{
	bit.WriteType((Byte)InsType);
	if (InsType == Ins::i32const)
	{
		WriteLEB128(bit, Const.AsUInt32);
	}
	else if (InsType == Ins::f32const)
	{
		bit.WriteType(Const.Asfloat32);
	}
	else if (InsType == Ins::Return
		|| InsType == Ins::Unreachable
		|| InsType == Ins::end
		|| InsType == Ins::i32store
		|| InsType == Ins::i32load)
	{

	}
	else
	{
		UCodeLangUnreachable();
	}
}

void WasmFile::Expr::FromBytes(BitReader& bit)
{
	bit.ReadType(*(Byte*)&InsType);
	if (InsType == Ins::i32const)
	{
		ReadLEB128(bit, Const.AsUInt32);
	}
	else if (InsType == Ins::Return
		|| InsType == Ins::Unreachable
		|| InsType == Ins::end
		|| InsType == Ins::i32store
		|| InsType == Ins::i32load)
	{

	}
	else
	{
		UCodeLangUnreachable();
	}
}


void WasmFile::ExportSection::ToBytes(BitMaker& bit) const
{
	bit.WriteType(SectionNumber);

	BitMaker tep;

	{
		WasmFile::WriteLEB128(tep, (VectorLength)Exports.size());

		for (auto& Item : Exports)
		{
			Write_String(tep, Item.Name);
			tep.WriteType((Byte)Item.Tag);
			WasmFile::WriteLEB128(tep, Item.Index);
		}
	}

	auto& bytes = tep.Get_Bytes();
	WasmFile::WriteLEB128(bit, (varU32)bytes.size());
	bit.WriteBytes(bytes.data(), bytes.size());
}

void WasmFile::ExportSection::FromBytes(BitReader& bit)
{
	varU32 L = 0;//SectionNumber
	WasmFile::ReadLEB128(bit, L);

	auto bytes = bit.ReadBytesAsSpan(L);
	BitReader v;
	v.SetBytes(bytes.Data(), bytes.Size());

	{
		VectorLength exportslength = 0;
		WasmFile::ReadLEB128(v, exportslength);

		Exports.resize(exportslength);

		for (auto& Item : Exports)
		{
			Read_String(v, Item.Name);

			v.ReadType(*(Byte*)&Item.Tag);

			WasmFile::ReadLEB128(v, Item.Index);
		}
	}
}

void WasmFile::FuncSection::ToBytes(BitMaker& bit) const
{
	bit.WriteType(SectionNumber);

	BitMaker tep;

	{
		WasmFile::WriteLEB128(tep, (VectorLength)TypesIndex.size());

		for (auto& Item : TypesIndex)
		{
			WasmFile::WriteLEB128(tep, (VectorLength)Item);
		}
	}

	auto& bytes = tep.Get_Bytes();
	WasmFile::WriteLEB128(bit, (varU32)bytes.size());
	bit.WriteBytes(bytes.data(), bytes.size());
}

void WasmFile::FuncSection::FromBytes(BitReader& bit)
{
	varU32 L = 0;//SectionNumber
	WasmFile::ReadLEB128(bit, L);

	auto bytes = bit.ReadBytesAsSpan(L);
	BitReader v;
	v.SetBytes(bytes.Data(), bytes.Size());

	{
		VectorLength typeslength = 0;
		WasmFile::ReadLEB128(v, typeslength);

		TypesIndex.resize(typeslength);

		for (auto& Item : TypesIndex)
		{
			WasmFile::ReadLEB128(v, *(varU32*)&Item);
		}
	}
}

void WasmFile::TypeSection::ToBytes(BitMaker& bit) const
{
	bit.WriteType(SectionNumber);

	BitMaker tep;

	{
		WasmFile::WriteLEB128(tep, (VectorLength)Types.size());

		for (auto& Item : Types)
		{
			Item.ToBytes(tep);
		}
	}

	auto& bytes = tep.Get_Bytes();
	WasmFile::WriteLEB128(bit, (varU32)bytes.size());
	bit.WriteBytes(bytes.data(), bytes.size());
}

void WasmFile::TypeSection::FromBytes(BitReader& bit)
{
	varU32 L = 0;
	WasmFile::ReadLEB128(bit, L);

	auto bytes = bit.ReadBytesAsSpan(L);
	BitReader v;
	v.SetBytes(bytes.Data(), bytes.Size());

	{
		VectorLength typeslength = 0;
		WasmFile::ReadLEB128(v, typeslength);

		Types.resize(typeslength);

		for (auto& Item : Types)
		{
			Item.FromBytes(v);
		}
	}
}

void WasmFile::FuncType::ToBytes(BitMaker& bit) const
{
	bit.WriteType(tag);

	WasmFile::WriteLEB128(bit, (VectorLength)Params.size());
	for (auto& Item : Params)
	{
		bit.WriteType((Byte)Item);
	}

	WasmFile::WriteLEB128(bit, (VectorLength)Results.size());
	for (auto& Item : Results)
	{
		bit.WriteType((Byte)Item);
	}
}

void WasmFile::FuncType::FromBytes(BitReader& bit)
{
	Tag V = 0;
	bit.ReadType(V);

	{
		VectorLength Size = 0;
		WasmFile::ReadLEB128(bit, Size);

		Params.resize(Size);

		for (size_t i = 0; i < Size; i++)
		{
			bit.ReadType(*(Byte*)&Params[i]);
		}
	}
	{
		VectorLength Size = 0;
		WasmFile::ReadLEB128(bit, Size);

		Results.resize(Size);

		for (size_t i = 0; i < Size; i++)
		{
			bit.ReadType(*(Byte*)&Results[i]);
		}
	}
}

void WasmFile::Code::ToBytes(BitMaker& bit) const
{


	BitMaker tep;

	{
		WasmFile::WriteLEB128(tep, (VectorLength)locals.size());

		for (auto& Item : locals)
		{
			Item.ToBytes(tep);
		}
	}
	{

		for (auto& Item : Ins)
		{
			Item.ToBytes(tep);
		}
	}

	varU32 Size = tep.size();
	WriteLEB128(bit, Size);
	bit.WriteBytes(tep.data(), tep.size());
}

void WasmFile::Code::FromBytes(BitReader& bit)
{
	varU32 Size = 0;
	ReadLEB128(bit, Size);


	{
		varU32 Count = 0;
		WasmFile::ReadLEB128(bit, Count);
		locals.resize(Count);

		for (size_t i = 0; i < Count; i++)
		{
			locals[i].FromBytes(bit);
		}
	}
	{
		Expr val = Expr();

		do
		{
			val = Expr();

			val.FromBytes(bit);

			Ins.push_back(std::move(val));


		} while (val.InsType != Expr::Ins::end);

	}
}

void WasmFile::CodeSection::ToBytes(BitMaker& bit) const
{
	bit.WriteType(SectionNumber);

	BitMaker tep;

	{
		WasmFile::WriteLEB128(tep, (VectorLength)code.size());

		for (auto& Item : code)
		{
			Item.ToBytes(tep);
		}
	}

	auto& bytes = tep.Get_Bytes();
	WasmFile::WriteLEB128(bit, (varU32)bytes.size());
	bit.WriteBytes(bytes.data(), bytes.size());
}

void WasmFile::CodeSection::FromBytes(BitReader& bit)
{
	varU32 L = 0;
	WasmFile::ReadLEB128(bit, L);

	auto bytes = bit.ReadBytesAsSpan(L);
	BitReader v;
	v.SetBytes(bytes.Data(), bytes.Size());

	{
		VectorLength typeslength = 0;
		WasmFile::ReadLEB128(v, typeslength);

		code.resize(typeslength);

		for (auto& Item : code)
		{
			Item.FromBytes(v);
		}
	}
}

void WasmFile::Section::ToBytes(BitMaker& bit) const
{
	if (auto V = Type.Get_If<TypeSection>())
	{
		V->ToBytes(bit);
	}
	else if (auto V = Type.Get_If<FuncSection>())
	{
		V->ToBytes(bit);
	}
	else if (auto V = Type.Get_If<CodeSection>())
	{
		V->ToBytes(bit);
	}
	else if (auto V = Type.Get_If<ExportSection>())
	{
		V->ToBytes(bit);
	}
	else if (auto V = Type.Get_If<MemSection>())
	{
		V->ToBytes(bit);
	}
	else
	{
		UCodeLangUnreachable();
	}
}

void WasmFile::Section::FromBytes(BitReader& bit)
{
	SectionID sectionNumber;
	bit.ReadType(sectionNumber);

	if (sectionNumber == TypeSection::SectionNumber)
	{
		TypeSection r;
		r.FromBytes(bit);
		Type = std::move(r);
	}
	else if (sectionNumber == FuncSection::SectionNumber)
	{
		FuncSection r;
		r.FromBytes(bit);

		Type = std::move(r);
	}
	else if (sectionNumber == CodeSection::SectionNumber)
	{
		CodeSection r;
		r.FromBytes(bit);

		Type = std::move(r);
	}
	else if (sectionNumber == ExportSection::SectionNumber)
	{
		ExportSection r;
		r.FromBytes(bit);

		Type = std::move(r);
	}
	else if (sectionNumber == MemSection::SectionNumber)
	{
		MemSection r;
		r.FromBytes(bit);

		Type = std::move(r);
	}
	else
	{
		UCodeLangUnreachable();
	}
}


void WasmFile::MemSection::ToBytes(BitMaker& bit) const
{
	bit.WriteType(SectionNumber);

	BitMaker tep;

	{
		WasmFile::WriteLEB128(tep, (VectorLength)limits.size());

		for (auto& Item : limits)
		{
			tep.WriteType((Byte)Item.hasmax);
			WasmFile::WriteLEB128(tep, Item.min);

			if (Item.hasmax == WasmFile::MemSection::Limits::HasMax::minAmax) {
				WasmFile::WriteLEB128(tep, Item.max);
			}
		}
	}

	auto& bytes = tep.Get_Bytes();
	WasmFile::WriteLEB128(bit, (varU32)bytes.size());
	bit.WriteBytes(bytes.data(), bytes.size());
}

void WasmFile::MemSection::FromBytes(BitReader& bit)
{
	varU32 L = 0;//SectionNumber
	WasmFile::ReadLEB128(bit, L);

	auto bytes = bit.ReadBytesAsSpan(L);
	BitReader v;
	v.SetBytes(bytes.Data(), bytes.Size());

	{
		VectorLength typeslength = 0;
		WasmFile::ReadLEB128(v, typeslength);

		limits.resize(typeslength);

		for (auto& Item : limits)
		{
			v.ReadType(*(Byte*)&Item.hasmax);
			WasmFile::ReadLEB128(v, Item.min);
			
			if (Item.hasmax == WasmFile::MemSection::Limits::HasMax::minAmax) {
				WasmFile::ReadLEB128(v, Item.max);
			}
		}

	}
}
UCodeLangEnd

#endif


