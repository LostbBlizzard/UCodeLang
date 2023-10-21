#pragma once
#include "../LangCore/UCodeLangNameSpace.hpp"
#include "RunTimeLib.hpp"
#include <functional>
UCodeLangStart
class TestRuner
{
public:
	struct TestInfo
	{
		String TestName;
		bool Passed = false;
	};
	struct TestsResult
	{
		size_t TestPassedCount = 0;
		size_t TestCount = 0;

		Vector<TestInfo> Tests;
	};
	enum class InterpreterType
	{
		Interpreter,
		JitInterpreter,
		NativeInterpreter,
	};
	using OnTestDone = std::function<void(TestInfo& test)>;
	TestsResult RunTests(UClib& lib, InterpreterType Type, Optional<OnTestDone> OnDone = {});

	static Vector<const ClassMethod*> GetTests(const ClassAssembly& Assembly);
private:
};
UCodeLangEnd