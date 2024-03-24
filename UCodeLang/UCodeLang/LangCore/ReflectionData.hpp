#pragma once
#include "../LangCore.hpp"
#include "TypeNames.hpp"
#include "ScopeHelper.hpp"

#include <cstring> //memcpy
UCodeLangStart


using ReflectionTypes_t = UInt8;
enum class ReflectionTypes : ReflectionTypes_t
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

	Uft8,
	Uft16,
	Uft32,

	CustomType,
	InternalType,
};
using ReflectionMoveData_t = UInt8;
enum class ReflectionMoveData : ReflectionMoveData_t
{
	None,
	Moved,
};

using ReflectionCustomTypeID = UInt64;
constexpr ReflectionCustomTypeID MaxReserved_ReflectionCustomTypeID = 32;

class ReflectionTypeInfo
{
public:
	ReflectionCustomTypeID _CustomTypeID =0;
	ReflectionTypes  _Type = ReflectionTypes::Null;

	bool _IsAddress = false;
	bool _IsAddressArray = false;
	bool _Isimmutable = false;
	bool _IsDynamic = false;
	ReflectionMoveData _MoveData = ReflectionMoveData::None;

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
	bool operator==(const ReflectionTypeInfo& Other) const
	{
		return _CustomTypeID == Other._CustomTypeID &&
			_Type == Other._Type &&
			_IsAddress == Other._IsAddress &&
			_Isimmutable == Other._Isimmutable &&
			_IsDynamic == Other._IsDynamic &&
			_MoveData == Other._MoveData;
	}
	bool operator!=(const ReflectionTypeInfo& Other) const
	{
		return !this->operator==(Other);
	}
	ReflectionTypeInfo()
	{

	}
	ReflectionTypeInfo(ReflectionTypes  Type)
		: _Type(Type)
	{

	}
};

class ReflectionRawData
{
public:
	size_t Size = 0;
	Unique_Array<Byte> Bytes;

	inline void Resize(size_t size)
	{
		Bytes.reset(new Byte[size]);
		Size = size;
	}
	inline void* Get_Data()
	{
		return Bytes.get();
	}
	inline const void* Get_Data() const
	{
		return Bytes.get();
	}
	ReflectionRawData() {};
	ReflectionRawData(const  ReflectionRawData& ToCopy)
	{
		Resize(ToCopy.Size);
		memcpy(Get_Data(), ToCopy.Get_Data(), Size);
	}
	ReflectionRawData& operator=(const ReflectionRawData& ToCopy)
	{
		Resize(ToCopy.Size);
		memcpy(Get_Data(), ToCopy.Get_Data(), Size);

		return *this;
	}

	template<typename T>
	inline T* Get_DataAs()
	{
		if (sizeof(T) != Size)
		{
			UCodeLangThrowException("bad type cast");
		}

		return (T*)Bytes.get();
	}

	template<typename T>
	inline const T* Get_DataAs() const
	{
		if (sizeof(T) != Size)
		{
			UCodeLangThrowException("bad type cast");
		}

		return (T*)Bytes.get();
	}
};


class ClassField
{
public:
	String Name;
	ReflectionTypeInfo Type;
	size_t offset = NullAddress;
	AccessModifierType Protection = AccessModifierType::Default;
};
class UsedTagValueData
{
public:
	ReflectionCustomTypeID  TypeID;
	~UsedTagValueData()
	{

	}
	ReflectionRawData _Data;
};
class UsedTags
{
public:
	Vector<UsedTagValueData> Attributes;
};

class ClassMethod
{
public:
	struct Par
	{
		bool IsOutPar = false;
		ReflectionTypeInfo Type;

		bool operator==(const Par& Other) const
		{
			return this->IsOutPar == Other.IsOutPar &&
				this->Type == Other.Type;
		}
		bool operator!=(const Par& Other) const
		{
			return !this->operator==(Other);
		}
	};
	String FullName;
	String DecorationName;
	
	
	ReflectionTypeInfo RetType;
	Vector<Par> ParsType;
	bool IsThisFunction = false;
	bool IsUnsafe = false;
	bool IsExternC = false;
	bool IsRemoved = false;
	bool IsExport = false;
	AccessModifierType Protection = AccessModifierType::Default;

	UsedTags Attributes;
};

class TypedRawReflectionData
{
public:
	ReflectionTypeInfo _Type;
	ReflectionRawData _Data;
};

class EnumValues
{
public:
	String Name;
	ReflectionRawData _Data;

	//The Type the Enum Variant Value  is associated with.
	Optional<ReflectionTypeInfo> EnumVariantType;
};

class InheritedTrait_Data
{
public:
	ReflectionCustomTypeID TraitID = {};
};

