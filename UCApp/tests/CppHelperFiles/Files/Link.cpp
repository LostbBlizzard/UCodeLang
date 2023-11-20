#include "UCodeLang/UCodeLang.hpp"
//include Made By UCodeAutoLink
#include "Func.hpp"
#include "Header.h"
//UCodeAutoLink include End

void LinkCPPFiles(UCodeLang::UClib& lib)
{
	using namespace CppLinkTest;
	UCodeLangAutoLink(lib, CppLinkTest);
}
