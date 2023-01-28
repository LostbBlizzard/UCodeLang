#pragma once
#include "UCodeLang/LangCore/ScopeHelper.hpp"
#include "IRCode.hpp"
UCodeLangStart


enum class TypesEnum :UInt8
{
	Null,
	Void,
	Var,
	Any,
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

enum class TypeValueInfo : UInt8
{
	IsValue,
	IsLocation,
};

struct TypeSymbol
{
	TypesEnum _Type = TypesEnum::Null;
	SymbolID  _CustomTypeSymbol = 0;

	

	bool _IsAddress=false;
	bool _IsAddressArray = false;
	bool _Isimmutable = false;
	TypeValueInfo _ValueInfo = TypeValueInfo::IsValue;


	
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
	void SetAsimmutable()
	{
		_Isimmutable = true;
	}
	void SetValueInfo(TypeValueInfo ValueInfo)
	{
		_ValueInfo = ValueInfo;
	}
	void SetAsLocation()
	{
		 SetValueInfo(TypeValueInfo::IsLocation);
	}
	void SetAsRawValue()
	{
		 SetValueInfo(TypeValueInfo::IsValue);
	}


	bool IsAddress()const
	{
		return _IsAddress;
	}
	bool IsAddressArray()const
	{
		return _IsAddressArray;
	}
	bool Isimmutable() const
	{
		return _Isimmutable;
	}
	bool Ismutable() const
	{
		return !Isimmutable();
	}
	bool IsLocationValue()const
	{
		return _ValueInfo == TypeValueInfo::IsLocation;
	}
	bool IsRawValue()const
	{
		return !IsLocationValue();
	}
	bool IsAn(TypesEnum Type)const
	{
		return _Type == Type;
	}
	bool IsnotAn(TypesEnum Type)const
	{
		return !IsAn(Type);
	}
};




enum class SymbolType : UInt8
{
	Null,
	Any,

	Varable_t,	
	StackVarable,
	ParameterVarable,

	Type,
	Type_alias,

	Type_class,
	Class_Field,
	Enum,

	Func,
	ImportedDllFunc,//
	ImportedLibFunc,

	FuncCall,
	GenericFunc,
	Generic_class,
	Unmaped_Generic_Type,
};
enum class SymbolValidState : UInt8
{
	Invalid,
	valid,
};


class Symbol_Info
{
public:
	Symbol_Info(){}
	virtual ~Symbol_Info(){}
};

class FieldInfo
{
public:
	String Name;
	TypeSymbol Type;
	FieldInfo()
	{

	}
	FieldInfo(String Name, TypeSymbol Type)
	{
		this->Name = Name;
		this->Type = Type;
	}
};

class FuncInfo :public Symbol_Info
{
public:
	String FullName;
	inline String_view Get_Name() const
	{
		return ScopeHelper::GetNameFromFullName((String_view)FullName);
	}
	Vector<TypeSymbol> Pars;
	TypeSymbol Ret;
	
	Vector<SymbolID> _Generic;


	bool IsObjectCall()
	{
		return Pars.size() && Pars.back().IsAddress();
	}
	TypeSymbol* GetObjectForCall()
	{
		if (IsObjectCall())
		{
			return &Pars.back();
		}
		return nullptr;
	}

	FuncInfo()
	{

	}
	~FuncInfo()
	{

	}

	SymbolValidState ValidState = SymbolValidState::valid;
	void SetToInvalid() { ValidState = SymbolValidState::Invalid; }
	void SetTovalid() { ValidState = SymbolValidState::valid; }
	bool IsInvalid()const { return ValidState == SymbolValidState::Invalid; }
	bool Isvalid()const { return !IsInvalid(); }
};

class ClassInfo:public Symbol_Info
{
public:
	String FullName;
	inline String_view Get_Name() const
	{
		return ScopeHelper::GetNameFromFullName((String_view)FullName);
	}
	UAddress Size = NullAddress;
	Vector<FieldInfo> Fields;
	bool SizeInitialized = false;

	Vector<SymbolID> _Generic;

	ClassInfo()
	{

	}
	~ClassInfo()
	{

	}

	FieldInfo* GetField(const String_view Name)
	{
		for (auto& Item : Fields)
		{
			if (Item.Name == Name) {
				return &Item;
			}
		}
		return nullptr;
	}

	void AddField(const String_view Name, TypeSymbol Type)
	{
		Fields.emplace_back((String)Name, Type);
	}
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
	void* NodePtr =nullptr;//most likey a node*
	Unique_ptr<Symbol_Info> Info;

	

	SymbolValidState ValidState = SymbolValidState::valid;
	void SetToInvalid(){ValidState = SymbolValidState::Invalid;}
	void SetTovalid(){ValidState = SymbolValidState::valid;}
	bool IsInvalid()const {return ValidState == SymbolValidState::Invalid;}
	bool Isvalid()const { return !IsInvalid(); }
	Symbol()
	{

	}
	Symbol(SymbolType Type,const String& FullName)
	{
		this->Type = Type;
		this->FullName = FullName;
	}
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
	Unordered_map<SymbolID,size_t> IDToSymbols;
};
UCodeLangEnd