using ClassType_t = UInt8;
enum class ClassType :ClassType_t
{
	Null,
	Class,
	Enum,
	Alias,
	Eval,
	Tag,
	Trait,
	StaticVarable,
	ThreadVarable,
	StaticArray,
	FuncPtr,
	GenericClass,
	GenericFunction,
	ForType,
	NameSpace,
};
struct Class_Data
{
	ReflectionCustomTypeID TypeID = {};
	size_t Size = 0;
	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
	UsedTags Attributes;
	Vector<ClassField> Fields;
	Vector<ClassMethod> Methods;
	Vector<InheritedTrait_Data> InheritedTypes;
		
	const ClassMethod* Get_ClassInit() const
	{
		return Get_ClassMethod(ClassInitializefuncName);
	}//This May be null.

	const ClassMethod* Get_ClassConstructor() const
	{
		return Get_ClassMethod(ClassConstructorfunc);
	}//This May be null.

	const ClassMethod* Get_ClassDestructor() const
	{
		return Get_ClassMethod(ClassDestructorFunc);
	}//This May be null.

	const ClassMethod* Get_ClassMethod(const String_view Name) const
	{
		for (auto& Item : Methods)
		{
			if (ScopeHelper::GetNameFromFullName(Item.FullName)
				== Name)
			{
				return &Item;
			}
		}
		return nullptr;
	}
	Vector<const ClassMethod*> Get_ClassMethods(const String_view Name) const
	{
		Vector<const ClassMethod*> r;
		for (auto& Item : Methods)
		{
			if (ScopeHelper::GetNameFromFullName(Item.FullName)
				== Name)
			{
				r.push_back(&Item);
			}
		}
		return r;
	}
	Vector<const ClassMethod*> Get_ClassConstructors() const
	{
		return Get_ClassMethods(ClassConstructorfunc);
	}//This May be null.

	const ClassField* Get_ClassField(const String_view Name) const
	{
		for (auto& Item : Fields)
		{
			if (Item.Name == Name)
			{
				return &Item;
			}
		}
		return nullptr;
	}//This May be null.

	ClassField* Get_ClassField(const String_view Name)
	{
		for (auto& Item : Fields)
		{
			if (Item.Name == Name)
			{
				return &Item;
			}
		}
		return nullptr;
	}//This May be null.
};
struct Enum_Data
{
	ReflectionCustomTypeID TypeID;
	ReflectionTypeInfo BaseType;
	Optional<ReflectionCustomTypeID> EnumVariantUnion;//is class

	Vector<EnumValues> Values;

	Optional<String> DestructorFuncFullName;
	Optional<String> MoveFuncFullName;
	Optional<String> CopyFuncFullName;
	
	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
};
struct Alias_Data
{
	ReflectionTypeInfo Type;
	Optional<ReflectionCustomTypeID> HardAliasTypeID;

	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
};
struct Eval_Data
{
	TypedRawReflectionData Value;

	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
};
struct Tag_Data
{
	ReflectionCustomTypeID TypeID = {};
	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
	Vector<ClassField> Fields;
	Vector<ClassMethod> Methods;
	~Tag_Data()
	{

	}
};
struct TraitMethod
{
	ClassMethod method;
	Optional<String> FuncBody;
};
struct Trait_Data
{
	ReflectionCustomTypeID TypeID = {};
	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
	Vector<ClassField> Fields;
	Vector<TraitMethod> Methods;
	~Trait_Data()
	{

	}
};
struct StaticVar_Data
{
	~StaticVar_Data()
	{

	}
};
struct ThreadVar_Data
{
	~ThreadVar_Data()
	{

	}
};
struct StaticArray_Data
{
	ReflectionCustomTypeID TypeID = {};
	ReflectionTypeInfo BaseType;
	size_t Count = 0;
	~StaticArray_Data()
	{

	}
};
struct FuncPtr_Data
{
	ReflectionTypeInfo RetType;
	Vector<ClassMethod::Par> ParsType;
	~FuncPtr_Data()
	{

	}
};
struct GenericBase_Data
{
	String Implementation;
};
struct GenericClass_Data
{
	GenericBase_Data Base;
	~GenericClass_Data()
	{

	}
	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
};


