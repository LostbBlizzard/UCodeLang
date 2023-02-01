#include "UCodeFrontEndObject.hpp"

UCodeLangStart


LangDefInfo& UCodeLangInfo::GetLangInfo()
{
	if (Init == false)
	{
		Info.LangName = "UCode";
		Info.FileTypes.push_back({ FileExt::SourceFileWithDot,FrontEndType::Text,(LangDefInfo::FileID)FileTypes::SourceFile });

		Info.FileTypes.push_back({ FileExt::LibWithDot,FrontEndType::Bytes,(LangDefInfo::FileID)FileTypes::Lib });
		Info.FileTypes.push_back({ FileExt::DllWithDot,FrontEndType::Bytes,(LangDefInfo::FileID)FileTypes::Lib});
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
	auto Err = Get_Errors();
	auto Sett = Get_Settings();

	_Lexer.Set_ErrorsOutput(Err);
	_Parser.Set_ErrorsOutput(Err);
	

	_Lexer.Set_Settings(Sett);
	_Parser.Set_Settings(Sett);
	
	//

	_Lexer.Lex(Text);
	
	if (Err->Has_Errors()) { return {}; }
	

	_Parser.Parse(_Lexer.Get_Tokens());

	if (Err->Has_Errors()) { return {}; }


	return Unique_ptr<FileNode_t>(new FileNode(std::move(_Parser.Get_Tree())));
}
void UCodeFrontEndObject::BuildIR(const Vector<Unique_ptr<FileNode_t>>& fileNode)
{
	auto Err = Get_Errors();
	auto Sett = Get_Settings(); 
	_Analyzer.Set_ErrorsOutput(Err); 
	_Analyzer.Set_Settings(Sett);

	//

	Vector<const FileNode*> V;
	V.resize(fileNode.size());
	for (size_t i = 0; i < fileNode.size(); i++)
	{
		V[i] = (const FileNode*)fileNode[i].get();
	}
	_Analyzer.Analyze(V, {});
}
UCodeLangEnd