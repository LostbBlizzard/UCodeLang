#pragma once
#include "../UCodeLangNameSpace.hpp"
#include "../LangDef.hpp"
#include <xhash>
UCodeLangStart


template<typename T> 
struct NeverNullPtr
{
public:
	using PtrType = T*;
	using ThisType = NeverNullPtr<T>;
	using NullableType= NullablePtr<T>;

	NeverNullPtr(PtrType Value)
		:Value(Value)
	{
		UCodeLangAssert(Value);//never null
	}
	static ThisType Make(PtrType Value)
	{
		return NeverNullPtr(Value);
	}
	static const ThisType Make(const PtrType Value)
	{
		return Make((PtrType)Value);
	}

	constexpr PtrType value() noexcept
	{
		return Value;
	}
	constexpr const PtrType value() const noexcept
	{
		return Value;
	}

	constexpr const T* operator->() const noexcept
	{
		return Value;
	}
	constexpr T* operator->() noexcept
	{
		return Value;
	}

	constexpr const T& operator*() const noexcept
	{
		return Value;
	}
	constexpr T& operator*() noexcept
	{
		return *Value;
	}

	bool operator==(const NeverNullPtr other) const
	{
		return Value == other.Value;
	}
	bool operator!=(const NeverNullPtr other) const
	{
		return Value == other.Value;
	}

	constexpr explicit operator NullableType() const
	{
		const NullableType r = NullableType(value());
		return r;
	}
	constexpr explicit operator NullableType() 
	{
		NullableType r = NullableType(value());
		return r;
	}
private:
	PtrType Value;
};

template<typename T> NeverNullPtr<T> NeverNullptr(T* Value)
{
	return NeverNullPtr<T>::Make(Value);
}

template<typename T> const NeverNullPtr<T> NeverNullptr(const T* Value)
{
	return NeverNullPtr<T>::Make(Value);
}




template<typename T> 
struct NullablePtr
{
public:
	using PtrType = T*;
	using ThisType = NullablePtr<T>;
	using NeverNullType = NeverNullPtr<T>;
	NullablePtr()
		:Value(nullptr)
	{

	}
	NullablePtr(PtrType Value)
		:Value(Value)
	{

	}
	static ThisType Make(PtrType Value)
	{
		return NeverNullPtr(Value);
	}
	static const ThisType Make(const PtrType Value)
	{
		return Make((PtrType)Value);
	}

	constexpr explicit operator bool() const
	{
		return Value == nullptr;
	}
	constexpr bool has_value() const noexcept
	{
		return Value != nullptr;
	}
	constexpr const NeverNullType value() const
	{
		UCodeLangAssert(Value);
		return NeverNullType::Make(Value);
	}
	constexpr NeverNullType value()
	{
		UCodeLangAssert(Value);
		return NeverNullType::Make(Value);
	}


	
	constexpr PtrType value_unchecked()noexcept
	{
		return Value;
	}
	constexpr const PtrType value_unchecked() const noexcept
	{
		return Value;
	}

	NeverNullType value_or(NeverNullType other)
	{
		if (has_value())
		{
			return value();
		}
		else
		{
			return other;
		}
	}

	const NeverNullType value_or(NeverNullType other) const
	{
		if (has_value())
		{
			return value()
		}
		else
		{
			return other;
		}
	}

	bool operator==(const NullablePtr other) const
	{
		return Value == other.Value;
	}
	bool operator!=(const NullablePtr other) const
	{
		return Value == other.Value;
	}
private:
	PtrType Value;
};


template<typename T> NullablePtr<T> Nullableptr(T* Value)
{
	return NullablePtr<T>::Make(Value);
}
template<typename T> const NullablePtr<T> Nullableptr(const T* Value)
{
	return NullablePtr<T>::Make(Value);
}

template<typename T>
struct OptionalRef
{
public:
	using NeverNullType = NeverNullPtr<T>;
	using NullableType = NullablePtr<T>;

	explicit OptionalRef(T& value)
		: base(&value)
	{

	}
	NullableType asNullable()
	{
		return NullableType::Make(base);
	}
	const NullableType asNullable() const
	{
		return NullableType::Make(base);
	}

	constexpr bool has_value() const noexcept
	{
		return Value != nullptr;
	}

	const T& value() const
	{
		UCodeLangAssert(Value);
		return *base;
	}
	T& value() 
	{
		UCodeLangAssert(Value);
		return *base;
	}
private:
	T* base =nullptr;
};

template<typename T> OptionalRef<T> Optionalref(T& Value)
{
	return OptionalRef(Value);
}
template<typename T> const OptionalRef<T> Nullableptr(const T& Value)
{
	return OptionalRef((T&)Value);
}

UCodeLangEnd


template<typename T>
struct std::hash<UCodeLang::NullablePtr<T>>
{
	std::size_t operator()(const UCodeLang::NullablePtr<T>& k) const
	{
		auto hasher = hash<T*>();

		return hasher(k.value_unchecked());
	}
};

template<typename T>
struct std::hash<UCodeLang::NeverNullPtr<T>>
{
	std::size_t operator()(const UCodeLang::NeverNullPtr<T>& k) const
	{
		auto hasher = hash<T*>();

		return hasher(k.value());
	}
};