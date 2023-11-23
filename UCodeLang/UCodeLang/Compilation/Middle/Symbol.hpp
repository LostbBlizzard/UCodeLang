#pragma once

#ifndef UCodeLangNoCompiler
#include "UCodeLang/LangCore/ScopeHelper.hpp"
#include "SymbolID.hpp"
#include "../Front/UCodeFrontEndNameSpace.hpp"
#include "UCodeLang/LangCore/DataType/UnorderedMap.hpp"
#include "UCodeLang/LangCore/ReflectionData.hpp"
UCodeLangStart
struct IRInstruction;
struct IRPar;
struct FileNode;
struct IRFunc;
struct FileNode;
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

using TypesEnum = ReflectionTypes;

using TypeValueInfo_t = UInt8;
enum class TypeValueInfo : TypeValueInfo_t
{
	IsValue,
	IsLocation,
};
using MoveData = ReflectionMoveData;

using TypeSymbolID = ReflectionCustomTypeID;

enum class TypeInfoPrimitive : UInt8
{
	Null,
	TypeInfo,
	ClassInfo,
	EnumInfo,
	AliasInfo,
	FuncInfo,
	FuncPtrInfo,
	TraitInfo,
	TagInfo,

	ClassFieldInfo,
	EnumFieldInfo,
	EnumVariantInfo,
};

class Symbol;
struct TypeSymbol
{
	SymbolID  _CustomTypeSymbol;
	
	TypesEnum _Type = TypesEnum::Null;
	bool _IsAddress=false;
	bool _IsAddressArray = false;
	bool _Isimmutable = false;
	bool _IsDynamic = false;
	TypeValueInfo _ValueInfo = TypeValueInfo::IsValue;
	MoveData _MoveData = MoveData::None;
	TypeInfoPrimitive _TypeInfo = TypeInfoPrimitive::Null;

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
		_CustomTypeSymbol = {};

