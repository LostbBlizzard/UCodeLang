#pragma once
#include "UCodeLang/UCodeLang.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
#include <fstream>
#include <iostream>

#include <future>
#include <optional>
#include "UCodeAnalyzer/Formater.hpp"
#include <sstream>
#include "UCodeLang/Compliation/Back/C89/C89Backend.hpp"
#include "UCodeLang/Compliation/Back/x86/X86BackEnd.hpp"
#include "UCodeLang/Compliation/ModuleFile.hpp"
#include "UCodeLang/RunTime/ProfilerDebuger.hpp"

#include "../tests/TestGenerator.hpp"
#include "../tests/Test.hpp"
#include <chrono>
class JitPerformance
{
public:
	template<typename... Pars>
	struct PerformanceInfo
	{
		using RetType = int;
		using NativeMain = RetType(*)(Pars... pars);
		using Func = void(*)();

		UCodeLang::String _UCode;
		
		NativeMain _Native = nullptr;
		Func _Init = nullptr;
		Func _DeInit = nullptr;
	};
	enum class Task
	{
		Main,
	};

	static void main(Task Test);

	template<typename... Pars>
	static void Run(size_t Count,const PerformanceInfo<Pars...>& Value,Pars&&... Args)
	{
		using Pinfo = PerformanceInfo<Pars...>;
		using namespace UCodeLang;

		Jit_Interpreter RunTime;
		Jit_Interpreter AllwaysJit;
		Interpreter OtherInterpreter;

		UCodeLang::RunTimeLangState State;
		UCodeLang::Compiler V;
		auto Lib = V.CompileText(Value._UCode);

		UCodeLang::RunTimeLib CodeLib;
		CodeLib.Init(Lib.OutPut);

		State.AddLib(&CodeLib);
		State.LinkLibs();


		RunTime.Init(&State);
		OtherInterpreter.Init(&State);
		AllwaysJit.Init(&State);

		auto FuncMain = State.Get_Assembly().Get_GlobalObject_Class()->Get_ClassMethod("main");

		const size_t MaxTimes = Count;
		using Clock = std::chrono::steady_clock;
		{

			auto OldTime = Clock::now();

			RunTime.Call(StaticVariablesInitializeFunc);
			RunTime.Call(ThreadVariablesInitializeFunc);
			for (size_t i = 0; i < MaxTimes; i++)
			{
				auto AutoPtr = RunTime.RCall<typename Pinfo::RetType>(FuncMain,Args...);
			}
			RunTime.Call(StaticVariablesUnLoadFunc);
			RunTime.Call(ThreadVariablesUnLoadFunc);

			auto newTime = Clock::now();

			auto MsCount = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - OldTime).count();
			std::cout << "Time:" <<
				MsCount
				<< " ms" << " for Jit Interpreter" << std::endl;
		}

		{

			auto OldTime = Clock::now();

			RunTime.Call(StaticVariablesInitializeFunc);
			RunTime.Call(ThreadVariablesInitializeFunc);
			for (size_t i = 0; i < MaxTimes; i++)
			{
				auto AutoPtr = AllwaysJit.RCall<typename Pinfo::RetType>(FuncMain, Args...);
			}
			RunTime.Call(StaticVariablesUnLoadFunc);
			RunTime.Call(ThreadVariablesUnLoadFunc);

			auto newTime = Clock::now();

			auto MsCount = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - OldTime).count();
			std::cout << "Time:" <<
				MsCount
				<< " ms" << " for allways Jit-Interpreter" << std::endl;
		}

		{
			auto OldTime = Clock::now();

			RunTime.Call(StaticVariablesInitializeFunc);
			RunTime.Call(ThreadVariablesInitializeFunc);
			for (size_t i = 0; i < MaxTimes; i++)
			{
				auto AutoPtr = OtherInterpreter.RCall<typename Pinfo::RetType>(FuncMain, Args...);
			}
			RunTime.Call(StaticVariablesUnLoadFunc);
			RunTime.Call(ThreadVariablesUnLoadFunc);

			auto newTime = Clock::now();
			auto MsCount = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - OldTime).count();
			std::cout << "Time:" <<
				MsCount
				<< " ms" << " for Base Interpreter" << std::endl;
		}

		{
		
			auto OldTime = Clock::now();
			
			if (Value._Init) { Value._Init(); }
			for (size_t i = 0; i < MaxTimes; i++)
			{
				typename Pinfo::RetType AutoPtr = Value._Native(Args...);
			}
			if (Value._DeInit) { Value._DeInit(); }


			auto newTime = Clock::now();
			auto MsCount = std::chrono::duration_cast<std::chrono::milliseconds>(newTime - OldTime).count();
			std::cout << "Time:" <<
				MsCount
				<< " ms" << " for Native C-Code" << std::endl;
		}

		RunTime.UnLoad();
		OtherInterpreter.UnLoad();
		AllwaysJit.UnLoad();
	}
};

