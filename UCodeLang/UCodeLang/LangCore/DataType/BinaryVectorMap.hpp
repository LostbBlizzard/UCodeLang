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
	using HashValue = Key;

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
	

	BinaryVectorMap() {}
	~BinaryVectorMap(){}

	
	void AddValue(ConstKey key,const Value& Item)
	{
		#if UCodeLangDebug
		if (HasValue(key))
		{
			UCodeLangThrowException("has same key");
		}
		#endif // DEBUG
		AddVectorBase(VectorBase(Hash(key), Item));
	}
	void AddValue(ConstKey key, Value&& Item)
	{
		#if UCodeLangDebug
		if (HasValue(key))
		{
			UCodeLangThrowException("has same key");
		}
		#endif // DEBUG
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
		#if UCodeLangDebug
		if (!HasValue(key))
		{
			UCodeLangThrowException("there no Value for the key");
		}
		#endif // DEBUG
		return 	GetBase(key)->_Value;
	}
	const Value& GetValue(ConstKey key) const
	{
		if (!HasValue(key))
		{
			UCodeLangThrowException("there no Value for the key");
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
	size_t count(ConstKey key) const { return HasValue(key); }

	//
	void clear() {Base.clear();}
	size_t size() const { return Base.size(); }
	size_t capacity() const { return Base.capacity(); }
	void reserve(size_t capacity) { Base.reserve(capacity); }

	using _VectorBase = Vector<VectorBase>;
	using iterator = typename _VectorBase::iterator;
	using const_iterator = typename _VectorBase::const_iterator;

	iterator  begin(){ OrderedCheck(); return Base.begin(); }
	iterator  end(){ return Base.end(); }

	const_iterator begin() const { return Base.begin(); }
	const_iterator end()const { return Base.end(); }

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
	_VectorBase Base;
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
		auto index = GetBaseIndex(V);

		if (index)
		{
			auto item = *index;
			return &Base[item];
		}
		return nullptr;
	}
	VectorBase* GetBase(ConstKey V)
	{
		auto index = GetBaseIndex(V);

		if (index)
		{
			auto item = *index;
			return &Base[item];
		}
		return nullptr;
	}

	const Optional<size_t> GetBaseIndex(ConstKey V) const
	{
		HashValue hash = Hash(V);
		for (size_t i = 0; i < Base.size(); i++)
		{
			if (Base[i]._Key == hash)
			{
				return i;
			}
		}

		return {};
	}
};


//Use this To Map  without the possibility of a unordered_map collision uses a linear search to Find Items
//Key should have the == overload
template<typename Key, typename Value>
class VectorMap
{

public:
	using ConstKey = const Key&;
	using HashValue = Key;

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
	
	VectorMap() {}
	~VectorMap() {}

	VectorMap(VectorMap&& Other) = default;
	VectorMap(const VectorMap& Other) = default;

	
	VectorMap& operator=(VectorMap&& Other) = default;
	VectorMap& operator=(const VectorMap & Other) = default;


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
		#if UCodeLangDebug
		if (!HasValue(key))
		{
			UCodeLangThrowException("there no Value for the key");
		}
		#endif // DEBUG
		
		return 	GetBase(key)->_Value;
	}
	const Value& GetValue(ConstKey key) const
	{
		if (!HasValue(key))
		{
			UCodeLangThrowException("there no Value for the key");
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
	size_t count(ConstKey key) const { return HasValue(key); }

	void clear() { Base.clear(); }
	size_t size() const { return Base.size(); }
	size_t capacity() const { return Base.capacity(); }
	void reserve(size_t capacity) { Base.reserve(capacity); }


	using _VectorBase = Vector<VectorBase>;
	using iterator = typename _VectorBase::iterator;
	using const_iterator = typename _VectorBase::const_iterator;

	iterator  begin(){return Base.begin(); }
	iterator  end() { return Base.end(); }

	const_iterator begin() const { return Base.begin(); }
	const_iterator end() const { return Base.end(); }

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
		auto index = GetBaseIndex(V);
		
		if (index)
		{
			auto item = *index;
			return &Base[item];
		}
		return nullptr;
	}
	VectorBase* GetBase(ConstKey V)
	{
		auto index = GetBaseIndex(V);

		if (index)
		{
			auto item = *index;
			return &Base[item];
		}
		return nullptr;
	}

	const Optional<size_t> GetBaseIndex(ConstKey V) const
	{
		HashValue hash = Hash(V);
		for (size_t i = 0; i < Base.size(); i++)
		{
			if (Base[i]._Key == hash)
			{
				return i;
			}
		}
	
		return {};
	}

};
UCodeLangEnd