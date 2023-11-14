#ifndef UCodeLangNoCompiler
#include "IRUBChecker.hpp"

#include "IR.hpp"
#include "../Helpers/CompilationErrors.hpp"
UCodeLangStart

void IRUBChecker::Reset()
{
	auto ErrorsOutput = _Errs;


	this->~IRUBChecker();
	new (this)  IRUBChecker;


	this->_Errs = ErrorsOutput;
}

void IRUBChecker::CheckForUB(const IRBuilder* Input)
{
	_Input = Input;
}
UCodeLangEnd

#endif