#pragma once

#include "UCodeLang/LangCore/LangTypes.hpp"
UCodeLangStart
class ParameterPassingHelper
{
public:
	struct LoopState
	{
		friend ParameterPassingHelper;
	private:
		LoopState():Value(0){}

		size_t Value;
	};
	struct LoopData//TO DO add range-for-loop
	{
		size_t DataSize;
		const void* Pointer;
	};

	void Push(const void* Value, size_t ValueSize);
	void Clear() { Bytes.clear(); }
	size_t GetParameterCount() const;
	//How to Loop
	
	//auto State = _Parameters.StartLoop();
	//while (_Parameters.Next(State))
	//{
	// size_t Size =_Parameters.GetSize(State);
	// void* Pointer =_Parameters.GetPointer(State);//or 
	// LoopData V =_Parameters.GetLoopData(State);
	//}
	LoopState StartLoop()const;
	bool Next(LoopState& State)const;

	size_t GetSize(LoopState State)const;
	const void* GetPointer(LoopState State)const;
	LoopData GetLoopData(LoopState State)const
	{
		return { GetSize(State),GetPointer(State) };
	}
private:
	Vector<UInt8> Bytes;
}; 
UCodeLangEnd