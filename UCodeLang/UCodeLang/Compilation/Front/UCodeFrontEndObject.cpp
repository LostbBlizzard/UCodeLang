#ifndef UCodeLangNoCompiler
#include "UCodeFrontEndObject.hpp"
#include "UCodeLang/Compilation/Compiler.hpp"
UCodeLangFrontStart


LangDefInfo& UCodeLangInfo::GetLangInfo()
{
	if (Init == false)
	{
		Init = true;
		Info.LangName = "UCode";
		Info.FileTypes.push_back({ FileExt::SourceFileWithDot,FrontEndType::Text,(LangDefInfo::FileID)FileTypes::SourceFile });

		Info.FileTypes.push_back({ FileExt::LibWithDot,FrontEndType::Bytes,(LangDefInfo::FileID)FileTypes::Lib });
		Info.FileTypes.push_back({ FileExt::DllWithDot,FrontEndType::Bytes,(LangDefInfo::FileID)FileTypes::Lib });
	}

	return Info;
}


void UCodeFrontEndObject::Reset()
{
	_Lexer.Reset();
	_Parser.Reset();
	_Analyzer.Reset();
}

Unique_ptr<FileNode_t> UCodeFrontEndObject::BuildFile(String_view Text)
{

	if (_FileType == UCodeLangInfo::FileTypes::SourceFile) {
		auto Err = Get_Errors();
		auto Sett = Get_Settings();


		_Lexer.Set_ErrorsOutput(Err);
		_Parser.Set_ErrorsOutput(Err);


		_Lexer.Set_Settings(Sett);
		_Parser.Set_Settings(Sett);

		//

		_Lexer.Lex(Text);

		if (Err->Has_Errors()) { return {}; }


		_Parser.Parse(Text, _Lexer.Get_Tokens());

		if (Err->Has_Errors()) { return {}; }

		auto V = new FileNode(std::move(_Parser.Get_Tree()));
		V->Tokens = std::move(_Lexer.Get_Tokens());
		return Unique_ptr<FileNode_t>(V);
	}
	else
	{
		LibImportNode tep;
		BytesView Bits((Byte*)Text.data(), Text.size());
		if (UClib::FromBytes(&tep.LIb, Bits))
		{
			return Unique_ptr<FileNode_t>(new LibImportNode(std::move(tep)));
		}
	}
	return nullptr;
}
Unique_ptr<FileNode_t> UCodeFrontEndObject::LoadIntFile(const BytesView Bytes, const Path& Ext)
{
	LibImportNode tep;
	tep.Mode = ImportMode::IntermediateFile;
	if (UClib::FromBytes(&tep.LIb, Bytes))
	{
		return Unique_ptr<FileNode_t>(new LibImportNode(std::move(tep)));
	}

	return nullptr;
}

Unique_ptr<FileNode_t> UCodeFrontEndObject::LoadExternFile(const BytesView Bytes, const Path& Ext)
{

	LibImportNode tep;
	tep.Mode = ImportMode::ExternalFile;
	if (UClib::FromBytes(&tep.LIb, Bytes))
	{
		return Unique_ptr<FileNode_t>(new LibImportNode(std::move(tep)));
	}
	return {};
}
Vector<NeverNullPtr<FileNode_t>> UCodeFrontEndObject::Get_DependenciesPostIR(FileNode_t* File)
{
	return  _Analyzer.GetFileDataPub(File)._Dependencys;
}
void UCodeFrontEndObject::ToIntFile(FileNode_t* File, const Path& path)
{
	_Analyzer.ToIntFile(File, path);
}

void UCodeFrontEndObject::BuildIR(const Vector<FileNode_t*>& fileNode)
{
	auto Err = Get_Errors();
	auto Sett = Get_Settings();
	_Analyzer.Set_ErrorsOutput(Err);
	_Analyzer.Set_Settings(Sett);

	//

	Vector<NeverNullPtr<FileNode>> V;
	Vector<NeverNullPtr<UClib>> L;
	Vector<Path> P;
	for (size_t i = 0; i < fileNode.size(); i++)
	{
		auto Item = (const FileNode*)fileNode[i];

		if (Item->Get_Type() == NodeType::LibImportNode)
		{
			auto N = (const LibImportNode*)Item;
			L.push_back(NeverNullptr(&N->LIb));
			P.push_back(N->FileName);
		}
		else
		{
			V.push_back(NeverNullptr(Item));
		}
	}
	_Analyzer.SetLibNames(&P);
	_Analyzer.Analyze(V, L);
}
UCodeLangFrontEnd

#endif