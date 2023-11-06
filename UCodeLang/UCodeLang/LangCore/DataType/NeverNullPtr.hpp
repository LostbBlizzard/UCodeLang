#pragma once
#include "../UCodeLangNameSpace.hpp"
#include "../LangDef.hpp"
#include <functional>//hash
UCodeLangStart


template<typename T>
struct NullablePtr;

template<typename T> 
struct NeverNullPtr
{
public:
	using PtrType = T*;
	using ThisType = NeverNullPtr<T>;
	using NullableType= NullablePtr<T>;

	NeverNullPtr()
		:Value(nullptr)
	{

	}
	NeverNullPtr(PtrType Value)
		:Value(Value)
	{
		UCodeLangAssert(Value);//never null
	}
	UCodeLangNoDiscard UCodeLangForceinline static const ThisType Make(const T* Value)
	{
		return ThisType((PtrType)Value);
	}
	UCodeLangNoDiscard UCodeLangForceinline static ThisType Make(T* Value)
	{
		return ThisType(Value);
	}
	
	UCodeLangNoDiscard UCodeLangForceinline constexpr PtrType value() noexcept
	{
		return Value;
	}
	UCodeLangNoDiscard UCodeLangForceinline constexpr const PtrType value() const noexcept
	{
		return Value;
	}

	UCodeLangNoDiscard UCodeLangForceinline constexpr const T* operator->() const noexcept
	{
		return Value;
	}
	UCodeLangNoDiscard UCodeLangForceinline constexpr T* operator->() noexcept
	{
		return Value;
	}

	UCodeLangNoDiscard UCodeLangForceinline constexpr const T& operator*() const noexcept
	{
		return *Value;
	}
	UCodeLangNoDiscard UCodeLangForceinline constexpr T& operator*() noexcept
	{
		return *Value;
	}

	UCodeLangNoDiscard UCodeLangForceinline bool operator==(const NeverNullPtr other) const
	{
		return Value == other.Value;
	}
	UCodeLangNoDiscard UCodeLangForceinline bool operator!=(const NeverNullPtr other) const
	{
		return Value != other.Value;
	}


	UCodeLangNoDiscard UCodeLangForceinline const NullableType AsNullable() const
	{
		const NullableType r = NullableType::Make(value());
		return r;
	}
	UCodeLangNoDiscard UCodeLangForceinline NullableType AsNullable()
	{
		NullableType r = NullableType::Make(value());
		return r;
	}
private:
	PtrType Value;
};

template<typename T> UCodeLangNoDiscard NeverNullPtr<T> NeverNullptr(T* Value)
{
	return NeverNullPtr<T>::Make(Value);
}

template<typename T> UCodeLangNoDiscard const NeverNullPtr<T> NeverNullptr(const T* Value)
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
	UCodeLangNoDiscard UCodeLangForceinline static ThisType Make(T* Value)
	{
		return ThisType(Value);
	}
	UCodeLangNoDiscard UCodeLangForceinline static const ThisType Make(const T* Value)
	{
		return ThisType((PtrType)Value);
	}

	UCodeLangNoDiscard UCodeLangForceinline constexpr explicit operator bool() const
	{
		return Value != nullptr;
	}
	UCodeLangNoDiscard UCodeLangForceinline constexpr bool has_value() const noexcept
	{
		return Value != nullptr;
	}
	UCodeLangNoDiscard UCodeLangForceinline constexpr const NeverNullType value() const
	{
		UCodeLangAssert(Value);
		return NeverNullType::Make(Value);
	}
	UCodeLangNoDiscard UCodeLangForceinline constexpr NeverNullType value()
	{
		UCodeLangAssert(Value);
		return NeverNullType::Make(Value);
	}


	
	UCodeLangNoDiscard UCodeLangForceinline constexpr PtrType value_unchecked()noexcept
	{
		return Value;
	}
	UCodeLangNoDiscard UCodeLangForceinline constexpr const PtrType value_unchecked() const noexcept
	{
		return Value;
	}

	UCodeLangNoDiscard UCodeLangForceinline NeverNullType value_or(NeverNullType other)
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

	UCodeLangNoDiscard UCodeLangForceinline const NeverNullType value_or(NeverNullType other) const
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

	UCodeLangNoDiscard UCodeLangForceinline bool operator==(const NullablePtr other) const
	{
		return Value == other.Value;
	}
	UCodeLangNoDiscard UCodeLangForceinline bool operator!=(const NullablePtr other) const
	{
		return Value == other.Value;
	}
private:
	PtrType Value;
};




template<typename T> UCodeLangNoDiscard NullablePtr<T> Nullableptr(T* Value)
{
	return NullablePtr<T>::Make(Value);
}
template<typename T> UCodeLangNoDiscard const NullablePtr<T> Nullableptr(const T* Value)
{
	return NullablePtr<T>::Make(Value);
}



template<typename T>
struct OptionalRef
{
public:
	using NeverNullType = NeverNullPtr<T>;
	using NullableType = NullablePtr<T>;

	constexpr OptionalRef()
	{

	}
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
		return base != nullptr;
	}

	const T& value() const
	{
		UCodeLangAssert(base);
		return *base;
	}
	T& value() 
	{
		UCodeLangAssert(base);
		return *base;
	}
private:
	T* base =nullptr;
};

template<typename T> OptionalRef<T> Optionalref(T& Value)
{
	return OptionalRef(Value);
}
template<typename T> const OptionalRef<T> Optionalref(const T& Value)
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