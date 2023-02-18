#include "C89Backend.hpp"
UCodeLangStart

C89Backend::C89Backend()
{

}
C89Backend::~C89Backend()
{

}
void C89Backend::Reset()
{
	OutBuffer.clear();
}
void C89Backend::Build(const IRBuilder* Input)
{
	_Input = Input; 
	
	AddTextSignature();
	
	
	//types
	AddBaseTypes();

}

void C89Backend::AddTextSignature()
{
	OutBuffer += "//Made using UCodeLang C89 Backend.\n";
	OutBuffer += "//\n";
	OutBuffer += "//\n";
}

void C89Backend::AddBaseTypes()
{
	OutBuffer += "typedef UInt8 unsigned char;\n";
	OutBuffer += "typedef UInt16 unsigned short;\n";
	OutBuffer += "typedef UInt32 unsigned int;\n";
	OutBuffer += "typedef UInt64 unsigned long long;\n";

	OutBuffer += "typedef SInt8 signed char;\n";
	OutBuffer += "typedef SInt16 signed short;\n";
	OutBuffer += "typedef SInt32 signed int;\n";
	OutBuffer += "typedef SInt32 signed long long;\n";
}

UCodeLangEnd


