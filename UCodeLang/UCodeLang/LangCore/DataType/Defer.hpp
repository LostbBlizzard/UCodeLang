#pragma once
#include <functional>
#include "../LangTypes.hpp"
UCodeLangStart
class Defer
{

public:
	UCodeLangForceinline Defer(const std::function<void()>& Func)
	{
		_ToCall = Func;
	}
	UCodeLangForceinline Defer(std::function<void()>&& Func)
	{
		_ToCall = Func;
	}
	UCodeLangForceinline ~Defer()
	{
		_ToCall();
	}

private:
	std::function<void()> _ToCall;
};
UCodeLangEnd