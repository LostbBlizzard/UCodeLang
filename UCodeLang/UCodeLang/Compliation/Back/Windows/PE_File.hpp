#pragma once
#include "UCodeLang/LangCore/BitMaker.hpp"

UCodeLangStart




//windows PortableExecutable
class PE_File
{
	PE_File();
	~PE_File();


	static BytesPtr ToRawBytes(const PE_File* In);
	static bool FromBytes(BytesView Bits, PE_File* Out);
	static bool ToFile(const PE_File* Lib, const Path& path);
	static bool FromFile(PE_File* Lib, const Path& path);

	//
	struct EXECUTABLE_DOS_HEADER
	{
		unsigned short MagicNumber = 0x5A4D;
		unsigned short UsedBytesInTheLastPage;
		unsigned short FileSizeInPages;
		unsigned short NumberOfRelocationItems;
		unsigned short HeaderSizeInParagraphs;
		unsigned short MinimumExtraParagraphs;
		unsigned short MaximumExtraParagraphs;
		unsigned short InitialRelativeSS;
		unsigned short InitialSP;
		unsigned short Checksum;
		unsigned short InitialIP;
		unsigned short InitialRelativeCS;
		unsigned short AddressOfRelocationTable;
		unsigned short OverlayNumber;
		unsigned short Reserved1[4];
		unsigned short OEMID;
		unsigned short OEMInfo;
		unsigned short Reserved2[10];
		unsigned long AddressOfNewEXEHeader;
	};
	struct EXECUTABLE_FILE_HEADER
	{
		unsigned long FileType;
		unsigned short Machine;
		unsigned short NumberOfSections;
		unsigned long TimeDateStamp;
		unsigned long PointToSymbolTable;
		unsigned long NumberOfSymbols;
		unsigned short SizeOfOptionalHeader;
		unsigned short Characteristics;
	};
};

UCodeLangEnd