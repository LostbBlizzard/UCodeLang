#include "set"
#include "../LangTypes.hpp"
UCodeLangStart

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
private:
	std::set<Key> _Base;
	
};
UCodeLangEnd