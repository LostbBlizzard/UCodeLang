#pragma once
#include <ULanguageNameSpace.hpp>
#include <memory>
#include <thread>

#include "ImGuiHelpers/TextEditor/TextEditor.h"
#include "LanguageSever.hpp"
#include "UCodeLang/UCodeLang.hpp"

#include "ImGuiHelpers/ImguiHelper.hpp"
UCodeIDEStart

struct SandBoxLanguageSever
{
	UCodeLanguageSever::LanguageSever _Sever;
};
class AppObject
{
public:
	AppObject() {};
	~AppObject() { EndApp(); };

	void Init();
	void OnDraw();

	static void ShowInFiles(const Path& path);

	void ShowUCodeVMWindow();



	static size_t GetColumn(const String& text, size_t line, size_t Pos);

	bool IsAppRuning()
	{
		return _IsAppRuning;
	}
	void EndApp()
	{
		if (_IsAppRuning)
		{
			OnAppEnd();
		}
	}
private:
	void OnAppEnd();
	bool _IsAppRuning = false;



	TextEditor _Editor;
	String GetTextEditorString()
	{
		return _Editor.GetText();
	}

	//SandBox
	SandBoxLanguageSever* SeverPtr = nullptr;
	std::unique_ptr<std::thread> _LangSeverThread;
	//Compiler
	String _CompilerStr;
	UCodeLang::Compiler _Compiler;
	UCodeLang::UClib _CompiledLib;
	String _LibInfoString;
	struct Error
	{
		UCodeLang::CompliationErrors::Error _Error;
		bool IsSelected = false;
	};
	void OnErrorListUpdated()
	{
		TextEditor::ErrorMarkers marks;
		for (auto& Item : Errors)
		{
			marks[Item._Error.Line - 1] = Item._Error._Msg;
		}
		_Editor.SetErrorMarkers(marks);
	}

	void CompileText(const UCodeAnalyzer::String& String);

	Vector<Error> Errors;
	//RunTime
	String _RunTimeStr;
	UCodeLang::RunTimeLangState _RunTimeState;
	UCodeLang::AnyInterpreter _AnyInterpreter;
	UCodeLang::RunTimeLib _RuntimeLib;
	//Windows

	enum class BackEndType
	{
		UCodeVM,
		C89,
		IR,
	};


	struct OutputWindowData
	{
		BackEndType Type = BackEndType::UCodeVM;

		bool AutoCompile = false;
		bool AutoReload = false;
		bool AutoHotReload = false;

		UCodeLang::OptimizationFlags Flags = UCodeLang::OptimizationFlags::NoOptimization;
		bool InDebug = true;
	};
	OutputWindowData OutputWindow;

	enum class NativeSet
	{
		x86,
		x86_64,


#if UCodeLang_CPUIs_x86
		Native = x86,
#else
		Native = x86_64,
#endif
	};
	enum class UCodeVMType
	{
		Interpreter,
		Jit_Interpreter,
		Native_Interpreter,
	};
	struct UCodeVMWindow
	{
		UCodeVMType VMType = UCodeVMType::Interpreter;
		NativeSet NativeCpuType = NativeSet::Native;
		bool ShowRegisters = false;
		bool ShowStack = false;
		bool ShowStaticMemory = false;
		bool ShowThreadMemory = false;
		bool ShowHeapMemory = false;

		struct InsData
		{
			UCodeLang::UAddress InsAddress = UCodeLang::NullAddress;
			UCodeLang::Instruction _Ins;
			String StringValue;
		};
		Vector<InsData> InsInfo;
		InsData& GetIns(UCodeLang::UAddress address)
		{
			return InsInfo[address];
		}
		const InsData& GetIns(UCodeLang::UAddress address) const
		{
			return InsInfo[address];
		}
	};
	UCodeVMWindow windowdata;
	void UpdateInsData(UCodeVMWindow& windowdata);

	void ShowDebugerMenu(UCodeVMWindow& windowdata);
	void OnRuntimeUpdated();

	struct CallFuncContext
	{
		const UCodeLang::ClassMethod* current_method = nullptr;
		Vector<BytesPtr> Args;

		UCodeLang::ReflectionTypeInfo _LastRetType;
		BytesPtr _LastRet;

		bool CallStaticAndThreadInit = true;
		bool CallStaticAndThreadDeInit = true;
	};
	CallFuncContext callFuncContext;
};

UCodeIDEEnd