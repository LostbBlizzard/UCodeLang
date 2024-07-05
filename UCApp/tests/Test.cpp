#include "Test.hpp"
#include <future>
#include <mutex>
#include <memory>
#include <fstream>

#include "UCodeLang/RunTime/TestRuner.hpp"

#if UCodeLang_Platform_Windows
#include <windows.h>
#elif UCodeLang_Platform_Posix
#include <dlfcn.h>
#endif
UCodeTestStart

	using namespace UCodeLang;

template <typename T>
String int_to_hex(T i)
{
	std::stringstream stream;
	stream << "0x"
		   << std::setfill('0') << std::setw(sizeof(T) * 2)
		   << std::hex << i;
	return stream.str();
}

template <typename T>
String int_to_hexV(T i)
{
	std::stringstream stream;
	stream
		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;
	return stream.str();
}





bool RunTest(const ModuleTest &Test, TestMode mode)
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
	bool PassedTests = false;

	size_t TestsPassed = 0;
	size_t TestsSkiped = 0;
	size_t TestsFail = 0;
	size_t TestsCount = 0;
};

void RunStandardLibraryTests(StandardLibraryTestInfo &Out, TestMode mode)
{
	if (mode == TestMode::WasmBackEnd)
	{
		Out.ProjectCompiled = true;
		Out.PassedTests = true;
		return;
	}

	ModuleFile Mfile;
	if (!ModuleFile::FromFile(&Mfile, UCodeLangVSAPIPath + "/StandardLibrary/ULangModule.ucm"))
	{
		return;
	}

	Compiler compiler;
	if (mode == TestMode::CLang89BackEnd)
	{
		compiler.Set_BackEnd(C11Backend::MakeObject);
	}
	else if (mode == TestMode::WasmBackEnd)
	{
		compiler.Set_BackEnd(WasmBackEnd::MakeObject);
	}

	auto index = ModuleIndex::GetModuleIndex();
	std::cout << "Runing StandardLibraryTests for " << TestModeToName(mode) << " ";

	auto OutData = Mfile.BuildModule(compiler, index);
	LogErrors(std::cout, compiler);

	if (OutData.CompilerRet.IsValue())
	{
		Out.ProjectCompiled = true;
		if (mode == TestMode::UCodeLangBackEnd)
		{

			UCodeLang::UClib MLib;
			UCodeLangAssert(UClib::FromFile(&MLib, OutData.OutputItemPath));

			UCodeLang::TestRuner runer;
			auto info = runer.RunTests(MLib, UCodeLang::TestRuner::InterpreterType::Interpreter, [](TestRuner::TestInfo &test)
									   {
						if (test.Passed)
						{
							std::cout << "Test :" << test.TestName << " Passed\n";
						}
						else
						{
							std::cout << "Test :" << test.TestName << " Fail\n";
						} });

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
				Out.PassedTests = true;
				std::cout << "Tests Passed.all 100% of tests passed\n";
			}
			else
			{
				std::cout << "Tests Failed about " << passnumber << "% passed\n";
			}
			Out.TestsPassed = info.TestPassedCount;
			Out.TestsSkiped = 0;
			Out.TestsFail = info.TestCount - info.TestPassedCount;
			Out.TestsCount = info.TestCount;
		}
		else if (mode == TestMode::CLang89BackEnd)
		{
			auto &rlib = OutData.CompilerRet.GetValue().OutPut;
			auto tests = UCodeLang::TestRuner::GetTests(rlib->Get_Assembly());
			UCodeLang::TestRuner::TestsResult info;
			info.TestCount = tests.size();

			auto cfilepath = OutData.OutputItemPath;
			Path dllfile = OutData.OutputItemPath.native() + Path(".lib").native();

			UCodeLangAssert(CompileC89ToLib(cfilepath, dllfile));

			auto staticinitname = C11Backend::UpdateToCindentifier(StaticVariablesInitializeFunc);
			auto threadinitname = C11Backend::UpdateToCindentifier(ThreadVariablesInitializeFunc);

			auto staticdeinitname = C11Backend::UpdateToCindentifier(StaticVariablesUnLoadFunc);
			auto threaddeinitname = C11Backend::UpdateToCindentifier(ThreadVariablesUnLoadFunc);

#if UCodeLang_Platform_Windows
			auto lib = LoadLibrary(dllfile.c_str());
			UCodeLangDefer(FreeLibrary(lib));
			auto staticinittocall = GetProcAddress(lib, staticinitname.c_str());
			auto threadinittocall = GetProcAddress(lib, threadinitname.c_str());
			auto staticdeinittocall = GetProcAddress(lib, staticdeinitname.c_str());
			auto threaddeinittocall = GetProcAddress(lib, threaddeinitname.c_str());
#elif UCodeLang_Platform_Posix
			auto lib = dlopen(dllfile.c_str(), RTLD_NOW);
			UCodeLangDefer(dlclose(lib));

			auto staticinittocall = dlsym(lib, staticinitname.c_str());
			auto threadinittocall = dlsym(lib, threadinitname.c_str());
			auto staticdeinittocall = dlsym(lib, staticdeinitname.c_str());
			auto threaddeinittocall = dlsym(lib, threaddeinitname.c_str());
#endif
			for (auto &Item : tests)
			{
				auto functocallStr = C11Backend::UpdateToCindentifier(Item->DecorationName);
#if UCodeLang_Platform_Windows
				auto functocall = GetProcAddress(lib, functocallStr.c_str());
#elif UCodeLang_Platform_Posix
				auto functocall = dlsym(lib, functocallStr.c_str());
#endif

				bool testpassed = false;
				if (Item->RetType._Type == ReflectionTypes::Bool)
				{
					using Func = bool (*)();
					testpassed = ((Func)functocall)();
				}
				else
				{
					testpassed = true;
				}

				if (testpassed)
				{
					std::cout << "Test :" << Item->FullName << " Passed\n";
					info.TestPassedCount++;
				}
				else
				{
					std::cout << "Test :" << Item->FullName << " Fail\n";
				}
			}

			int passnumber;
			if (info.TestPassedCount)
			{
				passnumber = ((float)info.TestPassedCount / (float)info.TestCount) * 100;
			}
			else
			{
				passnumber = 100;
			}

			bool passed = info.TestCount == info.TestPassedCount;
			std::cout << "Ran all " << info.TestCount << " Tests\n";

			std::cout << TestModeToName(mode) << " ";
			if (passed)
			{
				Out.PassedTests = true;
				std::cout << "Tests Passed.all 100% of tests passed\n";
			}
			else
			{
				std::cout << "Tests Failed about " << passnumber << "% passed\n";
			}

			Out.TestsPassed = info.TestPassedCount;
			Out.TestsSkiped = 0;
			Out.TestsFail = info.TestCount - info.TestPassedCount;
			Out.TestsCount = info.TestCount;
		}
		else
		{
			UCodeLangUnreachable();
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
	Array<TestBackEndGroup, BackEndsCount> TestInfo;

	Array<StandardLibraryTestInfo, BackEndsCount> StandardTestInfo;

	bool rununitTest = true;
	bool runStandardLibraryTest = true;
	bool runincrementalcompilationTestOnStandardLibrary = false;

	bool hasgcc = system("gcc -v") == EXIT_SUCCESS;
	bool hasnode = system("node -v") == EXIT_SUCCESS;

	if (rununitTest)
	{
		for (size_t i = 0; i < BackEndsCount; i++)
		{
			auto &MyTestInfo = TestInfo[i];
			TestMode mode = (TestMode)i;

			std::cout << "---running Test for " << TestModeToName(mode) << std::endl;

			Vector<std::future<bool>> List;
			List.resize(Tests.size());

			if (mode == TestMode::WasmBackEnd && hasnode == false)
			{
				MyTestInfo.TestsSkiped += Tests.size();
				continue;
			}

			/*
			// geting 32bit gcc is too much work.
			if (mode == TestMode::CLang89BackEnd)
			{
				if (hasgcc == false || UCodeLang_32BitSytem)
				{
					MyTestInfo.TestsSkiped += Tests.size();
					continue;
				}
			}
			*/

			for (size_t i = 0; i < Tests.size(); i++)
			{
				auto &Test = Tests[i];

				if (!ShouldSkipTests(i, mode))
				{
					if (MultThread == false)
					{
						auto TestR = RunTest(Test, mode);
						auto F = std::async(std::launch::async, [&]
											{ return TestR; });
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
									} });
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
				auto &Item = List[i];

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
					catch (const std::exception &why)
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
			index.AddModueToList(UCodeLangVSAPIPath + "\\CompilerAPI\\ULangModule.ucm");

			index.AddModueToList(UCodeLangVSAPIPath + "\\NStandardLibrary\\ULangModule.ucm");
			index.AddModueToList(UCodeLangVSAPIPath + "\\BuildSystem\\ULangModule.ucm");

			ModuleIndex::SaveModuleIndex(index);
		}
		for (size_t i = 0; i < BackEndsCount; i++)
		{
			auto &MyTestInfo = StandardTestInfo[i];
			TestMode mode = (TestMode)i;
			
			/*
			if (mode == TestMode::CLang89BackEnd)
			{
				if (hasgcc == false || UCodeLang_32BitSytem)
				{
					MyTestInfo.TestsSkiped += StandardTestInfo[0].TestsCount;
					MyTestInfo.PassedTests = true;
					MyTestInfo.ProjectCompiled = true;
					continue;
				}
				if (UCodeLang_Platform_MacOS)
				{
					MyTestInfo.TestsSkiped += StandardTestInfo[0].TestsCount;
					MyTestInfo.PassedTests = true;
					MyTestInfo.ProjectCompiled = true;
					//CI test are failing and dont have a mac to debug it.
					continue;
				}
			}
			*/

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

		for (const auto &dirEntry : recursive_directory_iterator(UCodeLangVSAPIPath + "\\StandardLibrary"))
		{
			if (dirEntry.path().extension() == FileExt::SourceFileWithDot)
			{

				{ // update file
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

	if (rununitTest)
	{
		std::cout << "---Init Tests Review" << std::endl;
		for (size_t i = 0; i < BackEndsCount; i++)
		{
			auto &MyTestInfo = TestInfo[i];
			TestMode mode = (TestMode)i;

			std::cout << "BackEnd :" << TestModeToName(mode) << std::endl;

			std::cout << "  passed :" << MyTestInfo.TestsPassed << "/" << Tests.size()
					  << " (" << (int)(((float)MyTestInfo.TestsPassed / (float)Tests.size()) * 100) << "%)"
					  << " Tests" << std::endl;

			std::cout << "  skiped :" << MyTestInfo.TestsSkiped << "/" << Tests.size()
					  << " (" << (int)(((float)MyTestInfo.TestsSkiped / (float)Tests.size()) * 100) << "%) "
					  << " Tests" << std::endl;

			std::cout << "  failed :" << MyTestInfo.TestsFail << "/" << Tests.size()
					  << " (" << (int)(((float)MyTestInfo.TestsFail / (float)Tests.size()) * 100) << "%) "
					  << " Tests" << std::endl;
		}
		std::cout << "---Init Tests Average" << std::endl;
		{
			size_t PassCount = 0;
			size_t SkipedCount = 0;
			size_t FailCount = 0;

			auto alltestcount = Tests.size() * BackEndsCount;
			for (size_t i = 0; i < BackEndsCount; i++)
			{
				auto &MyTestInfo = TestInfo[i];
				TestMode mode = (TestMode)i;
				PassCount += MyTestInfo.TestsPassed;
				SkipedCount += MyTestInfo.TestsSkiped;
				FailCount += MyTestInfo.TestsFail;
			}
			std::cout << "  passed :" << PassCount << "/" << alltestcount
					  << " (" << (int)(((float)PassCount / (float)alltestcount) * 100) << "%) "
					  << " Tests" << std::endl;

			std::cout << "  skiped :" << SkipedCount << "/" << alltestcount
					  << " (" << (int)(((float)SkipedCount / (float)alltestcount) * 100) << "%) "
					  << " Tests" << std::endl;

			std::cout << "  failed :" << FailCount << "/" << alltestcount
					  << " (" << (int)(((float)FailCount / (float)alltestcount) * 100) << "%) "
					  << " Tests" << std::endl;
		}
	}

	if (runStandardLibraryTest)
	{
		std::cout << "---StandardLibrary Tests Review" << std::endl;
		for (size_t i = 0; i < StandardTestInfo.size(); i++)
		{
			auto &MyTestInfo = StandardTestInfo[i];
			TestMode mode = (TestMode)i;

			std::cout << "BackEnd :" << TestModeToName(mode) << std::endl;

			std::cout << "  passed :" << MyTestInfo.TestsPassed << "/" << MyTestInfo.TestsCount
					  << " (" << (int)(((float)MyTestInfo.TestsPassed / (float)MyTestInfo.TestsCount) * 100) << "%)"
					  << " Tests" << std::endl;

			std::cout << "  skiped :" << MyTestInfo.TestsSkiped << "/" << MyTestInfo.TestsCount
					  << " (" << (int)(((float)MyTestInfo.TestsSkiped / (float)MyTestInfo.TestsCount) * 100) << "%) "
					  << " Tests" << std::endl;

			std::cout << "  failed :" << MyTestInfo.TestsFail << "/" << MyTestInfo.TestsCount
					  << " (" << (int)(((float)MyTestInfo.TestsFail / (float)MyTestInfo.TestsCount) * 100) << "%) "
					  << " Tests" << std::endl;
		}
		std::cout << "---StandardLibrary Tests Average" << std::endl;
		{
			size_t PassCount = 0;
			size_t SkipedCount = 0;
			size_t FailCount = 0;

			auto alltestcount = StandardTestInfo[0].TestsCount * BackEndsCount;
			for (size_t i = 0; i < StandardTestInfo.size(); i++)
			{
				auto &MyTestInfo = StandardTestInfo[i];
				TestMode mode = (TestMode)i;
				PassCount += MyTestInfo.TestsPassed;
				SkipedCount += MyTestInfo.TestsSkiped;
				FailCount += MyTestInfo.TestsFail;
			}
			std::cout << "  passed :" << PassCount << "/" << alltestcount
					  << " (" << (int)(((float)PassCount / (float)alltestcount) * 100) << "%) "
					  << " Tests" << std::endl;

			std::cout << "  skiped :" << SkipedCount << "/" << alltestcount
					  << " (" << (int)(((float)SkipedCount / (float)alltestcount) * 100) << "%) "
					  << " Tests" << std::endl;

			std::cout << "  failed :" << FailCount << "/" << alltestcount
					  << " (" << (int)(((float)FailCount / (float)alltestcount) * 100) << "%) "
					  << " Tests" << std::endl;
		}
	}

	bool isok = true;
	for (auto &Item : TestInfo)
	{
		if (Item.TestsFail)
		{
			isok = false;
		}
	}
	for (auto &Item : StandardTestInfo)
	{
		if (Item.ProjectCompiled == false || Item.PassedTests == false)
		{
			isok = false;
		}
	}

	return isok ? EXIT_SUCCESS : EXIT_FAILURE;
}


UCodeTestEnd