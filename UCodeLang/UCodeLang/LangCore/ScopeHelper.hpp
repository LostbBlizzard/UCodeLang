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
	static size_t Get_ScopeCount(const String_view& Name)
	{
		size_t r=0;


		for (size_t i = 0; i < Name.size(); i++)
		{
			if (Name[i] == _ScopeSep) { r++; }
		}

		return r;
	}

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
		if (ThisScope.size() == 0) { return; }
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

	static void ReMoveScope(String_view& ThisScope)
	{
		if (ThisScope.size() == 0) { return; }
		for (size_t i = ThisScope.size() - 1; i > 0; i--)
		{
			char C = ThisScope[i];
			if (C == _ScopeSep)
			{
				ThisScope = ThisScope.substr(0, i);
				return;
			}
		}
		ThisScope = "";
	};

	static String_view GetReMoveScope(String_view ThisScope)
	{
		ReMoveScope(ThisScope);
		return ThisScope;
	};
	static String GetReMoveScope(String ThisScope)
	{
		ReMoveScope(ThisScope);
		return ThisScope;
	};

	static String_view GetReMoveScopes(String_view ThisScope,size_t Count)
	{
		for (size_t i = 0; i < Count; i++)
		{
			ReMoveScope(ThisScope);
		}
		
		return ThisScope;
	};
	static String GetReMoveScopes(String ThisScope, size_t Count)
	{
		for (size_t i = 0; i < Count; i++)
		{
			ReMoveScope(ThisScope);
		}
		return ThisScope;
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
		if (FullName.size() == 0) { return FullName; }
		for (size_t i = FullName.size() - 1; i > 0; i--)
		{
			char C = FullName[i];
			if (C == _ScopeSep)
			{
				return FullName.substr(i+1);
			}
		}
		return FullName;
	}
};
UCodeLangEnd