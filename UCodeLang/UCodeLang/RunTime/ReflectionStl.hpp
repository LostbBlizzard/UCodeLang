#pragma once
#include "AnyInterpreter.hpp"
UCodeLangStart


//follows std::vector class kind of
//a wrapper to make use UCodeVectors easier.
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
		return SIZE_MAX;
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
		#if UCodeLangDebug
		if (OutSize != GetElemSize())
		{
			UCodeLangThrowException("bad cast");
		}
		if (size() == 0)
		{
			UCodeLangThrowException("out of bounds.");
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

		#if UCodeLangDebug
		if (!info.has_value())
		{
			UCodeLangThrowException("Calling Destructor fail");
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
		UCodeLangAssert(_Info->Push_copy_Method);//This Method may not exist if type is not copyable
		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Obj, sizeof(void*));
		
		_ptr.Call(_Info->Push_copy_Method);
	}
	void push(void*&& Obj)
	{
		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Obj, sizeof(void*));
		
		_ptr.Call(_Info->Push_moved_Method);
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
		UCodeLangAssert(_Info->Insert_Copy_Method);//This Method may not exist if type is not copyable
		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(index);
		_ptr.PushParameter(&Obj,sizeof(void*));
		
		_ptr.Call(_Info->Insert_Copy_Method);
	}
	inline void insert(size_t index,void*&& Obj)
	{
		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(index);
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
		#if UCodeLangDebug
		if (OutSize != GetElemSize())
		{
			UCodeLangThrowException("bad cast");
		}
		if (index >= size())
		{
			UCodeLangThrowException("out of bounds");
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

		#if UCodeLangDebug
		if (!info.has_value())
		{
			UCodeLangThrowException("Calling Destructor fail");
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
		#if UCodeLangDebug
		if (index >= size())
		{
			UCodeLangThrowException("out of bounds");
		}
		#endif // DEBUG

		return (void*)((uintptr_t)data() + (index * GetElemSize()));
	}

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

//follows std::string class kind of
//a wrapper to make use UCodeString easier.
struct ReflectionString
{
public:
	using Info_t = ClassAssembly::InfoString_t;
	ReflectionString(){}
	~ReflectionString(){}


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
		return _ptr.RThisCall<size_t>(_Info->Capacity_Method, _UObject);
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
		return SIZE_MAX;
	}
	inline void resize(size_t size)
	{
		_ptr.ThisCall(_Info->Resize_Method, _UObject, size);
	}
	inline void clear()
	{
		_ptr.ThisCall(_Info->Clear_Method, _UObject);
	}


	char pop_char()
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Char);


	}
	Utf8 pop_utf8()
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Uft8);

	}
	Utf16 pop_utf16()
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Uft16);


	}
	Utf32 pop_utf32()
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Uft32);

	}

	void push(char Item)
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Char);

		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Item, sizeof(char));

		_ptr.Call(_Info->Push_Method);
	}
	void push(Utf8 Item)
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Uft8);

		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Item, sizeof(Utf8));

		_ptr.Call(_Info->Push_Method);
	}
	void push(Utf16 Item)
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Uft16);

		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Item, sizeof(Utf16));

		_ptr.Call(_Info->Push_Method);
	}
	void push(Utf32 Item)
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Uft32);

		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Item, sizeof(Utf32));

		_ptr.Call(_Info->Push_Method);
	}

	inline void insert(size_t index,char Item)
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Char);

		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Item, sizeof(Item));

		_ptr.Call(_Info->Insert_Method);
	}
	inline void insert(size_t index,Utf8 Item)
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Uft8);

		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Item, sizeof(Item));

		_ptr.Call(_Info->Insert_Method);
	}
	inline void insert(size_t index, Utf16 Item)
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Uft16);

		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Item, sizeof(Item));

		_ptr.Call(_Info->Insert_Method);
	}
	inline void insert(size_t index, Utf32 Item)
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Uft32);

		_ptr.PushParameter(_UObject);
		_ptr.PushParameter(&Item, sizeof(Item));

		_ptr.Call(_Info->Insert_Method);
	}

	char remove_char(size_t index)
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Char);
		return char();
	}
	Utf8 remove_Uft8(size_t index)
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Uft8);
		return Utf8();
	}
	Utf16 remove_Uft16(size_t index)
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Uft16);
		return Utf16();
	}
	Utf32 remove_Uft32(size_t index)
	{
		UCodeLangAssert(GetElementType()._Type == ReflectionTypes::Uft32);
		return Utf32();
	}

	inline void* at(size_t index)
	{
#if UCodeLangDebug
		if (index >= size())
		{
			UCodeLangThrowException("out of bounds");
		}
#endif // DEBUG

		return (void*)((uintptr_t)data() + (index * GetElemSize()));
	}

	inline void* operator[](size_t index)
	{
		return  at(index);
	}


	ReflectionString& operator+=(char Item)
	{
		push(Item);
		return *this;
	}
	ReflectionString& operator+=(Utf8 Item)
	{
		push(Item);
		return *this;
	}
	ReflectionString& operator+=(Utf16 Item)
	{
		push(Item);
		return *this;
	}
	ReflectionString& operator+=(Utf32 Item)
	{
		push(Item);
		return *this;
	}


	ReflectionString& operator+=(String_view Item)
	{
		resize(size() + Item.size());
		for (auto CItem : Item) 
		{
			push(CItem);
		}
		return *this;
	}
	ReflectionString& operator+=(String_view8 Item)
	{
		resize(size() + Item.size());
		for (auto CItem : Item)
		{
			push(CItem);
		}
		return *this;
	}
	ReflectionString& operator+=(String_view16 Item)
	{
		resize(size() + Item.size());
		for (auto CItem : Item)
		{
			push(CItem);
		}
		return *this;
	}
	ReflectionString& operator+=(String_view32 Item)
	{
		resize(size() + Item.size());
		for (auto CItem : Item)
		{
			push(CItem);
		}
		return *this;
	}

	ReflectionString& operator+=(const String& Item)
	{
		return this->operator=(String_view(Item));
	}
	ReflectionString& operator+=(const String8& Item)
	{
		return this->operator=(String_view8(Item));
	}
	ReflectionString& operator+=(const String16& Item)
	{
		return this->operator=(String_view16(Item));
	}
	ReflectionString& operator+=(const String32& Item)
	{
		return this->operator+=(String_view32(Item));
	}

	ReflectionString& operator=(String_view Item)
	{
		resize(Item.size());
		
		auto dataptr = (char*)data();
		for (size_t i = 0; i < Item.size(); i++)
		{
			dataptr[i] = Item[i];
		}
		return *this;
	}
	ReflectionString& operator=(String_view8 Item)
	{
		resize(Item.size());

		auto dataptr = (Utf8*)data();
		for (size_t i = 0; i < Item.size(); i++)
		{
			dataptr[i] = Item[i];
		}
		return *this;
	}
	ReflectionString& operator=(String_view16 Item)
	{
		resize(Item.size());

		auto dataptr = (Utf16*)data();
		for (size_t i = 0; i < Item.size(); i++)
		{
			dataptr[i] = Item[i];
		}
		return *this;
	}
	ReflectionString& operator=(String_view32 Item)
	{
		resize(Item.size());

		auto dataptr = (Utf32*)data();
		for (size_t i = 0; i < Item.size(); i++)
		{
			dataptr[i] = Item[i];
		}
		return *this;
	}

	ReflectionString& operator=(const String& Item)
	{
		return this->operator=(String_view(Item));
	}
	ReflectionString& operator=(const String8& Item)
	{
		return this->operator=(String_view8(Item));
	}
	ReflectionString& operator=(const String16& Item)
	{
		return this->operator=(String_view16(Item));
	}
	ReflectionString& operator=(const String32& Item)
	{
		return this->operator=(String_view32(Item));
	}

	ReflectionString& operator=(ReflectionString& Item)
	{
		UCodeLangAssert(GetElementType() == Item.GetElementType());
		
		switch (Item.GetElementType()._Type)
		{
		case ReflectionTypes::Char:
			this->operator=(Item.AsCharView());
			break;
		case ReflectionTypes::Uft8:
			this->operator=(Item.AsCharView());
			break;
		case ReflectionTypes::Uft16:
			this->operator=(Item.AsCharView());
			break;
		case ReflectionTypes::Uft32:
			this->operator=(Item.AsCharView());
			break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
	ReflectionString& operator+=(ReflectionString& Item)
	{
		UCodeLangAssert(GetElementType() == Item.GetElementType());

		switch (Item.GetElementType()._Type)
		{
		case ReflectionTypes::Char:
			this->operator+=(Item.AsCharView());
			break;
		case ReflectionTypes::Uft8:
			this->operator+=(Item.AsUft8View());
			break;
		case ReflectionTypes::Uft16:
			this->operator+=(Item.AsUft16View());
			break;
		case ReflectionTypes::Uft32:
			this->operator+=(Item.AsUft32View());
			break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}


	bool operator==(String_view Item)
	{
		return AsCharView() == Item;
	}
	bool operator==(String_view8 Item)
	{
		return AsUft8View() == Item;
	}
	bool operator==(String_view16 Item)
	{
		return AsUft16View() == Item;
	}
	bool operator==(String_view32 Item)
	{
		return AsUft32View() == Item;
	}


	String_view AsCharView()
	{
		return { (char*)data(),size() };
	}
	String_view8 AsUft8View()
	{
		return { (Utf8*)data(),size() };
	}
	String_view16 AsUft16View()
	{
		return { (Utf16*)data(),size() };
	}
	String_view32 AsUft32View()
	{
		return { (Utf32*)data(),size() };
	}

	
	class iterator {
	public:
		iterator(ReflectionString* This) : This(This) {}
		iterator(ReflectionString* This, size_t Index) : This(This), Index(Index) {}
		inline iterator operator++() { Index++; return *this; }
		inline bool operator!=(const iterator& other) const { return Index != other.Index; }
		inline void* operator*() { return  This->at(Index); }
	private:
		ReflectionString* This = nullptr;
		size_t Index = 0;
	};

	iterator begin() { return iterator(this); }
	iterator end() { return iterator(this, size()); }

	void Set(void* UObject, const Info_t* Info,
		AnyInterpreterPtr ptr,
		const ClassAssembly& assembly,
		bool is32bit)
	{
		SetUObject(UObject);
		SetInfo(Info);
		SetInterpreter(ptr);
	}
	inline void SetUObject(void* UObject) { _UObject = UObject; }
	inline void SetInfo(const Info_t* Info) { _Info = Info; }
	inline void SetInterpreter(AnyInterpreterPtr ptr) { _ptr = ptr; }
	inline void SetAssembly(const ClassAssembly& assembly, bool is32bit)
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
	size_t elemsize = 0;
	const ClassAssembly* assembly = nullptr;
	bool is32bit = false;
};

//follows std::unordered_map class kind of
//a wrapper to make use UCodeMaps easier.
struct ReflectionMap
{
public:
	using Info_t = ClassAssembly::InfoMap_t;
	ReflectionMap() {}
	~ReflectionMap() {}

	bool empty()
	{
		return size() != 0;
	}
	size_t size()
	{
		return 0;
	}
	inline size_t max_size()
	{
		return SIZE_MAX;
	}

	void clear()
	{

	}
	void insert(void* key, void* maptype)
	{

	}
	void erase(void* key)
	{

	}
	bool haskey(void* key)
	{
		return false;
	}
	void* at(void* key)
	{
		return nullptr;
	}

	void Set(void* UObject, const Info_t* Info,
		AnyInterpreterPtr ptr,
		const ClassAssembly& assembly,
		bool is32bit)
	{
		SetUObject(UObject);
		SetInfo(Info);
		SetInterpreter(ptr);
	}
	inline void SetUObject(void* UObject) { _UObject = UObject; }
	inline void SetInfo(const Info_t* Info) { _Info = Info; }
	inline void SetInterpreter(AnyInterpreterPtr ptr) { _ptr = ptr; }
	inline void SetAssembly(const ClassAssembly& assembly, bool is32bit)
	{
		this->assembly = &assembly;
		this->is32bit = is32bit;
	}
private:
	void* _UObject = nullptr;
	AnyInterpreterPtr _ptr;
	const Info_t* _Info = nullptr;
	const ClassAssembly* assembly = nullptr;
	bool is32bit = false;
};

//follows std::array class kind of
//a wrapper to make use UCodeArray easier.
struct ReflectionArray
{
public:
	using Info_t = ClassAssembly::InfoArray_t;
	ReflectionArray() {}
	~ReflectionArray() {}

	inline void* data()
	{
		return _ptr.RCall<void*>(_Info->Data_Method, _UObject);
	}
	inline size_t size()
	{
		return _ptr.RCall<size_t>(_Info->Size_Method, _UObject);
	}
	inline void* at(size_t index)
	{
		#if UCodeLangDebug
		if (index >= size())
		{
			UCodeLangThrowException("out of bounds");
		}
		#endif // DEBUG

		return (void*)((uintptr_t)data() + (index * GetElemSize()));
	}

	inline void* operator[](size_t index)
	{
		return  at(index);
	}

	class iterator {
	public:
		iterator(ReflectionArray* This) : This(This) {}
		iterator(ReflectionArray* This, size_t Index) : This(This), Index(Index) {}
		inline iterator operator++() { Index++; return *this; }
		inline bool operator!=(const iterator& other) const { return Index != other.Index; }
		inline void* operator*() { return  This->at(Index); }
	private:
		ReflectionArray* This = nullptr;
		size_t Index = 0;
	};

	iterator begin() { return iterator(this); }
	iterator end() { return iterator(this, size()); }

	void Set(void* UObject, const Info_t* Info,
		AnyInterpreterPtr ptr,
		const ClassAssembly& assembly,
		bool is32bit)
	{
		SetUObject(UObject);
		SetInfo(Info);
		SetInterpreter(ptr);
	}
	inline void SetUObject(void* UObject) { _UObject = UObject; }
	inline void SetInfo(const Info_t* Info) { _Info = Info; }
	inline void SetInterpreter(AnyInterpreterPtr ptr) { _ptr = ptr; }
	inline void SetAssembly(const ClassAssembly& assembly, bool is32bit)
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
	const ClassAssembly* assembly = nullptr;
	bool is32bit = false;
	size_t elemsize = 0;
};

UCodeLangEnd