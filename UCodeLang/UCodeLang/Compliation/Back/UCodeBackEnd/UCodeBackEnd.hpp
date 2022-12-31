#pragma once
#include "../BackEndInterface.hpp"

UCodeLangStart

class UCodeBackEndObject
{
public:
	UCodeBackEndObject();
	~UCodeBackEndObject();


};
class UCodeBackEnd
{
	//You can Reuse some parts of the default backend if needed.
public:
	static void Bind(BackEndInterface& Obj);
	static const BackEndInterface* Get();

	static auto Gen() { return new UCodeBackEndObject(); }
	static auto Delete(UCodeBackEnd* Obj) { delete Obj; }
};
UCodeLangEnd

