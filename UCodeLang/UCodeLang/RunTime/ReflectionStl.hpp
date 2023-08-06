#pragma once
#include "AnyInterpreter.hpp"
UCodeLangStart


//follows std::vector class kind of
//a wapper to make use UCodeVectors easier.
struct ReflectionVector
{
public:
	using Info_t = ClassAssembly::InfoVector_t;
	ReflectionVector(){}
	~ReflectionVector(){}


	inline void* data()
	{
		return _ptr.RCall<void*>(_Info->Data_Method, _UObject);
	}
	inline size_t size()
	{
		return _ptr.RCall<size_t>(_Info->Size_Method, _UObject);
	}
	inline size_t capacity()
	{
		return _ptr.RThisCall<size_t>(_Info->Capacity_Method,_UObject);
	}
	inline bool empty()
	{
		return size() == 0;
	}
	inline void reserve(size_t size)
	{
		_ptr.ThisCall(_Info->Reserve_Method, _UObject, size);
	}
	inline size_t max_size()
	{
		return UInt64_MaxSize;
	}
	inline void resize(size_t size)
	{
		_ptr.ThisCall(_Info->Resize_Method, _UObject, size);
	}
	inline void clear()
	{
		_ptr.ThisCall(_Info->Clear_Method, _UObject);
	}
	

	//Call Destructor after
	inline void pop_p(void* Out,size_t OutSize)
	{
		#ifdef DEBUG
		if (OutSize != GetElemSize())
		{
			throw std::exception("bad cast");
		}
		if (size() == 0)
		{
			throw std::exception("out of bounds.");
		}
		#endif // DEBUG

		_ptr.PushParameter(_UObject);
		_ptr.Call(_Info->Pop_Method);

		_ptr.Get_Return(Out, OutSize);
	}
	inline void pop()
	{
		BytesPtr tep;
		tep.Resize(GetElemSize());

		pop_p(tep.Data(), tep.Size());
		auto info = assembly->CallDestructor(GetElementType(), tep.Data(), is32bit);

		#ifdef DEBUG
		if (!info.has_value())
		{
			throw std::exception("Calling Destructor fail");
		}
		#endif // DEBUG

		if (info.has_value()) 
		{
			auto& O = info.value();
			if (O.has_value())
			{
				for (auto& Item : O.value())
				{
					_ptr.ThisCall(Item.MethodToCall, Item.ThisPtr);
				}
			}
		}
	}
	template<typename T> void pop_t(T& Obj)
	{
		pop_p(&Obj,sizeof(T));
	}

	void push(const void* Obj)
	{
		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Obj, sizeof(void*));

		_ptr.Call(_Info->Insert_Copy_Method);
	}
	void push(void*&& Obj)
	{
		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Obj, sizeof(void*));

		_ptr.Call(_Info->Insert_Copy_Method);
	}
	template<typename T> void push_t(const T& Obj)
	{
		push(&Obj);
	}
	template<typename T> void push_t(T&& Obj)
	{
		push(std::move((void*)&Obj));
	}

	inline void insert(size_t index,const void* Obj)
	{
		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Obj,sizeof(void*));
		
		_ptr.Call(_Info->Insert_Copy_Method);
	}
	inline void insert(size_t index,void*&& Obj)
	{
		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Obj, sizeof(void*));

		_ptr.Call(_Info->Insert_Moved_Method);
	}


	template<typename T>
	void insert_t(size_t index,const T& Obj)
	{
		insert(index, &Obj);
	}
	
	template<typename T>
	void insert_t(size_t index,T&& Obj)
	{
		insert(index,std::move((void*)&Obj));
	}

	//Call Destructor after
	inline void remove_p(size_t index, void* Out, size_t OutSize)
	{
		#ifdef DEBUG
		if (OutSize != GetElemSize())
		{
			throw std::exception("bad cast");
		}
		if (index >= size())
		{
			throw std::exception("out of bounds");
		}
		#endif // DEBUG

		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(index);

		_ptr.Call(_Info->Remove_Method);

		_ptr.Get_Return(Out, OutSize);
	}
	template<typename T>  inline T remove_t(size_t index)
	{
		T b;
		remove_p(index,&b,sizeof(T));
		return b;
	}
	void remove(size_t index)
	{
		BytesPtr tep;
		tep.Resize(GetElemSize());

		remove_p(index,tep.Data(), tep.Size());
		auto info = assembly->CallDestructor(GetElementType(), tep.Data(), is32bit);

		#ifdef DEBUG
		if (!info.has_value())
		{
			throw std::exception("Calling Destructor fail");
		}
		#endif // DEBUG

		if (info.has_value())
		{
			auto& O = info.value();
			if (O.has_value())
			{
				for (auto& Item : O.value())
				{
					_ptr.ThisCall(Item.MethodToCall, Item.ThisPtr);
				}
			}
		}
	}

	inline void* at(size_t index)
	{
		#ifdef DEBUG
		if (index >= size())
		{
			throw std::exception("out of bounds");
		}
		#endif // DEBUG

		return (void*)((uintptr_t)data() + (index * GetElemSize()));
	}

	//Use at to remove memory allocations
	inline void* operator[](size_t index)
	{
		return  at(index);
	}

	class iterator {
	public:
		iterator(ReflectionVector* This) : This(This) {}
		iterator(ReflectionVector* This,size_t Index) : This(This), Index(Index) {}
		inline iterator operator++() { Index++; return *this; }
		inline bool operator!=(const iterator& other) const { return Index != other.Index; }
		inline void* operator*() { return  This->at(Index); }
	private:
		ReflectionVector* This =nullptr;
		size_t Index =0;
	};

	iterator begin() { return iterator(this); }
	iterator end() { return iterator(this,size()); }
	//
	void Set(void* UObject, const Info_t* Info, 
		AnyInterpreterPtr ptr,
		const ClassAssembly& assembly,
		bool is32bit)
	{
		SetUObject(UObject);
		SetInfo(Info);
		SetInterpreter(ptr);
	}
	inline void SetUObject(void* UObject){_UObject = UObject;}
	inline void SetInfo(const Info_t* Info){_Info = Info;}
	inline void SetInterpreter(AnyInterpreterPtr ptr){_ptr = ptr;}
	inline void SetAssembly(const ClassAssembly& assembly,bool is32bit)
	{
		elemsize = assembly.GetSize(GetElementType(), is32bit).value_or(0);
		this->assembly = &assembly;
		this->is32bit = is32bit;
	}

	inline ReflectionTypeInfo GetElementType()
	{
		return _Info->ElementType;
	}
	inline size_t GetElementTypeSize()
	{
		return elemsize;
	}
private:
	inline size_t GetElemSize()
	{
		return  elemsize;
	}
	void* _UObject = nullptr;
	AnyInterpreterPtr _ptr;
	const Info_t* _Info = nullptr;
	size_t elemsize =0;
	const ClassAssembly* assembly =nullptr;
	bool is32bit = false;
};

UCodeLangEnd