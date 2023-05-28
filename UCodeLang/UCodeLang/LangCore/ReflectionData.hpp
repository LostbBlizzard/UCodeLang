#pragma once
#include "../LangCore.hpp"
#include "TypeNames.hpp"
#include "ScopeHelper.hpp"
UCodeLangStart


class ReflectionTypeInfo
{
public:
	String FullNameType;
};

class ClassField
{
public:
	String Name;
	ReflectionTypeInfo Type;
	size_t offset=NullAddress;
};
class AttributeData
{
public:
	String Name;
};
class ClassMethod
{
public:
	String FullName;
	String DecorationName;
	
	
	ReflectionTypeInfo RetType;
	Vector<ReflectionTypeInfo> ParsType;

	Vector<AttributeData> Attributes;
};
class EnumValues
{
public:
	using State_t = UInt8;
	enum class State : State_t
	{
		Ok,
		Bad,
	};
	String Name;
	State _State;
	size_t Value;
};


using ClassType_t = UInt8;
enum class ClassType :ClassType_t
{
	Null,
	Class,
	Enum,
	Alias,
};
using EnumSizez_t = UInt8;
class EnumSizez
{
public:
	enum : EnumSizez_t
	{
		Null = 0,
		int8,
		int16,
		int32,
		int64,
	};
	inline static EnumSizez FromSize(size_t V)
	{
		switch (V)
		{
		case sizeof(Int8):return { EnumSizez::int8 };
		case sizeof(Int16):return { EnumSizez::int16 };
		case sizeof(Int32):return{ EnumSizez::int32 };
		case sizeof(Int64):return{ EnumSizez::int64 };
		default:return{ EnumSizez::Null };
		}
	}
	EnumSizez_t V = EnumSizez::Null;

	operator EnumSizez_t() { return V; };
	EnumSizez(EnumSizez_t Val) :V(Val)
	{

	}
};

class ClassData
{

public:
	
	struct Class_Data
	{
		size_t Size = 0;
		Vector<AttributeData> Attributes;
		Vector<ClassField> Fields;
		Vector<ClassMethod> Methods;
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
		inline const AttributeData* GetAttribute(const String& Name)
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
		EnumSizez Size = { EnumSizez::Null };
		Vector<EnumValues> Values;
	};
	struct Alias_Data
	{
		ReflectionTypeInfo Type;
	};
	
	String Name;
	String FullName;
	
	ClassType Type = ClassType::Null;
	//union 
	struct 
	{
		Class_Data _Class;
		Enum_Data _Enum;
		Alias_Data _Alias;
	};
	ClassData(ClassType type) : Type(type)
	{
		_Class = Class_Data();
		_Enum = Enum_Data();
		_Alias = Alias_Data();
	}
	~ClassData() 
	{
		
	}
};
class ClassAssembly
{
public:
	Vector<Unique_ptr<ClassData>> Classes;
	inline ClassData& AddClass(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<ClassData>(ClassType::Class);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r;
	}
	inline ClassData& AddEnum(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<ClassData>(ClassType::Enum);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r;
	}
	inline ClassData& AddAlias(const String& Name, const String& FullName = "")
	{
		auto V = std::make_unique<ClassData>(ClassType::Alias);
		Classes.push_back(std::move(V));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r;
	}
	

	inline ClassData::Class_Data& Add_Class(const String& Name, const String& FullName = "")
	{
		auto& r = AddAlias(Name, FullName);
		return r._Class;
	}
	inline ClassData::Enum_Data& Add_Enum(const String& Name, const String& FullName = "")
	{
		auto& r = AddAlias(Name, FullName);
		return r._Enum;
	}
	inline ClassData::Alias_Data& Add_Alias(const String& Name, const String& FullName = "")
	{
		auto& r = AddAlias(Name,FullName);
		return r._Alias;
	}
	inline void Clear()
	{
		Classes.clear();
	}
	ClassAssembly() {}
	~ClassAssembly() 
	{
		Clear();
	}
	ClassAssembly(ClassAssembly&& source) = default;
	ClassAssembly& operator=(ClassAssembly&& source) = default;
	static void PushCopyClasses(const ClassAssembly& source, ClassAssembly& Out)
	{
		for (auto& Item : source.Classes)
		{
			Out.Classes.push_back(std::make_unique<ClassData>(*Item));
		}
	}
	ClassData* Find_Class(const String& Name, const String& Scope ="")
	{
		return Find_Class((String_view)Name, (String_view)Scope);
	}
	ClassData* Find_Class(const String_view& Name, const String_view& Scope="")
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
	ClassData* Get_GlobalObject_Class()
	{
		return  Find_Class((String)ScopeHelper::_globalAssemblyObject);
	}

	const ClassData* Get_GlobalObject_Class()const
	{
		return  Find_Class((String)ScopeHelper::_globalAssemblyObject);
	}
	const ClassData* Find_Class(const String& Name, const String& Scope = "") const
	{
		return Find_Class((String_view)Name, (String_view)Scope);
	}
	const ClassData* Find_Class(const String_view& Name, const String_view& Scope = "") const
	{
		String Tep = String(Name);
		Tep += Scope;
		for (auto& Item : Classes)
		{
			if (ScopeHelper::GetNameFromFullName(Item->Name) == Name
		     || Item->FullName == Name
			 || Item->Name == Tep
			 || Item->FullName ==  Tep)
			{
				return Item.get();
			}
		}
		return nullptr;
	}
};
UCodeLangEnd