struct GenericFunction_Data
{
	GenericBase_Data Base;
	~GenericFunction_Data()
	{

	}
	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
};
struct GenericEnum_Data
{
	GenericBase_Data Base;
	~GenericEnum_Data()
	{

	}
	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
};
struct GenericAlias_Data
{
	GenericBase_Data Base;
	~GenericAlias_Data()
	{

	}
	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
};
struct GenericTrait_Data
{
	GenericBase_Data Base;
	~GenericTrait_Data()
	{

	}
	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
};
struct GenericTag_Data
{
	GenericBase_Data Base;
	~GenericTag_Data()
	{

	}
	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
};
struct GenericForType_Data
{
	GenericBase_Data Base;
	~GenericForType_Data()
	{

	}
	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
};
struct NameSpace_Data
{
	~NameSpace_Data()
	{

	}
};
struct ForType_Data
{
	ReflectionTypeInfo _TargetType;
	String _Scope;
	AccessModifierType AccessModifier = AccessModifierType::Default;
	bool IsExported = false;
	Vector<ClassMethod> _AddedMethods;
	~ForType_Data()
	{

	}
	
};
class AssemblyNode
{

public:
	String Name;
	String FullName;
	
	AssemblyNode(ClassType type);
	AssemblyNode(AssemblyNode&& node) noexcept
	{
		this->operator=(std::move(node));
	}
	AssemblyNode& operator=(AssemblyNode&& node);
	AssemblyNode(const AssemblyNode& node)
	{
		this->operator=(node);
	}
	AssemblyNode& operator=(const AssemblyNode& node);
	~AssemblyNode();

	Class_Data& Get_ClassData()
	{
		UCodeLangAssert(Type == ClassType::Class);
		return _Class;
	}
	Enum_Data& Get_EnumData()
	{
		UCodeLangAssert(Type == ClassType::Enum);
		return _Enum;
	}
	Alias_Data& Get_AliasData()
	{
		UCodeLangAssert(Type == ClassType::Alias);
		return _Alias;
	}
	Eval_Data& Get_EvalData()
	{
		UCodeLangAssert(Type == ClassType::Eval);
		return  _Eval;
	}
	Trait_Data& Get_TraitData()
	{
		UCodeLangAssert(Type == ClassType::Trait);
		return  _Trait;
	}
	Tag_Data& Get_TagData()
	{
		UCodeLangAssert(Type == ClassType::Tag);
		return  _Tag;
	}
	StaticVar_Data& Get_StaticVar()
	{
		UCodeLangAssert(Type == ClassType::StaticVarable);
		return  _StaticVar;
	}
	ThreadVar_Data& Get_ThreadVar()
	{
		UCodeLangAssert(Type == ClassType::ThreadVarable);
		return _ThreadVar;
	}
	StaticArray_Data& Get_StaticArray() 
	{
		UCodeLangAssert(Type == ClassType::StaticArray);
		return _StaticArr;
	}
	FuncPtr_Data& Get_FuncPtr() 
	{
		UCodeLangAssert(Type == ClassType::FuncPtr);
		return _FuncPtr;
	}
	GenericClass_Data& Get_GenericClass() 
	{
		UCodeLangAssert(Type == ClassType::GenericClass);
		return _GenericClass;
	}
	GenericFunction_Data& Get_GenericFunctionData() 
	{
		UCodeLangAssert(Type == ClassType::GenericFunction);
		return _GenericFunc;
	}
	ForType_Data& Get_ForType() 
	{
		UCodeLangAssert(Type == ClassType::ForType);
		return _ForType;
	}
	NameSpace_Data& Get_NameSpace() {
		UCodeLangAssert(Type == ClassType::NameSpace);
		return _NameSapce;
	}
	
