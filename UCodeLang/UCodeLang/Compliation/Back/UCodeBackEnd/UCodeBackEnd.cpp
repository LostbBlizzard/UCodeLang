#include "UCodeBackEnd.hpp"
UCodeLangStart
void UCodeBackEnd::Bind(BackEndInterface& Obj)
{
	Obj
	.Set_GenBackEnd(Gen)
	.Set_DeleteBackEnd(Delete)
		
		;
}
const BackEndInterface* UCodeBackEnd::Get()
{
	static bool IsMade = false;
	static BackEndInterface BackEnd;
	if (!IsMade)
	{
		Bind(BackEnd);
		IsMade = true;
	}

	return &BackEnd;
}

UCodeBackEndObject::UCodeBackEndObject()
{
}

UCodeBackEndObject::~UCodeBackEndObject()
{
}

UCodeLangEnd

