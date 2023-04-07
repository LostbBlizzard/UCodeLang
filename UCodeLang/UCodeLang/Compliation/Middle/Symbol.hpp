#pragma once
#include "UCodeLang/LangCore/ScopeHelper.hpp"
#include "SymbolID.hpp"
#include "../Front/UCodeFrontEndNameSpace.hpp"
#include "UCodeLang/LangCore/DataType/BinaryVectorMap.hpp"
UCodeLangStart
struct IRInstruction;
struct IRPar;
struct FileNode;
struct IRFunc;
UCodeLangEnd

UCodeLangFrontStart

using PassType_t = UInt8;
enum class PassType : PassType_t
{
	Null,
	GetTypes,
	FixedTypes,
	BuidCode,
	Done,
};

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

	float32,
	float64,

	CustomType,
};

enum class TypeValueInfo : UInt8
{
	IsValue,
	IsLocation,
};
enum class MoveData
{
	None,
	Moved,
};

struct TypeSymbol
{
	TypesEnum _Type = TypesEnum::Null;
	SymbolID  _CustomTypeSymbol = 0;

	

	bool _IsAddress=false;
	bool _IsAddressArray = false;
	bool _Isimmutable = false;
	TypeValueInfo _ValueInfo = TypeValueInfo::IsValue;
	MoveData _MoveData = MoveData::None;

	TypeSymbol()
	{

	}
	TypeSymbol(TypesEnum type)
	{
		SetType(type);
	}
	TypeSymbol(SymbolID type)
	{
		SetType(type);
	}
	