	const Class_Data& Get_ClassData() const
	{
		UCodeLangAssert(Type == ClassType::Class);
		return _Class;
	}
	const Enum_Data& Get_EnumData()const
	{
		UCodeLangAssert(Type == ClassType::Enum);
		return _Enum;
	}
	const Alias_Data& Get_AliasData()const
	{
		UCodeLangAssert(Type == ClassType::Alias);
		return _Alias;
	}
	const Eval_Data& Get_EvalData()const
	{
		UCodeLangAssert(Type == ClassType::Eval);
		return  _Eval;
	}
	const Trait_Data& Get_TraitData()const
	{
		UCodeLangAssert(Type == ClassType::Trait);
		return  _Trait;
	}
	const Tag_Data& Get_TagData()const
	{
		UCodeLangAssert(Type == ClassType::Tag);
		return  _Tag;
	}
	const StaticVar_Data& Get_StaticVar()const
	{
		UCodeLangAssert(Type == ClassType::StaticVarable);
		return  _StaticVar;
	}
	const ThreadVar_Data& Get_ThreadVar() const
	{
		UCodeLangAssert(Type == ClassType::ThreadVarable);
		return _ThreadVar;
	}
	const StaticArray_Data& Get_StaticArray() const
	{
		UCodeLangAssert(Type == ClassType::StaticArray);
		return _StaticArr;
	}
	const FuncPtr_Data& Get_FuncPtr() const
	{
		UCodeLangAssert(Type == ClassType::FuncPtr);
		return _FuncPtr;
	}
	const GenericClass_Data& Get_GenericClass() const
	{
		UCodeLangAssert(Type == ClassType::GenericClass);
		return _GenericClass;
	}
	const GenericFunction_Data& Get_GenericFunctionData() const
	{
		UCodeLangAssert(Type == ClassType::GenericFunction);
		return _GenericFunc;
	}
	const ForType_Data& Get_ForType() const
	{
		UCodeLangAssert(Type == ClassType::ForType);
		return _ForType;
	}
	const NameSpace_Data& Get_NameSpace() const
	{
		UCodeLangAssert(Type == ClassType::NameSpace);
		return _NameSapce;
	}
	inline ClassType Get_Type() const
	{
		return Type;
	}
private:
	ClassType Type = ClassType::Null;
	union 
	{
		Class_Data _Class;
		Enum_Data _Enum;
		Alias_Data _Alias;
		Eval_Data _Eval;
		Trait_Data _Trait;
		Tag_Data _Tag;
		StaticVar_Data _StaticVar;
		ThreadVar_Data _ThreadVar;
		StaticArray_Data _StaticArr;
		FuncPtr_Data _FuncPtr;
		GenericClass_Data _GenericClass;
		GenericFunction_Data _GenericFunc;
		ForType_Data _ForType;
		NameSpace_Data _NameSapce;
	};
};
class ClassAssembly
{
public:
	Vector<Unique_ptr<AssemblyNode>> Classes;
	
	ClassAssembly() {}
	~ClassAssembly() {}
	ClassAssembly(ClassAssembly&& source) = default;
	ClassAssembly& operator=(ClassAssembly&& source) = default;
	
	inline Class_Data& AddClass(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<AssemblyNode>(ClassType::Class);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r.Get_ClassData();
	}
	inline Enum_Data& AddEnum(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<AssemblyNode>(ClassType::Enum);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r.Get_EnumData();
	}
	inline Alias_Data& AddAlias(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<AssemblyNode>(ClassType::Alias);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r.Get_AliasData();
	}
	inline Trait_Data& AddTrait(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<AssemblyNode>(ClassType::Trait);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r.Get_TraitData();
	}
	inline Tag_Data& AddTag(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<AssemblyNode>(ClassType::Tag);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r.Get_TagData();
	}
	inline FuncPtr_Data& AddFuncPtr(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<AssemblyNode>(ClassType::FuncPtr);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r.Get_FuncPtr();
	}
	inline GenericClass_Data& AddGenericClass(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<AssemblyNode>(ClassType::GenericClass);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r.Get_GenericClass();
	}
	inline GenericFunction_Data& AddGenericFunc(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<AssemblyNode>(ClassType::GenericFunction);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r.Get_GenericFunctionData();
	}	
	inline StaticArray_Data& AddStaticArray(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<AssemblyNode>(ClassType::StaticArray);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r.Get_StaticArray();
	}
	inline NameSpace_Data& AddNameSpace(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<AssemblyNode>(ClassType::NameSpace);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r.Get_NameSpace();
	}
	inline ForType_Data& AddForType(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<AssemblyNode>(ClassType::ForType);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r.Get_ForType();
	}	
	
	static void PushCopyClasses(const ClassAssembly& source, ClassAssembly& Out);

	static Optional<ReflectionCustomTypeID> GetReflectionTypeID(const AssemblyNode* Item);

	AssemblyNode* Find_Node(const String& Name, const String& Scope ="")
	{
		return Find_Node((String_view)Name, (String_view)Scope);
	}
	AssemblyNode* Find_Node(const String_view& Name, const String_view& Scope="")
	{
		for (auto& Item : Classes)
		{
			if (ScopeHelper::GetNameFromFullName(Item->Name) == Name 
				|| Item->FullName == Name)
			{
				return Item.get();
			}
		}
		return nullptr;
	}
	AssemblyNode* Find_Node(ClassType Type,const String& Name, const String& Scope = "")
	{
		return Find_Node(Type,(String_view)Name, (String_view)Scope);
	}
	AssemblyNode* Find_Node(ClassType Type,const String_view& Name = "", const String_view& Scope = "")
	{
		auto v = ((const ClassAssembly*)this)->Find_Node(Type,Name,Scope);
		return *(AssemblyNode**)&v;
	}
	
