#include "UCodeLang/LangCore/UCodeLangNameSpace.hpp"
#include "UCodeLang/LangCore/LangDef.hpp"


#if UCodeLang_Platform_Windows
#include "Windows/WindowsBackEnd.hpp"
#else 

#endif// UCodeLang_Platform_Windows

#if UCodeLang_CPUIs_x86_64
#include "x86_64/X86_64UNativeBackEnd.hpp"
#else

#endif // UCodeLang_CPUIs_x86_64

UCodeLangStart

#if UCodeLang_Platform_Windows
using NativePlatformBackEnd = WindowsBackEnd;
#else
using NativePlatformBackEnd = void*;
#endif

#if UCodeLang_CPUIs_x86_64
using NativeULangBackEnd = X86_64UNativeBackEnd;
#else
using NativePlatformBackEnd = void*;
#endif


UCodeLangEnd