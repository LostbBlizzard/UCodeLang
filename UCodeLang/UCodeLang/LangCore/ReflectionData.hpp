#pragma once
#include "../LangCore.hpp"
#include "TypeNames.hpp"
#include "ScopeHelper.hpp"
UCodeLangStart


class ClassField
{
public:
	String Name;
	String FullNameType;
	size_t offset;
};
class AttributeData
{
public:
	String Name;
};
class ClassMethod
{
public:
	class ClassMethodPar
	{
	public:
		String FullNameType;
	};
	Vector<AttributeData> Attributes;
	String FullName;
	Vector<ClassMethodPar> ParsType;
	ClassMethodPar RetType;
};
class EnumValues
{
public:
	enum class State : UInt8
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
		const ClassMethod* Get_ClassCopy() const
		{
			return Get_ClassMethod(ClassCopyFunc);
		}//This May be null.

		const ClassMethod* Get_ClassMove() const
		{
			return Get_ClassMethod(ClassMoveFunc);
		}//This May be null.
		const ClassMethod* Get_ClassSwap() const
		{
			return Get_ClassMethod(ClassSwapFunc);
		}//This May be null.
		const ClassMethod* Get_ClassDestructor() const
		{
			return Get_ClassMethod(ClassDestructorFunc);
		}//This May be null.

		const ClassMethod* Get_ClassMethod(const String& Name) const
		{
			String TepString;
			for (auto& Item : Methods)
			{
				if (Item.FullName == Name)
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
		String StringValue;
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
	Vector<ClassData*> Classes;
	inline ClassData& AddClass(const String& Name, const String& FullName = "")
	{
		Classes.push_back(new ClassData(ClassType::Class));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r;
	}
	inline ClassData& AddEnum(const String& Name, const String& FullName = "")
	{
		Classes.push_back(new ClassData(ClassType::Enum));
		auto& r = *Classes.back();
		r.Name = Name;
		r.FullName = FullName;
		return r;
	}
	inline ClassData& AddAlias(const String& Name, const String& FullName = "")
	{
		Classes.push_back(new ClassData(ClassType::Alias));
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
		for (auto Item : Classes)
		{
			delete Item;
		}
	
		Classes.clear();
	}
	ClassAssembly() {}
	~ClassAssembly() 
	{
		Clear();
	}
	ClassAssembly(ClassAssembly&& source) noexcept
	{
		for (auto Item : source.Classes)
		{
			Classes.push_back(Item);
		}

		source.Classes.clear();
	}
	static void PushCopyClasses(const ClassAssembly& source, ClassAssembly& Out)
	{
		for (auto Item : source.Classes)
		{
			Out.Classes.push_back(new ClassData(*Item));
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
			if (Item->Name == Name || Item->FullName == Name)
			{
				return Item;
			}
		}
		return nullptr;
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
			if (Item->Name == Name 
		     || Item->FullName == Name
			 || Item->Name == Tep
			 || Item->FullName ==  Tep)
			{
				return Item;
			}
		}
		return nullptr;
	}
};
UCodeLangEnd