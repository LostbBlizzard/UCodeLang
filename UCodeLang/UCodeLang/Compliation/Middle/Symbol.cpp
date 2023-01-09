#include "Symbol.hpp"


UCodeLangStart

void SymbolTable::RemovePopUseing(size_t Index)
{

}

Vector<Symbol*>& SymbolTable::GetSymbolsWithName(const String_view& Name, SymbolType Type)
{
	auto& r = GetSymbolsWithName(Name);


	return r;
}
void SymbolTable::GetSymbolsInNameSpace(const String_view& NameSpace, const String_view& Name, Vector<Symbol*>& Output)
{
	String TepScope = _Scope.ThisScope;
	String TepNameSpace = (String)NameSpace + ScopeHelper::_ScopeSep + (String)Name;

	

	while (TepNameSpace.size())
	{

		ScopeHelper::ReMoveScope(TepNameSpace);
		for (auto& Item : Symbols)
		{
			String FullName = TepNameSpace.size() ?
				TepNameSpace + ScopeHelper::_ScopeSep + (String)Name : (String)Name;
			if (Item.FullName == FullName)
			{
				Output.push_back(&Item);
			}
		}
	}

}
 Vector<Symbol*>& SymbolTable::GetSymbolsWithName(const String_view& Name)
{
	thread_local Vector<Symbol*> Tep;
	Tep.clear();


	GetSymbolsInNameSpace(_Scope.ThisScope, Name, Tep);
	for (auto& Item : Useings)
	{
		GetSymbolsInNameSpace(Item, Name, Tep);
	}

	return Tep;
}

UCodeLangEnd

