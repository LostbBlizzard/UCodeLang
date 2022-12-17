#pragma once
#include "UCodeLang/LangCore/LangDef.hpp"



#ifdef UCodeLangHasIsCPU_AMD64
#include "Amd64Assembler.hpp"

UCodeLangStart
using NativeAssembler = Amd64Assembler;
#else

UCodeLangStart
using NativeAssembler = void;
#endif // UCodeLangHasIsCPU_AMD64


UCodeLangEnd
