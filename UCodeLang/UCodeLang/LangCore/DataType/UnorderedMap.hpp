#pragma once
#include "../LangTypes.hpp"
UCodeLangStart


// Unordered_Map but with safer Funcions
template<typename Key, typename Value>
class UnorderedMap
{
public:

	using ConstKey = const Key&;

	UnorderedMap() {}
	~UnorderedMap() {}


	void AddValue(ConstKey key, const Value& Item)
	{
		#if UCodeLangDebug
		if (HasValue(key))
		{
			UCodeLangThrowException("has same key");
		}
		#endif // DEBUG
		Base[key] = Item;
	}
	void AddValue(ConstKey key, Value&& Item)
	{
		#if UCodeLangDebug
		if (HasValue(key))
		{
			UCodeLangThrowException("has same key");
		}
		#endif // DEBUG
		Base[key] = Item;
	}

	bool HasValue(ConstKey key)
	{
		return Base.count(key);
	}
	bool HasValue(ConstKey key) const
	{
		return Base.count(key);
	}

	OptionalRef<Value> TryFindValue(ConstKey key)
	{
		if (HasValue(key))
		{
			return Optionalref(Base.at(key));
		}
		return {};
	}
	const OptionalRef<Value> TryFindValue(ConstKey key) const
	{
		if (HasValue(key))
		{
			return Optionalref(Base.at(key));
		}
		return {};
	}

	Value& GetOrAdd(ConstKey key,const Value& Or)
	{
		if (!HasValue(key))
		{
			AddValue(key, Or);
		}
		return Base.at(key);
	}
	Value& GetOrAdd(ConstKey key, Value&& Or)
	{
		if (!HasValue(key))
		{
			AddValue(key,std::move(Or));
		}
		return Base.at(key);
	}

	Value& GetValue(ConstKey key)
	{
		#if UCodeLangDebug
		if (!HasValue(key))
		{
			UCodeLangThrowException("there no Value for the key");
		}
		#endif // DEBUG
		return 	Base.at(key);
	}
	const Value& GetValue(ConstKey key) const
	{
		if (!HasValue(key))
		{
			UCodeLangThrowException("there no Value for the key");
		}
		return Base.at(key);
	}

	//
	void clear() { Base.clear(); }
	size_t size() const { return Base.size(); }
	size_t capacity() const { return Base.capacity(); }
	void reserve(size_t capacity) { Base.reserve(capacity); }

	using _UnorderedBase =typename Unordered_map<Key, Value>;
	using iterator = typename _UnorderedBase::iterator;
	using const_iterator = typename _UnorderedBase::const_iterator;

	iterator  begin() { return Base.begin(); }
	iterator  end() { return Base.end(); }

	const_iterator begin() const { return Base.begin(); }
	const_iterator end()const { return Base.end(); }

	iterator erase(iterator _Where)
	{
		return Base.erase(_Where);
	}
	iterator erase(const_iterator _Where)
	{
		return Base.erase(_Where);
	}
	iterator erase(ConstKey _Where)
	{
		#if UCodeLangDebug

		if (!HasBase(_Where))
		{
			UCodeLangThrowException("there no Value for the key");
		}

		#endif // DEBUG
		return Base.erase(Base.begin() + GetBaseIndex(_Where).value());
	}
private:
	Unordered_map<Key,Value> Base;
};
	


UCodeLangEnd