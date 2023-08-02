#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
#include "UCodeLang/LangCore/LangDef.hpp"

#if UCodeLang_CPUIs_x86_64
#if UCodeLang_Platform_Windows
#include "x86_64/X86_64JitCompiler.hpp"
#include "x86_64/X86_64UNativeBackEnd.hpp"
#include "Windows/X86_64PlatformWindows.hpp"
#else 
#endif // UCodeLang_Platform_Windows
#endif // UCodeLang_CPUIs_x86_64

UCodeLangStart

#if UCodeLang_CPUIs_x86_64

#if UCodeLang_Platform_Windows
using NativeULangBackEnd = X86_64UNativeBackEnd;
using NativeJitBackEnd = X86_64JitCompiler;
using NativePlatformBackEnd = X86_64PlatformWindows;
#else 
#endif // UCodeLang_Platform_Windows

#elif UCodeLang_CPUIs_x86

#if UCodeLang_Platform_Windows
using NativeULangBackEnd = X86JitCompiler;
using NativePlatformBackEnd = void*;
#else 
#warning "there's no support for jitCompiler for compilation Target"
#endif // DEBUG

#else
using NativeJitAssembler = void*;
#warning "there's no support for jitCompiler for compilation Target"
#endif

UCodeLangEnd