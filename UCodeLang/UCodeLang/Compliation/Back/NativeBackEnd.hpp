#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
#include "UCodeLang/LangCore/LangDef.hpp"
UCodeLangStart

#if UCodeLang_CPUIs_x86_64

#if UCodeLang_Platform_Windows
using NativeULangBackEnd = void*;
using NativePlatformBackEnd = void*;
#else 
#warning "there's no support for jitCompiler for compilation Target"
#endif // DEBUG

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