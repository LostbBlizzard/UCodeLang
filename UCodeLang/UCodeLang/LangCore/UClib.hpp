#pragma once

#include "../LangCore.hpp"
#include "../LangCore/ReflectionData.hpp"
#include "UCodeLang/LangCore/BitMaker.hpp"
#include "UCodeLang/LangCore/DataType/UnorderedMap.hpp"
#include "UCodeLang/LangCore/ULangDebugInfo.hpp"
UCodeLangStart



using LibType_t = UInt8;
enum class LibType : LibType_t
{
	Lib,
	Dll,

	Default =Dll,
};


struct CodeLayer
{
	struct JustData
	{
		//Will not be loaded
		Vector<Byte> _Data;
	};
	struct UCodeByteCode
	{
		//Loaded By UCodeVM
		Vector<Instruction>  _Instructions;

		//Function Names to Call displacement
		UnorderedMap<String, UAddress> _NameToPtr;

		//DebugInfo
		Optional<ULangDebugInfo> DebugInfo;

		UCodeLangForceinline void Add_NameToLastInstruction(const String& Name)
		{
			_NameToPtr.AddValue(Name,(UAddress)(_Instructions.size() - 1));
		}

		UCodeLangForceinline void Add_NameToInstruction(UAddress Index, const String& Name)
		{
			_NameToPtr.AddValue(Name, (UAddress)(Index));
		}
		inline UAddress Get_NameToInstruction(const String& Name) const
		{
			if (_NameToPtr.HasValue(Name))
			{
				return _NameToPtr.HasValue(Name);
			}
			else
			{
				return NullAddress;
			}
		}

		UCodeLangForceinline bool Get_HasNameToPtr(const String& Name) const
		{
			return _NameToPtr.HasValue(Name);
		}

		UCodeLangForceinline UAddress NothingThing_Instruction()
		{
			Instruction Data = Instruction(InstructionSet::DoNothing, Instruction::NoneOp());
			return Add_Instruction(Data);
		}
		UCodeLangForceinline UAddress Add_Instruction(const Instruction& V)
		{
			auto CompilerRet = (UAddress)_Instructions.size();
			_Instructions.push_back(V);
			return CompilerRet;
		}
		UCodeLangForceinline UAddress GetLastInstruction()
		{
			return (UAddress)(_Instructions.size() - 1);
		}


		UCodeLangForceinline const auto& Get_Instructions()const
		{
			return  _Instructions;
		}
		UCodeLangForceinline auto& Get_Instructions()
		{
			return  _Instructions;
		}
		UCodeLangForceinline void clear_Instructions()
		{
			_Instructions.clear();
		}

		UCodeLangForceinline auto& Get_NameToPtr() const
		{
			return _NameToPtr;
		}
		UCodeLangForceinline void clear_NameToPtr()
		{
			_NameToPtr.clear();
		}
	};
	struct MachineCode
	{
		//Will loaded in executable memory
		Vector<Byte> _Code;

		//Function Names to Call displacement
		UnorderedMap<String, UAddress> _NameToPtr;

		//CPU specufic Debug Info
		Optional<Vector<Byte>> DebugInfo;

		UCodeLangForceinline const auto& Get_Code()const
		{
			return  _Code;
		}
		UCodeLangForceinline auto& Get_Code()
		{
			return  _Code;
		}
		UCodeLangForceinline void clear_Code()
		{
			_Code.clear();
		}
	};

	String _Name;
	Variant<JustData,UCodeByteCode, MachineCode> _Data;


	using DataTypes_t = Byte;
	enum class DataTypes : DataTypes_t
	{
		JustData,
		UCodeByteCode,
		MachineCode,
	};
	DataTypes GetDataType() const
	{
		if (_Data.Is<JustData>())
		{
			return DataTypes::JustData;
		}
		else if (_Data.Is<UCodeByteCode>())
		{
			return DataTypes::UCodeByteCode;
		}
		else if (_Data.Is<MachineCode>())
		{
			return DataTypes::MachineCode;
		}
	#if	UCodeLangDebug
		UCodeLangThrowException("bad path");
	#endif
	}
};

