#pragma once
#include "UCodeLang/LangCore/UClib.hpp"
#include "UCodeLang/Compliation/Helpers/CompilerTypes.hpp"
#include "UCodeLang/Compliation/Helpers/CompliationErrors.hpp"
#include "UCodeLang/Compliation/CompliationSettings.hpp"
#include "UCodeLang/Compliation/Middle/IR.hpp"
UCodeLangStart


enum class FrontEndType : UInt8
{
	Text,
	Bytes,
};

class LangDefInfo
{
public:
	using FileID = size_t;//Enum

	static constexpr FileID NullFileID = -1;
	static constexpr FileID DefaultTextFileID = 0;
	struct FileInfo
	{
		String FileExtWithDot;
		FrontEndType Type;
		FileID FileID;
	};
	
	
	

	String LangName;
	Vector<FileInfo> FileTypes;
};


class FileNode_t
{
public:
	Path FileName;
	Vector<FileNode_t*> Dependencies;
	virtual ~FileNode_t() {}
};


class FrontEndObject
{
public:
	virtual void Reset() {}


	virtual void Set_FileIDType(LangDefInfo::FileID ID){}

	virtual Unique_ptr<FileNode_t> BuildFile(String_view Text) { return nullptr; }
	virtual Unique_ptr<FileNode_t> BuildFile(const BytesView Bytes) { return nullptr; }


	virtual Vector<FileNode_t*> Get_Dependencies(FileNode_t* File) { return {}; }

	virtual void BuildIR(const Vector<Unique_ptr<FileNode_t>>& FileNode) {}


	virtual ~FrontEndObject() {}

	virtual const LangDefInfo* GetInfo() { return nullptr; }
	virtual IRBuilder* Get_Builder(){ return nullptr; }
	virtual UClib* Get_Lib() { return nullptr; }

	void Set_ErrorsOutput(CompliationErrors* Err)
	{
		_ErrorsOutput = Err;
	}
	void Set_Settings(CompliationSettings* Err)
	{
		_Settings = Err;
	}
private:
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
protected:
	CompliationErrors* Get_Errors()
	{
		return _ErrorsOutput;
	}
	CompliationSettings* Get_Settings()
	{
		return _Settings;
	}
};

using FrontEndObject_Ptr = FrontEndObject * (*)();
UCodeLangEnd