#pragma once
#include "RunTimeLangState.hpp"
UCodeLangStart

//Use use these types to pass to and from UCode runtime and CPP.

struct ULangVector
{
	UAddress Pointer = NullAddress;
	UAddress Count = NullAddress;
	UAddress Capacity = NullAddress;
};

bool IsVector(ReflectionTypeInfo& Type, const ClassAssembly& Assembly);

ReflectionTypeInfo Get_VectorBaseType(ReflectionTypeInfo& Type, const ClassAssembly& Assembly);

bool IsOptional(TypedRawReflectionData& Type, const ClassAssembly& Assembly);

bool Get_OptionalBase(TypedRawReflectionData& Type, const ClassAssembly& Assembly);

struct ULangString
{
	UAddress CharPointer = NullAddress;
	UAddress Count = NullAddress;
	UAddress Capacity = NullAddress;
};

UCodeLangEnd