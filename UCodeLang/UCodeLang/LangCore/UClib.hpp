#pragma once

#include "../LangCore.hpp"
#include "../LangCore/ReflectionData.hpp"
UCodeLangStart

using LibType_t = UInt8;
enum class LibType : LibType_t
{
	Lib,
	Dll,

	Default =Dll,
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
		#elif
		intNative = int32,
		#endif // UCodeLang_64BitSytem

	};

	UClib();
	~UClib();

	UClib(UClib& GetFrom);

	UClib(const UClib& V) = delete;
	UClib& operator=(const UClib& V) = delete;



	UCodeLangForceinline void Add_NameToLastInstruction(const String& Name)
	{
		_NameToPtr[Name] = (UAddress)(_Instructions.size() - 1);
	}

	UCodeLangForceinline void Add_NameToInstruction(UAddress Index, const String& Name)
	{
		_NameToPtr[Name] = (UAddress)(Index);
	}
	inline UAddress Get_NameToInstruction(const String& Name) const
	{
		if (_NameToPtr.count(Name)) 
		{
			return _NameToPtr.at(Name);
		}
		else
		{
			return NullAddress;
		}
	}

	UCodeLangForceinline bool Get_HasNameToPtr(const String& Name) const
	{
		return _NameToPtr.count(Name);
	}

	UCodeLangForceinline UAddress NothingThing_Instruction()
	{
		Instruction Data;
		Data.OpCode = InstructionSet::DoNothing;
		Data.Value0.AsUInt64 = (UInt64)nullptr;
		Data.Value1.AsUInt64 = (UInt64)nullptr;
		return Add_Instruction(Data);
	}
	UCodeLangForceinline UAddress Add_Instruction(const Instruction& V)
	{
		auto R = (UAddress)_Instructions.size();
		_Instructions.push_back(V);
		return R;
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

	UCodeLangForceinline auto& Get_StaticBytes() const //Static Variables nad stuff
	{
		return _StaticBytes;
	};
	UCodeLangForceinline void clear_StaticBytes()
	{
		_StaticBytes.clear();
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
		clear_Instructions();
		clear_NameToPtr();
		clear_DebugBytes();
		_Assembly.Clear();
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

	//

	UCodeLangForceinline  Endian Get_LibEndianess() const { return LibEndianess; }
	struct LibRawBytes
	{
		LibRawBytes():Bytes(nullptr),Size(0)
		{
			
		}
		Byte* Bytes;
		size_t Size;
	};
	UCodeLangForceinline static void Free(const LibRawBytes& bits)
	{
		free(bits.Bytes);
	}

	static LibRawBytes ToRawBytes(const UClib* Lib);
	static bool FromBytes(UClib* Lib,const LibRawBytes& Data);

	static bool ToFile(const UClib* Lib,const Path& path);
	static bool FromFile(UClib* Lib, const Path& path);

	NTypeSize BitSize = NTypeSize::intNative;
	LibType _LibType = LibType::Default;
	inline ClassAssembly& Get_Assembly()
	{
		return _Assembly;
	}
	inline const ClassAssembly& Get_Assembly() const
	{
		return _Assembly;
	}
private:
	Endian LibEndianess;
	Vector<Byte> _StaticBytes;

	Vector<Instruction> _Instructions;
	Unordered_map<String, UAddress> _NameToPtr;
	//Debug
	Vector<Byte> _DebugBytes;
	//
	ClassAssembly _Assembly;
};
UCodeLangEnd