	const AssemblyNode* Find_Node(const String& Name, const String& Scope = "") const
	{
		return Find_Node((String_view)Name, (String_view)Scope);
	}
	const AssemblyNode* Find_Node(const String_view& Name, const String_view& Scope = "") const
	{
		for (auto& Item : Classes)
		{
			if (ScopeHelper::GetNameFromFullName(Item->Name) == Name
				|| Item->FullName == Name)
			{
				return Item.get();
			}
		}
		return nullptr;
	}
	const AssemblyNode* Find_Node(ClassType Type, const String& Name, const String& Scope = "") const
	{
		return Find_Node(Type, (String_view)Name, (String_view)Scope);
	}
	const AssemblyNode* Find_Node(ClassType Type, const String_view& Name = "", const String_view& Scope = "") const
	{

		for (auto& Item : Classes)
		{
			if (Item->Get_Type() == Type) 
			{
				if (ScopeHelper::GetNameFromFullName(Item->Name) == Name
					|| Item->FullName == Name)
				{
					return Item.get();
				}
			}
		}
		return nullptr;
	}

	
	Class_Data* Get_GlobalObject_Class()
	{
		return  Find_Class((String)ScopeHelper::_globalAssemblyObject);
	}

	const Class_Data* Get_GlobalObject_Class() const
	{
		return  Find_Class((String)ScopeHelper::_globalAssemblyObject);
	}
	//
	const Class_Data* Find_Class(const String& Name, const String& Scope = "") const
	{
		return Find_Class((String_view)Name, (String_view)Scope);
	}
	const Class_Data* Find_Class(const String_view& Name, const String_view& Scope = "") const
	{
		auto v = ((ClassAssembly*)this)->Find_Class(Name,Scope);
		return *(const Class_Data**)&v;	
	}

	Class_Data* Find_Class(const String& Name, const String& Scope = "")
	{
		return Find_Class((String_view)Name, (String_view)Scope);
	}
	Class_Data* Find_Class(const String_view& Name, const String_view& Scope = "") 
	{
		String Tep = String(Name);
		Tep += Scope;
		for (auto& Item : Classes)
		{
			if (Item->Get_Type() == ClassType::Class)
			{
				if (ScopeHelper::GetNameFromFullName(Item->Name) == Name
					|| Item->FullName == Name
					|| Item->Name == Tep)
				{
					return &Item->Get_ClassData();
				}
			}
		}
		return nullptr;
	}

	ClassMethod* Find_Func(const String_view& FullName)
	{
		auto v = ((const ClassAssembly*)this)->Find_Func(FullName);
		return *(ClassMethod**)&v;
	}

	Optional<ClassMethod> Remove_Func(const String_view& FullName) 
	{
		for (auto& Item : Classes)
		{
			if (Item->Get_Type() == ClassType::Class)
			{
				auto& methods = Item->Get_ClassData().Methods;
				for (size_t i = 0; i < methods.size(); i++)
				{
					auto& Item2 = methods[i];

					if (Item2.DecorationName == FullName
						|| Item2.FullName == FullName)
					{
						auto t = std::move(Item2);
						return t;
					}
				}
			}
		}
		return {};
	}
	void Remove_NullFunc() 
	{
		for (auto& Item : Classes)
		{
			if (Item->Get_Type() == ClassType::Class)
			{
				auto& methods = Item->Get_ClassData().Methods;
	
				methods.erase(std::remove_if(methods.begin(), methods.end(), [](ClassMethod& Item)
					{
						return Item.DecorationName.empty() && Item.FullName.empty();
					}), methods.end());
			}
		}
	}

	const ClassMethod* Find_Func(const String_view& FullName) const
	{
		for (auto& Item : Classes)
		{
			if (Item->Get_Type() == ClassType::Class)
			{
				for (auto& Item2 : Item->Get_ClassData().Methods)
				{
					if (Item2.DecorationName == FullName
						|| Item2.FullName == FullName)
					{
						return &Item2;
					}
				}

			}
		}
		return nullptr;
	}

	Vector<ClassMethod*>  Find_Funcs(const String_view& FullName)
	{
		Vector<ClassMethod*> r;
		for (auto& Item : Classes)
		{
			if (Item->Get_Type() == ClassType::Class)
			{
				for (auto& Item2 : Item->Get_ClassData().Methods)
				{
					if (Item2.DecorationName == FullName
						|| Item2.FullName == FullName)
					{
						r.push_back(&Item2);
					}
				}

			}
		}
		return r;
	}

	Vector<const ClassMethod*> Find_Funcs(const String_view& FullName) const
	{
		auto v = ((ClassAssembly*)this)->Find_Funcs(FullName);
		return *(Vector<const ClassMethod*>*)&v;
	}

