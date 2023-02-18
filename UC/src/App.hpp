#pragma once
#include "UCodeLang/Compliation/Compiler.hpp"
#include "UCodeLang/RunTime/UCodeRunTime.hpp"
class App
{
public:
	static int main(int argc, char* argv[]);
};
void ParseLine(UCodeLang::String_view Line);

void TokenCheck(const UCodeLang::String_view& Word1);
