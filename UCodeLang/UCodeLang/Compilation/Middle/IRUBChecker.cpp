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


template<typename T>
size_t GetIndex(const Span<T> span, const T* index)
{
	size_t i = index - &span[0];

#if UCodeLangDebug
	if (i > index)
	{
		UCodeLangThrowException("bad index");
	}
#endif
	return i;

}

void IRUBChecker::CheckForUB(const IRBuilder* Input)
{
	_Input = Input;

	for (auto& Item : Input->Funcs)
	{
		auto& block = *Item->Blocks.front();
		FuncInfo p;
		p.parInfo.resize(Item->Pars.size());

		for (auto& Item : block.Instructions)
		{
			if (Item->Type == IRInstructionType::Reassign)
			{

			}
		}

		FuncInfos.AddValue(Item.get(), std::move(p));
	}
}
UCodeLangEnd

#endif