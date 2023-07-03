#pragma once
#include "../LangCore.hpp"
#include "TypeNames.hpp"
#include "ScopeHelper.hpp"
#include "../LangCore/DataType/BinaryVectorMap.hpp"
#include "ReflectionData.hpp"
#include "BitMaker.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart

struct UDebugSetFile
{
	String FileName;
	UAddress ForIns = 0;
}; 
struct UDebugSetLineNumber
{
	size_t LineNumber;
	UAddress ForIns = 0;
};
struct UDebugSetVarableLoc
{
	struct UMaped{};
	struct StackSub { UAddress offset = 0; };
	struct StackAdd { UAddress offset = 0; };
	struct Static { UAddress offset = 0; };
	struct Thread { UAddress offset = 0; };



	String VarableFullName;
	Variant<UMaped, RegisterID, StackSub, StackAdd, Static, Thread> Type;
	UAddress ForIns = 0;

	using TypeV_t = Byte;
	enum class TypeV :TypeV_t
	{
		UMaped,
		RegisterID,
		StackSub,
		StackAdd,
		Static,
		Thread,
	};
	inline TypeV Get_Type() const
	{
		if (Type.Is<UMaped>())
		{
			return TypeV::UMaped;
		}
		else if (Type.Is<RegisterID>())
		{
			return TypeV::RegisterID;
		}
		else if (Type.Is<StackSub>())
		{
			return TypeV::StackSub;
		}
		else if (Type.Is<UDebugSetVarableLoc>())
		{
			return TypeV::StackAdd;
		}
		else if (Type.Is<Static>())
		{
			return TypeV::Static;
		}
		else if (Type.Is<Thread>())
		{
			return TypeV::Thread;
		}
		else
		{
			throw std::exception("bad path");
		}
	}
};

struct UDebugIns
{
	struct None{};
	Variant<None,UDebugSetFile, UDebugSetLineNumber, UDebugSetVarableLoc> Debug;

	using Type_t = Byte;
	enum class Type :Type_t
	{
		None,
		UDebugSetFile,
		UDebugSetLineNumber,
		UDebugSetVarableLoc,
	};

	inline Type Get_Type() const
	{
		if (Debug.Is<None>())
		{
			return Type::None;
		}
		else if (Debug.Is<UDebugSetFile>())
		{
			return Type::UDebugSetFile;
		}
		else if (Debug.Is<UDebugSetLineNumber>())
		{
			return Type::UDebugSetLineNumber;
		}
		else if (Debug.Is<UDebugSetVarableLoc>())
		{
			return Type::UDebugSetVarableLoc;
		}
		else 
		{
			throw std::exception("bad path");
		}
	}
};

using VarableInfoType_t = int;
enum class VarableInfoType : VarableInfoType_t
{
	Parameter,
	Stack,
	Static,
	Thread,
};
struct VarableInfo
{
	String FileDeclaredIn;
	size_t DeclaredLine;
	size_t DeclaredPos;

	VarableInfoType VarableType;
	ReflectionTypeInfo ReflectionType;

	struct None{};
	struct Static { UAddress offset = 0; };
	struct Thread { UAddress offset = 0; };
	Variant<None,Static,Thread> TypeLoc;


	using Typeloc_t = Byte;
	enum class Typeloc :Typeloc_t
	{
		None,
		Static,
		Thread,
	};

	Typeloc Get_Type() const
	{
		if (TypeLoc.Is<None>())
		{
			return Typeloc::None;
		}
		else if (TypeLoc.Is<Static>())
		{
			return Typeloc::Static;
		}
		else if (TypeLoc.Is<Thread>())
		{
			return Typeloc::Thread;
		}
		else
		{
			throw std::exception("bad path");
		}
	}
};

struct ULangDebugInfo
{
	Vector<UDebugIns> DebugInfo;
	BinaryVectorMap<String,VarableInfo> VarablesInfo;

	static void ToBytes(BitMaker& bit, const ULangDebugInfo& Value);
	static bool FromBytes(BitReader& bit, ULangDebugInfo& Value);
	static void ToBytes(BitMaker& bit, const UDebugIns& Value);
	
	static bool FromBytes(BitReader& bit, UDebugIns& Value);
	
	static void ToBytes(BitMaker& bit, const VarableInfo& Value);
	static bool FromBytes(BitReader& bit, VarableInfo& Value);
};

UCodeLangEnd