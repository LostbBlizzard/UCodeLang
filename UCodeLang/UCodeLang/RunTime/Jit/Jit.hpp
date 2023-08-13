#pragma once

#include "UCodeLang/LangCore.hpp"
#include "UCodeLang/RunTime/RunTimeLib.hpp"
UCodeLangStart
enum class CPUBitSize : UInt8
{
	Bit8,
	Bit16,
	Bit32,
	Bit64,

#if UCodeLang_64BitSytem
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
	Linux32,
	Linux64,

#if UCodeLang_Platform_Windows
	#if UCodeLang_64BitSytem
	ThisOs = Win64,
	#else
	ThisOs = Win32,
	#endif
#elif UCodeLang_Platform_Linux
    #if UCodeLang_64BitSytem
	ThisOs = Linux32,
	#else
	ThisOs = Linux64,
	#endif
#else
ThisOs = None,
#error "I Dont Know What OS Im runing on"
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
	AsmBuffer():Data(nullptr){}
	AsmBuffer(const size_t PageSize);
	AsmBuffer(const Byte* Asm,const size_t Size);
	AsmBuffer(AsmBuffer&& Other);
	AsmBuffer& operator=(AsmBuffer&& Other);
	void MemFree();
	void SetToExecuteMode();
	void SetToReadWriteMode();

	void Alloc(const Byte* Asm, const size_t Size);
	void Alloc(const size_t Size);
	~AsmBuffer();
};
void GetCPUData(EnvironmentData& Out);

const EnvironmentData& Get_EnvironmentData();

struct JitInfo
{
	using FuncType = RunTimeLib::CPPCallBack;
};

struct NullJitCalls
{
	UAddress UCodeAddress=NullAddress;
	uintptr_t CPPoffset=0;
	uintptr_t Nativeoffset = 0;
};

UCodeLangEnd