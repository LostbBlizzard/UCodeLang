#pragma once
#include "UCodeLang/LangCore/ScopeHelper.hpp"
#include "IRCode.hpp"
UCodeLangStart


enum class TypesEnum :UInt8
{
	Null,
	Void,
	Var,
	Int_t,
	uInt_t,
	sInt_t,

	uInt8,
	uInt16,
	uInt32,
	uInt64,

	sInt8,
	sInt16,
	sInt32,
	sInt64,

	

	Bool,

	CustomType,
};
struct TypeSymbol
{
	TypesEnum _Type = TypesEnum::Null;
	SymbolID  _CustomTypeSymbol = 0;

	void SetType(TypesEnum type)
	{
		_Type = type;
	}
};

enum class SymbolType : UInt8
{
	Null,

	Varable_t,	
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
	//var
	UAddress Size=NullAddress;
	TypeSymbol VarType;
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

		String TepScope =_Scope.ThisScope;
		String TepName = TepScope + ScopeHelper::_ScopeSep + (String)Name;

		for (auto& Item : Symbols)
		{
			if (Item.FullName == TepName)
			{
				Tep.push_back(&Item);
			}
		}

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
		_Scope.ThisScope.clear();
		Symbols.clear();
		IDToSymbols.clear();
	}

		
private:
	unordered_map<SymbolID,size_t> IDToSymbols;
};
UCodeLangEnd