	Vector<const ClassMethod*> Find_FuncsUsingName(const String_view& Name) const
	{
		Vector<const ClassMethod*> r;
		for (auto& Item : Classes)
		{
			if (Item->Get_Type() == ClassType::Class)
			{
				for (auto& Item2 : Item->Get_ClassData().Methods)
				{
					if (Item2.DecorationName == Name
						|| Item2.FullName == Name
						|| ScopeHelper::GetNameFromFullName(Item2.FullName) == Name)
					{
						r.push_back(&Item2);
					}
				}

			}
		}
		return r;
	}

	const AssemblyNode* Find_Node(ReflectionCustomTypeID TypeID) const;
	AssemblyNode* Find_Node(ReflectionCustomTypeID TypeID);


	const AssemblyNode* Find_Node(const ReflectionTypeInfo& Type) const;
	AssemblyNode* Find_Node(const ReflectionTypeInfo& Type);


	//may fail
	Optional<size_t> GetSize(const ReflectionTypeInfo& Type,bool Is32Bit) const;

	Optional<size_t> GetSize(const ClassMethod::Par& Type, bool Is32Bit) const;


	//Get the DefaultConstructor or directly does the operation if a Primitive.
	//if the first Optional is empty the operation failed
	struct OnDoDefaultConstructorCall
	{
		const ClassMethod* MethodToCall = nullptr;
		void* ThisPtr = nullptr;
	};

	struct OnMoveConstructorCall
	{
		const ClassMethod* MethodToCall = nullptr;
		void* ThisPtr = nullptr;
		void* OtherPtr = nullptr;
	};

	Optional<Optional<Vector<OnDoDefaultConstructorCall>>> CallDefaultConstructor(const ReflectionTypeInfo& Type, void* Object, bool Is32Bit) const;
	
	Optional<Optional<Vector<OnDoDefaultConstructorCall>>> CallDefaultConstructor(const ClassMethod::Par& Type, void* Object, bool Is32Bit) const;

	//Get the CopyConstructor or directly does the operation if a Primitive.
	//if the first Optional is empty the operation failed
	Optional<Optional<Vector<OnMoveConstructorCall>>> CallCopyConstructor(const ReflectionTypeInfo& Type, void* Source, void* Output, bool Is32Bit) const;

	//Get the MoveConstructor or directly does the operation if a Primitive.
	//if the first Optional is empty the operation failed
	Optional<Optional<Vector<OnMoveConstructorCall>>> CallMoveConstructor(const ReflectionTypeInfo& Type,void* Source, void* Output, bool Is32Bit) const;

	//Get the Destructor or directly does the operation if a Primitive.
	//if the first Optional is empty the operation failed
	Optional<Optional<Vector<OnDoDefaultConstructorCall>>> CallDestructor(const ReflectionTypeInfo& Type, void* Object, bool Is32Bit) const;

	struct ParsedValue
	{
		TypedRawReflectionData Value;
		BytesPtr other;
		Vector<OnDoDefaultConstructorCall> Calls;
		
		void* GetData()
		{
			return Value._Data.Get_Data();
		}
		ReflectionTypeInfo GetType()
		{
			return Value._Type;
		}
	};
	static Optional<ParsedValue> ParseToValue(const String_view txt, const ClassAssembly& Assembly, Vector<ReflectionTypeInfo> Hints);

	static String ToString(const ClassMethod::Par& data, const ClassAssembly& Assembly);
	static String ToString(const ReflectionTypeInfo& data, const ClassAssembly& Assembly);
	static String ToString(const TypedRawReflectionData& data, const ClassAssembly& Assembly,bool is32mode= sizeof(void*) == 4);
	static String ToStringJson(const TypedRawReflectionData& data,const ClassAssembly& Assembly, bool is32mode = sizeof(void*) == 4);

	enum class CompareType_t
	{
		Identical,
		Similar,
		CanTypeCoercion,
		TooDifferent,
	};
	static CompareType_t CompareType(
		const ReflectionTypeInfo& TypeA, const ClassAssembly& TypeAAssembly, 
		const ReflectionTypeInfo& TypeB, const ClassAssembly& TypeBAssembly);
	
	//Convert one type to Another even if not the same type int to char,int to int32,enum to int etc
	//if the first Optional is empty the operation failed
	//used in hot reloading
	static Optional<Optional<Vector<OnMoveConstructorCall>>> DoTypeCoercion(
		const ReflectionTypeInfo& TypeSource,
		void* Source,
		const ClassAssembly& SourceAssembly,
		const ReflectionTypeInfo& TypeOutput,
		void* Output,
		const ClassAssembly& OutputAssembly, bool Is32Bit);