class UClib
{
	
public:
	using NTypeSize_t = UInt8;
	enum class NTypeSize : NTypeSize_t
	{
		int32,
		int64,

		#if UCodeLang_64BitSytem
		intNative = int64,
		#else
		intNative = int32,
		#endif // UCodeLang_64BitSytem

	};

	UClib();
	~UClib();

	UClib(UClib&& source) = default;
	UClib& operator=(UClib&& V) = default;

	UClib(const UClib& V) = delete;
	UClib& operator=(const UClib& V) = delete;

	void Reset();
	
	UCodeLangForceinline auto& Get_StaticBytes() const //Static Variables nad stuff
	{
		return _StaticBytes;
	};
	UCodeLangForceinline void clear_StaticBytes()
	{
		_StaticBytes.clear();
	}


	UCodeLangForceinline auto& Get_ThreadBytes() const //Static Variables nad stuff
	{
		return _ThreadBytes;
	};
	UCodeLangForceinline void clear_ThreadBytes()
	{
		_ThreadBytes.clear();
	}
	
	UCodeLangForceinline const auto& Get_DebugByte()const
	{
		return  _DebugBytes;
	}
	UCodeLangForceinline auto& Get_DebugByte()
	{
		return  _DebugBytes;
	}
	UCodeLangForceinline void clear_DebugBytes()
	{
		_DebugBytes.clear();
	}
	
	UCodeLangForceinline void AddOffsetByte()
	{
		if (_StaticBytes.size() == 0) { _StaticBytes.push_back('\0'); }
	}
	UCodeLangForceinline  void AddOffsetDebug()
	{
		if (_DebugBytes.size() == 0) { _DebugBytes.push_back('\0'); }
	}

	inline void ClearState()
	{
		clear_StaticBytes();
		clear_DebugBytes();
		_Assembly.Classes.clear();
	}
	//

	UCodeLangForceinline  UAddress AddStaticBytes(const String& Str)
	{
		return AddStaticBytes(String_view(Str));
	}
	inline UAddress AddStaticBytes(const String_view& Str)
	{
		UAddress A = _StaticBytes.size();
		for (size_t i = 0; i < Str.size(); i++)
		{
			_StaticBytes.push_back(Str[i]);
		}
		_StaticBytes.push_back('\0');
		return A;
	}
	UCodeLangForceinline UAddress AddStaticBytes(Byte Byte)
	{
		UAddress A = _StaticBytes.size();
		_StaticBytes.push_back(Byte);
		return A;
	}
	inline UAddress AddStaticBytes(Byte Byte, size_t Size)
	{
		UAddress A = _StaticBytes.size();
		for (size_t i = 0; i < Size; i++)
		{
			_StaticBytes.push_back(Byte);
		}
		return A;
	}
	inline UAddress AddStaticBytes(const Byte* Bits, size_t Size)
	{
		UAddress A = (UAddress)_StaticBytes.size();
		for (size_t i = 0; i < Size; i++)
		{
			_StaticBytes.push_back(Bits[i]);
		}
		return A;
	}

	inline UAddress AddDebugBytes(const Byte* Bits,size_t Size)
	{
		UAddress A = (UAddress)_DebugBytes.size();
		for (size_t i = 0; i < Size; i++)
		{
			_DebugBytes.push_back(Bits[i]);
		}
		return A;
	}


