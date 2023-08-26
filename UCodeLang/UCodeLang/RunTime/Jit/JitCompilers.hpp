#pragma once
#include "UCodeLang/LangCore/LangDef.hpp"

#if UCodeLang_CPUIs_x86_64
#include "UCodeLang/Compliation/Back/x86_64/X86_64JitCompiler.hpp"
#elif UCodeLang_CPUIs_x86
#include "UCodeLang/Compliation/Back/x86/X86JitCompiler.hpp"
#elif UCodeLang_CPUIs_Arm
#include "UCodeLang/Compliation/Back/ARM/ARMJitCompiler.hpp"
#elif UCodeLang_CPUIs_Arm64
#include "UCodeLang/Compliation/Back/ARM64/ARM64JitCompiler.hpp"
#endif // CPUTypes


UCodeLangStart

#if UCodeLang_CPUIs_x86_64
using NativeJitAssembler = X86_64JitCompiler;
#elif UCodeLang_CPUIs_x86
using NativeJitAssembler = X86JitCompiler;
#elif UCodeLang_CPUIs_Arm
using NativeJitAssembler = ARMJitCompiler;
#elif UCodeLang_CPUIs_Arm64
using NativeJitAssembler = ARM64JitCompiler;
#else
using NativeJitAssembler = void*;
#define HasNoSupportforJit 1
#endif // CPUTypes
#define UCodeLang_KeepJitInterpreterFallback 1

#define HasSupportforJit !HasNoSupportforJit 

UCodeLangEnd

