#pragma once
#include "LangTypes.hpp"

UCodeLangStart
struct ScopeHelper
{
	static inline const char* _globalScope = "[_global]";
	static inline const char _ScopeSep = ':';

	String ThisScope;
	inline void AddScope(const String& Name)
	{
		ThisScope += _ScopeSep + Name;
	};
	static void ReMoveScope(String& ThisScope)
	{
		for (size_t i = ThisScope.size() - 1; i > 0; i--)
		{
			char C = ThisScope[i];
			if (C == _ScopeSep)
			{
				ThisScope = ThisScope.substr(0, i);
				break;
			}
		}
	};
	void ReMoveScope()
	{
		ReMoveScope(ThisScope);
	};
};
UCodeLangEnd