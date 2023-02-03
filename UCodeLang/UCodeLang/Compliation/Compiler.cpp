#include "Compiler.hpp"
#include <fstream>
#include <filesystem>
#include "../LangCore/FileHelper.hpp"
#include "DependencyFile.hpp"
UCodeLangStart
Compiler::CompilerRet Compiler::CompileText(const String_view& Text)
{
	//
	CompilerRet R;
	R._State = CompilerState::Fail;
	R.OutPut = nullptr;

	_Errors.FilePath = _Errors.FilePath;

	if (_FrontEndObject == nullptr)
	{
		_FrontEndObject.reset(_FrontEnd());
	}
	else
	{
		_FrontEndObject->Reset();
	}
	_FrontEndObject->Set_FileIDType(LangDefInfo::DefaultTextFileID);

	_FrontEndObject->Set_Settings(&_Settings);
	_FrontEndObject->Set_ErrorsOutput(&_Errors);

	if (_Errors.Has_Errors()) { return R; }


	if (_BackEndObject == nullptr) 
	{
		_BackEndObject.reset(_BackEnd());
	}
	else
	{
		_BackEndObject->Reset();
	}

	_BackEndObject->Set_Settings(&_Settings);
	_BackEndObject->Set_ErrorsOutput(&_Errors);


	_BackEndObject->Build(_FrontEndObject->Get_Builder());


	if (_Errors.Has_Errors()) { return R; }

	R._State = _Errors.Has_Errors() ? CompilerState::Fail : CompilerState::Success;
	R.OutPut = &_BackEndObject->Getliboutput();
	return R;
}
String Compiler::GetTextFromFile(const Path& path)
{
	std::ifstream File(path);
	if (File.is_open())
	{
		std::string Text;
		std::string line;
		while (std::getline(File, line)){Text += line + '\n';}
		File.close();

		return Text;
	}
	else
	{
		return "*null";
	}
}
BytesPtr Compiler::GetBytesFromFile(const Path& path)
{
	BytesPtr r;
	std::ifstream File(path);
	if (File.is_open())
	{
		throw std::exception("not added");
	}
	
	return r;
}
Compiler::CompilerRet Compiler::CompilePathToObj(const Path& path, const Path& OutLib)
{
	auto Text = GetTextFromFile(path);
	CompilerRet r = CompileText(Text);

	if (r._State == CompilerState::Success) {
		UClib::ToFile(r.OutPut, OutLib);
	}

	return r;
}
namespace fs = std::filesystem;
Compiler::CompilerRet Compiler::CompileFiles(const CompilerPathData& Data)
{


	//TODO check if File Int  Dir is in Dir
	Vector<Unique_ptr<String>> FilesStrings;
	Vector<BytesPtr> FilesBytes;
	Vector<Unique_ptr<FileNode_t>> Files;
	Vector<Unique_ptr<UClib>> Libs;
	//Refs

	if (_FrontEndObject == nullptr)
	{
		_FrontEndObject.reset(_FrontEnd());
	}
	else
	{
		_FrontEndObject->Reset();
	}

	const LangDefInfo* Lang = _FrontEndObject->GetInfo();
	_FrontEndObject->Set_Settings(&_Settings);
	_FrontEndObject->Set_ErrorsOutput(&_Errors);



	for (const auto& dirEntry : fs::recursive_directory_iterator(Data.FileDir))
	{

		if (!dirEntry.is_regular_file()) { continue; }

		auto Ext = dirEntry.path().extension();

		const LangDefInfo::FileInfo* FInfo = nullptr;
		for (auto& Item : Lang->FileTypes)
		{
			if (Ext == Item.FileExtWithDot)
			{
				FInfo = &Item;
			}
		}
		if (FInfo == nullptr) { continue; }

		String FilePath = dirEntry.path().generic_u8string();

		String RePath = FileHelper::RelativePath(FilePath, Data.FileDir);

		_Errors.FilePath = RePath;

		if (_FrontEndObject == nullptr)
		{
			_FrontEndObject.reset(_FrontEnd());
		}
		else
		{
			_FrontEndObject->Reset();
		}
		_FrontEndObject->Set_FileIDType(FInfo->FileID);


		if (FInfo->Type == FrontEndType::Text) 
		{
			auto V = std::make_unique<String>(GetTextFromFile(dirEntry.path()));
			auto Filenode = _FrontEndObject->BuildFile((String_view)*V);

			FilesStrings.push_back(std::move(V));
			if (Filenode) { Files.push_back(std::move(Filenode)); }
		}
		else
		{
			auto V = GetBytesFromFile(dirEntry.path());
			auto Filenode = _FrontEndObject->BuildFile(BytesView(V.Bytes.get(),V.Size));

			FilesBytes.push_back(std::move(V));
			if (Filenode) { Files.push_back(std::move(Filenode)); }
		}
	}

	
	CompilerRet r;
	r._State = CompilerState::Fail;
	r.OutPut = nullptr;
	if (!_Errors.Has_Errors())
	{
		_FrontEndObject->BuildIR(Files);
	

		if (!_Errors.Has_Errors())
		{
			if (_BackEndObject == nullptr)
			{
				_BackEndObject.reset(_BackEnd());
			}
			else
			{
				_BackEndObject->Reset();
			}
			_BackEndObject->Set_Settings(&_Settings);
			_BackEndObject->Set_ErrorsOutput(&_Errors);

			auto output = _FrontEndObject->Get_Builder();
			if (output) 
			{
				_BackEndObject->Set_Output(_FrontEndObject->Get_Lib());
				_BackEndObject->Build(output);

				r.OutPut = &_BackEndObject->Getliboutput();
				UClib::ToFile(r.OutPut, Data.OutFile);
			}
		}
	}

	
	r._State = _Errors.Has_Errors() ? CompilerState::Fail : CompilerState::Success;
	return r;
}

Compiler::CompilerRet Compiler::CompileFiles_UseIntDir(const CompilerPathData& Data)
{
	const Path DependencyPath = Data.IntDir + DependencyFile::FileName;

	DependencyFile File;

	CompilerRet r;
	r._State = CompilerState::Fail;
	r.OutPut = nullptr;
	return  r;
}

UCodeLangEnd