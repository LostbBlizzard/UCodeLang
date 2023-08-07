#pragma once
#include "UCodeLangNameSpace.hpp"
#include "LangDef.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include <filesystem>
#include <optional>
#include <array>
#include <memory>
#include <variant>
UCodeLangStart


using Int8  = int8_t;
using Int16 = int16_t;
using Int32 = int32_t;
using Int64 = int64_t;



using UInt8 = uint8_t;
using UInt16 = uint16_t;
using UInt32 = uint32_t;
using UInt64 = uint64_t;

using float32 = float;
using float64 = double;
#if UCodeLang_64BitSytem
using SIntNative = Int64;
using UIntNative = UInt64;
#else
using SIntNative = Int32;
using UIntNative = UInt32;
#endif

using Utf8 = Int8;
using Utf16 = Int16;
using Utf32 = Int32;


using PtrType = void*;
using NSize_t = UIntNative;

using Byte = UInt8;

//A int that can Address any ULang Value
using UAddress = UIntNative;
constexpr UAddress NullAddress = (UAddress)-1;
constexpr Byte DebugGarbageByte = 'Y';
//
constexpr UInt64 NullUInt64 = (UInt64)nullptr;

using String = std::string;
using String_view = std::string_view;
using Path = std::filesystem::path;




template<typename T> using Unique_ptr = std::unique_ptr<T>;
template<typename T> using Unique_Array = std::unique_ptr<T[]>;

template<typename T> using Shared_ptr = std::shared_ptr<T>;
template<typename T> using Weak_ptr = std::weak_ptr<T>;


template<typename T> using Optional = std::optional<T>;
template<typename T> using Vector = std::vector<T>;
template<typename T> using Stack = std::stack<T>;
template<typename T, typename T2> using Unordered_map = std::unordered_map<T, T2>;

template<typename T,size_t Size> using Array = std::array<T, Size>;

template<typename... Types>
struct Variant
{
public:
	using ThisType = Variant<Types...>;
	template<typename T>
	Variant(const T& Value) :_Base(Value){}

	template<typename T>Variant(T&& Value) : _Base(std::move(Value)) {}

	Variant()
	{

	}

	template<typename T> ThisType& operator=(const T& Value)
	{
		_Base = Value;
		return *this;
	}
	template<typename T> ThisType& operator=(T&& Value)
	{
		_Base = std::move(Value);
		return *this;
	}

	template<typename T> T& Get()
	{
		return std::get<T>(_Base);
	}
	template<typename T> const T& Get() const
	{
		return std::get<T>(_Base);
	}

	template<typename T> bool Is() const
	{
		return std::holds_alternative<T>(_Base);
	}

	template<typename T> T* Get_If()
	{
		if (Is<T>()) 
		{
			return &Get<T>();
		}
		else
		{
			return nullptr;
		}
	}
	template<typename T> const T* Get_If() const
	{
		if (Is<T>())
		{
			return &Get<T>();
		}
		else
		{
			return nullptr;
		}
	}


	template<typename T> T& GetOr(const T& Or) 
	{
		if (Is<T>())
		{
			return Get<T>();
		}
		return Or;
	}

	template<typename T> const T& GetOr(const T& Or) const
	{
		if (Is<T>())
		{
			return Get<T>();
		}
		return Or;
	}

	template<typename T> Optional<T> GetAsOptional() const
	{
		if (Is<T>())
		{
			return Get<T>();
		}
		return {};
	}
private:
	std::variant<Types...> _Base;
};

using RegisterID_t = UInt8;
enum class RegisterID : RegisterID_t
{
	A, B, C, D, E, F,

	//
	StartRegister = (RegisterID_t)RegisterID::A,
	EndRegister = (RegisterID_t)RegisterID::F,


	ThisRegister = (RegisterID_t)RegisterID::D,
	InPutRegister = (RegisterID_t)RegisterID::E,
	OuPutRegister = (RegisterID_t)RegisterID::F,

	MathOuPutRegister = OuPutRegister,
	BoolRegister = OuPutRegister,
	BitwiseRegister = OuPutRegister,

	StartParameterRegister = (RegisterID_t)RegisterID::D,//the range the runtime will pass funcion Parameters into Registers
	EndParameterRegister = (RegisterID_t)RegisterID::F + 1,


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

		float32 Asfloat32;
		float64 Asfloat64;

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

	UCodeLangForceinline void operator=(float32 V) { Asfloat32 = V; }
	UCodeLangForceinline void operator=(float64 V) { Asfloat64 = V; }

