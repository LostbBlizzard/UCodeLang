#include <fstream>
#include <iostream>
#include "../tests/Test.hpp"
#include "TestingGrounds.hpp"
int main(int argc, char* argv[])
{
	for (size_t i = 0; i < argc; i++)
	{
		char* Item = argv[i];
		if (UCodeLang::String_view(Item) == "--RunTests")
		{ 
			return ULangTest::RunTests();
		}
	}

	TestingGround();

}
