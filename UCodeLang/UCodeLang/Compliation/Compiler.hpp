#pragma once
#include "../LangCore.hpp"

#include "Front/FrontEndObject.hpp"
#include "Middle/IROptimizer.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
#include "Back/BackEndInterface.hpp"
#include "DependencyFile.hpp"
UCodeLangStart
class Compiler
{

public:
	enum class CompilerState : UInt8
	{
		Null,Fail,Success, CompilerError
	};
	struct CompilerRet
	{
		CompilerState _State= CompilerState::Null;
		UClib* OutPut =nullptr;
		BytesPtr OutFile;
	};
	struct CompilerPathData
	{
		String FileDir;
		String IntDir;
		String OutFile;
	};
	struct ExternalFiles
	{
		Vector<String> Files;
	};
	
	
	UCodeLangAPIExport CompilerRet CompileText(const String_view& Text);
	UCodeLangAPIExport static String GetTextFromFile(const Path& path);
	UCodeLangAPIExport static BytesPtr GetBytesFromFile(const Path& path);
	UCodeLangForceinline CompilerRet CompileFileToLib(const Path& path)
	{
		return  CompileText(GetTextFromFile(path));
	}
	UCodeLangAPIExport CompilerRet CompilePathToObj(const Path& path, const Path& OutLib);
	UCodeLangAPIExport CompilerRet CompileFiles(const CompilerPathData& Data);

	UCodeLangAPIExport CompilerRet CompileFiles_UseIntDir(const CompilerPathData& Data)
	{
		ExternalFiles External;
		return CompileFiles_UseIntDir(Data, External);
	}
	UCodeLangAPIExport CompilerRet CompileFiles_UseIntDir(const CompilerPathData& Data,const ExternalFiles& ExternalFiles);
	//CompilerRet CompileFiles_UseIntDir(const Vector<Path>& files, const Path& intDir, const  ExternalFiles& ExternalFiles = {});

	UCodeLangAPIExport BytesPtr OpenFile(const LangDefInfo::FileInfo* FInfo, const Path& path);

	
	UCodeLangForceinline UCodeLang::CompliationErrors& Get_Errors()
	{
		return _Errors;
	}
	UCodeLangForceinline void ReMove_Errors()
	{
		_Errors.Remove_Errors();
	}
	UCodeLangForceinline CompliationSettings& Get_Settings()
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
	Compiler();
private:
	CompliationSettings _Settings;
	CompliationErrors _Errors;
	//Front
	Unique_ptr<FrontEndObject> _FrontEndObject;
	//Middle
	IROptimizer _Optimizer;
	//Back
	Unique_ptr<BackEndObject> _BackEndObject;


	FrontEndObject_Ptr _FrontEnd = nullptr;
	BackEndObject_Ptr _BackEnd = nullptr;

	FrontEndObject_Ptr _oldFrontEnd = nullptr;
	BackEndObject_Ptr _oldBackEnd = nullptr;
	void Optimize(IRBuilder& IR);
};
UCodeLangEnd
