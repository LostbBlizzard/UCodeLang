#pragma once
#include "UCodeLangNameSpace.hpp"
UCodeLangStart


#if _WIN64
#define UCodeLang_64BitSytem 1
#define UCodeLang_32BitSytem 0
#else
#define UCodeLang_64BitSytem 0
#define UCodeLang_32BitSytem 1
#endif

#if _WIN64 || _WIN32
#define UCodeLang_Platform_Windows 1
#else
#define UCodeLang_Platform_Windows 0
#endif // _WIN64 || _WIN32



#ifdef DEBUG
#define CompliationTypeSafety 1
#else 
#define CompliationTypeSafety 1
#endif // DEBUG


#ifdef DEBUG
#define UCodeLangForceinline inline
#else 
#define UCodeLangForceinline __forceinline 
#endif // DEBUG

#define UCodeLangConstexprForceinline constexpr UCodeLangForceinline


#define UCodeLangHasMultipleThreads __STDCPP_THREADS__


#define UCodeLang_CPUIs_x86_64 _M_AMD64
#define UCodeLang_CPUIs_x86 _M_IX86

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
    defined(__BIG_ENDIAN__) || \
    defined(__ARMEB__) || \
    defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)

#define UCodeLang_CPUBIs_BigEndian 1
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
    defined(__LITTLE_ENDIAN__) || \
    defined(__ARMEL__) || \
    defined(__THUMBEL__) || \
    defined(__AARCH64EL__) || \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)

#define UCodeLang_CPUBIs_BigEndian 0
#else


#if  UCodeLang_Platform_Windows
#define UCodeLang_CPUBIs_BigEndian 0
#else
#error "I don't know what architecture this is!"
#endif //  UCodeLang_Platform_Windows

#endif


#define UCodeLang_CPUIs_littleEndian !UCodeLang_CPUBIs_BigEndian 
#define UCodeLangExportSymbol(Namespace)

//Use only in Classes
#define UCodeLangExport

#define UCodeLangImportSymbol(SymbolToImoport)

//use like this bool func(UCodeLangOutPartype(int) OutValue);
//it will become |func[out int OutValue] -> bool;
//make sure you update it
#define UCodeLangOutPartype(Type) Type*


//Excludes enum fields
#define UCodeLangExclude

#define UCodeLangAutoLink(Lib,CppFuncsNameSpace)

UCodeLangEnd