	UCodeLangForceinline  UAddress AddThreadBytes(const String& Str)
	{
		return AddThreadBytes(String_view(Str));
	}
	inline UAddress AddThreadBytes(const String_view& Str)
	{
		UAddress A = _ThreadBytes.size();
		for (size_t i = 0; i < Str.size(); i++)
		{
			_ThreadBytes.push_back(Str[i]);
		}
		_ThreadBytes.push_back('\0');
		return A;
	}
	UCodeLangForceinline UAddress AddThreadBytes(Byte Byte)
	{
		UAddress A = _ThreadBytes.size();
		_ThreadBytes.push_back(Byte);
		return A;
	}
	inline UAddress AddThreadBytes(Byte Byte, size_t Size)
	{
		UAddress A = _ThreadBytes.size();
		for (size_t i = 0; i < Size; i++)
		{
			_ThreadBytes.push_back(Byte);
		}
		return A;
	}
	inline UAddress AddThreadBytes(const Byte* Bits, size_t Size)
	{
		UAddress A = (UAddress)_ThreadBytes.size();
		for (size_t i = 0; i < Size; i++)
		{
			_ThreadBytes.push_back(Bits[i]);
		}
		return A;
	}


	UCodeLangForceinline UAddress AddDebugBytes(const String& Str)
	{
		return AddDebugBytes(String_view(Str));
	}
	inline UAddress AddDebugBytes(const String_view& Str)
	{
		UAddress A = (UAddress)_DebugBytes.size();
		for (size_t i = 0; i < Str.size(); i++)
		{
			_DebugBytes.push_back(Str[i]);
		}
		_DebugBytes.push_back('\0');
		return A;
	}
	UCodeLangForceinline  UAddress AddDebugBytes(Byte Byte)
	{
		UAddress A = (UAddress)_DebugBytes.size();
		_DebugBytes.push_back(Byte);
		return A;
	}
	CodeLayer* GetLayer(String_view Name)
	{
		for (auto& Item : _Layers)
		{
			if (Item->_Name == Name)
			{
				return Item.get();
			}
		}
		return nullptr;
	}
	const CodeLayer* GetLayer(String_view Name) const
	{
		for (auto& Item : _Layers)
		{
			if (Item->_Name == Name)
			{
				return Item.get();
			}
		}
		return nullptr;
	}
	CodeLayer* AddLayer(String_view Name)
	{
		CodeLayer* R = new CodeLayer();
		R->_Name = Name;
		_Layers.push_back(Unique_ptr<CodeLayer>(R));
		return R;
	}
	CodeLayer* GetLayerOrAddLayer(String_view Name)
	{
		auto V = GetLayer(Name);

if (V)
{
	return V;
}
else
{
	return AddLayer(Name);
}
	}

	inline ClassAssembly& Get_Assembly()
	{
		return _Assembly;
	}
	inline const ClassAssembly& Get_Assembly() const
	{
		return _Assembly;
	}

	static BytesPtr ToRawBytes(const UClib* Lib);


	static void ToBytes(BitMaker& Output, const CodeLayer& Data);

	static void ToBytes(BitMaker& Output, const Enum_Data& EnumData);
	static void ToBytes(BitMaker& Output, const ClassAssembly& Assembly);
	static void ToBytes(BitMaker& Output, const Alias_Data& Alias);
	static void ToBytes(BitMaker& Output, const Class_Data& ClassData);
	static void ToBytes(BitMaker& Output, const Vector<UsedTagValueData>& Attributes);
	static void ToBytes(BitMaker& Output, const UsedTagValueData& Attributes);
	static void ToBytes(BitMaker& Output, const ClassField& Item2);
	static void ToBytes(BitMaker& Output, const Tag_Data& Data);
	static void ToBytes(BitMaker& Output, const ClassMethod& Data);
	static void ToBytes(BitMaker& Output, const ReflectionTypeInfo& Data);
	static void ToBytes(BitMaker& Output, const ReflectionRawData& Data);
	static void ToBytes(BitMaker& Output, const TypedRawReflectionData& Data);
	static void ToBytes(BitMaker& Output, const Optional<ReflectionCustomTypeID>& Data);
	static void ToBytes(BitMaker& Output, const Optional<ReflectionTypeInfo>& Data);
	static void ToBytes(BitMaker& Output, const Trait_Data& TraitData);
	static void ToBytes(BitMaker& Output, const InheritedTrait_Data& TraitData);
	static void ToBytes(BitMaker& Output, const UsedTags& Tags)
	{
		ToBytes(Output, Tags.Attributes);
	}
	static void ToBytes(BitMaker& Output, const ClassMethod::Par& Par);
	static void ToBytes(BitMaker& Output, const FuncPtr_Data& FuncPtrData);
	static void ToBytes(BitMaker& Output, const GenericClass_Data& FuncPtrData);
	static void ToBytes(BitMaker& Output, const GenericFunction_Data& FuncPtrData);
	static void ToBytes(BitMaker& Output, const GenericBase_Data& FuncPtrData);
	static void ToBytes(BitMaker& Output, const TraitMethod& FuncPtrData);
	//
	static bool FromBytes(UClib* Lib, const BytesView& Data);

