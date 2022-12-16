#pragma once

#include "../LangCore.hpp"
#include "../LangCore/ReflectionData.hpp"
UCodeLangStart

using LibType_t = UInt8;
enum class LibType : LibType_t
{
	Object,
	Lib,
	Dll,
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

	UClib(const UClib& V) = delete;
	UClib& operator=(const UClib& V) = delete;


	inline void Add_NameToLastInstruction(const String& Name)
	{
		_NameToPtr[Name] = (UAddress)(_Instructions.size() - 1);
	}
	inline void Add_NameToInstruction(UAddress Index, const String& Name)
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
	inline bool Get_HasNameToPtr(const String& Name) const
	{
		return _NameToPtr.count(Name);
	}

	inline UAddress NothingThing_Instruction()
	{
		Instruction Data;
		Data.OpCode = InstructionSet::DoNothing;
		Data.Value0_AsUMaxSizeInt = (UInt64)nullptr;
		Data.Value0_AsUMaxSizeInt = (UInt64)nullptr;
		return Add_Instruction(Data);
	}
	inline UAddress Add_Instruction(const Instruction& V)
	{
		auto R = (UAddress)_Instructions.size();
		_Instructions.push_back(V);
		return R;
	}
	inline UAddress GetLastInstruction()
	{
		return (UAddress)(_Instructions.size() - 1);
	}
	

	inline const auto& Get_Instructions()const
	{
		return  _Instructions;
	}
	inline auto& Get_Instructions()
	{
		return  _Instructions;
	}
	inline void clear_Instructions()
	{
		_Instructions.clear();
	}

	inline auto& Get_NameToPtr() const
	{
		return _NameToPtr;
	}
	inline void clear_NameToPtr()
	{
		_NameToPtr.clear();
	}

	inline auto& Get_StaticBytes() const //Static Variables nad stuff
	{
		return _StaticBytes;
	};
	inline void clear_StaticBytes()
	{
		_StaticBytes.clear();
	}
	
	inline const auto& Get_DebugByte()const
	{
		return  _DebugBytes;
	}
	inline auto& Get_DebugByte()
	{
		return  _DebugBytes;
	}
	inline void clear_DebugBytes()
	{
		_DebugBytes.clear();
	}
	
	inline void AddOffsetByte()
	{
		if (_StaticBytes.size() == 0) { _StaticBytes.push_back('\0'); }
	}
	inline void AddOffsetDebug()
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

	inline UAddress AddStaticBytes(const String& Str)
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
	inline UAddress AddStaticBytes(unsigned char Byte)
	{
		UAddress A = _StaticBytes.size();
		_StaticBytes.push_back(Byte);
		return A;
	}
	inline UAddress AddStaticBytes(unsigned char Byte, size_t Size)
	{
		UAddress A = _StaticBytes.size();
		for (size_t i = 0; i < Size; i++)
		{
			_StaticBytes.push_back(Byte);
		}
		return A;
	}
	inline UAddress AddStaticBytes(const unsigned char* Bits, size_t Size)
	{
		UAddress A = (UAddress)_StaticBytes.size();
		for (size_t i = 0; i < Size; i++)
		{
			_StaticBytes.push_back(Bits[i]);
		}
		return A;
	}

	inline UAddress AddDebugBytes(const unsigned char* Bits,size_t Size)
	{
		UAddress A = (UAddress)_DebugBytes.size();
		for (size_t i = 0; i < Size; i++)
		{
			_DebugBytes.push_back(Bits[i]);
		}
		return A;
	}
	inline UAddress AddDebugBytes(const String& Str)
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
	inline UAddress AddDebugBytes(unsigned char Byte)
	{
		UAddress A = (UAddress)_DebugBytes.size();
		_DebugBytes.push_back(Byte);
		return A;
	}

	//

	inline Endian Get_LibEndianess() const { return LibEndianess; }
	struct LibRawBytes
	{
		LibRawBytes():Bytes(nullptr),Size(0)
		{
			
		}
		unsigned char* Bytes;
		size_t Size;
	};
	inline static void Free(const LibRawBytes& bits)
	{
		free(bits.Bytes);
	}

	static LibRawBytes ToRawBytes(const UClib* Lib);
	static bool FromBytes(UClib* Lib,const LibRawBytes& Data);

	static bool ToFile(const UClib* Lib,const String& Path);
	static bool FromFile(UClib* Lib, const String& Path);

	NTypeSize BitSize = NTypeSize::intNative;
	LibType _LibType = LibType::Object;
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
	Vector<unsigned char> _StaticBytes;

	Vector<Instruction> _Instructions;
	unordered_map<String, UAddress> _NameToPtr;
	//Debug
	Vector<unsigned char> _DebugBytes;
	//
	ClassAssembly _Assembly;
};
UCodeLangEnd


