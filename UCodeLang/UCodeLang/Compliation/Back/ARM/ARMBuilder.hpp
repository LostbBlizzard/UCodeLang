#pragma once
#include "../NativeCodeBuilder.hpp"
#include "ARMData.hpp"
UCodeLangStart


//resources
//https://www.amd.com/system/files/TechDocs/24592.pdf

class ARMBuilder
{
public:
	using Value8 = NativeCodeBuilder::Value8;
	using Value16 = NativeCodeBuilder::Value16;
	using Value32 = NativeCodeBuilder::Value32;
	using Value64 = NativeCodeBuilder::Value64;

	using ARMInstruction = UInt64;
	//ARM Ins list Here

};
UCodeLangEnd