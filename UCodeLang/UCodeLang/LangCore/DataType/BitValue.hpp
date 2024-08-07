#pragma once
#include "../LangTypes.hpp"
UCodeLangStart


template<typename T>
class BitValue
{
public:
	BitValue(){}
	explicit BitValue(T Value) { _Base = Value; }

	UCodeLangForceinline bool GetValue(size_t Index) const
	{
		#if UCodeLangDebug
		if (Index > BitsCount)
		{
			UCodeLangThrowException("Index out of range");
		}
		#endif // DEBUG

		return (_Base >> Index) & ((T)1);
	}
	UCodeLangForceinline void SetValue(size_t Index, bool Value)
	{
		#if UCodeLangDebug
		if (Index > BitsCount)
		{
			UCodeLangThrowException("Index out of range");
		}
		#endif // DEBUG

		if (Value) {
			_Base |= ((T)1) << Index;
		}
		else
		{
			_Base &= ~(((T)1) << Index);
		}
	}
	UCodeLangForceinline void FilpBit(size_t Index)
	{
		#if UCodeLangDebug
		if (Index > BitsCount)
		{
			UCodeLangThrowException("Index out of range");
		}
		#endif // DEBUG
		_Base ^= ((T)1) << Index;
	}
	
	T Get_Base() const
	{
		return _Base;
	}
	void Clearbits()
	{
		_Base = 0;
	}
private:
	constexpr static size_t BitsCount = 0;
	T _Base = 0;
};


using BitByte = BitValue<Byte>;


template<typename T>
class BitValueE
{
public:
	using BaseType = std::underlying_type_t<T>;
	BitValueE() {}
	explicit BitValueE(T Value) { _Base = (BaseType)Value; }

	UCodeLangForceinline bool GetValue(T Index) const
	{
		return _Base.GetValue((BaseType)Index);
	}
	UCodeLangForceinline void SetValue(T Index, bool Value)
	{
		_Base.SetValue(Index,(BaseType)Value);
	}
	UCodeLangForceinline void FilpBit(T Index)
	{
		_Base.FilpBit((BaseType)Index);
	}

	T Get_Base() const
	{
		return _Base.Get_Base();
	}
	void Clearbits()
	{
		_Base = BitValue<BaseType>(0);
	}
private:
	BitValue<BaseType> _Base;
};
UCodeLangEnd