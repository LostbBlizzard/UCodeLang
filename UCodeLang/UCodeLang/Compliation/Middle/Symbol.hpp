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

	Bool,
	Char,
	
	uInt8,
	uInt16,
	uInt32,
	uInt64,

	sInt8,
	sInt16,
	sInt32,
	sInt64,

	uIntPtr,
	sIntPtr,

	

	CustomType,
};
struct TypeSymbol
{
	TypesEnum _Type = TypesEnum::Null;
	SymbolID  _CustomTypeSymbol = 0;

	

	bool _IsAddress=false;
	bool _IsAddressArray = false;

	void SetType(TypesEnum type)
	{
		_Type = type;
		_CustomTypeSymbol = 0;
		_IsAddress = false;
	}
	void SetType(SymbolID CustomType)
	{
		_Type = TypesEnum::CustomType;
		_CustomTypeSymbol = CustomType;
		_IsAddress = false;
	}
	void SetAsAddress()
	{
		_IsAddress = true;
	}
	void SetAsAddressArray()
	{
		_IsAddressArray = true;
	}

	bool IsAddress()const
	{
		return _IsAddress;
	}
	bool IsAddressArray()const
	{
		return _IsAddressArray;
	}
};

class ClassInfo
{
public:
};


enum class SymbolType : UInt8
{
	Null,

	Varable_t,	
	StackVarable,
	ParameterVarable,

	Type,
	Type_alias,
	Type_class,
	Enum,

	Func,
	ImportedDllFunc,//
	ImportedLibFunc,

	FuncCall,
	GenericFunc,
	Generic_class
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
	//
	void* SomePtr =nullptr;//most likey a node* or ClassInfo*
};


class SymbolTable
{
public:
	Vector<String> Useings;
	ScopeHelper _Scope;
	Vector<Symbol> Symbols;



	void AddUseing(const String_view& Name) { Useings.push_back((String)Name); }
	void AddUseing(const String& Name) { Useings.push_back(Name); }
	void ClearUseings() { Useings.clear(); }

	size_t GetUseingIndex() { return Useings.size() - 1; }
	void RemovePopUseing(size_t Index);

	void AddScope(const String_view& Name) { _Scope.AddScope(Name); }
	void RemoveScope() { _Scope.ReMoveScope(); }

	Vector<Symbol*>& GetSymbolsWithName(const String_view& Name, SymbolType Type);
	void GetSymbolsInNameSpace(const String_view& NameSpace, const String_view& Name, Vector<Symbol*>& Output);
	Vector<Symbol*>& GetSymbolsWithName(const String_view& Name);

	Symbol& AddSybol(SymbolType type, const String& Name, const String& FullName);
	//use a ptr for the Id;
	Symbol& GetSymbol(SymbolID ID)
	{
		return Symbols[IDToSymbols[ID]];
	}

	void AddSymbolID(Symbol& Syb, SymbolID ID);
	void Reset();

		
private:
	unordered_map<SymbolID,size_t> IDToSymbols;
};
UCodeLangEnd