		_Type = type;
		_IsAddress = false;
		_IsAddressArray = false;
		_Isimmutable = false;
		_ValueInfo = TypeValueInfo::IsValue;
		_MoveData = MoveData::None;
		_TypeInfo = TypeInfoPrimitive::Null;
	}
	void SetType(SymbolID CustomType)
	{
		_CustomTypeSymbol = CustomType;
		_Type = TypesEnum::CustomType;

		_IsAddress = false;
		_IsAddressArray = false;
		_Isimmutable = false;
		_ValueInfo = TypeValueInfo::IsValue;
		_MoveData = MoveData::None;
		_TypeInfo = TypeInfoPrimitive::Null;
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
	bool IsDynamicTrait() const
	{
		return _IsDynamic;
	}

	bool IsMovedType() const
	{
		return _MoveData == ReflectionMoveData::Moved;
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

	bool IsTypeInfo() const
	{
		return  _TypeInfo != TypeInfoPrimitive::Null;
	}
	void SetAsTypeInfo()
	{
		_TypeInfo = TypeInfoPrimitive::TypeInfo;
	}
	void BindType()
	{
		_TypeInfo = TypeInfoPrimitive::Null;
	}
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
	Unmaped_Varable,
	Namespace,

	Hard_Func_ptr,//no one will use this
	Func_ptr,

	LambdaObject,

	ConstantExpression,
	Type_StaticArray,

	Trait_class,
	Tag_class,
	Type_Pack,

	Generic_Alias,
	Generic_Tag,
	Generic_Trait,
	Generic_Enum,

	UsedTag,
};
enum class SymbolValidState : UInt8
{
	//Symbol Got Errors so it should not be used
	Invalid,

	//Symbol Is Ok
	valid,
};



class Symbol_Info
{
public:
	Symbol_Info(){}
	virtual ~Symbol_Info(){}
};

class TypePackInfo :public Symbol_Info
{
public:
	Vector<TypeSymbol> List;
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
	enum class Type
	{
		Name,
		ConstantExpression,
		Pack,
	};
	SymbolID SybID;
	Type type = Type::Name;
	bool IsConstantExpression() const
	{
		return type == Type::ConstantExpression;
	}
};

struct Generic
{
	Vector<GenericData> _Genericlist;

	
	inline bool IsGeneric() const
	{
		return _Genericlist.size() != 0;
	}
	inline bool IsPack()const
	{
		if (_Genericlist.size())
		{
			return  _Genericlist.back().type == GenericData::Type::Pack;
		}
		else
		{
			return false;
		}
	}
	inline size_t GetMinimumCount()const
	{
		auto r = _Genericlist.size();
		if (IsPack())
		{
			r -= 1;
		}
		return r;
	}
};

class Symbol;
class ClassInfo;
struct ClassStackInfo
{
	Symbol* Syb = nullptr;
	ClassInfo* Info = nullptr;
	bool _InStatements = false;
};
struct SymbolContext
{
	ScopeHelper Scope;
	Vector<String> Useings;
	const FileNode* File = nullptr;
	Stack<ClassStackInfo> _ClassStack;
};

struct ParInfo
{
	bool IsOutPar = false;
	TypeSymbol Type;
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
		equal,
		notequal,

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
	

	Vector<ParInfo> Pars;
	TypeSymbol Ret;
	
	Generic _GenericData;
	Optional<SymbolContext> Context;

	bool FrontParIsUnNamed = false;
	bool IsUnsafe = false;
	bool IsExternC = false;
	bool IsRemoved = false;
	bool IsObjectCall() const
	{
		return Pars.size() && (Pars.front().Type.IsAddress() && Pars.front().IsOutPar == false && FrontParIsUnNamed);
	}
	TypeSymbol* GetObjectForCall()
	{
		if (IsObjectCall())
		{
			return &Pars.front().Type;
		}
		return nullptr;
	}
	const TypeSymbol* GetObjectForCall() const
	{
		if (IsObjectCall())
		{
			return &Pars.front().Type;
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

struct ClassInfo_InheritTypeInfo
{
	struct AddedFuncInfo
	{
		const void* FuncNode=nullptr;
		Symbol* Func=nullptr;
	};

	struct AddedVarInfo
	{
		const void* FuncNode = nullptr;
		Symbol* Func = nullptr;
	};

	enum class FuncType
	{
		Added,
		OverLoaded,
	};
	struct FuncIndex
	{
		size_t Index;
		FuncType Type;
	};

	Symbol* Syb = nullptr;
	Vector<AddedFuncInfo> AddedFuncs;
	Vector<AddedFuncInfo> OverLoadedFuncs;
	Vector<FuncIndex> Funcs;
	Vector<AddedVarInfo> AddedVar;
};

class ClassInfo:public Symbol_Info
{
public:
	String FullName;
	inline String_view Get_Name() const
	{
		return ScopeHelper::GetNameFromFullName((String_view)FullName);
	}


	UAddress Size = 0;
	Vector<FieldInfo> Fields;
	bool SizeInitialized = false;

	Generic _GenericData;

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
	Vector<ClassInfo_InheritTypeInfo> _InheritedTypes;
	Optional<size_t> Get_InheritedTypesIndex(const Symbol* ID) const
	{
		for (size_t i = 0; i < _InheritedTypes.size(); i++)
		{
			auto& Item = _InheritedTypes[i];
			if (Item.Syb == ID)
			{
				return i;
			}
		}

		return {};
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

	Optional<SymbolID> _ClassHasCopyConstructor;
	Optional<SymbolID> _ClassHasMoveConstructor;

	bool _IsExternalC = false;

	Optional<SymbolContext> Context;
};

struct RawEvaluatedObject
{

	Unique_Array<Byte> Object_AsPointer;
	size_t ObjectSize = 0;

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

struct EnumVariantField
{
	Optional<SymbolID> ClassSymbol;
	Vector <TypeSymbol> Types;
};

struct EnumVariantData
{
	Optional<size_t> VariantSize;

	Vector<EnumVariantField> Variants;
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

	Optional<size_t> GetFieldIndex(const String_view Name) const
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

	const NullablePtr<TypeSymbol> IsOptionalAddress() const
	{
		if (VariantData.has_value()) 
		{
			if (Fields.size() == 2)
			{
				bool hasnone = false;
				const TypeSymbol* hasvalue = nullptr;
				for (auto& Item : VariantData.value().Variants)
				{
					if (Item.Types.size() == 0)
					{
						hasnone = true;
					}
					else if (Item.Types.size() == 1)
					{
						if (Item.Types[0].IsAddress())
						{
							hasvalue = &Item.Types[0];
						}
					}
				}

				if (hasvalue && hasnone)
				{
					return Nullableptr(hasvalue);
				}
			}
		}
		return {};
	}

	Optional<EnumVariantData> VariantData;
	bool HasDestructer;
	bool MoveContructer;
	bool CopyContructer;

	Generic _GenericData;
	Optional<SymbolContext> Context;
};

class FuncPtrInfo :public Symbol_Info
{
public:
	String FullName;
	inline String_view Get_Name() const
	{
		return ScopeHelper::GetNameFromFullName((String_view)FullName);
	}

	Vector<ParInfo> Pars;
	TypeSymbol Ret;

	Generic _GenericData;
	Optional<SymbolContext> Context;
};


struct  ExpressionNodeType;

class ConstantExpressionInfo :public Symbol_Info
{
public:
	const ExpressionNodeType* Exnode = nullptr;
	RawEvaluatedObject Ex;

	Optional<SymbolContext> Context;
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


struct LambdaFieldInfo
{
	String Name;
	TypeSymbol Type;
	Symbol* Sym = nullptr;
};

class LambdaInfo :public Symbol_Info
{
public:
	bool UsesOuterScope()
	{
		return  _CapturedVarables.size()!=0;
	}

	Vector<ParInfo> Pars;
	TypeSymbol Ret;

	Vector<LambdaFieldInfo> _CapturedVarables;
	Vector<LambdaFieldInfo> _IndirectCapturedVarables;
	LambdaInfo()
	{

	}
	~LambdaInfo()
	{

	}
	FuncInfo* _ClassCall=nullptr;
	FuncInfo* _ClassDrop = nullptr;
};

struct TraitFunc
{
	Symbol* Syb = nullptr;
	bool HasBody = false;
};
struct TraitVar
{
	Symbol* Syb = nullptr;
};

class TraitInfo :public Symbol_Info
{
public:
	String FullName;
	inline String_view Get_Name() const
	{
		return ScopeHelper::GetNameFromFullName((String_view)FullName);
	}

	Optional<size_t> GetIndex_Func(Symbol* Syb) const
	{
		for (size_t i = 0; i < _Funcs.size(); i++)
		{
			if (_Funcs[i].Syb == Syb)
			{
				return i;
			}
		}
		return {};
	}

	Vector<TraitFunc> _Funcs;
	Vector<TraitVar> _Vars;
	Symbol* TraitClassInfo = nullptr;
	Generic _GenericData;
	Optional<SymbolContext> Context;
};


class AliasInfo :public Symbol_Info
{
public:
	Optional<SymbolContext> Context;
};

class Generic_AliasInfo :public Symbol_Info
{
public:
	Generic _GenericData;
	Optional<SymbolContext> Context;
};

enum class ImportBindType
{
	Null,
	Type,
	Func,
	GenericFunc,
};
class Imports_Info :public Symbol_Info
{
public:
	struct ImportSymbolInfo
	{
		ImportBindType Type= ImportBindType::Null;
		Symbol* Sym;

	};
	struct ImportAliases
	{
		bool IsUsed = false;
		Vector<ImportSymbolInfo> NewSymbols;
	};
	Vector<ImportAliases> NewAliases;

};

class TagInfo :public Symbol_Info
{
public:
	Generic _GenericData;
	Optional<SymbolContext> Context;

};
class DeclareVariableInfo :public Symbol_Info
{
public:
	//for stack varables.
	size_t LineNumber = 0;

	Optional<SymbolContext> Context;
};

class Symbol
{
public:
	SymbolType Type = SymbolType::Null;
	String FullName;
	SymbolID ID;

	bool OutputIR = true;
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
	AccessModifierType Access = AccessModifierType::Public;


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
		UCodeLangAssert(Info.get());
		return (T*)Info.get();
	}	
	template<typename T> const T* Get_Info() const
	{
		UCodeLangAssert(Info.get());
		return (T*)Info.get();
	}

	
	#if UCodeLangMSVC //On GCC explicit specialization in non-namespace scope
	template<> FuncInfo* Get_Info()
	{
		#if UCodeLangDebug
		if (Type != SymbolType::Func && Type != SymbolType::GenericFunc)
		{
			UCodeLangThrowException("bad cast");
		}
		#endif // DEBUG

		UCodeLangAssert(Info.get());
		return (FuncInfo*)Info.get();
	}
	
	template<> const FuncInfo* Get_Info() const
	{
		#if UCodeLangDebug
		if (Type != SymbolType::Func && Type != SymbolType::GenericFunc)
		{
			UCodeLangThrowException("bad cast");
		}
		#endif // DEBUG

		UCodeLangAssert(Info.get());
		return (FuncInfo*)Info.get();
	}
	

	template<> FuncPtrInfo* Get_Info()
	{
		#if UCodeLangDebug
		if (Type != SymbolType::Func_ptr)
		{
			UCodeLangThrowException("bad cast");
		}
		#endif // DEBUG
		UCodeLangAssert(Info.get());
		return (FuncPtrInfo*)Info.get();
	}
	template<> const FuncPtrInfo* Get_Info() const
	{
		#if UCodeLangDebug
		if (Type != SymbolType::Func_ptr)
		{
			UCodeLangThrowException("bad cast");
		}
		#endif // DEBUG

		UCodeLangAssert(Info.get());
		return (FuncPtrInfo*)Info.get();
	}
	#endif // DEBUG



	template<typename T> const T* Get_NodeInfo() const
	{
		UCodeLangAssert(NodePtr);
		return (T*)NodePtr;
	}
	void Set_NodeInfo(const void* NodeObject)
	{
		UCodeLangAssert(NodePtr == nullptr);//reassigning Node its read-only?
		NodePtr = NodeObject;
	}
};

class ParameterInfo :public Symbol_Info
{
public:
	bool IsOutValue = false;
	FuncInfo* MyFunc = nullptr;

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

	const Vector<const Symbol*>& GetSymbolsWithName(const String_view& Name, SymbolType Type) const;
	void GetSymbolsInNameSpace(const String_view& NameSpace, const String_view& Name, Vector<const Symbol*>& Output) const;
	Vector<const Symbol*>& GetSymbolsWithName(const String_view& Name) const;

	Symbol& AddSymbol(SymbolType type, const String& Name, const String& FullName,AccessModifierType Access);
	//use a ptr for the Id;
	Symbol& GetSymbol(SymbolID ID)
	{
		return *IDToSymbols.GetValue(ID);
	}
	const Symbol& GetSymbol(SymbolID ID) const
	{
		return *IDToSymbols.GetValue(ID);
	}

	void AddSymbolID(Symbol& Syb, SymbolID ID);
	void Reset();

		
private:
	UnorderedMap<SymbolID,Symbol*> IDToSymbols;
	UnorderedMap<String, Vector<Symbol*>> NameToSymbol;
};
UCodeLangFrontEnd

#endif