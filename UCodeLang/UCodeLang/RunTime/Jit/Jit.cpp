#include "Jit.hpp"

#if UCodeLang_Platform_Windows
#include <Windows.h>
#elif UCodeLang_Platform_Posix
#include <sys/mman.h>
#endif
#include <cstring>
UCodeLangStart
void GetCPUData(EnvironmentData& Out)
{
	#if UCodeLang_Platform_Windows
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
		UCodeLangUnreachable();
		break;
	}


	Out.ProcessorsCount = sysInfo.dwNumberOfProcessors;
	Out.PageSize = sysInfo.dwPageSize;
	#endif
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
	Alloc(PageSize);
}
AsmBuffer::AsmBuffer(const Byte* Asm, const size_t Size)
{
	Alloc(Asm,Size);
}

AsmBuffer::AsmBuffer(AsmBuffer&& Other)
	:Data(Other.Data)
{
	Other.Data = nullptr;
}
AsmBuffer& AsmBuffer::operator=(AsmBuffer&& Other)
{
	if (Data)
	{
		MemFree();
	}
	Data = Other.Data;

	Other.Data = nullptr;
	return *this;
}

void AsmBuffer::MemFree()
{
#if  UCodeLang_Platform_Windows
	VirtualFree(Data, 0, MEM_RELEASE);
#endif
}

void AsmBuffer::SetToExecuteMode()
{
	#if  UCodeLang_Platform_Windows
	DWORD old;
	VirtualProtect(Data, sizeof(Data), PAGE_EXECUTE_READ, &old);
	#elif UCodeLang_Platform_Linux
	mmap(Data, sizeof(Data), PROT_EXEC,MAP_PRIVATE | MAP_ANONYMOUS ,-1, 0);
	#endif
}

void AsmBuffer::SetToReadWriteMode()
{
	#if  UCodeLang_Platform_Windows
	DWORD old;
	VirtualProtect(Data, sizeof(Data), PAGE_READWRITE, &old);
	#elif UCodeLang_Platform_Posix
	mmap(Data, sizeof(Data), PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS ,-1, 0);
	#endif
}

void AsmBuffer::Alloc(const Byte* Asm, const size_t Size)
{
	Alloc(Size);
	memcpy(Data, Asm, Size);
}
void AsmBuffer::Alloc(const size_t Size)
{
	if (Data)
	{
		MemFree();
	}

	#if UCodeLang_Platform_Windows
	DWORD type = MEM_RESERVE | MEM_COMMIT;
	Data = VirtualAlloc(NULL, Size, type, PAGE_READWRITE);
	#elif UUCodeLang_Platform_Posix
	Data = mmap(nullptr,Size, PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS ,-1, 0);
	#endif
	
	#if UCodeLang_CPUIs_x86_64 || UCodeLang_CPUIs_x86
	memset(Data, 0xcc, Size);
	#endif // debug on hit ins


}

AsmBuffer::~AsmBuffer()
{
	if (Data) 
	{
		MemFree();
	}
}

UCodeLangEnd
