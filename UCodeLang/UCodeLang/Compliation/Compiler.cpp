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


	Vector<FileNode_t*> Files;
	Files.push_back(Item.get());
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

	_FrontEndObject->SetSourcePath(path);
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
				break;
			}
		}
		if (FInfo == nullptr) { continue; }

		const Path FilePath = dirEntry.path();
		Path RePath = FileHelper::RelativePath(FilePath, Data.FileDir);

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

		_FrontEndObject->SetSourcePath(FilePath);
		if (FInfo->Type == FrontEndType::Text) 
		{
			auto V = std::make_unique<String>(GetTextFromFile(dirEntry.path()));

			
			auto Filenode = _FrontEndObject->BuildFile(String_view(*V));

			FilesStrings.push_back(std::move(V));
			if (Filenode) 
			{
				Filenode->FileName = RePath;
				Files.push_back(std::move(Filenode)); 
			}
		}
		else
		{
			auto V = GetBytesFromFile(dirEntry.path());
			auto Filenode = _FrontEndObject->BuildFile(BytesView(V.Bytes.get(),V.Size));

			FilesBytes.push_back(std::move(V));
			if (Filenode) 
			{ 
				Filenode->FileName = RePath; 
				Files.push_back(std::move(Filenode)); 
			}
		}
	}

	
	CompilerRet r;
	r._State = CompilerState::Fail;
	r.OutPut = nullptr;
	if (!_Errors.Has_Errors())
	{
		Vector<FileNode_t*>& _Files = *(Vector<FileNode_t*>*)&Files;
		_FrontEndObject->BuildIR(_Files);
	

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
	enum class MyEnumClass :UInt8
	{
		FileNotChanged,
		NewFile,
		RemovedFile,
		UpdatedFile,
	};
	struct MyStruct
	{
		Path path;
		BytesPtr OpenedFile;
		MyEnumClass Type;
		DependencyFile::FileInfo* FileInfo = nullptr;

		Path InterPath;

		Unique_ptr<FileNode_t> _File;
		Unique_ptr<FileNode_t> _IntFile;
		MyStruct()
		{

		}
		const LangDefInfo::FileInfo* _FInfo=nullptr;
	};
	struct MyStructTreeNode
	{
		MyStruct* _This;
		Vector<MyStructTreeNode*> _MustBeWith;//must be compiled with.
		Vector<MyStructTreeNode*> _Dependencies;//must be compiled first.
	};

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


	Vector<MyStruct> FilesInfo;

	//get file info
	{
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
				MyStruct T;
				T.Type = MyEnumClass::RemovedFile;

				FilesInfo.push_back(std::move(T));
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
					break;
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

			const Path IntermediatePath = Path(Data.IntDir).native() + Path(RePath).native() + Path(FileExt::ObjectWithDot).native();

			MyStruct T;
			T.path = Path(RePath);
			T.InterPath = IntermediatePath;
			T._FInfo = FInfo;
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
				else if (!fs::exists(IntermediatePath))
				{
					NeedToBeRecomiled = true;
				}
				else
				{
					BytesPtr V = OpenFile(FInfo, dirEntry.path());
					String_view Bits = String_view((const char*)V.Bytes.get(), V.Size);

					auto hasher = std::hash<String_view>();
					UInt64 BitsHash = hasher(Bits);


					if (BitsHash != F.FileHash)
					{
						NeedToBeRecomiled = true;
						F.FileHash = BitsHash;
					}

					T.OpenedFile = std::move(V);
				}

				T.FileInfo = FileInfo;
				if (NeedToBeRecomiled)
				{
					T.Type = MyEnumClass::UpdatedFile;
				}
				else
				{
					T.Type = MyEnumClass::FileNotChanged;
				}
			}
			else
			{
				T.Type = MyEnumClass::NewFile;
			}
			FilesInfo.push_back(std::move(T));
		}
	}

	Vector<MyStruct*> UnChangedFiles;
	Vector<MyStruct*> ChangedFiles;

	//spit Changed and unchangedfiles
	{


		for (auto& Item : FilesInfo)
		{
			if (Item.Type == MyEnumClass::RemovedFile
				|| Item.Type == MyEnumClass::NewFile
				|| Item.Type == MyEnumClass::UpdatedFile)
			{
				ChangedFiles.push_back(&Item);
			}

		}


		size_t OldSize = ChangedFiles.size();

		do
		{
			//if dependence is an updated file
			for (auto& Item : FilesInfo)
			{
				if (Item.Type == MyEnumClass::UpdatedFile)
				{
					bool IsDependence = false;

					for (auto& Item2 : ChangedFiles)
					{
						if (Item.FileInfo->HasDependence(Item2->path))
						{
							IsDependence = true;
							break;
						}
					}

					if (IsDependence)
					{
						bool HasIt = false;

						for (auto& Item2 : ChangedFiles)
						{
							if (Item2 == &Item)
							{
								HasIt = true;
								break;
							}
						}

						if (HasIt)
						{
							ChangedFiles.push_back(&Item);
						}
					}
				}
			}

		} while (OldSize != ChangedFiles.size());



		for (auto& Item : FilesInfo)
		{
			bool InList = false;
			for (auto& Item2 : ChangedFiles)
			{
				if (Item2 == &Item)
				{
					InList = true;
					break;
				}
			}

			if (!InList)
			{
				UnChangedFiles.push_back(&Item);
			}
		}


	}//spit Changed and unchangedfiles


	Vector<FileNode_t*> Files;
	bool CanFindDependencyBeforIR = true;
	{
		for (auto& Item : UnChangedFiles)
		{
			Item->_IntFile = _FrontEndObject->LoadIntFile(Item->InterPath);

			Files.push_back(Item->_IntFile.get());
		}
		for (auto& Item : ChangedFiles)
		{
			if (!Item->OpenedFile.Bytes)
			{
				Item->OpenedFile = OpenFile(Item->_FInfo, Item->path);
			}

			_FrontEndObject->SetSourcePath(Item->path);

			Item->_File = _FrontEndObject->BuildFile(Item->OpenedFile.AsView());

			auto V = _FrontEndObject->Get_DependenciesPreIR(Item->_File.get());
			if (V.CanGetDependencies == false)
			{
				CanFindDependencyBeforIR = false;
			}
			else
			{
				Item->FileInfo->Dependencies =std::move(V._Files);
			}
				

			Files.push_back(Item->_File.get());
		}
	}



	CompilerRet r;
	r._State = CompilerState::Fail;
	r.OutPut = nullptr;
	if (!_Errors.Has_Errors())
	{

		if (!CanFindDependencyBeforIR) 
		{





		}
		else
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


					//spit file the get inters
					for (auto& Item : ChangedFiles)
					{
						_FrontEndObject->ToIntFile(Item->_File.get(),Item->InterPath);
					}
				}
			}
		}
	}


	DependencyFile::ToFile(&NewFile, DependencyPath);


	return  r;

}

BytesPtr Compiler::OpenFile(const LangDefInfo::FileInfo* FInfo, const Path& path)
{
	if (FInfo->Type == FrontEndType::Text)
	{
		return GetBytesFromFile(path);
	}
	else
	{
		String tep = GetTextFromFile(path);
		BytesPtr V;
		V.Bytes.reset((Byte*)tep.data());
		V.Size = tep.size();
		return V;
	}
}

void Compiler::Optimize(IRBuilder& IR)
{
	_Optimizer.Reset();
	_Optimizer.Set_Settings(&_Settings);
	_Optimizer.Set_ErrorsOutput(&_Errors);
	_Optimizer.Optimized(IR);
}

UCodeLangEnd