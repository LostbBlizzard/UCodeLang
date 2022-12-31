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

	//Just Copy and paste for your backend
	using BackEnd = UCodeBackEndObject;
	static auto Gen() { return new BackEnd(); }
	static auto Delete(BackEnd* Obj) { delete Obj; }
};
UCodeLangEnd

