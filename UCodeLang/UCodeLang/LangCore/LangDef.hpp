#pragma once
#include "UCodeLangNameSpace.hpp"

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

#if __gnu_linux__ || __linux__
#define UCodeLang_Platform_Linux 1
#else
#define UCodeLang_Platform_Linux 0
#endif // linux 

#if __APPLE__ && __MACH__
#define UCodeLang_Platform_MacOS 1
#else
#define UCodeLang_Platform_MacOS 0
#endif // linux 

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


#if  UCodeLang_Platform_Windows || UCodeLang_Platform_Linux
#define UCodeLang_CPUBIs_BigEndian 0
#else
#error "I don't know what architecture this is!"
#endif //  UCodeLang_Platform_Windows

#endif

#define UCodeLang_CPUIs_littleEndian !UCodeLang_CPUBIs_BigEndian 

#if defined(_MSC_VER)
#define UCodeLangMSVC 1
#endif

#if defined(UCodeLangMSVC)
#define UCodeLangDEPRECATED __declspec(deprecated)
#else
#define UCodeLangDEPRECATED
#endif

#if UCodeLangMSVC
#define UCodeLangAPIExportDLLEXPORT __declspec(dllexport)
#define UCodeLangAPIExportDLLIMPORT __declspec(dllexport)
#endif

#ifdef DEBUG
#define UCodeLangUnreachable() throw std::exception("bad path");
#else

#if defined(_MSC_VER)
#define UCodeLangUnreachable() __assume(0);
#elif defined(__GNUC__)
#define UCodeLangUnreachable() __builtin_unreachable()
#else
#define UCodeLangUnreachable()
#endif

#endif

#ifdef Published
#define UCodeLangToDo() static_assert(true,"Add Code Path");
#else
#define UCodeLangToDo() throw std::exception("Code Path not vaild Is On ToDolist");
#endif

#ifdef Published
#define UCodeLangThrowException() static_assert(true,"UCodeLang should not be throwing exceptions we have a C-API ");
#else
#define UCodeLangThrowException(Ex) throw std::exception(Ex);
#endif

#ifdef DEBUG
#define UCodeLangAssert(condition) if (!(condition)){UCodeLangThrowException("bad");}
#else
#define UCodeLangAssert(condition)
#endif

#if UCodeLangAPIDLLBuild
#if defined(UCodeLangAPIDLLBuildEXPORT)
# define UCodeLangAPIExport UCodeLangAPIExportDLLEXPORT
#else
#define UCodeLangAPIExport UCodeLangAPIExportDLLIMPORT
#endif
#else
#define UCodeLangAPIExport
#endif


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

