#pragma once
#ifndef UCodeLangNoCompiler
#include "FrontEndObject.hpp"

#include "Lexer.hpp"
#include "Parser.hpp"
#include "SystematicAnalysis.hpp"
#include "UCodeFrontEndNameSpace.hpp"


UCodeLangFrontStart
class UCodeLangInfo
{
public:
	enum class FileTypes : LangDefInfo::FileID
	{
		SourceFile = LangDefInfo::DefaultTextFileID,
		Lib,
	};

	static LangDefInfo& GetLangInfo();
private:

	inline static bool Init = false;
	inline static LangDefInfo Info;
};

class UCodeFrontEndObject : FrontEndObject
{
public:
	UCodeFrontEndObject() : FrontEndObject()
	{

	}
	~UCodeFrontEndObject() override
	{

	};

	void Reset() override;
	void Set_FileIDType(LangDefInfo::FileID ID)  override { _FileType = (UCodeLangInfo::FileTypes)ID; }

	Unique_ptr<FileNode_t> BuildFile(String_view Text)  override;
	Unique_ptr<FileNode_t> BuildFile(const BytesView Bytes)  override{ return BuildFile(String_view((const char*)Bytes.Data(), Bytes.Size()) ); }


	Unique_ptr<FileNode_t> LoadIntFile(const BytesView Bytes, const Path& Ext) override;
	Unique_ptr<FileNode_t> LoadExternFile(const BytesView Bytes, const Path& Ext) override;
	Vector<NeverNullPtr<FileNode_t>> Get_DependenciesPostIR(FileNode_t* File);

	void ToIntFile(FileNode_t* File, const Path& path) override;

	void BuildIR(const Vector<FileNode_t*>& FileNode) override;

	IRBuilder* Get_Builder() override {return &_Analyzer.Get_Output();}

	const LangDefInfo* GetInfo() override { return  &UCodeLangInfo::GetLangInfo(); }

	UClib* Get_Lib() override {return &_Analyzer.Get_Lib(); }

	static FrontEndObject* MakeObject() { return new UCodeFrontEndObject(); }
private:
	Lexer _Lexer;
	Parser _Parser;
	SystematicAnalysis _Analyzer;
	UCodeLangInfo::FileTypes _FileType = UCodeLangInfo::FileTypes::SourceFile;
};

UCodeLangFrontEnd

#endif