#pragma once
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
#include <UCodeLang/LangCore/BitMaker.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
UCodeLangStart

//made using https://learn.microsoft.com/en-us/windows/win32/debug/pe-format
struct PEFile
{
public:
	enum class MachineType
	{
		ARM = 0x1c0,
		ARM64 = 0xaa64,

		x64 = 0x8664,

		RiscV32 = 0x5032,
		RiscV64 = 0x5064,

	};
	void ToBytes(BitMaker& bit) const;
	bool FromBytes(BitReader& bit);

	static bool FromFile(const Path& path, PEFile& file);
	static bool ToFile(const PEFile& file, const Path& path);

	static bool FromBytes(PEFile& file, const BytesView Bytes);
	static BytesPtr ToBytes(const PEFile& file);
private:

};
UCodeLangEnd