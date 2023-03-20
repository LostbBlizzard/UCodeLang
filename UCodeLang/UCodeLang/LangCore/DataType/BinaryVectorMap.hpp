#pragma once
#include "../LangTypes.hpp"
UCodeLangStart


//Use this To Map a Key to an Value without the possibility of a unordered_map collision 
//uses binary search to find Items or a linear search on some cases.
//Key should be unsigned,T*,or void* or have < overload
//else use VectorMap
template<typename Key, typename Value> 
class BinaryVectorMap
{

public:
	using ConstKey = const Key&;
	using HashValue = size_t;
private:
	struct VectorBase
	{
		HashValue _Key{};
		Value _Value{};
		
		VectorBase(HashValue key,const Value& value)
			:_Key(key), _Value(value)
		{
		}
		VectorBase(HashValue key,Value&& value)
			:_Key(key), _Value(value)
		{
		}

		VectorBase(){}


		~VectorBase(){}
	};
public:

	BinaryVectorMap() {}
	~BinaryVectorMap(){}

	
	void AddValue(ConstKey key,const Value& Item)
	{
		AddVectorBase(VectorBase(Hash(key), Item));
	}
	void AddValue(ConstKey key, Value&& Item)
	{
		AddVectorBase(VectorBase(Hash(key), Item));
	}

	bool HasValue(ConstKey key)
	{
		return HasBase(key);
	}
	bool HasValue(ConstKey key) const
	{
		return HasBase(key);
	}

	Optional<Value*> TryFindValue(ConstKey key)
	{
		if (HasValue(key))
		{
			return &GetBase(key)._Value;
		}
		return {};
	}
	Optional<const Value*> TryFindValue(ConstKey key) const
	{
		if (HasValue(key))
		{
			return &GetBase(key)._Value;
		}
		return {};
	}

	Value& GetValue(ConstKey key)
	{
		if (!HasValue(key))
		{
			throw std::exception("there no Value for the key");
		}
		return 	GetBase(key)->_Value;
	}
	const Value& GetValue(ConstKey key) const
	{
		if (!HasValue(key))
		{
			throw std::exception("there no Value for the key");
		}
		return 	GetBase(key)->_Value;
	}

	//Unordered_map API
	Value& operator[](ConstKey key)
	{

		if (!HasValue(key))
		{
			Value tep;
			AddValue(key, tep);
		}
		return at(key);
	}

	Optional<const Value*> operator[](ConstKey key) const
	{
		return  TryFindValue(key);
	}
	Value& at(ConstKey key)
	{
		return  GetValue(key);
	}
	const Value& at(ConstKey key) const
	{
		return  GetValue(key);
	}
	size_t count() const { return HasValue(); }

	void clear() {Base.clear();}
	size_t size() const { return Base.size(); }
	size_t capacity() const { return Base.capacity(); }
private:
	Vector<VectorBase> Base;
	bool IsOrdered = false;
	void AddVectorBase(VectorBase&& Value)
	{
		IsOrdered = false;
		Base.push_back(Value);
	}
	static HashValue Hash(ConstKey key)
	{
		return (HashValue)key;
	}
	void OrderedCheck()
	{
		if (IsOrdered == false)
		{
			SortVector();
		}
	}
	void SortVector()
	{

	}
	bool HasBase(ConstKey V) const
	{
		return  GetBase(V) != nullptr;
	}
	const VectorBase* GetBase(ConstKey V) const
	{
		HashValue hash = Hash(V);
		for (auto& Item : Base)
		{
			if (Item._Key == hash)
			{
				return &Item;
			}
		}
		return nullptr;
	}

	VectorBase* GetBase(ConstKey V)
	{
		OrderedCheck();

		HashValue hash = Hash(V);
		for (auto& Item : Base)
		{
			if (Item._Key == hash)
			{
				return &Item;
			}
		}
		return nullptr;
	}
};


//Use this To Map  without the possibility of a unordered_map collision uses a linear searc to Find Items
//Key should have the == overload
template<typename Key, typename Value>
class VectorMap
{

public:
	using ConstKey = const Key&;
	using HashValue = Key;
private:
	struct VectorBase
	{
		HashValue _Key{};
		Value _Value{};

		VectorBase(HashValue key, const Value& value)
			:_Key(key), _Value(value)
		{
		}
		VectorBase(HashValue key, Value&& value)
			:_Key(key), _Value(value)
		{
		}

		VectorBase() {}


		~VectorBase() {}
	};
public:

	VectorMap() {}
	~VectorMap() {}


	void AddValue(ConstKey key, const Value& Item)
	{
		AddVectorBase(VectorBase(Hash(key), Item));
	}
	void AddValue(ConstKey key, Value&& Item)
	{
		AddVectorBase(VectorBase(Hash(key), Item));
	}

	bool HasValue(ConstKey key)
	{
		return HasBase(key);
	}
	bool HasValue(ConstKey key) const
	{
		return HasBase(key);
	}

	

	Optional<Value*> TryFindValue(ConstKey key)
	{
		if (HasValue(key))
		{
			return &GetBase(key)._Value;
		}
		return {};
	}
	Optional<const Value*> TryFindValue(ConstKey key) const
	{
		if (HasValue(key))
		{
			return &GetBase(key)._Value;
		}
		return {};
	}

	Value& GetValue(ConstKey key)
	{
		if (!HasValue(key))
		{
			throw std::exception("there no Value for the key");
		}
		return 	GetBase(key)->_Value;
	}
	const Value& GetValue(ConstKey key) const
	{
		if (!HasValue(key))
		{
			throw std::exception("there no Value for the key");
		}
		return 	GetBase(key)->_Value;
	}

	
	
	//Unordered_map API
	Value& operator[](ConstKey key)
	{

		if (!HasValue(key))
		{
			Value tep;
			AddValue(key, tep);
		}
		return at(key);
	}

	Optional<const Value*> operator[](ConstKey key) const
	{
		return  TryFindValue(key);
	}
	Value& at(ConstKey key)
	{
		return  GetValue(key);
	}
	const Value& at(ConstKey key) const
	{
		return  GetValue(key);
	}
	size_t count(ConstKey key) const { return HasValue(); }

	void clear() { Base.clear(); }
	size_t size() const { return Base.size(); }
	size_t capacity() const { return Base.capacity(); }
private:
	Vector<VectorBase> Base;
	void AddVectorBase(VectorBase&& Value)
	{
		Base.push_back(Value);
	}
	static HashValue Hash(ConstKey key)
	{
		return key;
	}
	bool HasBase(ConstKey V) const
	{
		return  GetBase(V) != nullptr;
	}
	const VectorBase* GetBase(ConstKey V) const
	{
		HashValue hash = Hash(V);
		for (auto& Item : Base)
		{
			if (Item._Key == hash)
			{
				return &Item;
			}
		}
		return nullptr;
	}

	VectorBase* GetBase(ConstKey V)
	{
		
		HashValue hash = Hash(V);
		for (auto& Item : Base)
		{
			if (Item._Key == hash)
			{
				return &Item;
			}
		}
		return nullptr;
	}
};
UCodeLangEnd