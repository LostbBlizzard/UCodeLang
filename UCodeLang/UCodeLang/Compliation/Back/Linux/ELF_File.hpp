#pragma once
#include <UCodeLang/LangCore/BitMaker.hpp>
#include <UCodeLang/LangCore/LangTypes.hpp>
UCodeLangStart

//made using https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
struct ELFFile
{
public:
	enum class Classformat :Byte
	{
		bit32 = 1,
		bit64 = 2,
	};
	enum class Data :Byte
	{
		Little = 1,
		Big = 2,
	};
	enum class OSAbi : Byte
	{
		SystemV = 0x00,
		HPUX = 0x01,
		NetBSD = 0x02,
		Linux = 0x03,
	};
	enum class Type :UInt16
	{
		//Unknown
		None = 0x00,
		//Relocatable file.
		Relocatable =0x01,
		//Executable file.
		Executable = 0x02,
		//Shared object.
		Shared =0x03,
		//Core file.
		Core =0x04,
	};
	enum class Machine :UInt16
	{
		Any = 0x00,
		AMD_X86 =0x03,
		Arm = 0x28,
		AMD_X8664 = 0x3E,
		Arm64 = 0xB7,
		RISCV = 0xF3,
	};
	void ToBytes(BitMaker& bit) const;
	bool FromBytes(BitReader& bit);


	Classformat format = Classformat::bit64;
	Data data = Data::Little;
	Byte version = 1;
	OSAbi Abi = OSAbi::Linux;
	Type type = Type::Executable;

	//e_ident[EI_ABIVERSION]
	Machine machine = Machine::Any;

	UInt64 entry=0;
	UInt64 ProgramheaderTableoffset = 0;
	UInt64 SectionheaderTableoffset = 0;

	//Interpretation of this field depends on the target architecture.
	UInt32 flags=0;
	//TODO e_ehsize
private:
};

UCodeLangEnd
