#include "set"
#include "unordered_set"
#include "../LangTypes.hpp"
UCodeLangStart

/*
template<typename Key>
class Set
{
public:
	using ConstKey = const Key&;

	void AddValue(ConstKey key)
	{
		_Base.insert(key);
	}
	void AddValue(Key&& key)
	{
		_Base.insert(std::move(key));
	}
	
	bool HasValue(ConstKey key) const
	{
		return _Base.count(key);
	}
	void Erase(ConstKey key)
	{
		_Base.erase(key);
	}
	size_t size() const
	{
		return _Base.size();
	}
private:
	std::set<Key> _Base;
	
};
*/

template<typename Key>
class UnorderedSet
{
public:
	using ConstKey = const Key&;

	void AddValue(ConstKey key)
	{
		_Base.insert(key);
	}
	void AddValue(Key&& key)
	{
		_Base.insert(std::move(key));
	}
	
	bool HasValue(ConstKey key) const
	{
		return _Base.count(key);
	}
	void Erase(ConstKey key)
	{
		_Base.erase(key);
	}
	size_t size() const
	{
		return _Base.size();
	}
private:
	std::unordered_set<Key> _Base;
	
};
UCodeLangEnd