	inline static bool IsJust(const ReflectionTypeInfo& Type) 
	{
		if (Type.IsAddress() 
			&& Type.IsAddressArray()
			&& Type.IsDynamicTrait()
			&& Type.IsMovedType())
		{
			return false;
		}
		return true;
	}
	inline ReflectionTypeInfo CollapseAliases(const ReflectionTypeInfo& Type) const
	{
		return Type;
	}

	//Is Char or Uf8,Uf16,Uf32 etc also includes type aliases
	// use IsJust to exclude pointer and move's etc.
	inline bool IsChar_t(const ReflectionTypeInfo& TypeA) const
	{
		switch (TypeA._Type)
		{
		case ReflectionTypes::Char:
		case ReflectionTypes::Uft8:
		case ReflectionTypes::Uft16:
		case ReflectionTypes::Uft32:
			return true;
		default:
			break;
		}
		return false;
	}


	//Is uint8 or uint16,utint32 etc also includes type aliases
	// use IsJust to exclude pointer and move's etc.
	inline bool IsUint_t(const ReflectionTypeInfo& TypeA) const
	{
		switch (TypeA._Type)
		{
		case ReflectionTypes::uInt8:
		case ReflectionTypes::uInt16:
		case ReflectionTypes::uInt32:
		case ReflectionTypes::uInt64:
		case ReflectionTypes::uIntPtr:
			return true;
		default:
			break;
		}
		return false;
	}

	//Is uint8 or uint16,utint32 etc also includes type aliases
	// use IsJust to exclude pointer and move's etc.
	inline bool IsSint_t(const ReflectionTypeInfo& TypeA) const
	{
		switch (TypeA._Type)
		{
		case ReflectionTypes::sInt8:
		case ReflectionTypes::sInt16:
		case ReflectionTypes::sInt32:
		case ReflectionTypes::sInt64:
		case ReflectionTypes::sIntPtr:
			return true;
		default:
			break;
		}
		return false;
	}

	//Is float32 or float64 etc also includes type aliases
	// use IsJust to exclude pointer and move's etc.
	inline bool Isfloat_t(const ReflectionTypeInfo& Type) const
	{
		switch (Type._Type)
		{
		case ReflectionTypes::float32:
		case ReflectionTypes::float64:
			return true;
		default:
			break;
		}
		return false;
	}

	//Is uint or sint etc also includes type aliases
	// use IsJust to exclude pointer and move's etc.
	inline bool IsAnyint_t(const ReflectionTypeInfo& Type) const
	{
		return IsSint_t(Type) || IsUint_t(Type);
	}


	// includes type aliases
	// use IsJust to exclude pointer and move's etc.
	inline bool IsPrimitve(const ReflectionTypeInfo& Type) const
	{
		if (Type._Type == ReflectionTypes::Bool)
		{
			return true;
		}
		return IsAnyint_t(Type) || IsChar_t(Type) || Isfloat_t(Type);
	}

	struct InfoVec2_t
	{
		ReflectionTypeInfo XAndYType;
	};
	using InfoVec3_t = InfoVec2_t;

	// includes type aliases
	// if it's looks like an Vec2 then it is an Vec2
	Optional<InfoVec2_t> IsVec2_t(const ReflectionTypeInfo& Type) const;

	// includes type aliases
	// if it's looks like an Vec3 then it is an Vec3
	Optional<InfoVec3_t> IsVec3_t(const ReflectionTypeInfo& Type) const;

	// includes type aliases
	inline Optional<InfoVec2_t> IsVec2f_t(const ReflectionTypeInfo& Type) const
	{
		auto V = IsVec2_t(Type);
		if (V.has_value())
		{
			if (IsJust(V.value().XAndYType) && Isfloat_t(V.value().XAndYType))
			{
				return V.value();
			}
		}
		return {};
	}

	// includes type aliases
	inline Optional<InfoVec2_t> IsVec2i_t(const ReflectionTypeInfo& Type) const
	{
		auto V = IsVec2_t(Type);
		if (V.has_value())
		{
			if (IsJust(V.value().XAndYType) && IsAnyint_t(V.value().XAndYType))
			{
				return V.value();
			}
		}
		return {};
	}

	// includes type aliases
	inline Optional<InfoVec2_t> IsVec3f_t(const ReflectionTypeInfo& Type) const
	{
		auto V = IsVec3_t(Type);
		if (V.has_value())
		{
			if (IsJust(V.value().XAndYType) && Isfloat_t(V.value().XAndYType))
			{
				return V.value();
			}
		}
		return {};
	}

	// includes type aliases
	inline Optional<InfoVec2_t> IsVec3i_t(const ReflectionTypeInfo& Type) const
	{
		auto V = IsVec3_t(Type);
		if (V.has_value())
		{
			if (IsJust(V.value().XAndYType) && IsAnyint_t(V.value().XAndYType))
			{
				return V.value();
			}
		}
		return {};
	}


