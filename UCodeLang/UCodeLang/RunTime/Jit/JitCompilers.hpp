#pragma once
#include "UCodeLang/LangCore/LangDef.hpp"

#if UCodeLang_CPUIs_x86_64
#include "UCodeLang/Compilation/Back/x86_64/X86_64JitCompiler.hpp"
#elif UCodeLang_CPUIs_x86
#include "UCodeLang/Compilation/Back/x86/X86JitCompiler.hpp"
#elif UCodeLang_CPUIs_Arm
#include "UCodeLang/Compilation/Back/ARM/ARMJitCompiler.hpp"
#elif UCodeLang_CPUIs_Arm64
#include "UCodeLang/Compilation/Back/ARM64/ARM64JitCompiler.hpp"
#endif // CPUTypes


UCodeLangStart



#define IsOnNoJitPlatform  UCodeLang_Platform_MacOS || UCodeLang_Platform_IPHONE

#if UCodeLang_CPUIs_x86_64
using NativeJitAssembler = X86_64JitCompiler;

#if !UCodeLangExperimental || IsOnNoJitPlatform
#define HasNoSupportforJit 1
#endif
#elif UCodeLang_CPUIs_x86
using NativeJitAssembler = X86JitCompiler;

#if !UCodeLangExperimental || IsOnNoJitPlatform
#define HasNoSupportforJit 1
#endif
#elif UCodeLang_CPUIs_Arm
using NativeJitAssembler = ARMJitCompiler;

#if !UCodeLangExperimental || IsOnNoJitPlatform
#define HasNoSupportforJit 1
#endif
#elif UCodeLang_CPUIs_Arm64
using NativeJitAssembler = ARM64JitCompiler;

#if !UCodeLangExperimental || IsOnNoJitPlatform
#define HasNoSupportforJit 1
#endif
#else
using NativeJitAssembler = void*;
#define HasNoSupportforJit 1
#endif // CPUTypes
#define UCodeLang_KeepJitInterpreterFallback 1

#define HasSupportforJit !HasNoSupportforJit 

UCodeLangEnd

