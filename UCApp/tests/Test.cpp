#include "Test.hpp"
#include <future>
#include <memory>
#include <sstream>
#include <fstream>
#include <UCodeLang/Compliation/UAssembly/UAssembly.hpp>
#include <UCodeLang/Compliation/ModuleFile.hpp>
#include <UCodeLang/Compliation/Back/C89/C89Backend.hpp>

#if UCodeLang_Platform_Windows
#include <Windows.h>
#elif UCodeLang_Platform_Posix
#include <dlfcn.h>
#endif
UCodeTestStart

using namespace UCodeLang;

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


	const UCodeLang::Array<OptimizationFlags, 1> OptimizationFlagsToCheck
	{
		OptimizationFlags::NoOptimization,
		//OptimizationFlags::ForDebuging,
		//OptimizationFlags::ForSize,
		//OptimizationFlags::ForSpeed,
		//OptimizationFlags::ForMaxSpeed,
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
			LogStream << "Success from test '" << Test.TestName  << "'" << ModeType(flag) << " " << Type << std::endl;
		}
		else
		{
			ErrStream << "fail from got value '";
			ErrStream << OutputBytesToString(RetState.get(), Test.RunTimeSuccessSize);

			ErrStream << "' but expecting '";
			ErrStream << OutputBytesToString(Test.RunTimeSuccess.get(), Test.RunTimeSuccessSize);
			ErrStream << ": '" << Type << "," << ModeType(flag) << "'" << Type << std::endl;
			return false;
		}
		return true;
	}

	bool RunTestForFlag(const TestInfo& Test, OptimizationFlags flag, std::ostream& LogStream, std::ostream& ErrStream, TestMode mode)
	{
		return false;
		#if UCodeLangDebug
		Compiler::CompilerPathData paths;
		Compiler Com;
		Com.Get_Settings()._Flags = flag;
		Com.Get_Settings().PtrSize = IntSizes::Native;
		
		Compiler::CompilerRet Com_r;
		std::string InputFilesPath = UCodeLang_UCAppDir_Test_UCodeFiles + Test.InputFilesOrDir;
		std::string OutFileDir = UCodeLang_UCAppDir_Test_OutputFiles + Test.TestName;
		std::filesystem::path p = OutFileDir;
		OutFileDir = p.parent_path().generic_string() + "/" + +Test.TestName + "/";

		std::filesystem::create_directories(OutFileDir);	
		
		if (mode == TestMode::CLang89BackEnd)
		{
			Com.Set_BackEnd(C89Backend::MakeObject);
		}
		std::string OutFilePath = OutFileDir + Test.TestName + ModeType(flag) + ".ulibtest" + Com.GetOutputExtWithDot();




		paths.FileDir = InputFilesPath;
		paths.OutFile = OutFilePath;

	

		try
		{
			if (std::filesystem::is_directory(paths.FileDir))
			{
				Com_r = Com.CompileFiles(paths);
			}
			else
			{
				Com_r = Com.CompilePathToObj(paths.FileDir, paths.OutFile);
			}
		}
		catch (const std::exception& ex)
		{
			ErrStream << "fail from Compile [exception] '" << ex.what() << "' : " << "'" << Test.TestName << "'" << std::endl;
			return false;
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
				LogStream << "Success from test '" << Test.TestName << ModeType(flag) << "'" << std::endl;
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
			ErrStream << "fail from test [Cant Compile File/Files] '" << Test.TestName << ModeType(flag) << "'" << std::endl;


			LogErrors(ErrStream, Com);
			return false;
		}

		if (mode == TestMode::UCodeLangBackEnd) 
		{
			RunTimeLangState state;
			UClib lib;
			if (!UClib::FromFile(&lib, OutFilePath))
			{


				ErrStream << "fail from test [Cant Open ULib File] '" << Test.TestName << ModeType(flag) << "'" << std::endl;
				return false;
			}

			//{
			auto Text = UAssembly::UAssembly::ToString(&lib);
			String Path = OutFilePath + ".UA";
			std::ofstream out(Path);
			if (out.is_open()) {
				out << Text;
				out.close();
			}
			//}
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
					ErrStream << "fail from test [exception] '" << ex.what() << "' : " << "'" << Test.TestName << "'" << ModeType(flag) << std::endl;
					return false;
				}

				if (Test.Condition == SuccessCondition::RunTimeValue)
				{
					std::unique_ptr<Byte[]> RetState = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
					RunTime.Get_Return(RetState.get(), Test.RunTimeSuccessSize);

					if (!RunTimeOutput(LogStream, ErrStream, Test, flag, RetState, "Interpreter"))
					{
						return false;
					}
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
					ErrStream << "fail from jit test [exception] '" << ex.what() << "' : " << "'" << Test.TestName << ModeType(flag) << "'" << std::endl;
					return false;
				}
				RunTime.UnLoad();

				if (Test.Condition == SuccessCondition::RunTimeValue)
				{
					std::unique_ptr<Byte[]> RetState = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
					RunTime.Get_Return(RetState.get(), Test.RunTimeSuccessSize);

					if (!RunTimeOutput(LogStream, ErrStream, Test, flag, RetState, "Jit_Interpreter"))
					{
						return false;
					}
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
					ErrStream << "fail from UCodeRunTime test [exception] '" << ex.what() << "' : " << "'" << Test.TestName << ModeType(flag) << "'" << std::endl;
					return false;
				}
				RunTime.UnLoad();





				if (Test.Condition == SuccessCondition::RunTimeValue)
				{
					std::unique_ptr<Byte[]> RetState = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
					RunTime.Get_Return(RetState.get(), Test.RunTimeSuccessSize);

					if (!RunTimeOutput(LogStream, ErrStream, Test, flag, RetState, "UCodeRunTime"))
					{
						return false;
					}
				}
			}
			rLib.UnLoad();
			return true;
		}
		else if (mode == TestMode::CLang89BackEnd)
		{
			UClib& ulib = *Com_r.OutPut;


			auto ufunc = ulib.Get_Assembly().Find_Func(Test.FuncToCall);
			UCodeLangAssert(ufunc);
			{
				// String filetxt = Compiler::GetTextFromFile(OutFilePath);

				Path  dllfile = OutFilePath + ".lib";
				Path Testablefile = OutFilePath;
				String Cmd = "gcc " + Testablefile.generic_string();
				Cmd += " -shared -std=c89";
				Cmd += " -o " + dllfile.generic_string();
				system(Cmd.c_str());

				auto cfuncname = C89Backend::UpdateToCindentifier(ufunc->DecorationName);

				auto staticinitname = C89Backend::UpdateToCindentifier(StaticVariablesInitializeFunc);
                auto threadinitname = C89Backend::UpdateToCindentifier(ThreadVariablesInitializeFunc);

                auto staticdeinitname = C89Backend::UpdateToCindentifier(StaticVariablesUnLoadFunc);
                auto threaddeinitname = C89Backend::UpdateToCindentifier(ThreadVariablesUnLoadFunc);


				#if UCodeLang_Platform_Windows
				auto lib = LoadLibrary(dllfile.c_str());
				auto staticinittocall = GetProcAddress(lib,staticinitname.c_str());
                auto threadinittocall = GetProcAddress(lib,threadinitname.c_str());
                auto staticdeinittocall = GetProcAddress(lib,staticdeinitname.c_str());
                auto threaddeinittocall = GetProcAddress(lib,threaddeinitname.c_str());


				auto functocall = GetProcAddress(lib, cfuncname.c_str());
				#elif UCodeLang_Platform_Posix
				auto lib = dlopen(dllfile.c_str(), RTLD_NOW);
				auto staticinittocall = dlsym(lib,staticinitname.c_str());
                auto threadinittocall = dlsym(lib,threadinitname.c_str());
            	auto staticdeinittocall = dlsym(lib,staticdeinitname.c_str());
                auto threaddeinittocall = dlsym(lib,threaddeinitname.c_str());
                        

                auto functocall = dlsym(lib,cfuncname.c_str());
				#endif  
				
				UCodeLangAssert(functocall);

				bool hasautocall = cfuncname == "main";
                if (!hasautocall)
                {
                    using Func = void(*)();
                    ((Func)staticinittocall)();
                    ((Func)threadinittocall)();
                }

				auto RetValue = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
				{
					if (ufunc->RetType._Type == ReflectionTypes::Bool)
					{
						using GetValueFunc = bool(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else if (ufunc->RetType._Type == ReflectionTypes::sInt8
						|| ufunc->RetType._Type == ReflectionTypes::uInt8
						|| ufunc->RetType._Type == ReflectionTypes::Char)
					{
						using GetValueFunc = UInt8(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else  if (ufunc->RetType._Type == ReflectionTypes::uInt16
						|| ufunc->RetType._Type == ReflectionTypes::sInt16)
					{
						using GetValueFunc = Int16(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else  if (ufunc->RetType._Type == ReflectionTypes::uInt32
						|| ufunc->RetType._Type == ReflectionTypes::sInt32)
					{
						using GetValueFunc = Int32(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else  if (ufunc->RetType._Type == ReflectionTypes::float32)
					{
						using GetValueFunc = float32(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else  if (ufunc->RetType._Type == ReflectionTypes::float64)
					{
						using GetValueFunc = float64(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else  if (ufunc->RetType._Type == ReflectionTypes::uIntPtr
						|| ufunc->RetType._Type == ReflectionTypes::sIntPtr)
					{
						using GetValueFunc = uintptr_t(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else
					{
						UCodeLangUnreachable();
					}
				}

				if (!hasautocall)
                {
                    using Func = void(*)();
                    ((Func)staticdeinittocall)();
                    ((Func)threaddeinittocall)();
                }

				#if UCodeLang_Platform_Windows
				FreeLibrary(lib);
				#elif UCodeLang_Platform_Posix
                dlclose(lib);
				#endif      
			}
		}
		else
		{
			UCodeLangUnreachable();
		}


		
		return true;
		#endif
	}



	std::mutex Coutlock;

	bool RunTest(const TestInfo& Test, TestMode mode)
	{
		bool V = true;

		std::stringstream Log;
		std::stringstream Err;
		Log << "Runing Test '" << Test.TestName << "'" << std::endl;

		if (Test.Condition == SuccessCondition::RunTimeValue) 
		{

			for (auto Flag : OptimizationFlagsToCheck)
			{
				if (!RunTestForFlag(Test, Flag, Log, Err, mode))
				{
					V = false;
					break;
				}
			}
		}
		else
		{
			if (!RunTestForFlag(Test, OptimizationFlagsToCheck[0], Log, Err,mode))
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

	bool RunTest(const ModuleTest& Test, TestMode mode)
	{
		std::stringstream Log;
		std::stringstream Err;
		Log << "Runing ModuleTest '" << Test.TestName << "'" << std::endl;

		UCodeLang::ModuleFile file;

		Path pathdir = UCodeLang_UCAppDir_Test_UCodeFiles + Test.Modulefile;
		Path modulefilepath = pathdir / Path(UCodeLang::ModuleFile::FileNameWithExt);
		bool r = false;
		if (file.FromFile(&file, modulefilepath))
		{
			Compiler compiler;
			ModuleIndex LangIndex = UCodeLang::ModuleIndex::GetModuleIndex();



			auto ret = file.BuildModule(compiler, LangIndex);
			if (ret.CompilerRet._State == Compiler::CompilerState::Success)
			{
				r = true;
			}


		}


		Coutlock.lock();

		std::cout << Log.str();
		std::cout << Err.str();

		Coutlock.unlock();

		return r;
	}


	int RunTests(bool MultThread)
	{
		size_t TestPassed = 0;
		{

			std::cout << "---runing Test" << std::endl;

			Vector<std::future<bool>> List;

			UCodeLang::UAssembly::Get_InsToInsMapValue();
			for (auto& Test : Tests)
			{
				//if (RunTest(Test)) { TestPassed++; }

				if (MultThread == false)
				{
					auto TestR = RunTest(Test, TestMode::UCodeLangBackEnd);
				}
				else
				{
					auto F = std::async(std::launch::async, [&]
						{
							try
							{
								return RunTest(Test, TestMode::UCodeLangBackEnd);
							}
							catch (const std::exception& why)
							{
								std::cout << why.what();
								return false;
							}
						}
					);
					List.push_back(std::move(F));
				}
			}

			for (auto& Item : List)
			{
				try
				{
					Item.wait();
					if (Item.get()) { TestPassed++; };
				}
				catch (const std::exception& why)
				{
					std::cout << why.what();
				}

			}

			std::cout << "---Tests ended" << std::endl;
			std::cout << "passed " << TestPassed << "/" << Tests.size() << " Tests" << std::endl;
		}

		size_t TestModulePassed = 0;
		{

			std::cout << "---runing Module Tests" << std::endl;

			Vector<std::future<bool>> List;

			UCodeLang::UAssembly::Get_InsToInsMapValue();
			for (auto& Test : ModuleTests)
			{
				//if (RunTest(Test)) { TestPassed++; }

				if (MultThread == false)
				{

					auto TestR = RunTest(Test, TestMode::UCodeLangBackEnd);
				}
				else
				{
					auto F = std::async(std::launch::async, [&]
						{
							try
							{
								return RunTest(Test, TestMode::UCodeLangBackEnd);
							}
							catch (const std::exception& why)
							{
								std::cout << why.what();
								return false;
							}
						}
					);
					List.push_back(std::move(F));
				}
			}

			for (auto& Item : List)
			{
				try
				{
					Item.wait();
					if (Item.get()) { TestModulePassed++; };
				}
				catch (const std::exception& why)
				{
					std::cout << why.what();
				}

			}

			std::cout << "---Module ended" << std::endl;
			std::cout << "passed " << TestModulePassed << "/" << ModuleTests.size() << "Module Tests" << std::endl;
		}


		size_t CTestPassed = 0;
		{
			std::cout << "---runing Module Tests" << std::endl;

			Vector<std::future<bool>> List;

			UCodeLang::UAssembly::Get_InsToInsMapValue();
			for (auto& Test : ModuleTests)
			{
				//if (RunTest(Test)) { TestPassed++; }

				if (MultThread == false)
				{

					auto TestR = RunTest(Test, TestMode::CLang89BackEnd);
				}
				else
				{
					auto F = std::async(std::launch::async, [&]
						{
							try
							{
								return RunTest(Test, TestMode::CLang89BackEnd);
							}
							catch (const std::exception& why)
							{
								std::cout << why.what();
								return false;
							}
						}
					);
					List.push_back(std::move(F));
				}
			}

			for (auto& Item : List)
			{
				try
				{
					Item.wait();
					if (Item.get()) { CTestPassed++; };
				}
				catch (const std::exception& why)
				{
					std::cout << why.what();
				}

			}

			std::cout << "---Module ended" << std::endl;
			std::cout << "passed " << CTestPassed << "/" << Tests.size() << "Module Tests" << std::endl;
		}
		size_t CTestModulePassed = 0;
		{


			std::cout << "---runing Module Tests" << std::endl;

			Vector<std::future<bool>> List;

			UCodeLang::UAssembly::Get_InsToInsMapValue();
			for (auto& Test : ModuleTests)
			{
				//if (RunTest(Test)) { TestPassed++; }

				if (MultThread == false)
				{

					auto TestR = RunTest(Test, TestMode::CLang89BackEnd);
				}
				else
				{
					auto F = std::async(std::launch::async, [&]
						{
							try
							{
								return RunTest(Test, TestMode::CLang89BackEnd);
							}
							catch (const std::exception& why)
							{
								std::cout << why.what();
								return false;
							}
						}
					);
					List.push_back(std::move(F));
				}
			}

			for (auto& Item : List)
			{
				try
				{
					Item.wait();
					if (Item.get()) { CTestModulePassed++; };
				}
				catch (const std::exception& why)
				{
					std::cout << why.what();
				}

			}

			std::cout << "---Module ended" << std::endl;
			std::cout << "passed " << CTestModulePassed << "/" << ModuleTests.size() << "Module Tests" << std::endl;
		}

		return TestPassed == Tests.size()
			&& TestModulePassed == ModuleTests.size()
			&& CTestPassed == Tests.size()
			&& CTestModulePassed == ModuleTests.size();
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

UCodeTestEnd