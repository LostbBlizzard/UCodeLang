#include "ScopeHelper.hpp"


UCodeLangStart
void ScopeHelper::ReMoveScope(String_view& ThisScope)
{
	size_t scope = 0;
	if (ThisScope.size() == 0) { return; }
	for (size_t i = ThisScope.size() - 1; i > 0; i--)
	{
		char C = ThisScope[i];
		if (C == _ScopeSep && scope == 0)
		{
			ThisScope = ThisScope.substr(0, i);
			return;
		}
		else if (C == '>')
		{
			scope++;
		}
		else if (C == '<')
		{
			scope--;
		}
		else if (C == '-' && ThisScope.size() > i +1)//because of funcion_pointors [...] -> RetType
		{
			auto lastc = ThisScope[i + 1];
			
			if (lastc == '>') 
			{
				scope--;
			}
		}
	}
	ThisScope = "";
}

String_view ScopeHelper::GetNameFromFullName(String_view FullName)
{
	size_t scope = 0;
	if (FullName.size() == 0) { return FullName; }
	for (size_t i = FullName.size() - 1; i > 0; i--)
	{
		char C = FullName[i];
		if (C == _ScopeSep && scope == 0)
		{
			return FullName.substr(i + 1);
		}
		else if (C== '>')
		{
			scope++;
		}
		else if (C == '<')
		{
			scope--;
		}
	}
	return FullName;
}
UCodeLangEnd