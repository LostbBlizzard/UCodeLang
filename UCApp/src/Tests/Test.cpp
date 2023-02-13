#include "Test.hpp"
#include <future>
#include <memory>
#include <sstream>
#include <fstream>

namespace ULangTest
{
	using namespace UCodeLang;
	String ModeType(OptimizationFlags Flags)
	{
		if (Flags == OptimizationFlags::O_None)
		{
			return "flagsnone";
		}
		String r;
		auto flags = (OptimizationFlags_t)Flags;
		if (flags & (OptimizationFlags_t)OptimizationFlags::ForSize)
		{
			r += "Size";
		}
		if (flags & (OptimizationFlags_t)OptimizationFlags::ForSpeed)
		{
			r += "Speed";
		}
		if (flags & (OptimizationFlags_t)OptimizationFlags::Debug)
		{
			r += "Debug";
		}
		return r;
	}

	template< typename T >
	String int_to_hex(T i)
	{
		std::stringstream stream;
		stream << "0x"
			<< std::setfill('0') << std::setw(sizeof(T) * 2)
			<< std::hex << i;
		return stream.str();
	}

	template< typename T >
	String int_to_hexV(T i)
	{
		std::stringstream stream;
		stream
			<< std::setfill('0') << std::setw(sizeof(T) * 2)
			<< std::hex << i;
		return stream.str();
	}


	String OutputBytesToString(Byte* Bytes, size_t Size)
	{
		std::stringstream stream;
		for (size_t i = 0; i < Size; i++)
		{
			if (i == 0)
			{
				//stream << "0x";
			}
			stream << std::to_string(Bytes[i]);
			if (i+1 != Size)
			{
				stream << ",";
			}
		}
		stream << '\0';
		return stream.str();
	}


	const UCodeLang::Array<OptimizationFlags, 3> OptimizationFlagsToCheck
	{
		OptimizationFlags::ForDebuging,
		OptimizationFlags::O_2,
		OptimizationFlags::O_3,
	};
	bool RunTimeOutput(
		std::ostream& LogStream,
		std::ostream& ErrStream,
		const TestInfo& Test,
		OptimizationFlags flag,
		std::unique_ptr<Byte[]>& RetState, std::string Type = "jit")
	{
		bool IsSame = true;

		for (size_t i = 0; i < Test.RunTimeSuccessSize; i++)
		{
			if (RetState[i] != Test.RunTimeSuccess[i])
			{
				IsSame = false;
				break;
			}
		}

		if (IsSame)
		{
			LogStream << "Success from test '" << Test.TestName << "'" << std::endl;
		}
		else
		{
			ErrStream << "fail from got value '";
			ErrStream << OutputBytesToString(RetState.get(), Test.RunTimeSuccessSize);

			ErrStream << "' but expecting '";
			ErrStream << OutputBytesToString(Test.RunTimeSuccess.get(), Test.RunTimeSuccessSize);
			ErrStream << ": '" << Type << "," << ModeType(flag) << "'" << std::endl;
			return false;
		}
		return true;
	}

