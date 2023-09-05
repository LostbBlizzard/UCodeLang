#pragma once
#include "../UCodeLangNameSpace.hpp"
#include "../LangDef.hpp"
#include <xhash>
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

	NeverNullPtr(PtrType Value)
		:Value(Value)
	{
		UCodeLangAssert(Value);//never null
	}
	UCodeLangForceinline UCodeLangNoDiscard static const ThisType Make(const T* Value)
	{
		return ThisType((PtrType)Value);
	}
	UCodeLangForceinline UCodeLangNoDiscard static ThisType Make(T* Value)
	{
		return ThisType(Value);
	}
	
	UCodeLangForceinline UCodeLangNoDiscard constexpr PtrType value() noexcept
	{
		return Value;
	}
	UCodeLangForceinline UCodeLangNoDiscard constexpr const PtrType value() const noexcept
	{
		return Value;
	}

	UCodeLangForceinline UCodeLangNoDiscard constexpr const T* operator->() const noexcept
	{
		return Value;
	}
	UCodeLangForceinline UCodeLangNoDiscard constexpr T* operator->() noexcept
	{
		return Value;
	}

	UCodeLangForceinline UCodeLangNoDiscard constexpr const T& operator*() const noexcept
	{
		return *Value;
	}
	UCodeLangForceinline UCodeLangNoDiscard constexpr T& operator*() noexcept
	{
		return *Value;
	}

	UCodeLangForceinline UCodeLangNoDiscard bool operator==(const NeverNullPtr other) const
	{
		return Value == other.Value;
	}
	UCodeLangForceinline UCodeLangNoDiscard bool operator!=(const NeverNullPtr other) const
	{
		return Value != other.Value;
	}


	UCodeLangForceinline UCodeLangNoDiscard const NullableType AsNullable() const
	{
		const NullableType r = NullableType::Make(value());
		return r;
	}
	UCodeLangForceinline UCodeLangNoDiscard NullableType AsNullable()
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
	UCodeLangForceinline UCodeLangNoDiscard static ThisType Make(T* Value)
	{
		return ThisType(Value);
	}
	UCodeLangForceinline UCodeLangNoDiscard static const ThisType Make(const T* Value)
	{
		return ThisType((PtrType)Value);
	}

	UCodeLangForceinline UCodeLangNoDiscard constexpr explicit operator bool() const
	{
		return Value != nullptr;
	}
	UCodeLangForceinline UCodeLangNoDiscard constexpr bool has_value() const noexcept
	{
		return Value != nullptr;
	}
	UCodeLangForceinline UCodeLangNoDiscard constexpr const NeverNullType value() const
	{
		UCodeLangAssert(Value);
		return NeverNullType::Make(Value);
	}
	UCodeLangForceinline UCodeLangNoDiscard constexpr NeverNullType value()
	{
		UCodeLangAssert(Value);
		return NeverNullType::Make(Value);
	}


	
	UCodeLangForceinline UCodeLangNoDiscard constexpr PtrType value_unchecked()noexcept
	{
		return Value;
	}
	UCodeLangForceinline UCodeLangNoDiscard constexpr const PtrType value_unchecked() const noexcept
	{
		return Value;
	}

	UCodeLangForceinline UCodeLangNoDiscard NeverNullType value_or(NeverNullType other)
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

	UCodeLangForceinline UCodeLangNoDiscard const NeverNullType value_or(NeverNullType other) const
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

	UCodeLangForceinline UCodeLangNoDiscard bool operator==(const NullablePtr other) const
	{
		return Value == other.Value;
	}
	UCodeLangForceinline UCodeLangNoDiscard bool operator!=(const NullablePtr other) const
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