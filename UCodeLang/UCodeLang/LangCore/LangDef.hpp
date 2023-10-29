#pragma once

#ifdef __cplusplus
#include "UCodeLangNameSpace.hpp"
#endif


#define UCodeLangMajorVersion 0
#define UCodeLangMinorVersion 2
#define UCodeLangPatchVersion 3

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

#if defined(__clang__)
#define UCodeLangClang 1
#else
#define UCodeLangClang 0
#endif


#if defined(__EMSCRIPTEN__)
#define UCodeLangEmscripten 1
#else
#define UCodeLangEmscripten 0
#endif



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
#endif // MacOs


#if __APPLE__ 
#include "TargetConditionals.h"

#if TARGET_OS_IPHONE
#define UCodeLang_Platform_IPHONE 1
#else 
#define UCodeLang_Platform_IPHONE 0
#endif 

#else
#define UCodeLang_Platform_IPHONE 0
#endif // MacOs

#if __ANDROID__
#define UCodeLang_Platform_ANDROID 1
#else
#define UCodeLang_Platform_ANDROID 0
#endif // ANDROID

#if  defined(__wasm32__) || defined(__wasm64__)
#define UCodeLang_Platform_Wasm 1
#else
#define UCodeLang_Platform_Wasm 0
#endif // Wasm 

#if defined(__wasm32__)
#define UCodeLang_Platform_Wasm32 1
#else
#define UCodeLang_Platform_Wasm32 0
#endif // wasm32

#if defined(__wasm64__)
#define UCodeLang_Platform_Wasm64 1
#else
#define UCodeLang_Platform_Wasm64 0
#endif // wasm64


#if defined(__unix__) || (__APPLE__ && __MACH__)
#define UCodeLang_Platform_Posix 1
#else
#define UCodeLang_Platform_Posix 0
#endif // MacOs


#ifndef UCodeLangDebug
#if Debug
#define UCodeLangDebug 1
#else 
#define UCodeLangDebug 0
#endif // DEBUG
#endif

#if UCodeLang_Platform_Wasm64 || defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)
#define UCodeLang_64BitSytem 1
#else
#define UCodeLang_64BitSytem 0
#endif




#define UCodeLang_32BitSytem !UCodeLang_64BitSytem

#ifndef UCodeLangExperimental
#define UCodeLangExperimental 0
#endif

#if UCodeLangDebug
#define UCodeLangForceinline inline
#else 

#if UCodeLangMSVC
#define UCodeLangForceinline __forceinline 
#elif UCodeLangGNUC
#define UCodeLangForceinline __attribute__((always_inline))
#else
#define UCodeLangForceinline inline
#endif

#endif // DEBUG


#if UCodeLangMSVC
#define UCodeLangNoDiscard [[nodiscard]]
#elif UCodeLangGNUC
#define UCodeLangNoDiscard __attribute__((__warn_unused_result__))
#else

#define UCodeLangNoDiscard [[nodiscard]]
#endif


#define UCodeLangConstexprForceinline constexpr UCodeLangForceinline


#define UCodeLangHasMultipleThreads __STDCPP_THREADS__

#if  defined(__x86_64__) || defined(_M_X64)
#define UCodeLang_CPUIs_x86_64 1
#else
#define UCodeLang_CPUIs_x86_64 0
#endif

#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define UCodeLang_CPUIs_x86 1
#else
#define UCodeLang_CPUIs_x86 0
#endif

#if defined(__arm__)
#define UCodeLang_CPUIs_Arm 1
#else
#define UCodeLang_CPUIs_Arm 0
#endif

#if defined(__aarch64__)
#define UCodeLang_CPUIs_Arm64 1
#else
#define UCodeLang_CPUIs_Arm64 0
#endif


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


#ifdef PUBLISHED

#define UCodeLangThrowException(Ex) static_assert(true,"UCodeLang should not be throwing exceptions we have a C-API ");

#else

#if UCodeLangMSVC
#define UCodeLangThrowException(Ex) throw std::exception(Ex);
#else
#define UCodeLangThrowException(Ex) throw std::runtime_error(Ex);
#endif 

#endif


#if UCodeLangDebug

#if UCodeLangMSVC
#define UCodeLangBreakPoint() __debugbreak()
#else
#define UCodeLangBreakPoint() __builtin_trap()
#endif 

#endif

#if UCodeLangDebug
#define UCodeLangUnreachable() UCodeLangBreakPoint();
#else



#if UCodeLangMSVC
#define UCodeLangUnreachable() __assume(0)
#elif UCodeLangGNUC
#define UCodeLangUnreachable() __builtin_unreachable()
#else
#define UCodeLangUnreachable()  __builtin_unreachable()
#endif

#endif


#if PUBLISHED
#define UCodeLangToDo() static_assert(true,"Add Code Path");
#else
#define UCodeLangToDo() UCodeLangThrowException("Code Path not vaild Is On ToDolist");
#endif

#if UCodeLangDebug
#define UCodeLangAssert(condition) if (!(condition)){UCodeLangBreakPoint();}
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


#if UCodeLangMSVC 
#define UCodeLangAPI 
#else
#define UCodeLangAPI 
#endif

//Use this To Export Class,Enum,Funcions to .uc 
//using UCodeAnalyzer/Preprocessors/CppHelper ParseCppfileAndOutULang

#define UCodeLangExportSymbol(Namespace)


//Use only in side of Classes
#define UCodeLangExport

#define UCodeLangImportSymbol(SymbolToImoport)

//use like this bool func(UCodeLangOutPartype(int) OutValue);
//it will become |func[out int OutValue] -> bool;
//make sure you update it
#define UCodeLangOutPartype(Type) Type*

//Adds the text to the .uc
#define UCodeLangEmbed(Txt) 

#define UCodeLangExportTrait struct

//Excludes enum fields
#define UCodeLangExclude

//Use this To Link All the Cpp Calls to a RuntimeLib
#define UCodeLangAutoLink(Lib,CppFuncsNameSpace)

