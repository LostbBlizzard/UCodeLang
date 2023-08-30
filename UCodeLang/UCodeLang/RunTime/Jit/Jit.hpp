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


struct NullJitCalls
{
	UAddress UCodeAddress=NullAddress;
	uintptr_t CPPoffset=0;
	uintptr_t Nativeoffset = 0;
};

#define JitCompilerAPIChecks UCodeLangDebug

#if JitCompilerAPIChecks
#define JitCompilerOverride override
#define JitCompilerVirual virtual
#define JitCompilerPure = 0;
#else
#define JitCompilerOverride 
#define JitCompilerVirual 
#define JitCompilerPure 
#endif

class RunTimeLangState;
class JitCompiler
{
public:

	struct UnLoadedFuncPlaceHolder
	{
		size_t Offset = 0;
	};
	using FuncType = RunTimeLib::CPPCallBack;
	using InterpreterCPPinterface_SetRet = void(*)(void* Input);
	using InterpreterCPPinterface_GetParm = void(*)(void* Input, void* Pointer, size_t ParSize);
	using JitBuildAddress = void(*)(UAddress Input);

	Vector<NullJitCalls> NullCalls;
	UAddress OnUAddressPar = 0;//Should all be move in

	#if JitCompilerAPIChecks 
	JitCompilerVirual void Reset() JitCompilerPure
	JitCompilerVirual bool BuildFunc(Vector<Instruction>& Ins, UAddress funcAddress, Vector<UInt8>& X86Output) JitCompilerPure
	JitCompilerVirual void SubCall(FuncType Value, uintptr_t CPPOffset, void* X64Output) JitCompilerPure;
	#endif

	struct FuncToLink
	{
		size_t CPPOffset = 0;
		UAddress OnUAddress = 0;
	};

	BinaryVectorMap<UAddress, UnLoadedFuncPlaceHolder> FuncsPlaceHolder;
	Vector<FuncToLink> LinkingData;
	InterpreterCPPinterface_SetRet  InterpreterCPPinterface_Set_ReturnPtr = nullptr;
	InterpreterCPPinterface_GetParm InterpreterCPPinterface_Get_Par = nullptr;
	JitBuildAddress BuildAddressPtr = nullptr;
	const ClassMethod* Func = nullptr;
	RunTimeLangState* State = nullptr;
	size_t Out_CppCallOffset = 0;
	size_t Out_NativeCallOffset = 0;
	size_t BufferOffset = 0;
};
UCodeLangEnd