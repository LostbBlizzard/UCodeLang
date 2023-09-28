#pragma once
#include "UCodeLang/Compliation/Helpers/CompilerTypes.hpp"
UCodeLangStart

class CompliationErrors;
class IRBuilder;
class UClib;
struct CompliationSettings;

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
		FileID FileId;
	};
	
	
	

	String LangName;
	Vector<FileInfo> FileTypes;
};


class FileNode_t
{
public:
	Path FileName;
	virtual ~FileNode_t() {}
};


class FrontEndObject
{
public:
	virtual void Reset() {}
	virtual ~FrontEndObject() {}

	virtual void Set_FileIDType(LangDefInfo::FileID ID){}

	virtual Unique_ptr<FileNode_t> BuildFile(String_view Text) { return nullptr; }
	virtual Unique_ptr<FileNode_t> BuildFile(const BytesView Bytes) { return nullptr; }

	virtual void SetSourcePath(const Path& filepath) {};
	struct Gep_DepPreIR
	{
		bool CanGetDependencies = false;
		Vector<Path> _Files;
	};

	virtual Gep_DepPreIR Get_DependenciesPreIR(FileNode_t* File) { return {}; }
	virtual Vector<NeverNullPtr<FileNode_t>> Get_DependenciesPostIR(FileNode_t* File) { return {}; }
	
	//
	Unique_ptr<FileNode_t> LoadExternFile(const Path& path);
	virtual Unique_ptr<FileNode_t> LoadExternFile(const BytesView Bytes, const Path& Ext){return nullptr;}

	Unique_ptr<FileNode_t> LoadIntFile(const Path& path);
	virtual Unique_ptr<FileNode_t> LoadIntFile(const BytesView Bytes, const Path& Ext) { return nullptr; }
	

	virtual void ToIntFile(FileNode_t* File,const Path& path) { }

	void BuildIR(const Vector<Unique_ptr<FileNode_t>>& FileNode)
	{
		Vector<FileNode_t*>& _Files = *(Vector<FileNode_t*>*) &FileNode;
		return  BuildIR(_Files);
	}

	virtual void BuildIR(const Vector<FileNode_t*>& FileNode) {}




	virtual const LangDefInfo* GetInfo() { return nullptr; }
	
	virtual IRBuilder* Get_Builder(){ return nullptr; }
	
	
	//used if we can get the Dependencies befor IR may be used in 
	// void BuildIR(const Vector<Unique_ptr<FileNode_t>>& FileNode)
	// called after Get_Builder() 
	virtual Unique_ptr<FileNode_t> ConvertOutputAsFile() { return nullptr; }

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