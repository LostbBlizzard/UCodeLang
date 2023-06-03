#pragma once

#include "Interpreters/Interpreter.hpp"
#include "UCodeLang/LangCore/DataType/BinaryVectorMap.hpp"
#include <chrono>
UCodeLangStart


using ProfilerTimePoint = std::chrono::milliseconds;
struct ProfilerFuncStats
{
	ProfilerTimePoint AverageTime = ProfilerTimePoint();
	size_t AverageMemoryAlloc = 0;
	size_t AverageMemoryFree = 0;

	Vector<ProfilerTimePoint> _LastTime;
	Vector<size_t> _LastMemoryAlloc;
	Vector<size_t> _LastMemoryFree;

	size_t _LastMemoryAllocIndex = 0;
	size_t _LastMemoryFreeIndex = 0;
};

struct HotPathStats
{

};

struct ProfilerStats
{
	BinaryVectorMap<UAddress, ProfilerFuncStats> _Funcs;
	Vector<HotPathStats> HotPaths;
};

class Profiler
{
public:
	Profiler(){}
	~Profiler(){}

	void Attach(Interpreter* interpreter)
	{
		_Interpreter = interpreter;
	}

	const ProfilerStats& Get_Stats()
	{
		return _Stats;
	}
	void Clear_Stats()
	{

	}
private:
	Interpreter* _Interpreter = nullptr;
	ProfilerStats _Stats;
	size_t AverageSize = 50;
	size_t HotPathsToCount = 3;
};
UCodeLangEnd

