#pragma once
#include <ULanguageNameSpace.hpp>
#include <memory>
#include <thread>

#include "ImGuiHelpers/TextEditor/TextEditor.h"
#include "LanguageSever.hpp"
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

	//SandBox
	SandBoxLanguageSever* SeverPtr = nullptr;
	std::unique_ptr<std::thread> _LangSeverThread;
};

UCodeIDEEnd