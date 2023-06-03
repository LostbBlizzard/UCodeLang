#pragma once
#include "Interpreters/Interpreter.hpp"
#include "UCodeLang/LangCore/DataType/BinaryVectorMap.hpp"
UCodeLangStart

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
	UAddress _Funcion;
	Vector<DebugVarable> _Varables;
};
struct DebugData
{
	Optional<UAddress> _BreakPoint;

	Vector<DebugStackFrame> _StackFrames;
};

class Debuger
{
public:
	using OnBreakPointCallBack = int;
	Debuger(){}
	~Debuger(){}

	void Attach(Interpreter* interpreter);
	void AddRunTimeBreakPoint(UAddress Item, OnBreakPointCallBack OnHit);
	void RemoveRunTimeBreakPoint(UAddress Item);
	void UpdateDebugData(DebugData& Out);

private:
	Interpreter* _Interpreter = nullptr;
	UAddress _CurrentBreakPoint = 0;
	struct BreakPointData
	{
		UAddress _BreakPoint;
		OnBreakPointCallBack _CallBack;
		Instruction _OldIns;
	};
	BinaryVectorMap<UAddress,BreakPointData> _CodeBreakPoints;
	static void Debuger::DebugerOnHit(void* inter, void* Object);
};

UCodeLangEnd