	//should be cached
	struct InfoVector_t
	{
		ReflectionTypeInfo ElementType;

		const ClassMethod* Data_Method = nullptr;//|Data[imut this&] -> ElementType[&];
		const ClassMethod* Size_Method = nullptr;//|Size[imut this&] -> uintptr;
		const ClassMethod* Capacity_Method = nullptr;//|Capacity[imut this&] -> uintptr;

		const ClassMethod* Resize_Method = nullptr;//|Resize[this&,uintptr Size] -> void;
		const ClassMethod* Reserve_Method = nullptr;//|Reserve[this&,uintptr Size] -> void;
		const ClassMethod* Clear_Method = nullptr;//|Clear[this&] -> void;

		//if Element is copyable
		const ClassMethod* Push_copy_Method = nullptr;//|Push[this&,imut ElementType& Item] -> void;
		const ClassMethod* Push_moved_Method = nullptr;//|Push[this&,moved ElementType Item] -> void;
		const ClassMethod* Pop_Method = nullptr;//|Pop[this&] -> ElementType;

		const ClassMethod* Remove_Method = nullptr;//|Remove[this&,uintptr Index] -> ElementType;

		//if Element is copyable
		const ClassMethod* Insert_Copy_Method = nullptr;//|Insert[this&,uintptr Index,imut ElementType& Item] -> void;
		const ClassMethod* Insert_Moved_Method = nullptr;//|Insert[this&,uintptr Index,moved ElementType Item] -> void;
	};
	// includes type aliases
	Optional<InfoVector_t> IsVector_t(const ReflectionTypeInfo& Type) const;

	struct InfoOptional_t
	{
		ReflectionTypeInfo OptType;
	};
	// includes type aliases
	Optional<InfoOptional_t> IsOptional_t(const ReflectionTypeInfo& Type) const;

	struct InfoResult_t
	{
		ReflectionTypeInfo ValType;
		ReflectionTypeInfo ErrType;
	};
	// includes type aliases
	Optional<InfoResult_t> IsResult_t(const ReflectionTypeInfo& Type) const;

	struct InfoString_t
	{
		ReflectionTypeInfo ElementType;//is any of the char_t types

		const ClassMethod* Data_Method = nullptr;//|Data[imut this&] -> ElementType[&];
		const ClassMethod* Size_Method = nullptr;//|Size[imut this&] -> uintptr;
		const ClassMethod* Capacity_Method = nullptr;//|Capacity[imut this&] -> uintptr;

		const ClassMethod* Resize_Method = nullptr;//|Resize[this&,uintptr Size] -> void;
		const ClassMethod* Reserve_Method = nullptr;//|Reserve[this&,uintptr Size] -> void;
		const ClassMethod* Clear_Method = nullptr;//|Clear[this&] -> void;

		const ClassMethod* Push_Method = nullptr;//|Push[this&,ElementType Item] -> void;
		const ClassMethod* Pop_Method = nullptr;//|Pop[this&] -> ElementType;

		const ClassMethod* Remove_Method = nullptr;//|Remove[this&,uintptr Index] -> ElementType;

		const ClassMethod* Insert_Method = nullptr;//|Insert[this&,uintptr Index,ElementType Item] -> void;
	};
	// includes type aliases
	Optional<InfoString_t> IsString_t(const ReflectionTypeInfo& Type) const;

	struct InfoStringView_t
	{
		ReflectionTypeInfo ElementType;//is any of the char_t types
	};
	// includes type aliases
	Optional<InfoStringView_t> IsStringView_t(const ReflectionTypeInfo& Type) const;

	struct InfoSpan_t
	{
		ReflectionTypeInfo ElementType;
	};
	// includes type aliases
	Optional<InfoSpan_t> IsSpan_t(const ReflectionTypeInfo& Type) const;


	struct InfoMap_t
	{
		ReflectionTypeInfo KeyType;
		ReflectionTypeInfo MappedType;
	};
	// includes type aliases
	Optional<InfoMap_t> IsMap_t(const ReflectionTypeInfo& Type) const;


	struct InfoArray_t
	{
		ReflectionTypeInfo ElementType;
		const ClassMethod* Data_Method = nullptr;//|Data[imut this&] -> ElementType[&];
		const ClassMethod* Size_Method = nullptr;//|Size[imut this&] -> uintptr;
	};
	// includes type aliases
	Optional<InfoArray_t> IsArray_t(const ReflectionTypeInfo& Type) const;
};
UCodeLangEnd