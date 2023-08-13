#pragma once
#include "UCodeLangNameSpace.hpp"

#if defined(_MSC_VER)
#define UCodeLangMSVC 1
#else
#define UCodeLangMSVC 0
#endif

#if defined(__GNUC__)
#define UCodeLangGNUC 1
#else
#define UCodeLangGNUC 0
#endif

#if UCodeLangGNUC
#if __x86_64__ || __ppc64__
#define UCodeLang_64BitSytem 1
#else
#define UCodeLang_64BitSytem 0
#endif

#else

#if _WIN64
#define UCodeLang_64BitSytem 1
#else
#define UCodeLang_64BitSytem 0
#endif

#endif

#define UCodeLang_32BitSytem !UCodeLang_64BitSytem

#if _WIN64 || _WIN32
#define UCodeLang_Platform_Windows 1
#else
#define UCodeLang_Platform_Windows 0
#endif // _WIN64 || _WIN32

#if __gnu_linux__ || __linux__
#define UCodeLang_Platform_Linux 1
#include <endian.h> //For UCodeLang_CPUBIs_BigEndian
#else
#define UCodeLang_Platform_Linux 0
#endif // linux 

#if __APPLE__ && __MACH__
#define UCodeLang_Platform_MacOS 1
#else
#define UCodeLang_Platform_MacOS 0
#endif // linux 

#ifndef UCodeLangDebug

#if UCodeLangDebug
#define UCodeLangDebug 1
#else 
#define UCodeLangDebug 0
#endif // DEBUG

#endif

#if UCodeLangDebug
#define UCodeLangForceinline inline
#else 
#define UCodeLangForceinline __forceinline 
#endif // DEBUG

#define UCodeLangConstexprForceinline constexpr UCodeLangForceinline


#define UCodeLangHasMultipleThreads __STDCPP_THREADS__


#define UCodeLang_CPUIs_x86_64 defined(__x86_64__) || defined(_M_X64)
#define UCodeLang_CPUIs_x86 defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)

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



#if UCodeLangMSVC
#define UCodeLangDEPRECATED __declspec(deprecated)
#elif UCodeLangGNUC
#define UCodeLangDEPRECATED __attribute__((deprecated))
#else
#define UCodeLangDEPRECATED
#endif

#if UCodeLangMSVC
#define UCodeLangAPIExportDLLEXPORT __declspec(dllexport)
#define UCodeLangAPIExportDLLIMPORT __declspec(dllexport)
#endif

#if UCodeLangDebug
#define UCodeLangUnreachable() throw std::exception("bad path");
#else

#if UCodeLangMSVC
#define UCodeLangUnreachable() __assume(0);
#elif UCodeLangGNUC
#define UCodeLangUnreachable() __builtin_unreachable()
#else
#define UCodeLangUnreachable()
#endif

#endif



#ifdef Published
#define UCodeLangThrowException() static_assert(true,"UCodeLang should not be throwing exceptions we have a C-API ");
#else

#if UCodeLangMSVC
#define UCodeLangThrowException(Ex) throw std::exception(Ex);
#else
#define UCodeLangThrowException(Ex) throw std::runtime_error(Ex);
#endif 


#endif

#if Published
#define UCodeLangToDo() static_assert(true,"Add Code Path");
#else
#define UCodeLangToDo() UCodeLangThrowException("Code Path not vaild Is On ToDolist");
#endif

#if UCodeLangDebug
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

