#include "Symbol.hpp"


UCodeLangFrontStart
size_t SymbolTable::GetUseingIndex() { return Useings.size(); }
void SymbolTable::RemovePopUseing(size_t Index)
{
	size_t toremove =Useings.size() - Index;
	for (size_t i = 0; i < toremove; i++)
	{
		Useings.pop_back();
	}
}

Vector<Symbol*>& SymbolTable::GetSymbolsWithName(const String_view& Name, SymbolType Type)
{
	auto& r = GetSymbolsWithName(Name);


	return r;
}
void SymbolTable::GetSymbolsInNameSpace(const String_view& NameSpace, const String_view& Name, Vector<Symbol*>& Output)
{
	String TepScope = (String)NameSpace;
	String TepNameSpace = (String)NameSpace + ScopeHelper::_ScopeSep + (String)Name;

	

	while (TepNameSpace.size())
	{

		ScopeHelper::ReMoveScope(TepNameSpace);
		for (auto& Item : Symbols)
		{
			String FullName = TepNameSpace.size() ?
				TepNameSpace + ScopeHelper::_ScopeSep + (String)Name : (String)Name;
			if (Item->FullName == FullName)
			{


				bool HasItem = false;
				for (auto& Item2 : Output)
				{
					if (Item.get() == Item2)
					{
						HasItem = true;
						break;
					}
				}

				if (!HasItem) 
				{
					Output.push_back(Item.get());
				}
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

 const Vector<const Symbol*>& SymbolTable::GetSymbolsWithName(const String_view& Name, SymbolType Type) const
 {
	 auto& r = GetSymbolsWithName(Name);


	 return r;
 }

 void SymbolTable::GetSymbolsInNameSpace(const String_view& NameSpace, const String_view& Name, Vector<const Symbol*>& Output) const
 {
	 String TepScope = (String)NameSpace;
	 String TepNameSpace = (String)NameSpace + ScopeHelper::_ScopeSep + (String)Name;



	 while (TepNameSpace.size())
	 {

		 ScopeHelper::ReMoveScope(TepNameSpace);
		 for (auto& Item : Symbols)
		 {
			 String FullName = TepNameSpace.size() ?
				 TepNameSpace + ScopeHelper::_ScopeSep + (String)Name : (String)Name;
			 if (Item->FullName == FullName)
			 {


				 bool HasItem = false;
				 for (auto& Item2 : Output)
				 {
					 if (Item.get() == Item2)
					 {
						 HasItem = true;
						 break;
					 }
				 }

				 if (!HasItem)
				 {
					 Output.push_back(Item.get());
				 }
			 }
		 }
	 }

 }

 Vector<const Symbol*>& SymbolTable::GetSymbolsWithName(const String_view& Name) const
 {
	 thread_local Vector<const Symbol*> Tep;
	 Tep.clear();


	 GetSymbolsInNameSpace(_Scope.ThisScope, Name, Tep);
	 for (auto& Item : Useings)
	 {
		 GetSymbolsInNameSpace(Item, Name, Tep);
	 }

	 return Tep;
 }

 Symbol& SymbolTable::AddSybol(SymbolType type, const String& Name, const String& FullName, AccessModifierType Access)
 {
	 Symbols.push_back(std::make_unique<Symbol>(type, FullName));
	 auto& Item = *Symbols.back();

	 Item.Access = Access;
	 return Item;
 }

 void SymbolTable::AddSymbolID(Symbol& Syb, SymbolID ID)
 {
	 Syb.ID = ID;
	 IDToSymbols.AddValue(ID,&Syb);
 }

 void SymbolTable::Reset()
 {
	 ClearUseings();
	 _Scope.ThisScope.clear();
	 Symbols.clear();
	 IDToSymbols.clear();
 }

 UCodeLangFrontEnd

