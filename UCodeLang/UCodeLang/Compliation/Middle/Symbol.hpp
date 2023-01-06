#pragma once
#include "UCodeLang/LangCore/ScopeHelper.hpp"
#include "IRCode.hpp"
UCodeLangStart

enum class SymbolType : UInt8
{
	Null,
	StackVarable,

	Type,
	Type_alias,
	Enum,

	Func,
	ImportedDllFunc,//
	ImportedLibFunc,
};
class Symbol
{
public:
	SymbolType Type;
	String FullName;
	SymbolID ID;
};

class SymbolTable
{
public:
	Vector<String> Useings;
	ScopeHelper _Scope;
	Vector<Symbol> Symbols;



	void AddUseing(const String_view& Name) { Useings.push_back(Name.data()); }
	void AddUseing(const String& Name) { Useings.push_back(Name); }
	void ClearUseings() { Useings.clear(); }

	size_t GetUseingIndex() { return Useings.size() - 1; }
	void RemovePopUseing(size_t Index)
	{

	}

	void AddScope(const String_view& Name) { _Scope.AddScope(Name); }
	void RemoveScope() { _Scope.ReMoveScope(); }

	Vector<Symbol*>& GetSymbolsWithName(const String_view& Name,SymbolType Type)
	{
		auto& r = GetSymbolsWithName(Name);


		return r;
	}

	Vector<Symbol*>& GetSymbolsWithName(const String_view& Name)
	{
		thread_local Vector<Symbol*> Tep;


		return Tep;
	}

	Symbol& AddSybol(SymbolType type, const String& Name, const String& FullName)
	{
		Symbols.push_back({});
		Symbol& V = Symbols.back();
		V.Type = type;
		V.FullName = FullName;

		return V;
	}
	//use a ptr for the Id;
	Symbol& GetSymbol(SymbolID ID)
	{
		return Symbols[IDToSymbols[ID]];
	}

	void AddSymbolID(Symbol& Syb, SymbolID ID)
	{
		Syb.ID = ID;

		Symbol* Pointer = &Symbols[0];
		Symbol* SybPointer = &Syb;
		size_t Index = SybPointer - Pointer;
		IDToSymbols[ID] = Index;
	}
	void Reset()
	{
		ClearUseings();
	}

		
private:
	unordered_map<SymbolID,size_t> IDToSymbols;
};
UCodeLangEnd