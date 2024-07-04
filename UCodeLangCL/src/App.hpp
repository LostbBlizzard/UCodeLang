#pragma once
#include "UCodeLang/Compilation/Compiler.hpp"
#include "UCodeLang/RunTime/UCodeRunTime.hpp"
class App
{
public:
	static int main(int argc, char* argv[]);
};
void ParseLine(UCodeLang::String_view& Line);

bool buildfile(UCodeLang::Path& filetorun, UCodeLang::Compiler& _Compiler);
bool buildfile2(UCodeLang::Path& filetorun, UCodeLang::Compiler& _Compiler, UCodeLang::Optional<UCodeLang::UClib>& outlibOp,bool allwaysoutputfile = false);

void TokenCheck(const UCodeLang::String_view& Word1);
