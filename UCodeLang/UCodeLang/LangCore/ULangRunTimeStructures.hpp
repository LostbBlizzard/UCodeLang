#pragma once
#include "LangInfo.hpp"
UCodeLangStart

//Use use these types to pass to UCode runtime
//Note UCode May reallocate/free to the types and may cause problems with your custom allocater
struct String_structure
{
	UAddress CharPointer = NullAddress;
	UAddress Count = NullAddress;
	UAddress Capacity= NullAddress;

	inline void As(String& Value)
	{
		CharPointer = (UAddress)Value.c_str();
		Count = Value.size();
		Capacity = Value.capacity();
	}

	template<typename T>
	UCodeLangForceinline static String_structure Make(String& Value)
	{
		String_structure r;
		r.As(Value);
		return r;
	}
};


//Lets hope you pass this the right function.
struct Vector_structure
{
	UAddress Pointer = NullAddress;
	UAddress Count = NullAddress;
	UAddress Capacity = NullAddress;

	template<typename T>
	inline void As(Vector<T>& Value)
	{
		Pointer = Value.capacity() ? (PtrType)Value[0] : NullAddress;
		Count = Value.size();
		Capacity = Value.capacity();
	}

	template<typename T>
	UCodeLangForceinline static Vector_structure Make(Vector<T>& Value)
	{
		Vector_structure r;
		r.As(Value);
		return r;
	}
};


UCodeLangEnd