	bool RunTestForFlag(const TestInfo& Test, OptimizationFlags flag, std::ostream& LogStream, std::ostream& ErrStream)
	{
		Compiler::CompilerPathData paths;
		Compiler Com;
		Com.Get_Settings()._Flags = flag;
		Com.Get_Settings().PtrSize = IntSizes::Native;
		
		Compiler::CompilerRet Com_r;
		std::string InputFilesPath = Test_UCodeFiles + Test.InputFilesOrDir;
		std::string OutFileDir = Test_OutputFiles + Test.TestName;
		std::filesystem::path p = OutFileDir;
		OutFileDir = p.parent_path().generic_string() + "/" + +Test.TestName + "/";

		std::filesystem::create_directories(OutFileDir);
		std::string OutFilePath = OutFileDir + Test.TestName + ModeType(flag) + ".ulibtest";




		paths.FileDir = InputFilesPath;
		paths.OutFile = OutFilePath;




		if (std::filesystem::is_directory(paths.FileDir))
		{
			Com_r = Com.CompileFiles(paths);
		}
		else
		{
			Com_r = Com.CompilePathToObj(paths.FileDir, paths.OutFile);
		}

		if (Test.Condition == SuccessCondition::Compilation
			|| Test.Condition == SuccessCondition::CompilationFail)
		{
			if (
				(Com_r._State == Compiler::CompilerState::Success && Test.Condition == SuccessCondition::Compilation)
				||
				(Com_r._State == Compiler::CompilerState::Fail && Test.Condition == SuccessCondition::CompilationFail)
				)
			{
				LogStream << "Success from test '" << Test.TestName << "'" << std::endl;
				return true;
			}
			else
			{
				ErrStream << "fail from test '" << Test.TestName << "'" << std::endl;
				
				LogErrors(ErrStream, Com);
				return false;
			}

		}

		if (Com_r._State != Compiler::CompilerState::Success)
		{
			ErrStream << "fail from test [Cant Compile File/Files] '" << Test.TestName << "'" << std::endl;


			LogErrors(ErrStream, Com);
			return false;
		}


		RunTimeLangState state;
		UClib lib;
		if (!UClib::FromFile(&lib, OutFilePath))
		{


			ErrStream << "fail from test [Cant Open ULib File] '" << Test.TestName << "'" << std::endl;
			return false;
		}

		{
			auto Text = UAssembly::UAssembly::ToString(&lib);
			String Path = OutFilePath + ".UA";
			std::ofstream out(Path);
			if (out.is_open()) {
				out << Text;
				out.close();
			}
		}
		RunTimeLib rLib;
		rLib.Init(&lib);
		state.AddLib(&rLib);
		state.LinkLibs();

		{
			Interpreter RunTime;
			RunTime.Init(&state);

			Interpreter::Return_t r;
			try
			{
				r = RunTime.Call(Test.FuncToCall);
			}
			catch (const std::exception& ex)
			{
				ErrStream << "fail from test [exception] " << ex.what() << ": " << "'" << Test.TestName << "'" << std::endl;
				return false;
			}

			if (Test.Condition == SuccessCondition::RunTimeValue)
			{
				std::unique_ptr<Byte[]> RetState = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
				RunTime.Get_Return(RetState.get(), Test.RunTimeSuccessSize);

				RunTimeOutput(LogStream, ErrStream, Test, flag, RetState, "Interpreter");
			}
			RunTime.UnLoad();
		}

		{
			Jit_Interpreter RunTime;
			RunTime.Init(&state);

			Interpreter::Return_t r;
			try
			{
				r = RunTime.Call(Test.FuncToCall);
			}
			catch (const std::exception& ex)
			{
				RunTime.UnLoad();
				ErrStream << "fail from jit test [exception] " << ex.what() << ": " << "'" << Test.TestName << ModeType(flag) << "'" << std::endl;
				return false;
			}
			RunTime.UnLoad();

			if (Test.Condition == SuccessCondition::RunTimeValue)
			{
				std::unique_ptr<Byte[]> RetState = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
				RunTime.Get_Return(RetState.get(), Test.RunTimeSuccessSize);

				RunTimeOutput(LogStream, ErrStream, Test, flag, RetState, "Jit_Interpreter");
			}
		}

		{
			UCodeRunTime RunTime;
			RunTime.Init(&state);

			Interpreter::Return_t r;
			try
			{
				r = RunTime.Call(Test.FuncToCall);
			}
			catch (const std::exception& ex)
			{
				RunTime.UnLoad();
				ErrStream << "fail from UCodeRunTime test [exception] " << ex.what() << ": " << "'" << Test.TestName << ModeType(flag) << "'" << std::endl;
				return false;
			}
			RunTime.UnLoad();





			if (Test.Condition == SuccessCondition::RunTimeValue)
			{
				std::unique_ptr<Byte[]> RetState = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
				RunTime.Get_Return(RetState.get(), Test.RunTimeSuccessSize);

				RunTimeOutput(LogStream, ErrStream, Test, flag, RetState, "UCodeRunTime");
			}
		}


		rLib.UnLoad();
		return true;
	}



	std::mutex Coutlock;

	bool RunTest(const TestInfo& Test)
	{
		bool V = true;

		std::stringstream Log;
		std::stringstream Err;
		Log << "Runing Test '" << Test.TestName << "'" << std::endl;

		if (Test.Condition == SuccessCondition::RunTimeValue) 
		{

			for (auto Flag : OptimizationFlagsToCheck)
			{
				if (!RunTestForFlag(Test, Flag, Log, Err))
				{
					V = false;
				}
			}
		}
		else
		{
			if (!RunTestForFlag(Test, OptimizationFlagsToCheck[0], Log, Err))
			{
				V = false;
			}
		}
		Coutlock.lock();

		std::cout << Log.str();
		std::cout << Err.str();

		Coutlock.unlock();

		return V;
	}


	void RunTests()
	{
		size_t TestPassed = 0;
		std::cout << "---runing Test" << std::endl;

		Vector<std::future<bool>> List;

		UCodeLang::UAssembly::Get_InsToInsMapValue();
		for (auto& Test : Tests)
		{
			//if (RunTest(Test)) { TestPassed++; }

			auto F = std::async(std::launch::async, [&]
				{
					return RunTest(Test);
				}
			);
			List.push_back(std::move(F));
		}

		for (auto& Item : List)
		{
			Item.wait();
			if (Item.get()) { TestPassed++; };
		}

		std::cout << "---Tests ended" << std::endl;
		std::cout << "passed " << TestPassed << "/" << Tests.size() << " Tests" << std::endl;
	}

	bool LogErrors(std::ostream& out, Compiler& _Compiler)
	{
		out << "[\n";
		auto& Errors = _Compiler.Get_Errors().Get_Errors();
		for (auto& Item : Errors)
		{
			out << Item.ToString() << std::endl;
		}
		out << "]\n";
		return _Compiler.Get_Errors().Has_Errors();
	}
}