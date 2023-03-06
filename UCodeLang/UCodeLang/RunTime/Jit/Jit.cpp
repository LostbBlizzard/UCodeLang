#include "Jit.hpp"
#include <Windows.h>
UCodeLangStart
void GetCPUData(EnvironmentData& Out)
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);


	switch (sysInfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		Out.Architecture = ProcessorArchitecture::INTEL_x86;
		break;
	case PROCESSOR_ARCHITECTURE_AMD64:
		Out.Architecture = ProcessorArchitecture::AMD64;
		break;
	default:
		throw std::bad_alloc();
		break;
	}


	Out.ProcessorsCount = sysInfo.dwNumberOfProcessors;
	Out.PageSize = sysInfo.dwPageSize;
}

const EnvironmentData& Get_EnvironmentData()
{
	static EnvironmentData Data;
	if (Data.Architecture == ProcessorArchitecture::Null)
	{
		GetCPUData(Data);
	}
	return Data;
}

AsmBuffer::AsmBuffer(const size_t PageSize)
{
	DWORD type = MEM_RESERVE | MEM_COMMIT;
	Data = VirtualAlloc(NULL, PageSize, type, PAGE_READWRITE);
}
AsmBuffer::AsmBuffer(const Byte* Asm, const size_t Size)
{
	DWORD type = MEM_RESERVE | MEM_COMMIT;
	Data = VirtualAlloc(NULL, Size, type, PAGE_READWRITE);

	memcpy(Data, Asm, Size);
}

void AsmBuffer::SetToExecuteMode()
{
	DWORD old;
	VirtualProtect(Data, sizeof(Data), PAGE_EXECUTE_READ, &old);
}

void AsmBuffer::SetToReadWriteMode()
{
	DWORD old;
	VirtualProtect(Data, sizeof(Data), PAGE_READWRITE, &old);
}

AsmBuffer::~AsmBuffer()
{
	VirtualFree(Data, 0, MEM_RELEASE);
}

class AMD64_Set
{
public:
	static constexpr UINT8 Mov = 0x48;
	static constexpr UINT8 Ret = 0xC3;
	static constexpr UINT8 rax = 0x89;
	static constexpr UINT8 rdi = 0xf8;
};
UCodeLangEnd
