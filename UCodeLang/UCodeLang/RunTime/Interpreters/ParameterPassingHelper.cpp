#include "ParameterPassingHelper.hpp"

UCodeLangStart
void ParameterPassingHelper::Push(const void* Value, size_t ValueSize)
{
	for (size_t i = 0; i < sizeof(size_t); i++)//Push The Size
	{
		Bytes.push_back(((const UInt8*)&ValueSize)[i]);
	}
	for (size_t i = 0; i < ValueSize; i++)//PushTheValue
	{
		Bytes.push_back(((const UInt8*)Value)[i]);
	}
}
size_t ParameterPassingHelper::GetParameterCount() const
{
	size_t count = 0;
	LoopState State = StartLoop();

	while (Next(State))
	{
		count++;
	}
	return count;
}
constexpr size_t NanLoopState = -1;
ParameterPassingHelper::LoopState ParameterPassingHelper::StartLoop()const
{
	LoopState V;
	V.Value = NanLoopState;
	return V;
}

bool ParameterPassingHelper::Next(LoopState& State)const
{
	if (State.Value == NanLoopState) 
	{
		if (Bytes.size() == 0)
		{
			return false;
		}
	}
	else if (State.Value + GetSize(State) + sizeof(size_t) >= Bytes.size())
	{
		return false;
	}

	if (State.Value == NanLoopState)
	{
		State.Value = 0;
	}
	else
	{
		auto Size = GetSize(State);
		State.Value += sizeof(size_t) + Size;
	}

	return true;
}

size_t ParameterPassingHelper::GetSize(LoopState State)const
{
	const size_t* ByteCountPointer = (const size_t*)&Bytes[State.Value];
	return  *ByteCountPointer;
}

const void* ParameterPassingHelper::GetPointer(LoopState State)const
{
	const size_t* ByteCountPointer = (const size_t*)&Bytes[State.Value];
	ByteCountPointer++;
	return (const void*)ByteCountPointer;
}

UCodeLangEnd
