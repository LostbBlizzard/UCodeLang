#pragma once
#include "../LangCore.hpp"
#include "TypeNames.hpp"
#include "ScopeHelper.hpp"
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
};

class ClassField
{
public:
	String Name;
	ReflectionTypeInfo Type;
	size_t offset=NullAddress;
};
class UsedTagValueData
{
public:
	String Name;
	~UsedTagValueData()
	{

	}
};
class ClassMethod
{
public:
	String FullName;
	String DecorationName;
	
	
	ReflectionTypeInfo RetType;
	Vector<ReflectionTypeInfo> ParsType;

	Vector<UsedTagValueData> Attributes;
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
			throw std::exception("bad type cast");
		}

		return (T*)Bytes.get();
	}

	template<typename T>
	inline const T* Get_DataAs() const
	{
		if (sizeof(T) != Size)
		{
			throw std::exception("bad type cast");
		}

		return (T*)Bytes.get();
	}
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
	GenericFuncion,
};
struct Class_Data
{
	ReflectionCustomTypeID TypeID = {};
	size_t Size = 0;
	Vector<UsedTagValueData> Attributes;
	Vector<ClassField> Fields;
	Vector<ClassMethod> Methods;
	Vector<InheritedTrait_Data> InheritedTypes;
	inline bool HasAttribute(const String& Name)
	{
		for (const auto& Item : Attributes)
		{
			if (Item.Name == Name)
			{
				return true;
			}
		}
		return true;
	}
	inline const UsedTagValueData* GetAttribute(const String& Name)
	{
		for (const auto& Item : Attributes)
		{
			if (Item.Name == Name)
			{
				return &Item;
			}
		}
		return nullptr;
	}

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

	const ClassMethod* Get_ClassMethod(const String& Name) const
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
};
struct Enum_Data
{
	ReflectionCustomTypeID TypeID;
	ReflectionTypeInfo BaseType;
	Optional<ReflectionCustomTypeID> EnumVariantUnion;//is class

	Vector<EnumValues> Values;
};
struct Alias_Data
{
	ReflectionTypeInfo Type;
	Optional<ReflectionCustomTypeID> HardAliasTypeID;
};
struct Eval_Data
{
	TypedRawReflectionData Value;

};
struct Tag_Data
{
	~Tag_Data()
	{

	}
};
struct Trait_Data
{
	ReflectionCustomTypeID TypeID = {};
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
	~StaticArray_Data()
	{

	}
};
struct FuncPtr_Data
{
	~FuncPtr_Data()
	{

	}
};
struct GenericClass_Data
{
	~GenericClass_Data()
	{

	}
};
struct GenericFuncion_Data
{
	~GenericFuncion_Data()
	{

	}
};
class AssemblyNode
{

public:
	String Name;
	String FullName;
	
	AssemblyNode(ClassType type);
	AssemblyNode(AssemblyNode&& node)
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
		if (Type != ClassType::Class) {throw std::exception("bad access");}
		return _Class;
	}
	Enum_Data& Get_EnumData()
	{
		if (Type != ClassType::Enum) { throw std::exception("bad access"); }
		return _Enum;
	}
	Alias_Data& Get_AliasData()
	{
		if (Type != ClassType::Alias) { throw std::exception("bad access"); }
		return _Alias;
	}
	Eval_Data& Get_EvalData()
	{
		if (Type != ClassType::Eval) { throw std::exception("bad access"); }
		return  _Eval;
	}
	Trait_Data& Get_TraitData()
	{
		if (Type != ClassType::Trait) { throw std::exception("bad access"); }
		return  _Trait;
	}
	Tag_Data& Get_TagData()
	{
		if (Type != ClassType::Tag) { throw std::exception("bad access"); }
		return  _Tag;
	}
	StaticVar_Data& Get_StaticVar()
	{
		if (Type != ClassType::StaticVarable) { throw std::exception("bad access"); }
		return  _StaticVar;
	}
	ThreadVar_Data& Get_ThreadVar()
	{
		if (Type != ClassType::ThreadVarable) { throw std::exception("bad access"); }
		return _ThreadVar;
	}
	StaticArray_Data& Get_StaticArray() 
	{
		if (Type != ClassType::StaticArray) { throw std::exception("bad access"); }
		return _StaticArr;
	}
	FuncPtr_Data& Get_FuncPtr() 
	{
		if (Type != ClassType::FuncPtr) { throw std::exception("bad access"); }
		return _FuncPtr;
	}
	GenericClass_Data& Get_GenericClass() 
	{
		if (Type != ClassType::GenericClass) { throw std::exception("bad access"); }
		return _GenericClass;
	}
	GenericFuncion_Data& Get_GenericFuncionData() 
	{
		if (Type != ClassType::GenericFuncion) { throw std::exception("bad access"); }
		return _GenericFunc;
	}
	
	const Class_Data& Get_ClassData() const
	{
		if (Type != ClassType::Class) { throw std::exception("bad access"); }
		return _Class;
	}
	const Enum_Data& Get_EnumData()const
	{
		if (Type != ClassType::Enum) { throw std::exception("bad access"); }
		return _Enum;
	}
	const Alias_Data& Get_AliasData()const
	{
		if (Type != ClassType::Alias) { throw std::exception("bad access"); }
		return _Alias;
	}
	const Eval_Data& Get_EvalData()const
	{
		if (Type != ClassType::Eval) { throw std::exception("bad access"); }
		return  _Eval;
	}
	const Trait_Data& Get_TraitData()const
	{
		if (Type != ClassType::Trait) { throw std::exception("bad access"); }
		return  _Trait;
	}
	const Tag_Data& Get_TagData()const
	{
		if (Type != ClassType::Tag) { throw std::exception("bad access"); }
		return  _Tag;
	}
	const StaticVar_Data& Get_StaticVar()const
	{
		if (Type != ClassType::StaticVarable) { throw std::exception("bad access"); }
		return  _StaticVar;
	}
	const ThreadVar_Data& Get_ThreadVar() const
	{
		if (Type != ClassType::ThreadVarable) { throw std::exception("bad access"); }
		return _ThreadVar;
	}
	const StaticArray_Data& Get_StaticArray() const
	{
		if (Type != ClassType::StaticArray) { throw std::exception("bad access"); }
		return _StaticArr;
	}
	const FuncPtr_Data& Get_FuncPtr() const
	{
		if (Type != ClassType::FuncPtr) { throw std::exception("bad access"); }
		return _FuncPtr;
	}
	const GenericClass_Data& Get_GenericClass() const
	{
		if (Type != ClassType::GenericClass) { throw std::exception("bad access"); }
		return _GenericClass;
	}
	const GenericFuncion_Data& Get_GenericFuncionData() const
	{
		if (Type != ClassType::GenericFuncion) { throw std::exception("bad access"); }
		return _GenericFunc;
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
		GenericFuncion_Data _GenericFunc;
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
	static void PushCopyClasses(const ClassAssembly& source, ClassAssembly& Out);
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


	const AssemblyNode* Find_Node(ReflectionCustomTypeID TypeID) const;
	AssemblyNode* Find_Node(ReflectionCustomTypeID TypeID);


	const AssemblyNode* Find_Node(const ReflectionTypeInfo& Type) const;
	AssemblyNode* Find_Node(const ReflectionTypeInfo& Type);


	//may fail
	Optional<size_t> GetSize(const ReflectionTypeInfo& Type,bool Is32Bit) const;
};
UCodeLangEnd