	void SetType(TypesEnum type)
	{
		_Type = type;
		_CustomTypeSymbol = 0;

		_IsAddress = false;
		_IsAddressArray = false;
		_Isimmutable = false;
		_ValueInfo = TypeValueInfo::IsValue;
		_MoveData = MoveData::None;
	}
	void SetType(SymbolID CustomType)
	{
		_Type = TypesEnum::CustomType;
		_CustomTypeSymbol = CustomType;

		_IsAddress = false;
		_IsAddressArray = false;
		_Isimmutable = false;
		_ValueInfo = TypeValueInfo::IsValue;
		_MoveData = MoveData::None;
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

	void SetAsMoved()
	{
		_MoveData = MoveData::Moved;
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
	bool IsNull()const
	{
		return  IsAn(TypesEnum::Null);
	}
	bool IsBadType()const { return IsAn(TypesEnum::Null); }
};




enum class SymbolType: UInt8
{
	Null,
	Any,

	Varable_t,	
	StackVarable,
	ParameterVarable,

	StaticVarable,
	ThreadVarable,

	Type,
	Type_alias,
	Hard_Type_alias,


	Type_class,
	Class_Field,
	Enum,
	Enum_Field,

	Func,
	ImportedDllFunc,//
	ImportedLibFunc,

	FuncCall,
	GenericFunc,
	Generic_class,
	Unmaped_Generic_Type,
	Namespace,

	Hard_Func_ptr,//no one will use this
	Func_ptr,

	LambdaObject,

	ConstantExpression,
	Type_StaticArray,
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
struct GenericData
{
	SymbolID SybID=0;
	bool IsConstantExpression = false;
};


class FuncInfo :public Symbol_Info
{
public:
	String FullName;
	inline String_view Get_Name() const
	{
		return ScopeHelper::GetNameFromFullName((String_view)FullName);
	}
	enum class FuncType
	{
		NameFunc,
		New,
		Drop,
		

		//operator overloading
		plus,
		minus,

		//Postfix
		increment,
		decrement,
		//Urinary
		Not,
		//Compound
		Compound_plus,
		Compound_Minus,
		//Member
		IndirectMember,
		OptionalDot,
		ExclamationDot,
		
		//other operators
		Index,
		Invoke,
		For,
		Cast
	};
		


	FuncType _FuncType = FuncType::NameFunc;


	Vector<TypeSymbol> Pars;
	TypeSymbol Ret;
	
	Vector<GenericData> _Generic;

	bool FrontParIsUnNamed = false;
	bool IsObjectCall() const
	{
		return Pars.size() && Pars.front().IsAddress() && FrontParIsUnNamed;
	}
	TypeSymbol* GetObjectForCall()
	{
		if (IsObjectCall())
		{
			return &Pars.front();
		}
		return nullptr;
	}
	const TypeSymbol* GetObjectForCall() const
	{
		if (IsObjectCall())
		{
			return &Pars.front();
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

	Vector<GenericData> _Generic;

	ClassInfo()
	{

	}
	~ClassInfo()
	{

	}

	Optional< FieldInfo*> GetField(const String_view Name)
	{
		for (auto& Item : Fields)
		{
			if (Item.Name == Name) {
				return &Item;
			}
		}
		return {};
	}
	Optional<size_t> GetFieldIndex(const String_view Name) const
	{
		for (size_t i = 0; i < Fields.size(); i++)
		{
			if (Fields[i].Name == Name) {
				return i;
			}
		}
	
		return {};
	}

	void AddField(const String_view Name, TypeSymbol Type)
	{
		Fields.emplace_back((String)Name, Type);
	}



	bool _WillHaveFieldInit = false;
	bool _WillHaveFielddeInit = false;
	IRFunc* _ClassFieldInit=nullptr;
	IRFunc* _ClassField_deInit = nullptr;
	//auto made funcs
	bool _ClassAutoGenerateDrop = false;
	bool _ClassAutoGenerateDefaultConstructor = false;
	bool _ClassAutoGenerateCopyConstructor = false;
	bool _ClassAutoGenerateMoveConstructor = false;
};

struct RawEvaluatedObject
{
	//union 
	struct
	{
		Unique_Array<Byte> Object_AsPointer;
		size_t ObjectSize = 0;
	};
	bool HasValue()
	{
		return Object_AsPointer.operator bool();
	}
	RawEvaluatedObject()
	{

	}
	RawEvaluatedObject(const RawEvaluatedObject& ToCopyFrom)
		:Object_AsPointer(new Byte[ToCopyFrom.ObjectSize]), ObjectSize(ToCopyFrom.ObjectSize)
	{
		memcpy(Object_AsPointer.get(), ToCopyFrom.Object_AsPointer.get(), ToCopyFrom.ObjectSize);
	}
	RawEvaluatedObject& operator=(const RawEvaluatedObject& ToCopyFrom)
	{
		Object_AsPointer.reset(new Byte[ToCopyFrom.ObjectSize]);
		ObjectSize = ToCopyFrom.ObjectSize;

		memcpy(Object_AsPointer.get(), ToCopyFrom.Object_AsPointer.get(), ToCopyFrom.ObjectSize);

		return *this;
	}
};


class EnumFieldInfo
{
public:
	String Name;
	RawEvaluatedObject Ex;
	EnumFieldInfo()
	{

	}
};

struct EnumVariantFeild
{
	Optional<SymbolID> ClassSymbol;
	Vector <TypeSymbol> Types;
};

struct EnumVariantData
{
	Optional<size_t> VariantSize;

	Vector<EnumVariantFeild> Variants;
};

class EnumInfo :public Symbol_Info
{
public:
	String FullName;
	inline String_view Get_Name() const
	{
		return ScopeHelper::GetNameFromFullName((String_view)FullName);
	}

	TypeSymbol Basetype;
	Vector<EnumFieldInfo> Fields;


	void SetBaseType(const TypeSymbol& Symbol)
	{
		Basetype = Symbol;
	}

	Optional<EnumFieldInfo*> GetField(const String_view Name)
	{
		for (auto& Item : Fields)
		{
			if (Item.Name == Name) {
				return &Item;
			}
		}
		return {};
	}

	Optional<size_t> GetFieldIndex(const String_view Name)
	{
		for (size_t i = 0; i < Fields.size(); i++)
		{
			auto& Item = Fields[i];
			if (Item.Name == Name) {
				return i;
			}
		}
		return {};
	}

	EnumFieldInfo& AddField(const String_view Name)
	{
		EnumFieldInfo& r = Fields.emplace_back();
		r.Name = (String)Name;
		return r;
	}

	Optional<EnumVariantData> VariantData;
};

class FuncPtrInfo :public Symbol_Info
{
public:
	String FullName;
	inline String_view Get_Name() const
	{
		return ScopeHelper::GetNameFromFullName((String_view)FullName);
	}

	Vector<TypeSymbol> Pars;
	TypeSymbol Ret;
};


struct  ExpressionNodeType;

class ConstantExpressionInfo :public Symbol_Info
{
public:
	ExpressionNodeType* Exnode = nullptr;
	

	ConstantExpressionInfo()
	{

	}
	~ConstantExpressionInfo()
	{

	}
};

class StaticArrayInfo :public Symbol_Info
{
public:
	TypeSymbol Type;
	size_t Count = 0;
	bool IsCountInitialized = false;

	ExpressionNodeType* Exnode = nullptr;

	StaticArrayInfo()
	{

	}
	~StaticArrayInfo()
	{

	}
};

class LambdaInfo :public Symbol_Info
{
public:
	bool UsesOuterScope = false;

	Vector<TypeSymbol> Pars;
	TypeSymbol Ret;
	LambdaInfo()
	{

	}
	~LambdaInfo()
	{

	}
};

class Symbol
{
public:
	SymbolType Type = SymbolType::Null;
	String FullName;
	SymbolID ID = 0;
	//var
	TypeSymbol VarType;
	//
	const void* NodePtr =nullptr;//most likey a node*
	Unique_ptr<Symbol_Info> Info;
	const FileNode* _File = nullptr;
	union 
	{
		IRPar* IR_Par;
		IRInstruction* IR_Ins = nullptr;
	};


	SymbolValidState ValidState = SymbolValidState::valid;
	PassType PassState = PassType::Null;

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

	template<typename T> T* Get_Info()
	{
		return (T*)Info.get();
	}
	template<typename T> const T* Get_Info() const
	{
		return (T*)Info.get();
	}
	
};


class SymbolTable
{
public:
	Vector<String> Useings;
	ScopeHelper _Scope;
	Vector<Unique_ptr<Symbol>> Symbols;


	void AddUseing(const String_view& Name) { Useings.push_back((String)Name); }
	void AddUseing(const String& Name) { Useings.push_back(Name); }
	void ClearUseings() { Useings.clear(); }

	size_t GetUseingIndex();
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
		return *IDToSymbols.at(ID);
	}

	void AddSymbolID(Symbol& Syb, SymbolID ID);
	void Reset();

		
private:
	BinaryVectorMap<SymbolID,Symbol*> IDToSymbols;
};
UCodeLangFrontEnd