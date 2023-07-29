#pragma once
#include <ULanguageNameSpace.hpp>
#include <memory>
#include <thread>

#include "ImGuiHelpers/TextEditor/TextEditor.h"
#include "LanguageSever.hpp"
#include "UCodeLang/UCodeLang.hpp"
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
		bool IsSelected=false;
	};
	void OnErrorListUpdated()
	{
		TextEditor::ErrorMarkers marks;
		for (auto& Item : Errors)
		{
			marks[Item._Error.Line-1] = Item._Error._Msg;
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
};

UCodeIDEEnd