	UCodeLangForceinline void operator=(RegisterID V) { AsRegister = V; }
	//inline void operator=(UIntNative V) { AsUIntNative = V; }
	UCodeLangForceinline void operator=(PtrType V) { AsPtr = V; }
	
	
	
};
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
	inline static const char* Object = "uo";

	inline static const char* Asm = "ua";
	inline static const char* AsmWithDot = ".ua";

	inline static const char* SourceFileWithDot = ".uc";
	inline static const char* LibWithDot = ".ulib";
	inline static const char* DllWithDot = ".udll";
	inline static const char* ObjectWithDot = ".uo";
};

template<typename T>
struct Span
{
	Span() :_Data(nullptr), _Size(0)
	{

	}
	Span(T* ptr, size_t size) :_Data(ptr), _Size(size)
	{

	}

	static Span Make(T* ptr, size_t size)
	{
		return Span(ptr, size);
	}
	static const Span Make(const T* ptr, size_t size)
	{
		return Span((T*)ptr, size);
	}

	constexpr T& operator[](size_t Index)
	{
		#ifdef DEBUG
		if (Index > _Size)
		{
			UCodeLangThrowException("Index out of range");
		}
		#endif // DEBUG

		return _Data[Index];
	}

	constexpr const T& operator[](size_t Index) const
	{
		#ifdef DEBUG
		if (Index > _Size)
		{
			UCodeLangThrowException("Index out of range");
		}
		#endif // DEBUG

		return _Data[Index];
	}

	constexpr const T* Data() const
	{
		return _Data;
	}
	constexpr T* Data()
	{
		return _Data;
	}

	constexpr size_t Size() const
	{
		return  _Size;
	}
private:
	T* _Data;
	size_t _Size;
};

template<typename T>
struct SpanPtr
{
	using MySpan = Span<T>;
	SpanPtr() :_Data(nullptr), _Size(0)
	{

	}
	SpanPtr(SpanPtr&& Value) :_Data(nullptr), _Size(0)
	{
		this->operator=(std::move(Value));
	}
	
	inline MySpan AsSpan()
	{
		return MySpan::Make(Data(),Size());
	}
	inline const MySpan AsSpan() const
	{
		return MySpan::Make(Data(), Size());
	}
	SpanPtr& operator=(SpanPtr&& Value)
	{
		_Data = std::move(Value._Data);
		_Size = Value.Size();

		Value._Size = 0;
		return *this;
	}
	void Resize(size_t Count)
	{
		_Data.reset(new T[Count]);
		_Size = Count;
	}
	void Copyfrom(const MySpan& Values)
	{
		Resize(Values.Size());
		for (size_t i = 0; i < Values.Size(); i++)
		{
			_Data[i] = Values[i];
		}
	}
	void Copyfrom(MySpan&& Values)
	{
		Resize(Values.Size());
		for (size_t i = 0; i < Values.Size(); i++)
		{
			_Data[i] = std::move(Values[i]);
		}

		Values._Size = 0;
	}

	void Copyfrom(const Vector<T>& Values)
	{
		Resize(Values.size());
		for (size_t i = 0; i < Values.size(); i++)
		{
			_Data[i] = Values[i];
		}
	}
	void Copyfrom(Vector<T>&& Values)
	{
		Resize(Values.size());
		for (size_t i = 0; i < Values.size(); i++)
		{
			_Data[i] = std::move(Values[i]);
		}

		Values.resize(0);
	}
	Vector<T> ToVector() const
	{
		Vector<T> R;
		R.resize(_Size);

		for (size_t i = 0; i < _Size; i++)
		{
			R[i] = this->operator[](i);
		}

		return R;
	}
	Vector<T> MoveToVector()
	{
		Vector<T> R;
		R.resize(_Size);

		for (size_t i = 0; i < _Size; i++)
		{
			R[i] = std::move(this->operator[](i));
		}

		_Size = 0;

		return R;
	}
	
	constexpr T& operator[](size_t Index)
	{
		#ifdef DEBUG
		if (Index >= _Size)
		{
			UCodeLangThrowException("Index out of range");
		}
		#endif // DEBUG

		return _Data[Index];
	}

	constexpr const T& operator[](size_t Index) const
	{
		#ifdef DEBUG
		if (Index >= _Size)
		{
			UCodeLangThrowException("Index out of range");
		}
		#endif // DEBUG

		return _Data[Index];
	}

	const T* Data() const
	{
		return _Data.get();
	}
	T* Data()
	{
		return _Data.get();
	}

	size_t  Size() const
	{
		return  _Size;
	}
private:
	Unique_Array<T> _Data;
	size_t _Size=0;
};


using BytesView = Span<Byte>;
using BytesPtr = SpanPtr<Byte>;




enum class IntSizes : UInt8
{
	Int8,
	Int16,
	Int32,
	Int64,

	#if UCodeLang_64BitSytem
	Native = Int64,
	#else
	Native = Int32,
	#endif // 0
};

using AccessModifierType_t = UInt8;
enum class AccessModifierType :AccessModifierType_t
{
	Public,
	Private,

	Default = Public,
};

UCodeLangEnd