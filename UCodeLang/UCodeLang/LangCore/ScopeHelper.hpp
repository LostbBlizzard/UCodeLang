#pragma once
#include "LangTypes.hpp"

UCodeLangStart
struct ScopeHelper
{
	static inline const char* _globalAssemblyObject = "[_global]";//where none class objs wiil be.
	static inline const char _ScopeSep = ':';

	String ThisScope;
	inline void AddScope(const String_view& Name)
	{
		GetApendedString(ThisScope,Name);
	};
	

	inline void AddScope(const String& Name) 
	{
		AddScope(String_view(Name));
	};

	static void GetApendedString(String& Out,const String_view& Name)
	{
		if (Out.size())
		{
			Out += _ScopeSep;
		}
		Out += Name;
	}
	
	String GetApendedString(const String_view& Name)const
	{
		String r = ThisScope;
		GetApendedString(r, Name);
		return r;
	}

	String GetApendedString(const String& Name)const
	{
		return GetApendedString((String_view)Name);
	}
	static void ReMoveScope(String& ThisScope)
	{
		for (size_t i = ThisScope.size() - 1; i > 0; i--)
		{
			char C = ThisScope[i];
			if (C == _ScopeSep)
			{
				ThisScope = ThisScope.substr(0, i);
				return;
			}
		}
		return ThisScope.clear();
	};
	void ReMoveScope()
	{
		ReMoveScope(ThisScope);
	};
	static String GetNameFromFullName(String FullName)
	{
		return (String)GetNameFromFullName((String_view)FullName);
	}

	static String_view GetNameFromFullName(String_view FullName)
	{
		for (size_t i = FullName.size() - 1; i > 0; i--)
		{
			char C = FullName[i];
			if (C == _ScopeSep)
			{
				return FullName.substr(0, i);
			}
		}
		return FullName;
	}
};
UCodeLangEnd