#pragma once

#ifndef UCodeLangNoCompiler

#include "../LangCore.hpp"

#include "Front/FrontEndObject.hpp"
#include "Middle/IROptimizer.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
#include "Back/BackEndInterface.hpp"
#include "DependencyFile.hpp"

#include "CompilationSettings.hpp"
UCodeLangStart
class Compiler
{

public:

	using OnRebuild = std::function<void()>;
	struct CompilationSuccess
	{
		NeverNullPtr<UClib> OutPut;
		Optional<BytesPtr> OutFile;
		CompilationSuccess()
		{

		}
		CompilationSuccess(NeverNullPtr<UClib> OutPut)
			: OutPut(OutPut)
		{
		}
		CompilationSuccess(NeverNullPtr<UClib> OutPut, BytesPtr&& OutFile)
			: OutPut(OutPut)
		{
			this->OutFile = std::move(OutFile);
		}
	};
	using CompilerRet = Result<CompilationSuccess, NeverNullPtr<CompilationErrors>>;

	struct CompilerPathData
	{
		Path FileDir;
		Path IntDir;
		Path OutFile;
	};
	struct ExternalFiles
	{
		Vector<Path> Files;
	};


	UCodeLangAPIExport CompilerRet CompileText(const String_view& Text, const ExternalFiles& ExternalFiles = {});
	UCodeLangAPIExport static String GetTextFromFile(const Path& path);
	UCodeLangAPIExport static BytesPtr GetBytesFromFile(const Path& path);
	UCodeLangForceinline CompilerRet CompileFileToLib(const Path& path, const ExternalFiles& ExternalFiles = {})
	{
		return  CompileText(GetTextFromFile(path), ExternalFiles);
	}
	UCodeLangAPIExport CompilerRet CompilePathToObj(const Path& path, const Path& OutLib, const ExternalFiles& ExternalFiles = {});



	UCodeLangAPIExport CompilerRet CompileFiles(const CompilerPathData& Data, const ExternalFiles& ExternalFiles = {});


	UCodeLangAPIExport CompilerRet CompileFiles_UseIntDir(const CompilerPathData& Data, const ExternalFiles& ExternalFiles = {});

	UCodeLangAPIExport BytesPtr OpenFile(const LangDefInfo::FileInfo* FInfo, const Path& path);


	UCodeLangForceinline UCodeLang::CompilationErrors& Get_Errors()
	{
		return _Errors;
	}
	UCodeLangForceinline void ReMove_Errors()
	{
		_Errors.Remove_Errors();
	}
	UCodeLangForceinline CompilationSettings& Get_Settings()
	{
		return _Settings;
	}


	UCodeLangForceinline void Set_FrontEnd(FrontEndObject_Ptr Value)
	{
		_FrontEnd = Value;
	}
	UCodeLangForceinline void Set_BackEnd(BackEndObject_Ptr Value)
	{
		_BackEnd = Value;
	}

	String GetBackEndName()
	{
		Unique_ptr<BackEndObject> obj = Unique_ptr<BackEndObject>(_BackEnd());
		return obj->GetBackEndName();
	}
	String GetOutputExtWithDot()
	{
		Unique_ptr<BackEndObject> obj = Unique_ptr<BackEndObject>(_BackEnd());
		return obj->GetOutputExtWithDot();
	}
	CompilationBackEndInfo GetOutputBackEndInfo()
	{
		CompilationBackEndInfo V;
		Unique_ptr<BackEndObject> obj = Unique_ptr<BackEndObject>(_BackEnd());
		obj->UpdateBackInfo(V);
		return V;
	}
	void SetLog(OnRebuild log)
	{
		_rebuildcallback = log;
	}
	void RemoveLog()
	{
		_rebuildcallback = {};
	}

	Compiler();
private:
	CompilationSettings _Settings;
	CompilationErrors _Errors;
	//Front
	Unique_ptr<FrontEndObject> _FrontEndObject;
	//Middle
	IROptimizer _Optimizer;
	//Back
	Unique_ptr<BackEndObject> _BackEndObject;
	//Log
	Optional<OnRebuild> _rebuildcallback;

	FrontEndObject_Ptr _FrontEnd = nullptr;
	BackEndObject_Ptr _BackEnd = nullptr;

	FrontEndObject_Ptr _oldFrontEnd = nullptr;
	BackEndObject_Ptr _oldBackEnd = nullptr;
	void Optimize(IRBuilder& IR);
};
UCodeLangEnd

#endif