
#ifndef UCodeLangNoCompiler
#include "RegistersManager.hpp"
UCodeLangStart

RegistersManager::RegistersManager()
{
}

RegistersManager::~RegistersManager()
{
}

void RegistersManager::Reset() 
{
	for (size_t i = 0; i < RegisterSize; i++)
	{
		RegisterInfo& Register = Registers[i];
		Register = RegisterInfo();
	}
}
UCodeLangEnd

#endif