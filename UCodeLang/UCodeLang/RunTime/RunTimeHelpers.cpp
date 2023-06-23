#include "RunTimeHelpers.hpp"
UCodeLangStart

bool IsVector(ReflectionTypeInfo& Type, const ClassAssembly& Assembly)
{
	return false;

}

ReflectionTypeInfo Get_VectorBaseType(ReflectionTypeInfo& Type, const ClassAssembly& Assembly)
{
#ifdef DEBUG
	if (!IsVector(Type, Assembly)) { throw std::exception("Bad Type"); }
#endif // DEBUG


	return {};
}

bool IsOptional(TypedRawReflectionData& Type, const ClassAssembly& Assembly)
{
	return false;
}

bool Get_OptionalBase(TypedRawReflectionData& Type, const ClassAssembly& Assembly)
{
	return false;
}

UCodeLangEnd