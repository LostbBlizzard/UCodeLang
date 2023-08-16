#pragma once
#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
#include "UCodeLang/Compliation/Back/x86/X86JitCompiler.hpp"
#include "UCodeLang/Compliation/Back/x86_64/X86_64JitCompiler.hpp"
UCodeLangStart

#if UCodeLang_CPUIs_x86_64
using NativeJitAssembler = X86_64JitCompiler;
#elif UCodeLang_CPUIs_x86
using NativeJitAssembler = X86JitCompiler;
#define HasNoSupportforJit 1
#else
using NativeJitAssembler = void*;
#define HasNoSupportforJit 1
#endif // CPUTypes
#define UCodeLang_KeepJitInterpreterFallback 1

#define HasSupportforJit !HasNoSupportforJit 

UCodeLangEnd