	static void FromBytes(BitReader& Input, CodeLayer& Data);

	static void FromBytes(BitReader& reader, ClassAssembly& Assembly);
	static void FromBytes(BitReader& reader, Enum_Data& Enum);
	static void FromBytes(BitReader& reader, Class_Data& Class);
	static void FromBytes(BitReader& reader, Vector<UsedTagValueData>& Attributes);
	static void FromBytes(BitReader& reader, ClassField& Item2);
	static void FromBytes(BitReader& reader, Alias_Data& Alias);
	static void FromBytes(BitReader& Input, Tag_Data& Data);
	static void FromBytes(BitReader& Input, ClassMethod& Data);
	static void FromBytes(BitReader& Input, ReflectionTypeInfo& Data);
	static void FromBytes(BitReader& Input, UsedTagValueData& Data);
	static void FromBytes(BitReader& Input, Optional<ReflectionCustomTypeID>& Data);
	static void FromBytes(BitReader& Input, Optional<ReflectionTypeInfo>& Data);
	static void FromBytes(BitReader& Input, ReflectionRawData& Data);
	static void FromBytes(BitReader& Input, Trait_Data& Data);
	static void FromBytes(BitReader& Input, InheritedTrait_Data& Data);
	static void FromBytes(BitReader& reader, UsedTags& Attributes)
	{
		FromBytes(reader, Attributes.Attributes);
	}
	static void FromBytes(BitReader& reader, FuncPtr_Data& Ptr);
	static void FromBytes(BitReader& Input, ClassMethod::Par& Data);


	static void FromBytes(BitReader& reader, GenericClass_Data& Ptr);
	static void FromBytes(BitReader& reader, GenericFunction_Data& Ptr);
	static void FromBytes(BitReader& Input, GenericBase_Data& Data);

	static void FromBytes(BitReader& Input, TraitMethod& Data);
	//

	static void FixRawValue(Endian AssemblyEndian, NTypeSize BitSize, const ClassAssembly& Types, TypedRawReflectionData& RawValue)
	{
		FixRawValue(AssemblyEndian, BitSize, Types, RawValue._Data, RawValue._Type);
	}
	static void FixRawValue(Endian AssemblyEndian, NTypeSize BitSize, const ClassAssembly& Types, ReflectionRawData& RawValue, const ReflectionTypeInfo& Type);
	static void FixAssemblyRawValues(Endian AssemblyEndian, NTypeSize BitSize, const ClassAssembly& Assembly);


	static bool ToFile(const UClib* Lib, const Path& path);
	static bool FromFile(UClib* Lib, const Path& path);

	static Optional<UClib> OpenFile(const Path& path)
	{
		UClib r;
		if (FromFile(&r, path))
		{
			return r;
		}
		return {};
	}
	bool WriteToFile(const Path& path)
	{
		return ToFile(this, path);
	}

	NTypeSize BitSize = NTypeSize::intNative;
	LibType _LibType = LibType::Default;
	
	Endian LibEndianess;
	Vector<Byte> _StaticBytes;
	Vector<Byte> _ThreadBytes;
	Vector<Byte> _DebugBytes;

	Vector<Unique_ptr<CodeLayer>> _Layers;

	ClassAssembly _Assembly;
};
UCodeLangEnd


