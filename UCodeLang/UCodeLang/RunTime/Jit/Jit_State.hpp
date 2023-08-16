#pragma once
#include "Jit.hpp"
#include "JitCompilers.h"
#include "UCodeLang/RunTime/UPackagedTask.hpp"
#include <atomic>
UCodeLangStart


class Jit_State
{
public:

	struct APIKey
	{
	public:
		friend Jit_State;
		

		APIKey(const APIKey& other) = delete;
		APIKey& operator=(const APIKey& other) = delete;


		APIKey(APIKey&& other)
			:This(other.This)
			, ConstThis(other.ConstThis)
		{


			other.This = nullptr;
		}
		APIKey& operator=(APIKey&& other) 
		{
			This = other.This;
			ConstThis = other.ConstThis;

			other.This = nullptr;

			return *this;
		}

		APIKey(const APIKey&& other) : ConstThis(other.ConstThis)
		{

		}
		APIKey& operator=(const APIKey&& other)
		{
			ConstThis = other.ConstThis;

			return *this;
		}

	
		~APIKey() 
		{
			bool Isconst = !This;
			if (!Isconst)
			{
				This->RemoveOwner();
			}
		}
		//
		void AddToDoList(UPackagedTask_t&& Item)
		{		
			This->ToDoList.push_back(Item);
		}
	private:
		APIKey(Jit_State* Value):This(Value){}
		APIKey(const Jit_State* Value) :ConstThis(Value) {}
		
		Jit_State* This = nullptr;
		const Jit_State* ConstThis=nullptr;
		
		const Jit_State* GetState() const
		{
			if (This) 
			{
				return This;
			}
			else
			{
				return ConstThis;
			}
		}
		
		Jit_State* GetMutState() const
		{
			if (This)
			{
				return This;
			}
		}
	};

	enum class WrittenToState
	{
		None,
		API,
		AsmBuffer,
		AddingToDoList,
	};
	friend APIKey;


	Optional<APIKey> GetMutKey_Mut(void* This)
	{
		if (IsBeingWrittenTo == WrittenToState::None)
		{
			IsBeingWrittenTo = WrittenToState::API;
			WrittenToInfo = This;
			return { APIKey(this) };
		}
		else
		{
			return {};
		}
	}

	const APIKey GetConstKey(void* This) const
	{
		return APIKey(this);
	}
private:
	std::atomic<WrittenToState> IsBeingWrittenTo = WrittenToState::None;
	void* WrittenToInfo = nullptr;
	void* OwnerOfKey = nullptr;
	void RemoveOwner()
	{
		IsBeingWrittenTo = WrittenToState::None;
		OwnerOfKey = nullptr;
		WrittenToInfo = nullptr;
	}
	Vector<UPackagedTask_t> ToDoList;
};

UCodeLangEnd