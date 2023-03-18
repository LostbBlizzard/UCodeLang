#pragma once
#include "../LangCore.hpp"

#include "Front/FrontEndObject.hpp"
#include "Middle/IROptimizer.hpp"
#include "UCodeLang/LangCore/UClib.hpp"



#include "Front/UCodeFrontEndObject.hpp"
#include "Back/UCodeBackEnd/UCodeBackEnd.hpp"

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
	};
	struct CompilerPathData
	{
		String FileDir;
		String IntDir;
		String OutFile;
	};
	
	
	CompilerRet CompileText(const String_view& Text);
	static String GetTextFromFile(const Path& path);
	static BytesPtr GetBytesFromFile(const Path& path);
	UCodeLangForceinline CompilerRet CompileFileToLib(const Path& path)
	{
		return  CompileText(GetTextFromFile(path));
	}
	CompilerRet CompilePathToObj(const Path& path, const Path& OutLib);
	CompilerRet CompileFiles(const CompilerPathData& Data);

	CompilerRet CompileFiles_UseIntDir(const CompilerPathData& Data);
	CompilerRet CompileFiles_UseIntDir(const Vector<Path>& files, const Path& intDir);

	BytesPtr Compiler::OpenFile(const LangDefInfo::FileInfo* FInfo, const Path& path);

	
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



private:
	CompliationSettings _Settings;
	CompliationErrors _Errors;
	//Front
	Unique_ptr<FrontEndObject> _FrontEndObject;
	//Middle
	IROptimizer _Optimizer;
	//Back
	Unique_ptr<BackEndObject> _BackEndObject;


	FrontEndObject_Ptr _FrontEnd = &FrontEnd::UCodeFrontEndObject::MakeObject;
	BackEndObject_Ptr _BackEnd = &UCodeBackEndObject::MakeObject;
	void Optimize(IRBuilder& IR);
};
UCodeLangEnd
