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
	size_t offset=NullAddress;
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
			return Other.IsOutPar == Other.IsOutPar &&
				Other.Type == Other.Type;
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
	bool IsThisFuncion = false;

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
	GenericFuncion,
};
struct Class_Data
{
	ReflectionCustomTypeID TypeID = {};
	size_t Size = 0;
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
	Vector<const ClassMethod*> Get_ClassMethods(const String& Name) const
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
	ReflectionCustomTypeID TypeID = {};
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
	ReflectionTypeInfo RetType;
	Vector<ClassMethod::Par> ParsType;
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

struct GenericBase_Data
{
	String Implementation;
};

struct GenericFuncion_Data
{
	GenericBase_Data Base;
	~GenericFuncion_Data()
	{

	}
};
struct GenericEnum_Data
{
	GenericBase_Data Base;
	~GenericEnum_Data()
	{

	}
};
struct GenericAlias_Data
{
	GenericBase_Data Base;
	~GenericAlias_Data()
	{

	}
};
struct GenericTrait_Data
{
	GenericBase_Data Base;
	~GenericTrait_Data()
	{

	}
};
struct GenericTag_Data
{
	GenericBase_Data Base;
	~GenericTag_Data()
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
		if (Type != ClassType::Class) {UCodeLangThrowException("bad access");}
		return _Class;
	}
	Enum_Data& Get_EnumData()
	{
		if (Type != ClassType::Enum) { UCodeLangThrowException("bad access"); }
		return _Enum;
	}
	Alias_Data& Get_AliasData()
	{
		if (Type != ClassType::Alias) { UCodeLangThrowException("bad access"); }
		return _Alias;
	}
	Eval_Data& Get_EvalData()
	{
		if (Type != ClassType::Eval) { UCodeLangThrowException("bad access"); }
		return  _Eval;
	}
	Trait_Data& Get_TraitData()
	{
		if (Type != ClassType::Trait) { UCodeLangThrowException("bad access"); }
		return  _Trait;
	}
	Tag_Data& Get_TagData()
	{
		if (Type != ClassType::Tag) { UCodeLangThrowException("bad access"); }
		return  _Tag;
	}
	StaticVar_Data& Get_StaticVar()
	{
		if (Type != ClassType::StaticVarable) { UCodeLangThrowException("bad access"); }
		return  _StaticVar;
	}
	ThreadVar_Data& Get_ThreadVar()
	{
		if (Type != ClassType::ThreadVarable) { UCodeLangThrowException("bad access"); }
		return _ThreadVar;
	}
	StaticArray_Data& Get_StaticArray() 
	{
		if (Type != ClassType::StaticArray) { UCodeLangThrowException("bad access"); }
		return _StaticArr;
	}
	FuncPtr_Data& Get_FuncPtr() 
	{
		if (Type != ClassType::FuncPtr) { UCodeLangThrowException("bad access"); }
		return _FuncPtr;
	}
	GenericClass_Data& Get_GenericClass() 
	{
		if (Type != ClassType::GenericClass) { UCodeLangThrowException("bad access"); }
		return _GenericClass;
	}
	GenericFuncion_Data& Get_GenericFuncionData() 
	{
		if (Type != ClassType::GenericFuncion) { UCodeLangThrowException("bad access"); }
		return _GenericFunc;
	}
	
	const Class_Data& Get_ClassData() const
	{
		if (Type != ClassType::Class) { UCodeLangThrowException("bad access"); }
		return _Class;
	}
	const Enum_Data& Get_EnumData()const
	{
		if (Type != ClassType::Enum) { UCodeLangThrowException("bad access"); }
		return _Enum;
	}
	const Alias_Data& Get_AliasData()const
	{
		if (Type != ClassType::Alias) { UCodeLangThrowException("bad access"); }
		return _Alias;
	}
	const Eval_Data& Get_EvalData()const
	{
		if (Type != ClassType::Eval) { UCodeLangThrowException("bad access"); }
		return  _Eval;
	}
	const Trait_Data& Get_TraitData()const
	{
		if (Type != ClassType::Trait) { UCodeLangThrowException("bad access"); }
		return  _Trait;
	}
	const Tag_Data& Get_TagData()const
	{
		if (Type != ClassType::Tag) { UCodeLangThrowException("bad access"); }
		return  _Tag;
	}
	const StaticVar_Data& Get_StaticVar()const
	{
		if (Type != ClassType::StaticVarable) { UCodeLangThrowException("bad access"); }
		return  _StaticVar;
	}
	const ThreadVar_Data& Get_ThreadVar() const
	{
		if (Type != ClassType::ThreadVarable) { UCodeLangThrowException("bad access"); }
		return _ThreadVar;
	}
	const StaticArray_Data& Get_StaticArray() const
	{
		if (Type != ClassType::StaticArray) { UCodeLangThrowException("bad access"); }
		return _StaticArr;
	}
	const FuncPtr_Data& Get_FuncPtr() const
	{
		if (Type != ClassType::FuncPtr) { UCodeLangThrowException("bad access"); }
		return _FuncPtr;
	}
	const GenericClass_Data& Get_GenericClass() const
	{
		if (Type != ClassType::GenericClass) { UCodeLangThrowException("bad access"); }
		return _GenericClass;
	}
	const GenericFuncion_Data& Get_GenericFuncionData() const
	{
		if (Type != ClassType::GenericFuncion) { UCodeLangThrowException("bad access"); }
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

	ClassMethod* Find_Func(const String_view& FullName)
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
		Vector<const ClassMethod*> r;
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
	
	//Convert one type to Another even if not the same type int to char,int to int32,enum to int ect
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
	// use IsJust to exclude pointer and move's ect.
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


	//should be chached
	struct InfoVector_t
	{
		ReflectionTypeInfo ElementType;

		const ClassMethod* Data_Method = nullptr;//|Data[umut this&] -> ElementType[&];
		const ClassMethod* Size_Method = nullptr;//|Size[umut this&] -> uintptr;
		const ClassMethod* Capacity_Method = nullptr;//|Capacity[umut this&] -> uintptr;

		const ClassMethod* Resize_Method = nullptr;//|Resize[this&,uintptr Size] -> void;
		const ClassMethod* Reserve_Method = nullptr;//|Reserve[this&,uintptr Size] -> void;
		const ClassMethod* Clear_Method = nullptr;//|Clear[this&] -> void;

		//if Element is copyable
		const ClassMethod* Push_copy_Method = nullptr;//|Push[this&,umut ElementType& Item] -> void;
		const ClassMethod* Push_moved_Method = nullptr;//|Push[this&,moved ElementType Item] -> void;
		const ClassMethod* Pop_Method = nullptr;//|Pop[this&] -> ElementType;

		const ClassMethod* Remove_Method = nullptr;//|Remove[this&,uintptr Index] -> ElementType;

		//if Element is copyable
		const ClassMethod* Insert_Copy_Method = nullptr;//|Insert[this&,uintptr Index,umut ElementType& Item] -> void;
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

		const ClassMethod* Data_Method = nullptr;//|Data[umut this&] -> ElementType[&];
		const ClassMethod* Size_Method = nullptr;//|Size[umut this&] -> uintptr;
		const ClassMethod* Capacity_Method = nullptr;//|Capacity[umut this&] -> uintptr;

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
};
UCodeLangEnd