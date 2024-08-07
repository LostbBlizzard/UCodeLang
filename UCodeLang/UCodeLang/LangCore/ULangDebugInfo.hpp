#pragma once
#include "../LangCore/DataType/UnorderedMap.hpp"
#include "ReflectionData.hpp"
#include "BitMaker.hpp"
UCodeLangStart

struct UDebugSetFile
{
	String FileName;
	UAddress ForIns = 0;
}; 
struct UDebugSetLineNumber
{
	//This LineNumber and not LineIndex do LineNumber -1 for Index
	size_t LineNumber=0;
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
		else if (Type.Is<StackAdd>())
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
			UCodeLangThrowException("bad path");
		}
	}
};
struct UDebugSetFuncStackFrameSize
{
	UAddress StackFrameSize = 0;
	UAddress ForIns = 0;
};

struct UDebugIns
{
	struct None{};
	Variant<None,UDebugSetFile, UDebugSetLineNumber, UDebugSetVarableLoc, UDebugSetFuncStackFrameSize> Debug;

	using Type_t = Byte;
	enum class Type :Type_t
	{
		None,
		UDebugSetFile,
		UDebugSetLineNumber,
		UDebugSetVarableLoc,
		UDebugSetFuncStackFrameSize,
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
		else if (Debug.Is<UDebugSetFuncStackFrameSize>())
		{
			return Type::UDebugSetFuncStackFrameSize;
		}
		else 
		{
			UCodeLangThrowException("bad path");
		}
	}

	Optional<size_t> Get_Ins() const
	{
		if (Debug.Is<None>())
		{
			return {};
		}
		else if (Debug.Is<UDebugSetFile>())
		{
			return Debug.Get<UDebugSetFile>().ForIns;
		}
		else if (Debug.Is<UDebugSetLineNumber>())
		{
			return Debug.Get<UDebugSetLineNumber>().ForIns;
		}
		else if (Debug.Is<UDebugSetVarableLoc>())
		{
			return Debug.Get<UDebugSetVarableLoc>().ForIns;
		}
		else if (Debug.Is<UDebugSetFuncStackFrameSize>())
		{
			return Debug.Get<UDebugSetFuncStackFrameSize>().ForIns;
		}
		else
		{
			UCodeLangThrowException("bad path");
		}

	}
	void Set_Ins(size_t Value) 
	{
		if (Debug.Is<None>())
		{
			
		}
		else if (Debug.Is<UDebugSetFile>())
		{
			Debug.Get<UDebugSetFile>().ForIns = Value;
		}
		else if (Debug.Is<UDebugSetLineNumber>())
		{
			Debug.Get<UDebugSetLineNumber>().ForIns = Value;
		}
		else if (Debug.Is<UDebugSetVarableLoc>())
		{
			Debug.Get<UDebugSetVarableLoc>().ForIns = Value;
		}
		else if (Debug.Is<UDebugSetFuncStackFrameSize>())
		{
			Debug.Get<UDebugSetFuncStackFrameSize>().ForIns = Value;
		}
		else
		{
			UCodeLangThrowException("bad path");
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
	Null,
};
struct VarableInfo
{
	String FileDeclaredIn;
	size_t DeclaredLine = 0;
	size_t DeclaredPos = 0;

	VarableInfoType VarableType = VarableInfoType::Null;
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
			UCodeLangThrowException("bad path");
		}
	}
};

struct ULangDebugInfo
{
	Vector<UDebugIns> DebugInfo;
	UnorderedMap<String, VarableInfo> VarablesInfo;

	static void ToBytes(BitMaker& bit, const ULangDebugInfo& Value);
	static bool FromBytes(BitReader& bit, ULangDebugInfo& Value);

	static void ToBytes(BitMaker& bit, const UDebugIns& Value);
	static bool FromBytes(BitReader& bit, UDebugIns& Value);

	static void ToBytes(BitMaker& bit, const VarableInfo& Value);
	static bool FromBytes(BitReader& bit, VarableInfo& Value);


	void Add_SetFile(const String& file, size_t Ins)
	{
		UDebugSetFile V2;
		V2.FileName = file;
		V2.ForIns = Ins;
		UDebugIns V;
		V.Debug = std::move(V2);
		DebugInfo.push_back(std::move(V));
	}
	void Add_SetLineNumber(size_t LineNumber, size_t Ins)
	{
		UDebugSetLineNumber V2;
		V2.LineNumber= LineNumber;
		V2.ForIns = Ins;
		UDebugIns V;
		V.Debug = std::move(V2);
		DebugInfo.push_back(std::move(V));
	}
	void Add_UDebugSetFuncStackFrameSize(size_t StackFrameSize, size_t Ins)
	{
		UDebugSetFuncStackFrameSize V2;
		V2.StackFrameSize = StackFrameSize;
		V2.ForIns = Ins;
		UDebugIns V;
		V.Debug = std::move(V2);
		DebugInfo.push_back(std::move(V));
	}
	void Add_SetVarableLoc(UDebugSetVarableLoc&& Info)
	{
		UDebugIns V;
		V.Debug = std::move(Info);
		DebugInfo.push_back(std::move(V));
	}
	void Add_SetVarableName(const String& name,VarableInfo&& Info)
	{
		VarablesInfo.AddValue(name, Info);
	}

	struct Cach
	{
		UnorderedMap<size_t, Vector<const UDebugIns*>> IndexToIns;
	};
	Cach MakeCach() const
	{
		return MakeCach(DebugInfo);
	}
	static Cach MakeCach(const Vector<UDebugIns>& DebugInfo)
	{
		Cach r = {};

		for (auto& Item : DebugInfo)
		{
			if (Item.Get_Ins().has_value())
			{
				size_t val = Item.Get_Ins().value();

				auto& list = r.IndexToIns.GetOrAdd(val, {});
				list.push_back(&Item);
			}
		}

		return r;
	}
	NullablePtr<Vector<UDebugIns*>> GetForIns(size_t Ins,Cach& chach)
	{
		return GetForIns(DebugInfo, Ins,chach);
	}
	NullablePtr<Vector<const UDebugIns*>> GetForIns(size_t Ins,Cach& chach) const
	{
		return GetForIns(DebugInfo, Ins,chach);
	}
	
	static NullablePtr<Vector<const UDebugIns*>> GetForIns(const Vector<UDebugIns>& DebugInfo, size_t Ins,Cach& chach)
	{
		NullablePtr<Vector<const UDebugIns*>> R;


		GetForIns(DebugInfo, Ins, R,chach);

		return R;
	}
	static NullablePtr<Vector<UDebugIns*>> GetForIns(Vector<UDebugIns>& DebugInfo,size_t Ins,Cach& chach)
	{
		NullablePtr<Vector<UDebugIns*>> R;

		GetForIns(DebugInfo, Ins, R,chach);

		return R;
	}

	static void GetForIns(const Vector<UDebugIns>& DebugInfo, size_t Ins, NullablePtr<Vector<const UDebugIns*>>& Out, Cach& chach)
	{
		if (chach.IndexToIns.HasValue(Ins))
		{
			Out = Nullableptr(&chach.IndexToIns.GetValue(Ins));
		}
		else
		{
			Out = {};
		}
	}
	static void GetForIns(Vector<UDebugIns>& DebugInfo, size_t Ins, NullablePtr<Vector<UDebugIns*>>& Out, Cach& chach)
	{
		auto& val = *(NullablePtr<Vector<const UDebugIns*>>*) & Out;
		return GetForIns(DebugInfo,Ins, val,chach);
	}
};

UCodeLangEnd