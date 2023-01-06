#pragma once
#include "UCodeLangNameSpace.hpp"
#include "LangDef.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include <filesystem>
#include <optional>
UCodeLangStart


using Int8  = signed char;
using Int16 = signed short;
using Int32 = signed int;
using Int64 = signed long long;



using UInt8 = unsigned char;
using UInt16 = unsigned short;
using UInt32 = unsigned int;
using UInt64 = unsigned long long;

using float32 = float;
using float64 = double;
#if UCodeLang_64BitSytem
using SIntNative = Int64;
using UIntNative = UInt64;
#else
using Int32 SIntNative;
using UInt32 UIntNative;
#endif


using PtrType = void*;
using NSize_t = UIntNative;

//A int that can Address any ULang Value
using UAddress = UIntNative;
constexpr UAddress NullAddress = (UAddress)-1;

//
constexpr UInt64 NullUInt64 = (UInt64)nullptr;

using String = std::string;
using String_view = std::string_view;
using Path = std::filesystem::path;

template<typename T> using Optional = std::optional<T>;
template<typename T> using Vector = std::vector<T>;
template<typename T, typename T2> using unordered_map = std::unordered_map<T, T2>;

using RegisterID_t = UInt8;
enum class RegisterID : RegisterID_t
{
	A, B, C, D, E, F,

	//
	StartRegister = (RegisterID_t)RegisterID::A,
	EndRegister = (RegisterID_t)RegisterID::F,


	ThisRegister = (RegisterID_t)RegisterID::D,
	OuPutRegister = (RegisterID_t)RegisterID::E,
	InPutRegister = (RegisterID_t)RegisterID::F,

	PopAndTrashRegister = (RegisterID_t)RegisterID::D,
	MathOuPutRegister = OuPutRegister,
	BoolRegister = OuPutRegister,
	BitwiseRegister = OuPutRegister,

	NullRegister = 155,
};

struct AnyInt64
{
	
	constexpr AnyInt64() :Value(0)
	{

	}
	constexpr AnyInt64(UInt64 V) : Value(V)
	{

	}

	union
	{
		UInt64 Value;

		bool Asbool;
		Int8  AsInt8;
		Int16 AsInt16;
		Int32 AsInt32;
		Int64 AsInt64;

		UInt8  AsUInt8;
		UInt16 AsUInt16;
		UInt32 AsUInt32;
		UInt64 AsUInt64;

		RegisterID AsRegister;

		UIntNative AsUIntNative;
		PtrType AsPtr;
		UAddress AsAddress;
	};
	UCodeLangForceinline void operator=(bool V) { Asbool = V; }
	UCodeLangForceinline void operator=(Int8 V) { AsInt8 = V; }
	UCodeLangForceinline void operator=(Int16 V) { AsInt16 = V; }
	UCodeLangForceinline void operator=(Int32 V) { AsInt32 = V; }
	UCodeLangForceinline void operator=(Int64 V) { AsInt64 = V; }

	UCodeLangForceinline void operator=(UInt8 V) { AsUInt8 = V; }
	UCodeLangForceinline void operator=(UInt16 V) { AsUInt16 = V; }
	UCodeLangForceinline void operator=(UInt32 V) { AsUInt32 = V; }
	UCodeLangForceinline void operator=(UInt64 V) { AsUInt64 = V; }


	UCodeLangForceinline void operator=(RegisterID V) { AsRegister = V; }
	//inline void operator=(UIntNative V) { AsUIntNative = V; }
	UCodeLangForceinline void operator=(PtrType V) { AsPtr = V; }
	
	
	
};
struct parameters
{
	void* Data;
	NSize_t Size;
	constexpr parameters() :Data(nullptr), Size(0) {}
	constexpr parameters(void* data, NSize_t size) : Data(data), Size(size) {}

	template<typename T> UCodeLangForceinline static parameters As(const T& Value)
	{
		return  parameters((void*)&Value,sizeof(T));
	}

	template<typename T> UCodeLangForceinline static const T& From(const parameters& Value)
	{
		if (Value.Size != sizeof(T)) { throw std::exception("type mismatch"); }
		//it not a really a type mismatch but you get what I mean. 

		return  *(T*)Value.Data;
	}
};
static constexpr parameters NullParameters = parameters(nullptr, 0);
//MaxSize
constexpr UInt8 UInt8_MinSize = 0x00;
constexpr UInt8 UInt8_MaxSize = 0xff;

constexpr Int8 Int8_MinSize = INT8_MIN;
constexpr Int8 Int8_MaxSize = INT8_MAX;

constexpr UInt16 UInt16_MinSize = 0x0000;
constexpr UInt16 UInt16_MaxSize = 0xffff;

constexpr Int16 Int16_MinSize = INT16_MIN;
constexpr Int16 Int16_MaxSize = INT16_MAX;

constexpr UInt32 UInt32_MinSize = 0x00000000;
constexpr UInt32 UInt32_MaxSize = 0xffffffff;

constexpr Int32 Int32_MinSize = INT32_MIN;
constexpr Int32 Int32_MaxSize = INT32_MAX;

constexpr Int64 Int64_MinSize = INT64_MIN;
constexpr Int64 Int64_MaxSize = INT64_MAX;

constexpr UInt64 UInt64_MinSize = 0x00000000;
constexpr UInt64 UInt64_MaxSize = UINT64_MAX;

#if UCodeLang_64BitSytem
constexpr UIntNative UIntNative_MinSize = UInt64_MinSize;
constexpr UIntNative UIntNative_MaxSize = UInt64_MaxSize;

constexpr SIntNative IntNative_MinSize = Int64_MinSize;
constexpr SIntNative IntNative_MaxSize = Int64_MaxSize;
#else
constexpr UIntNative UIntNative_MinSize = UInt32_MinSize;
constexpr UIntNative UIntNative_MaxSize = UInt32_MaxSize;

constexpr UIntNative IntNative_MinSize = Int32_MinSize;
constexpr UIntNative IntNative_MaxSize = Int32_MaxSize;
#endif

class FileExt
{
public:
	inline static const char* SourceFile = "uc";
	inline static const char* Lib = "ulib";
	inline static const char* Dll = "udll";

	inline static const char* Asm = "ua";
	inline static const char* AsmWithDot = ".ua";

	inline static const char* SourceFileWithDot = ".uc";
	inline static const char* LibWithDot = ".ulib";
	inline static const char* DllWithDot = ".udll";
};

UCodeLangEnd