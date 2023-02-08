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


		_Parser.Parse(_Lexer.Get_Tokens());

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
			FileNode* tepn = (FileNode*)&tep;
			return Unique_ptr<FileNode_t>(new FileNode(std::move(*tepn)));
		}
	}
	return nullptr;
}
void UCodeFrontEndObject::BuildIR(const Vector<Unique_ptr<FileNode_t>>& fileNode)
{
	auto Err = Get_Errors();
	auto Sett = Get_Settings(); 
	_Analyzer.Set_ErrorsOutput(Err); 
	_Analyzer.Set_Settings(Sett);

	//

	Vector<const FileNode*> V;
	Vector<const UClib*> L;
	for (size_t i = 0; i < fileNode.size(); i++)
	{
		auto Item = (const FileNode*)fileNode[i].get();

		if (Item->Get_Type() == NodeType::LibImportNode)
		{
			auto N = (const LibImportNode*)Item;
			L.push_back(&N->LIb);
		}
		else
		{
			V.push_back(Item);
		}
	}
	_Analyzer.Analyze(V, L);
}
UCodeLangEnd