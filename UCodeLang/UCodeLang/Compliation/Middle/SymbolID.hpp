#pragma once
#include "UCodeLang/LangCore/ScopeHelper.hpp"
UCodeLangStart


//To Stop missuse
struct SymbolID
{
public:
	SymbolID() noexcept
	{
		Base = NullVal;
	}
	SymbolID(const SymbolID& Src) noexcept
	{
		Base = Src.Base;
	}
	SymbolID(SymbolID&& Src) noexcept
	{
		 Base = Src.Base;
	}

	SymbolID& operator=(const SymbolID& Src) noexcept
	{
		Base = Src.Base;
		return *this;
	}
	SymbolID& operator=(SymbolID&& Src) noexcept
	{
		Base = Src.Base;
		return *this;
	}
	explicit SymbolID(uintptr_t Value)
	{
		Base =Value;
	}
	bool operator==(const SymbolID& other) const
	{
		return other.Base == this->Base;
	}
	bool operator!=(const SymbolID& other) const
	{
		return other.Base != this->Base;
	}

	uintptr_t AsInt() const
	{
		return Base;
	}

	bool operator<(const SymbolID& other) const
	{
		return other.Base < this->Base;
	}
	bool operator>(const SymbolID& other) const
	{
		return other.Base > this->Base;
	}
	bool HasValue() const
	{
		return Base != NullVal;
	}
private:
	static constexpr uintptr_t NullVal = 0;
	uintptr_t Base;
};
UCodeLangEnd


template <>
struct std::hash<UCodeLang::SymbolID>
{
	std::size_t operator()(const UCodeLang::SymbolID& k) const
	{
		return hash<uintptr_t>()(k.AsInt());

	}
};