#pragma once
#include "UCodeLangNameSpace.hpp"
UCodeLangStart


#ifdef _WIN64
#define UCodeLang_64BitSytem 1
#define UCodeLang_32BitSytem 0
#else
#define UCodeLang_64BitSytem 0
#define UCodeLang_32BitSytem 1

static_assert(false,"UCode doesn't support 32 Bit")
#endif

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

UCodeLangEnd