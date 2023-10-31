#include "Test.hpp"
#include <future>
#include <mutex>
#include <memory>
#include <sstream>
#include <fstream>
#include <UCodeLang/Compliation/UAssembly/UAssembly.hpp>
#include <UCodeLang/Compliation/ModuleFile.hpp>

#include <UCodeLang/Compliation/Back/WebAssembly/WasmBackEnd.hpp>

#include "../src/UCodeLangProjectPaths.hpp"

#include "UCodeLang/RunTime/TestRuner.hpp"

#if UCodeLang_Platform_Windows
#include <windows.h>
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
		#if UCodeLangDebug
		Compiler::CompilerPathData paths;
		Compiler Com;
		Com.Get_Settings()._Flags = flag;
		Com.Get_Settings().PtrSize = IntSizes::Native;
		
		Compiler::CompilerRet Com_r =NeverNullptr(&Com.Get_Errors());
		std::string InputFilesPath = UCodeLang_UCAppDir_Test_UCodeFiles + Test.InputFilesOrDir;
		std::string OutFileDir = UCodeLang_UCAppDir_Test_OutputFiles + Test.TestName;
		std::filesystem::path p = OutFileDir;
		OutFileDir = p.parent_path().generic_string() + "/" + +Test.TestName + "/";

		std::filesystem::create_directories(OutFileDir);	
		
		if (mode == TestMode::CLang89BackEnd)
		{
			Com.Set_BackEnd(C89Backend::MakeObject);
		}
		else if (mode == TestMode::WasmBackEnd)
		{
			Com.Set_BackEnd(WasmBackEnd::MakeObject);
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
				(Com_r.IsValue() && Test.Condition == SuccessCondition::Compilation)
				||
				(Com_r.IsError() && Test.Condition == SuccessCondition::CompilationFail)
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

		if (Com_r.IfError())
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
		UClib& ulib = *Com_r.GetValue().OutPut;


			auto ufunc = ulib.Get_Assembly().Find_Func(Test.FuncToCall);
			UCodeLangAssert(ufunc);
			{
				// String filetxt = Compiler::GetTextFromFile(OutFilePath);

				Path  dllfile = OutFilePath + ".lib";
				Path Testablefile = OutFilePath;
				UCodeLangAssert(CompileC89ToLib(Testablefile, dllfile));


				auto& Assembly = ulib.Get_Assembly();
				auto cfuncname = C89Backend::UpdateToCindentifier(ufunc->DecorationName);

				auto staticinitname = C89Backend::UpdateToCindentifier(StaticVariablesInitializeFunc);
                auto threadinitname = C89Backend::UpdateToCindentifier(ThreadVariablesInitializeFunc);

                auto staticdeinitname = C89Backend::UpdateToCindentifier(StaticVariablesUnLoadFunc);
                auto threaddeinitname = C89Backend::UpdateToCindentifier(ThreadVariablesUnLoadFunc);


				#if UCodeLang_Platform_Windows
				auto lib = LoadLibrary(dllfile.c_str());
				UCodeLangDefer(FreeLibrary(lib));

				auto staticinittocall = GetProcAddress(lib,staticinitname.c_str());
                auto threadinittocall = GetProcAddress(lib,threadinitname.c_str());
                auto staticdeinittocall = GetProcAddress(lib,staticdeinitname.c_str());
                auto threaddeinittocall = GetProcAddress(lib,threaddeinitname.c_str());
				

				auto functocall = GetProcAddress(lib, cfuncname.c_str());
				#elif UCodeLang_Platform_Posix
				auto lib = dlopen(dllfile.c_str(), RTLD_NOW);
				UCodeLangDefer(dlclose(lib));
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

				auto rettype = ufunc->RetType;
				if (auto val = Assembly.Find_Node(rettype))
				{
					if (val->Get_Type() == ClassType::Enum)
					{
						auto& Enum = val->Get_EnumData();
						rettype = Enum.BaseType;
					}
				}

				auto RetValue = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
				{
					if (rettype._Type == ReflectionTypes::Bool)
					{
						using GetValueFunc = bool(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else if (rettype._Type == ReflectionTypes::sInt8
						|| rettype._Type == ReflectionTypes::uInt8
						|| rettype._Type == ReflectionTypes::Char)
					{
						using GetValueFunc = UInt8(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else  if (rettype._Type == ReflectionTypes::uInt16
						|| rettype._Type == ReflectionTypes::sInt16)
					{
						using GetValueFunc = Int16(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else  if (rettype._Type == ReflectionTypes::uInt32
						|| rettype._Type == ReflectionTypes::sInt32)
					{
						using GetValueFunc = Int32(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else  if (rettype._Type == ReflectionTypes::float32)
					{
						using GetValueFunc = float32(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else  if (rettype._Type == ReflectionTypes::float64)
					{
						using GetValueFunc = float64(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else  if (rettype._Type == ReflectionTypes::uIntPtr
						|| rettype._Type == ReflectionTypes::sIntPtr)
					{
						using GetValueFunc = uintptr_t(*)();
						auto val = ((GetValueFunc)functocall)();

						UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
						memcpy(RetValue.get(), &val, sizeof(val));
					}
					else if (auto typenod = Assembly.Find_Node(rettype))
					{
						if (StringHelper::StartWith(typenod->FullName, "Vec2")
							|| StringHelper::StartWith(typenod->FullName, "vec2"))
						{
							// using GetValueFunc = Vec2(*)();//I have no idea why this does not work
							// auto val = ((GetValueFunc)functocall)();



							using GetValueFunc2 = Int64(*)();
							auto val2 = ((GetValueFunc2)functocall)();

							Vec2& val = *(Vec2*)&val2;

							UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
							memcpy(RetValue.get(), &val, sizeof(val));
						}
						else
						{
							UCodeLangUnreachable();
						}
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
			}
		}
		else if (mode == TestMode::WasmBackEnd)
		{
			UClib& ulib = *Com_r.GetValue().OutPut;
			auto& OutFile = Com_r.GetValue().OutFile.value();

			auto ufunc = ulib.Get_Assembly().Find_Func(Test.FuncToCall);
			UCodeLangAssert(ufunc);

			String JsString = "const wasm = new Uint8Array([";

			std::stringstream ss;
			ss << "const wasm = new Uint8Array([";
			for (const auto& b : OutFile) {
				ss << "0x" << std::hex << static_cast<int>(b) << ", ";
			}
			ss << "]);\n";
			ss << "const m = new WebAssembly.Module(wasm);\n";
			ss << "const instance = new WebAssembly.Instance(m, {});\n";
			ss << "console.log(instance.exports.";
			ss << WasmBackEnd::ToWebName(ufunc->DecorationName);
			ss << "());";

			Path node_file = paths.OutFile.native() + Path("test.js").native();
			Path out_file = paths.OutFile.native() + Path("test.js.out").native();


			std::ofstream nf(node_file);
			nf << ss.str();
			nf << std::flush;

			String expected;
			auto rettype = ufunc->RetType;

			if (rettype._Type == ReflectionTypes::sInt32)
			{
				expected += std::to_string(*(int*)Test.RunTimeSuccess.get());
			}
			else if (rettype._Type == ReflectionTypes::sInt16)
			{
				expected += std::to_string(*(Int16*)Test.RunTimeSuccess.get());
			}
			else if (rettype._Type == ReflectionTypes::sInt8)
			{
				expected += std::to_string(*(Int8*)Test.RunTimeSuccess.get());
			}
			else if (rettype._Type == ReflectionTypes::uInt16)
			{
				expected += std::to_string(*(UInt16*)Test.RunTimeSuccess.get());
			}
			else if (rettype._Type == ReflectionTypes::uInt8)
			{
				expected += std::to_string(*(UInt8*)Test.RunTimeSuccess.get());
			}
			else if (rettype._Type == ReflectionTypes::Char)
			{
				expected += std::to_string(*(Int8*)Test.RunTimeSuccess.get());
			}
			else if (rettype._Type == ReflectionTypes::float32)
			{
				expected += std::to_string(*(float32*)Test.RunTimeSuccess.get());
			}
			else if (rettype._Type == ReflectionTypes::float64)
			{
				expected += std::to_string(*(float64*)Test.RunTimeSuccess.get());
			}
			else
			{
				UCodeLangUnreachable();
			}


			expected += '\n';

			{
				std::system(("node " + node_file.generic_string() + " > " + out_file.generic_string()).c_str());
			}

			std::stringstream ss_out;
			ss_out << std::ifstream(out_file).rdbuf();
			auto outstr = ss_out.str();
			
			{
				//C++ adds trailing zeros but node.js does not
				if (rettype._Type == ReflectionTypes::float32)
				{
					float newfloat = std::stof(outstr.substr(0, outstr.size() - 1));//-1 to remove /n
					outstr = std::to_string(newfloat);
					outstr += '\n';
				}
				else if (rettype._Type == ReflectionTypes::float64)
				{
					float64 newfloat = std::stof(outstr.substr(0, outstr.size() - 1));//-1 to remove /n
					outstr = std::to_string(newfloat);
					outstr += '\n';
				}

			}
			if (outstr != expected) {

				

				std::cerr << "got: " << ss_out.str();
				std::cerr << "expected: " << expected;
				return false;
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
			if (ret.CompilerRet.IsValue())
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

	constexpr size_t BackEndsCount = (size_t)TestMode::Max;

	struct StandardLibraryTestInfo
	{
		bool ProjectCompiled = true;
	};


	void RunStandardLibraryTests(StandardLibraryTestInfo& Out,TestMode mode)
	{
		if (mode == TestMode::WasmBackEnd)
		{
			return;
		}

		ModuleFile Mfile;
		ModuleFile::FromFile(&Mfile, UCodeLangVSAPIPath + "\\StandardLibrary\\ULangModule.ucm");

		Compiler compiler;


		auto index = ModuleIndex::GetModuleIndex();
		std::cout << "Runing StandardLibraryTests for " << TestModeToName(mode) << " ";

		auto OutData = Mfile.BuildModule(compiler, index);
		LogErrors(std::cout, compiler);

		if (OutData.CompilerRet.IsValue())
		{
			UCodeLang::UClib MLib;
			UCodeLangAssert(UClib::FromFile(&MLib, OutData.OutputItemPath));
			


			UCodeLang::TestRuner runer;
			auto info = runer.RunTests(MLib, UCodeLang::TestRuner::InterpreterType::Interpreter, [](TestRuner::TestInfo& test)
				{
					if (test.Passed)
					{
						std::cout << "Test :" << test.TestName << " Passed\n";
					}
					else
					{
						std::cout << "Test :" << test.TestName << " Fail\n";
					}
				});

			bool passed = info.TestCount == info.TestPassedCount;
			std::cout << "Ran all " << info.TestCount << " Tests\n";

			int passnumber;
			if (info.TestPassedCount)
			{
				passnumber = ((float)info.TestPassedCount / (float)info.TestCount) * 100;
			}
			else
			{
				passnumber = 100;
			}


			std::cout << TestModeToName(mode) << " ";
			if (passed)
			{
				std::cout << "Tests Passed.all 100% of tests passed\n";
			}
			else
			{
				std::cout << "Tests Failed about " << passnumber << "% passed\n";
			}
		}

		
	}

	int RunTests(bool MultThread)
	{
		struct TestBackEndGroup
		{
			size_t TestsPassed = 0;
			size_t TestsSkiped = 0;
			size_t TestsFail = 0;

			size_t TestModulePassed = 0;
		};
		Array< TestBackEndGroup, BackEndsCount> TestInfo;

		Array< StandardLibraryTestInfo, BackEndsCount> StandardTestInfo;

		bool rununitTest = true;
		bool runStandardLibraryTest = false;
		bool runincrementalcompilationTestOnStandardLibrary = false;

		bool hasgcc = system("gcc -v") == EXIT_SUCCESS;

		if (rununitTest)
		{
			for (size_t i = 0; i < BackEndsCount; i++)
			{
				auto& MyTestInfo = TestInfo[i];
				TestMode mode = (TestMode)i;

				std::cout << "---runing Test for " << TestModeToName(mode) << std::endl;
				
				Vector<std::future<bool>> List;
				List.resize(Tests.size());

				
				//geting gcc on 32 bit is a hassle
				#if UCodeLang_32BitSytem
				if (mode == TestMode::CLang89BackEnd) {
					continue;
				}
				#endif

				if (mode == TestMode::CLang89BackEnd && hasgcc == false)
				{
					continue;
				}

				for (size_t i = 0; i < Tests.size(); i++)
				{
					auto& Test = Tests[i];

					if (!ShouldSkipTests(i, mode))
					{
						if (MultThread == false)
						{
							auto TestR = RunTest(Test, mode);
							auto F = std::async(std::launch::async, [&]
								{
									return TestR;
								});
							List[i] = std::move(F);
						}
						else
						{
							auto F = std::async(std::launch::async, [&]
								{
									try
									{
										return RunTest(Test, mode);
									}
									catch (const std::exception& why)
									{
										std::cout << why.what();
										return false;
									}
								}
							);
							List[i] = std::move(F);
						}
					}
					else
					{
						MyTestInfo.TestsSkiped++;
					}
				}

				for (size_t i = 0; i < Tests.size(); i++)
				{
					auto& Item = List[i];

					if (!ShouldSkipTests(i, mode))
					{
						try
						{
							Item.wait();
							if (Item.get())
							{
								MyTestInfo.TestsPassed++;
							}
							else
							{
								MyTestInfo.TestsFail++;
							}
						}
						catch (const std::exception& why)
						{
							std::cout << why.what();
							MyTestInfo.TestsFail++;
						}
					}


				}

				std::cout << "---Tests ended for " << TestModeToName(mode) << std::endl;
				std::cout << "passed " << MyTestInfo.TestsPassed << "/" << Tests.size() << " Tests" << std::endl;
				std::cout << "skiped " << MyTestInfo.TestsSkiped << "/" << Tests.size() << " Tests" << std::endl;
				std::cout << "failed " << MyTestInfo.TestsFail << "/" << Tests.size() << " Tests" << std::endl;
			}
		}


		if (runStandardLibraryTest)
		{
			{
				auto index = ModuleIndex::GetModuleIndex();

				index.AddModueToList(UCodeLangVSAPIPath + "\\StandardLibrary\\ULangModule.ucm");
				index.AddModueToList(UCodeLangVSAPIPath + "\\Win32\\ULangModule.ucm");

				index.AddModueToList(UCodeLangVSAPIPath + "\\NStandardLibrary\\ULangModule.ucm");
				index.AddModueToList(UCodeLangVSAPIPath + "\\NWin32\\ULangModule.ucm");
				
				ModuleIndex::SaveModuleIndex(index);
			}
			for (size_t i = 0; i < BackEndsCount; i++)
			{
				auto& MyTestInfo = StandardTestInfo[i];
				TestMode mode = (TestMode)i;
				RunStandardLibraryTests(MyTestInfo, mode);
			}
		}
		
		if (runincrementalcompilationTestOnStandardLibrary)
		{
			UCodeLang::Compiler _Compiler;

			auto index = ModuleIndex::GetModuleIndex();

			ModuleFile Mfile;
			ModuleFile::FromFile(&Mfile, UCodeLangVSAPIPath + "\\StandardLibrary\\ULangModule.ucm");

			using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

			for (const auto& dirEntry : recursive_directory_iterator(UCodeLangVSAPIPath + "\\StandardLibrary"))
			{
				if (dirEntry.path().extension() == FileExt::SourceFileWithDot)
				{

					{//update file
						auto txt = Compiler::GetTextFromFile(dirEntry.path());

						std::ofstream out(dirEntry.path());
						out << txt;
						out.close();
					}

					auto OutData = Mfile.BuildModule(_Compiler, index);

					if (_Compiler.Get_Errors().Has_Warning())
					{
						std::cout << "Errors on incremental compilation ";
						std::cout << "On updated file " << dirEntry.path().generic_string();
						std::cout << '\n';

						return false;
					}
				}
			}
		}

		std::cout << "---Init Tests Review" << std::endl;
		for (size_t i = 0; i < BackEndsCount; i++)
		{
			auto& MyTestInfo = TestInfo[i];
			TestMode mode = (TestMode)i;

			std::cout << "BackEnd :" << TestModeToName(mode) << std::endl;
			
			std::cout << "  passed :" << MyTestInfo.TestsPassed << "/" << Tests.size()
				<< " (" << (int)(((float)MyTestInfo.TestsPassed / (float)Tests.size()) * 100) << "%)" << " Tests" << std::endl;
			
			std::cout << "  skiped :" << MyTestInfo.TestsSkiped << "/" << Tests.size()
				<< " (" << (int)(((float)MyTestInfo.TestsSkiped / (float)Tests.size())*100) << "%) " << " Tests" << std::endl;
			
			std::cout << "  failed :" << MyTestInfo.TestsFail << "/" << Tests.size()
				<< " (" << (int)(((float)MyTestInfo.TestsFail / (float)Tests.size())*100) << "%) "  << " Tests" << std::endl;
		}
		std::cout << "---Init Tests Average" << std::endl;
		{
			size_t PassCount = 0;
			size_t SkipedCount = 0;
			size_t FailCount = 0;

			auto alltestcount = Tests.size() * BackEndsCount;
			for (size_t i = 0; i < BackEndsCount; i++)
			{
				auto& MyTestInfo = TestInfo[i];
				TestMode mode = (TestMode)i;
				PassCount += MyTestInfo.TestsPassed;
				SkipedCount += MyTestInfo.TestsSkiped;
				FailCount += MyTestInfo.TestsFail;
			}
			std::cout << "  passed :" << PassCount << "/" << alltestcount 
				<< " (" << (int)(((float)PassCount/(float)alltestcount)*100) << "%) " << " Tests" << std::endl;
			
			std::cout << "  skiped :" << SkipedCount << "/" << alltestcount 
				<< " (" << (int)(((float)SkipedCount / (float)alltestcount) * 100) << "%) " << " Tests" << std::endl;
			
			std::cout << "  failed :" << FailCount << "/" << alltestcount 
				<< " (" << (int)(((float)FailCount / (float)alltestcount) * 100) << "%) " << " Tests" << std::endl;
		}

		bool isok = true;
		for (auto& Item : TestInfo)
		{
			if (Item.TestsFail)
			{
				isok = false;
			}
		}

		
		return isok ? EXIT_SUCCESS : EXIT_FAILURE;
	}

	bool LogErrors(std::ostream& out, Compiler& _Compiler)
	{
		out << "[\n";
		auto& Errors = _Compiler.Get_Errors().Get_Errors();
		for (auto& Item : Errors)
		{
			out << Item.ToString() << std::endl;
			out << '\n';
		}
		out << "]\n";
		return _Compiler.Get_Errors().Has_Errors();
	}

UCodeTestEnd