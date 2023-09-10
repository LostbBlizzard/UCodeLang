#include "TestRuner.hpp"
#include "AnyInterpreter.hpp"
UCodeLangStart

TestRuner::TestsResult TestRuner::RunTests(UClib& lib, InterpreterType Type, Optional<OnTestDone> OnDone)
{
	RunTimeLangState state;
	AnyInterpreter Interpreter;


	TestsResult r;
	switch (Type)
	{
	case InterpreterType::Interpreter:
		Interpreter.SetAsInterpreter();
		break;
	case InterpreterType::JitInterpreter:
		Interpreter.SetAsJitInterpreter();
		break;
	case InterpreterType::NativeInterpreter:
		Interpreter.SetAsNativeInterpreter();
		break;
	default:
		UCodeLangUnreachable();
		break;
	}

	RunTimeLib rlib;
	rlib.Init(&lib);
	state.AddLib(&rlib);
	state.LinkLibs();

	Interpreter.Init(&state);

	auto& Assembly = state.Get_Assembly();
	Vector<const ClassMethod*> tests;
	for (auto& Item : Assembly.Classes)
	{
		if (Item->Get_Type() == ClassType::Class) 
		{
			auto& classInfo = Item->Get_ClassData();
			
			for (auto& Func : classInfo.Methods)
			{
				if (Func.ParsType.size()) { continue; }

				if (!Assembly.IsJust(Func.RetType)) { continue; }

				if (Func.RetType._Type == ReflectionTypes::Bool || Func.RetType._Type == ReflectionTypes::Void)
				{

					bool hastesttag = false;
					for (auto& tag : Func.Attributes.Attributes)
					{
						if (auto val = Assembly.Find_Node(tag.TypeID))
						{
							if (StringHelper::Contains(val->FullName, "test") || StringHelper::Contains(val->FullName, "Test"))
							{
								hastesttag = true;
								break;
							}
						}
					}

					if (hastesttag)
					{
						tests.push_back(&Func);
					}
				}
			}
		}
	}
	r.Tests.resize(tests.size());

	for (size_t i = 0; i < r.Tests.size(); i++)
	{
		auto& TestOut = r.Tests[i];
		auto& TestFunc = tests[i];

		bool passed = true;
		{
			Interpreter.Call(StaticVariablesInitializeFunc);

			Interpreter.Call(ThreadVariablesInitializeFunc);

			Interpreter.Call(TestFunc);

			if (TestFunc->RetType._Type == ReflectionTypes::Bool)
			{
				passed = Interpreter.Get_Return<bool>();
			}

			Interpreter.Call(ThreadVariablesUnLoadFunc);

			Interpreter.Call(StaticVariablesUnLoadFunc);
		}

		TestOut.Passed = passed;
		TestOut.TestName = TestFunc->DecorationName;

		if (passed)
		{
			r.TestPassedCount++;
		}
		r.TestCount++;

		if (OnDone.has_value())
		{
			OnDone.value()(TestOut);
		}

	}

	return r;
}
UCodeLangEnd


