#pragma once
#include "IOLink.hpp"
#include "Interpreters/Interpreter.hpp"
UCodeLangStart

template<typename T, typename... Pars>
using NativeCall = RunTimeLib::NativeCall<T, Pars...>;


void IOLink::Link(RunTimeLib& lib, const LinkSettings& settings)
{
	if (settings.AddCInAndOut) 
	{
		lib.Add_CPPCall("putchar", [](InterpreterCPPinterface& Input)
			{
				int Par0 = Input.GetParameter<int>();
				Input.Set_Return(putchar(Par0));
			}, putchar);

		lib.Add_CPPCall("putspan", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<const char*>();
				auto Par1 = Input.GetParameter<size_t>();

				Input.Set_Return(fwrite(Par0, 1, Par1, stdout));
			}, (NativeCall<size_t, const char*, size_t>)[](const char* pointer, size_t size)
				{
					return fwrite(pointer, 1, size, stdout);
				});
	}
}

UCodeLangEnd