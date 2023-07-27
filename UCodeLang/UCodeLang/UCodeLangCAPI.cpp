#include "UCodeLangCAPI.h"
#include "UCodeLang.hpp"


//Casts
UCodeLang::Compiler* As(UCodeLangCAPI_Compiler* Value)
{
	return (UCodeLang::Compiler*)Value;
}
const UCodeLang::Compiler* As(UCodeLangCAPI_Const_Compiler* Value)
{
	return (const UCodeLang::Compiler*)Value;
}

void UCodeLangAPIExport UCodeLangCAPI_Init()
{
	//make all static varables
}

void UCodeLangAPIExport UCodeLangCAPI_DeInit()
{
	//free all static varables
}

//Funcs
UCodeLangCAPI_Compiler* UCodeLangAPIExport UCodeLangCAPI_New_Compiler()
{
	return As(new UCodeLang::Compiler());
}

void  UCodeLangAPIExport UCodeLangCAPI_Destroy_Compiler(UCodeLangCAPI_Compiler* Value)
{
	delete Value;
}

void UCodeLangAPIExport UCodeLangCAPI_Compiler_Build(UCodeLangCAPI_Compiler* This, UCodeLangCAPI_CharSpan String, UCodeLangCAPI_CompilerOutput* Output)
{

}
