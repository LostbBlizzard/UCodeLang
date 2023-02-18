#include "Compiler.hpp"
#include <fstream>
#include <filesystem>
#include "../LangCore/FileHelper.hpp"
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

	auto Item = _FrontEndObject->BuildFile(Text);



	if (Item == nullptr || _Errors.Has_Errors()) { return R; }


	Vector < Unique_ptr<FileNode_t>> Files;
	Files.push_back(std::move(Item));
	_FrontEndObject->BuildIR(Files);

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

	
	_BackEndObject->Set_OutputLib(_FrontEndObject->Get_Lib());

	auto& IRCode = *_FrontEndObject->Get_Builder();
	Optimize(IRCode);
	
	_BackEndObject->Build(&IRCode);

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
	std::ifstream File(path, std::ios::binary);
	if (File.is_open())
	{
		File.seekg(0, std::ios::end);
		r.Size = File.tellg();
		File.seekg(0, std::ios::beg);
		
		Byte* Ptr = new Byte[r.Size];
		File.read((char*)Ptr, r.Size);
		r.Bytes.reset(Ptr);

		File.close();
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
			auto Filenode = _FrontEndObject->BuildFile(String_view(*V));

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

				Optimize(*output);

				_BackEndObject->Set_OutputLib(_FrontEndObject->Get_Lib());
				_BackEndObject->Build(output);

				auto Output = _BackEndObject->GetOutput();
				if (Output.Size)
				{
					std::ofstream File(Data.OutFile, std::ios::binary);
					if (File.is_open())
					{
						File.write((const char*)Output.Bytes, Output.Size);
						File.close();
					}
				}
				else
				{
					r.OutPut = &_BackEndObject->Getliboutput();
					UClib::ToFile(r.OutPut, Data.OutFile);
				}
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
	if (fs::exists(DependencyPath))
	{
		if (!DependencyFile::FromFile(&File, DependencyPath))
		{
			File = DependencyFile();
		}
	}
	DependencyFile NewFile;


	const LangDefInfo* Lang = _FrontEndObject->GetInfo();
	_FrontEndObject->Set_Settings(&_Settings);
	_FrontEndObject->Set_ErrorsOutput(&_Errors);


	Vector<MyStruct> FilesInfo;

	//check for removed.
	for (auto& Item : NewFile.Files)
	{
		bool IsRemoved = false;

		Path FileP = Path(Data.FileDir).native() + Item.FilePath.native();
		if (!fs::exists(FileP))
		{
			IsRemoved = true;
		}



		if (IsRemoved)
		{
			//removed file
		}
	}
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

		auto& FilePath_t = dirEntry.path();
		String FilePath = FilePath_t.generic_u8string();

		String RePath = FileHelper::RelativePath(FilePath, Data.FileDir);

		_Errors.FilePath = RePath;
		DependencyFile::FileInfo F;
		F.FilePath = RePath;	
		F.FileLastUpdated = *(UInt64*)&fs::last_write_time(FilePath_t);
		F.FileSize = fs::file_size(FilePath_t);
		F.FileHash = 0;
	
	

		auto FileInfo = File.Get_Info(RePath);
		if (FileInfo)
		{
			bool NeedToBeRecomiled = false;

			if (FileInfo->FileLastUpdated != F.FileLastUpdated)
			{
				NeedToBeRecomiled = true;
			}
			else if (FileInfo->FileSize != F.FileSize)
			{
				NeedToBeRecomiled = true;
			}
			else
			{
				auto V = GetBytesFromFile(dirEntry.path());
				String_view Bits = String_view((const char*)V.Bytes.get(),V.Size);
			
				auto hasher = std::hash<String_view>();
				UInt64 BitsHash = hasher(Bits);
				F.FileHash = BitsHash;
				
				if (BitsHash == F.FileHash)
				{
					NeedToBeRecomiled = true;
				}
			}
			if (NeedToBeRecomiled)
			{
				//DO Cool Stuff
			}
 		}
		else
		{
			//new file
		}
	}



	DependencyFile::ToFile(&NewFile, DependencyPath);


	CompilerRet r;
	r._State = CompilerState::Fail;
	r.OutPut = nullptr;
	return  r;
}

void Compiler::Optimize(IRBuilder& IR)
{
	_Optimizer.Reset();
	_Optimizer.Set_Settings(&_Settings);
	_Optimizer.Set_ErrorsOutput(&_Errors);
	_Optimizer.Optimized(IR);
}

UCodeLangEnd