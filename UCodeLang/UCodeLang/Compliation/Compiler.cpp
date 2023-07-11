#include "Compiler.hpp"
#include <fstream>
#include <filesystem>
#include "../LangCore/FileHelper.hpp"


#include "Front/UCodeFrontEndObject.hpp"
#include "Back/UCodeBackEnd/UCodeBackEnd.hpp"
UCodeLangStart
Compiler::CompilerRet Compiler::CompileText(const String_view& Text)
{
	//
	CompilerRet CompilerRet;
	CompilerRet._State = CompilerState::Fail;
	CompilerRet.OutPut = nullptr;

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



	if (Item == nullptr || _Errors.Has_Errors()) { return CompilerRet; }


	Vector<FileNode_t*> Files;
	Files.push_back(Item.get());
	_FrontEndObject->BuildIR(Files);

	if (_BackEndObject == nullptr 
		|| (_oldFrontEnd != _FrontEnd)
		|| (_oldBackEnd != _BackEnd))
	{
		_BackEndObject.reset(_BackEnd());
		_oldFrontEnd = _FrontEnd;
		_oldBackEnd = _BackEnd;
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
	
	if (_Errors.Has_Errors()) { return CompilerRet; }

	_BackEndObject->Build(&IRCode);

	if (_Errors.Has_Errors()) { return CompilerRet; }

	CompilerRet._State = _Errors.Has_Errors() ? CompilerState::Fail : CompilerState::Success;

	
	CompilerRet.OutPut = &_BackEndObject->Getliboutput();
	return CompilerRet;
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
	if (_FrontEndObject == nullptr)
	{
		_FrontEndObject.reset(_FrontEnd());
	}
	else
	{
		_FrontEndObject->Reset();
	}

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

	if (_FrontEndObject == nullptr
		|| (_oldFrontEnd != _FrontEnd)
		|| (_oldBackEnd != _BackEnd))
	{
		_FrontEndObject.reset(_FrontEnd());
		_oldFrontEnd = _FrontEnd;
		_oldBackEnd = _BackEnd;
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

				if (!_Errors.Has_Errors()) {

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
	}

	
	r._State = _Errors.Has_Errors() ? CompilerState::Fail : CompilerState::Success;
	return r;
}

Compiler::CompilerRet Compiler::CompileFiles_UseIntDir(const CompilerPathData& Data, const ExternalFiles& ExternalFiles)
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
		Path Repath;
		BytesPtr OpenedFile;
		MyEnumClass Type= MyEnumClass::FileNotChanged;

		bool IsExternal = false;

		DependencyFile::FileInfo* FileInfo = nullptr;
		DependencyFile::ExternalFileInfo* ExternalFileInfo = nullptr;
		
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

	if (_FrontEndObject == nullptr
		|| (_oldFrontEnd != _FrontEnd)
		|| (_oldBackEnd != _BackEnd))
	{
		_FrontEndObject.reset(_FrontEnd());
		_oldFrontEnd = _FrontEnd;
		_oldBackEnd = _BackEnd;

	}
	else
	{
		_FrontEndObject->Reset();
	}
	const LangDefInfo* Lang = _FrontEndObject->GetInfo();
	_FrontEndObject->Set_Settings(&_Settings);
	_FrontEndObject->Set_ErrorsOutput(&_Errors);

	Vector<Unique_ptr<DependencyFile::FileInfo>> NewFilesInfo;
	Vector<MyStruct> FilesInfo;

	Vector<Unique_ptr<DependencyFile::ExternalFileInfo>> NewExternalInfo;
	//ExternalFiles
	{
		for (auto& Item : NewFile.ExternalFiles)
		{
			bool IsRemoved = false;

			if (!fs::exists(Item.FullFilePath))
			{
				IsRemoved = true;
			}
			else
			{
				bool HasFile = false;
				for (auto& Item2 : ExternalFiles.Files)
				{
					if (Item2 == Item.FullFilePath)
					{
						HasFile = true;
						break;
					}
				}

				IsRemoved = !HasFile;
			}



			if (IsRemoved)
			{
				MyStruct T;
				T.Type = MyEnumClass::RemovedFile;

				FilesInfo.push_back(std::move(T));
				//removed file
			}
		}

		for (auto& Item2 : ExternalFiles.Files)
		{
			Path path = Item2;
			

			const LangDefInfo::FileInfo* FInfo = nullptr;
			for (auto& Item : Lang->FileTypes)
			{
				if (path.extension() == Item.FileExtWithDot)
				{
					FInfo = &Item;
					break;
				}
			}
			if (FInfo == nullptr) { continue; }


			MyStruct T;
			T.path = path;
			DependencyFile::ExternalFileInfo F;
			F.FullFilePath = path;
			F.FileLastUpdated = *(UInt64*)&fs::last_write_time(path);
			F.FileSize = fs::file_size(path);
			F.FileHash = 0;

			auto FileInfo = File.Get_ExternalFile_Info(path);
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
					BytesPtr V = OpenFile(FInfo,path);
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

				T.ExternalFileInfo= FileInfo;
				T.IsExternal = true;
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
				auto V = std::make_unique<DependencyFile::ExternalFileInfo>();
				FileInfo = V.get();
				NewExternalInfo.push_back(std::move(V));

				*FileInfo = F;

				T.Type = MyEnumClass::NewFile;
				T.ExternalFileInfo = FileInfo;
				T.IsExternal = true;
			}
			FilesInfo.push_back(std::move(T));
		}

	}
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
			T.path = FilePath_t;
			T.Repath = Path(RePath);
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
					BytesPtr V = OpenFile(FInfo, FilePath_t);
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
				auto V = std::make_unique<DependencyFile::FileInfo>();
				FileInfo = V.get();
				NewFilesInfo.push_back(std::move(V));
				
				*FileInfo = F;

				T.Type = MyEnumClass::NewFile;
				T.FileInfo = FileInfo;
			
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
			OldSize = ChangedFiles.size();
			//if dependence is an updated file
			for (auto& Item : FilesInfo)
			{
				if (Item.Type == MyEnumClass::UpdatedFile)
				{
					bool IsDependence = false;

					for (auto& Item2 : ChangedFiles)
					{
						
						if (Item.FileInfo) //Is not External File
						{
							if (Item.IsExternal ?
								Item.FileInfo->HasExternDependence(Item2->path)
								:
								Item.FileInfo->HasDependence(Item2->Repath))
							{
								IsDependence = true;
								break;
							}
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

						if (!HasIt)
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

	if (ChangedFiles.size())
	{
		for (auto& Item : UnChangedFiles)
		{
			if (Item->IsExternal)
			{
				Item->_IntFile = _FrontEndObject->LoadExternFile(Item->path);
				Files.push_back(Item->_IntFile.get());
			}
			else
			{
				Item->_IntFile = _FrontEndObject->LoadIntFile(Item->InterPath);
				Files.push_back(Item->_IntFile.get());
			}
		}
		

		for (auto& Item : ChangedFiles)
		{
			if (Item->IsExternal) 
			{
				if (!Item->OpenedFile.Bytes)
				{
					Item->_IntFile = _FrontEndObject->LoadExternFile(Item->path);
				}
				else
				{
					Item->_IntFile = _FrontEndObject->LoadExternFile(Item->path.extension());
				}

				Files.push_back(Item->_IntFile.get());
				continue;
			}
			if (!Item->OpenedFile.Bytes)
			{
				Item->OpenedFile = OpenFile(Item->_FInfo, Item->path);
			}

			_Errors.FilePath = Item->Repath;
			_FrontEndObject->SetSourcePath(Item->path);

			Item->_File = _FrontEndObject->BuildFile(Item->OpenedFile.AsView());
			if (Item->_File) 
			{
				Item->_File->FileName = Item->Repath;


				auto V = _FrontEndObject->Get_DependenciesPreIR(Item->_File.get());
				if (V.CanGetDependencies == false)
				{
					CanFindDependencyBeforIR = false;
				}
				else
				{
					Item->FileInfo->Dependencies = std::move(V._Files);
				}


				Files.push_back(Item->_File.get());
			}
		}

	}



	CompilerRet r;
	r._State = CompilerState::Fail;
	r.OutPut = nullptr;
	if (!_Errors.Has_Errors())
	{

		if (CanFindDependencyBeforIR) 
		{

			//Not added yet.



		}
		//else
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
					if (!_Errors.Has_Errors()) {
						_BackEndObject->Set_OutputLib(_FrontEndObject->Get_Lib());
						_BackEndObject->Build(output);

						auto Output = _BackEndObject->GetOutput();
						if (!_BackEndObject->HasLibOutput())
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
							if (Item->IsExternal) { continue; }


							{
								Path InterDirPath = Item->InterPath.parent_path();
								if (!fs::exists(InterDirPath))
								{
									std::filesystem::create_directories(InterDirPath);
								}
							}

							_FrontEndObject->ToIntFile(Item->_File.get(), Item->InterPath);
							auto FileDeps = _FrontEndObject->Get_DependenciesPostIR(Item->_File.get());

							auto Str = String_view((char*)Item->OpenedFile.Bytes.get(), Item->OpenedFile.Size);
							Item->FileInfo->FileHash = (UInt64)std::hash<String_view>()(Str);
							Item->FileInfo->Dependencies.clear();
							Item->FileInfo->ExternDependencies.clear();
							for (auto& Item2 : FileDeps)
							{

								bool IsExtern = false;
								for (auto& Item3 : NewExternalInfo)
								{
									if (Item3->FullFilePath == Item2->FileName)
									{
										IsExtern = true;
									}
								}

								if (IsExtern)
								{
									Item->FileInfo->ExternDependencies.push_back(Item2->FileName);
								}
								else
								{
									Item->FileInfo->Dependencies.push_back(Item2->FileName);
								}

							}
						}
					}
				}
			}
		}
	}


	r._State = _Errors.Has_Errors() ? CompilerState::Fail : CompilerState::Success;

	for (auto& Item : NewFilesInfo)
	{
		NewFile.Files.push_back(std::move(*Item));
	}
	for (auto& Item : UnChangedFiles)
	{
		NewFile.Files.push_back(std::move(*Item->FileInfo));
	}
	for (auto& Item : NewExternalInfo)
	{
		NewFile.ExternalFiles.push_back(std::move(*Item));
	}

	if (r._State == CompilerState::Success) {
		DependencyFile::ToFile(&NewFile, DependencyPath);
	}

	return  r;

}


BytesPtr Compiler::OpenFile(const LangDefInfo::FileInfo* FInfo, const Path& path)
{
	if (FInfo->Type != FrontEndType::Text)
	{
		return GetBytesFromFile(path);
	}
	else
	{
		String tep = GetTextFromFile(path);
		BytesPtr V;
		V.Bytes.reset(new Byte[tep.size()]);
		V.Size = tep.size();
		memcpy(V.Bytes.get(), tep.data(), V.Size);
		return V;
	}
}

Compiler::Compiler()
{
	_FrontEnd = &FrontEnd::UCodeFrontEndObject::MakeObject;
	_BackEnd = &UCodeBackEndObject::MakeObject;
}

void Compiler::Optimize(IRBuilder& IR)
{
	_Optimizer.Reset();
	_Optimizer.Set_Settings(&_Settings);
	_Optimizer.Set_ErrorsOutput(&_Errors);
	_Optimizer.Optimized(IR);
}

UCodeLangEnd