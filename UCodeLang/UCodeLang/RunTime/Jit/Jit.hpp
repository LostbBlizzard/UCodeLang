#pragma once
#include "LangCore.hpp"

UCodeLangStart
enum class CPUBitSize : UInt8
{
	Bit8,
	Bit16,
	Bit32,
	Bit64,

#ifdef _WIN64
	This_BitSize = Bit64,
#else
	This_BitSize = Bit32,
#endif // 
};
enum class ProcessorArchitecture : UInt8
{
	Null,
	INTEL_x86,
	AMD64,
};
enum class OsType : UInt8
{
	None,
	Win32,
	Win64,

#ifdef _WIN64
	ThisOs = Win64,
#elif _WIN32
	ThisOs = Win32,
#else
	ThisOs = None,
#endif // 
};
struct EnvironmentData
{
	ProcessorArchitecture Architecture = ProcessorArchitecture::Null;
	CPUBitSize _BitSize = CPUBitSize::This_BitSize;
	OsType OpType = OsType::ThisOs;
	size_t PageSize = 0;
	int ProcessorsCount = 0;
};
struct AsmBuffer
{
	void* Data;
	AsmBuffer(const size_t PageSize);
	void SetToExecuteMode();
	~AsmBuffer();
};
void GetCPUData(EnvironmentData& Out);

const EnvironmentData& Get_EnvironmentData();




class Jit
{
};

UCodeLangEnd