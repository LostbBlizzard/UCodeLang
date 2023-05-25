#include "DependencyFile.hpp"
#include <fstream>

UCodeLangStart
BytesPtr DependencyFile::ToRawBytes(const DependencyFile* Lib)
{
	BitMaker bits;
	bits.WriteType((BitMaker::SizeAsBits)Lib->Files.size());

	for (auto& Item : Lib->Files)
	{
		Item.ToBytes(bits);
	}


	bits.WriteType((BitMaker::SizeAsBits)Lib->ExternalFiles.size());

	for (auto& Item : Lib->ExternalFiles)
	{
		Item.ToBytes(bits);
	}

	//

	BytesPtr V;
	V.Bytes.reset(new Byte[bits.size()]);
	std::memcpy(V.Bytes.get(), bits.data(), bits.size());
	V.Size = bits.size();
    return V;
}

bool DependencyFile::FromBytes(DependencyFile* Lib, const BytesView& Data)
{
	BitReader bits;
	bits.SetBytes(Data.Bytes, Data.Size);

	union
	{
		BitMaker::SizeAsBits BitSize = 0;
		size_t Size;
	};
	bits.ReadType(BitSize, BitSize);
	Size = BitSize;

	Lib->Files.resize(Size);
	for (size_t i = 0; i < Size; i++)
	{
		FileInfo::FromBytes(bits, Lib->Files[i]);
	}



	BitSize = 0;
	bits.ReadType(BitSize, BitSize);
	Size = BitSize;

	Lib->ExternalFiles.resize(Size);
	for (size_t i = 0; i < Size; i++)
	{
		ExternalFileInfo::FromBytes(bits, Lib->ExternalFiles[i]);
	}

    return true;
}

bool DependencyFile::ToFile(const DependencyFile* Lib, const Path& path)
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

bool DependencyFile::FromFile(DependencyFile* Lib, const Path& path)
{
	std::ifstream File(path, std::ios::binary);
	if (File.is_open())
	{
		BytesPtr Bits;
		File.seekg(0, File.end);
		Bits.Size = File.tellg();
		File.seekg(0, File.beg);
		Bits.Bytes = std::make_unique<Byte[]>(Bits.Size);

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
DependencyFile::FileInfo* DependencyFile::Get_Info(const Path& Path)
{
	for (auto& Item : Files)
	{
		if (Item.FilePath == Path)
		{
			return &Item;
		}
	}
	return nullptr;
}

DependencyFile::ExternalFileInfo* DependencyFile::Get_ExternalFile_Info(const Path& Path)
{
	for (auto& Item : ExternalFiles)
	{
		if (Item.FullFilePath == Path)
		{
			return &Item;
		}
	}
	return nullptr;
}
void DependencyFile::FileInfo::ToBytes(BitMaker& Output)const
{

	Output.WriteType(FilePath.generic_string());
	//
	Output.WriteType(FileLastUpdated);
	Output.WriteType(FileSize);
	Output.WriteType(FileHash);
	//


	Output.WriteType((BitMaker::SizeAsBits)Dependencies.size());
	for (auto& Item : Dependencies)
	{
		Output.WriteType(Item.generic_string());
	}
}

void DependencyFile::FileInfo::FromBytes(BitReader& Input, FileInfo& Out)
{

	String TepS;
	Input.ReadType(TepS,TepS);
	Out.FilePath = TepS;

	Input.ReadType(Out.FileLastUpdated, Out.FileLastUpdated);
	Input.ReadType(Out.FileSize, Out.FileSize);
	Input.ReadType(Out.FileHash, Out.FileHash);


	union 
	{
		BitMaker::SizeAsBits BitSize=0;
		size_t Size;
	};
	Input.ReadType(BitSize, BitSize);
	Size = BitSize;

	for (size_t i = 0; i < Size; i++)
	{
		TepS = "";
		Input.ReadType(TepS, TepS);
		Out.Dependencies.emplace_back(Path(TepS));
	}
}

void DependencyFile::ExternalFileInfo::ToBytes(BitMaker& Output) const
{
	Output.WriteType(FullFilePath.generic_string());
	//
	Output.WriteType(FileLastUpdated);
	Output.WriteType(FileSize);
	Output.WriteType(FileHash);
	//
}

void DependencyFile::ExternalFileInfo::FromBytes(BitReader& Input, ExternalFileInfo& Out)
{
	String TepS;
	Input.ReadType(TepS, TepS);
	Out.FullFilePath = TepS;

	Input.ReadType(Out.FileLastUpdated, Out.FileLastUpdated);
	Input.ReadType(Out.FileSize, Out.FileSize);
	Input.ReadType(Out.FileHash, Out.FileHash);
}

UCodeLangEnd
