#pragma once
#ifndef UCodeLangNoCompiler

#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
#include "UCodeLang/LangCore/LangDef.hpp"

#if UCodeLang_Platform_Windows
#include "Windows/WindowsBackEnd.hpp"
#elif UCodeLang_Platform_Linux
#include "Linux/LinuxBackEnd.hpp"
#elif UCodeLang_Platform_MacOS
#include "MacOs/MacOSBackEnd.hpp"
#endif

#if UCodeLang_CPUIs_x86_64
#include "x86_64/X86_64UNativeBackEnd.hpp"
#else

#endif // UCodeLang_CPUIs_x86_64

UCodeLangStart

#if UCodeLang_Platform_Windows
using NativePlatformBackEnd = WindowsBackEnd;
#elif UCodeLang_Platform_Linux
using NativePlatformBackEnd = LinuxBackEnd;
#elif UCodeLang_Platform_MacOS 
using NativePlatformBackEnd = MacOSBackEnd;
#else
using NativePlatformBackEnd = void*;
#define UCodeLang_HasNoPlatformBackEndBackEnd 
#endif

#if UCodeLang_CPUIs_x86_64
using NativeULangBackEnd = X86_64UNativeBackEnd;
#else
using NativeULangBackEnd = void*;
#define UCodeLang_HasNoNativeULangBackEnd 
#endif


UCodeLangEnd

#endif