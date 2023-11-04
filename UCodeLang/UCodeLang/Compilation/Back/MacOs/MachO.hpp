#pragma once
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
#include <UCodeLang/LangCore/BitMaker.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
UCodeLangStart

class MachO
{
public:
	enum class CPUtype
	{
		x86 = 0x00000007,
		Arm = 0x0000000C,
	};


	void ToBytes(BitMaker& bit) const;
	bool FromBytes(BitReader& bit);

	static bool FromFile(const Path& path, MachO& file);
	static bool ToFile(const MachO& file, const Path& path);

	static bool FromBytes(MachO& file, const BytesView Bytes);
	static BytesPtr ToBytes(const MachO& file);
private:
};

UCodeLangEnd

