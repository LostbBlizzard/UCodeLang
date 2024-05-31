#pragma once

#include "AnyInterpreter.hpp"
#include "UCodeLang/LangCore/DataType/UnorderedMap.hpp"
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
	UnorderedMap<UAddress, ProfilerFuncStats> _Funcs;
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


	void* GetObjectPtr()
	{
		return Object;
	}

	void* Object = nullptr;
};
struct DebugStackFrame
{
	UAddress _Function =NullAddress;

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
	struct Cach
	{
		Optional<ULangDebugInfo::Cach> cach;
		void Clear()
		{
			cach = {};
		}

	};

	using OnBreakPointCallBack = int;
	ProfilerDebuger(){}
	~ProfilerDebuger(){}

	UCodeLangAPIExport void Attach(RunTimeLangState* state);

	const ProfilerStats& Get_Stats()
	{
		return _Stats;
	}
	UCodeLangAPIExport void Clear_Stats()
	{

	}
	UCodeLangAPIExport void ClearBreakPoints()
	{

	}

	UCodeLangAPIExport void AddRunTimeBreakPoint(UAddress Item, OnBreakPointCallBack OnHit);
	UCodeLangAPIExport void RemoveRunTimeBreakPoint(UAddress Item);
	UCodeLangAPIExport void UpdateDebugData(DebugData& Out,Cach& cach);
	DebugData GetDebugData(Cach& cach)
	{
		DebugData v;
		UpdateDebugData(v,cach);
		return v;
	}
	
	UCodeLangAPIExport void StepInto(Interpreter* Interpreter, UAddress Address);
	UCodeLangAPIExport void StepInto(Interpreter* Interpreter, const String& FunctionName)
	{
		return StepInto(Interpreter, _state->FindAddress(FunctionName).value());
	}
	UCodeLangAPIExport void StepInto(Interpreter* Interpreter, const ClassMethod* Function)
	{
		return StepInto(Interpreter, Function->DecorationName);
	}
	UCodeLangAPIExport Interpreter::Register& GetReg(RegisterID Reg)
	{
		return StepedInterpreter->Get_Register(Reg);
	}


	UCodeLangAPIExport bool HasExitedFunc();
	UCodeLangAPIExport bool IsinFunc() {return IsInFunc;}
	UCodeLangAPIExport void StepOutof();
	UCodeLangAPIExport UAddress GetCurrentInstruction()
	{
		UCodeLangAssert(IsinFunc());
		return StepedInterpreter->_CPU.ProgramCounter;
	}
	UCodeLangAPIExport UAddress GetStackOffset()
	{
		UCodeLangAssert(IsinFunc());
		return StepedInterpreter->_CPU.Stack.StackOffSet;
	}

	UCodeLangAPIExport UAddress GetStartofFunc(UAddress Ins);
	
	UCodeLangAPIExport void StepIn();
	UCodeLangAPIExport void StepOver();
	UCodeLangAPIExport void StepOut();
	UCodeLangAPIExport void Continue();

	UCodeLangAPIExport void VM_StepIn();
	UCodeLangAPIExport void VM_StepOver();
	UCodeLangAPIExport void VM_StepOut();
	
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

	//StepInto
	Interpreter* StepedInterpreter = nullptr;
	bool IsInFunc = false;
	UAddress _OldStackPrePars = NullAddress;
	UAddress _OldStackoffset = NullAddress;

	void OnFuncStart(AnyInterpreterPtr Ptr);
	void OnFuncEnd(AnyInterpreterPtr Ptr);
	void OnFuncOnline(AnyInterpreterPtr Ptr);

};
UCodeLangEnd

