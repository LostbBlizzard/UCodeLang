#pragma once

#include "AnyInterpreter.hpp"
#include "UCodeLang/LangCore/DataType/BinaryVectorMap.hpp"
#include <chrono>
UCodeLangStart


using ProfilerClock = std::chrono::high_resolution_clock;
using ProfilerTime_point = ProfilerClock::time_point;
using ProfilerDuration_point = ProfilerClock::duration;
struct ProfilerFuncStats
{
	ProfilerDuration_point AverageTime = {};
	size_t AverageMemoryAlloc = 0;
	size_t AverageMemoryFree = 0;

	Vector<ProfilerDuration_point> _LastTime;
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

struct DebugVarable
{
	enum class VarType
	{
		Stack,
		Parameter,
		Static,
		Thread,
	};

	String VarableName;
	ReflectionTypeInfo Type;
	VarType VarableType = VarType::Stack;


	void* GetObject()
	{
		return Object;
	}
	AnyInt64* GetObjectAsAnyInt()
	{
		return (AnyInt64*)GetObject();
	}

private:
	void* Object = nullptr;
};
struct DebugStackFrame
{
	UAddress _Funcion =NullAddress;
	const ClassMethod* _Method=nullptr;

	Optional<ProfilerTime_point> FuncStart;

	Vector<DebugVarable> _Varables;
};
struct DebugData
{
	Vector<DebugStackFrame> _StackFrames;
};

class ProfilerDebuger
{
public:
	using OnBreakPointCallBack = int;
	ProfilerDebuger(){}
	~ProfilerDebuger(){}

	UCodeLangAPIExport void Attach(RunTimeLangState* state);

	const ProfilerStats& Get_Stats()
	{
		return _Stats;
	}
	void Clear_Stats()
	{

	}

	UCodeLangAPIExport void AddRunTimeBreakPoint(UAddress Item, OnBreakPointCallBack OnHit);
	UCodeLangAPIExport void RemoveRunTimeBreakPoint(UAddress Item);
	UCodeLangAPIExport void UpdateDebugData(DebugData& Out);
private:
	RunTimeLangState* _state = nullptr;
	AnyInterpreterPtr _Interpreter;
	ProfilerStats _Stats;
	size_t AverageSize = 50;
	size_t HotPathsToCount = 3;
	ProfilerTime_point FuncStartTime;
	ProfilerTime_point LastOnLineTime;
	Vector<ProfilerDuration_point> InFunc;
	//Debug
	DebugData _Debug;

	void OnFuncStart(AnyInterpreterPtr Ptr);
	void OnFuncEnd(AnyInterpreterPtr Ptr);
	void OnFuncOnline(AnyInterpreterPtr Ptr);

};
UCodeLangEnd

