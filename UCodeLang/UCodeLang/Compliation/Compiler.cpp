#include "Compiler.hpp"
#include <fstream>
#include <filesystem>
#include "../LangCore/FileHelper.hpp"


#include "Front/UCodeFrontEndObject.hpp"
#include "Back/UCodeBackEnd/UCodeBackEnd.hpp"
UCodeLangStart
Compiler::CompilerRet Compiler::CompileText(const String_view& Text, const ExternalFiles& ExternalFiles)
{
	//
	CompilerRet CompilerRet;
	CompilerRet._State = CompilerState::Fail;
	CompilerRet.OutPut = nullptr;

	_Errors.FilePath = _Errors.FilePath;

	if (_FrontEndObject == nullptr
		|| (_oldFrontEnd != _FrontEnd))
	{
		_FrontEndObject.reset(_FrontEnd());
	}
	else
	{
		_FrontEndObject->Reset();
	}
	_Settings._BackEndInfo = GetOutputBackEndInfo();

	_FrontEndObject->Set_FileIDType(LangDefInfo::DefaultTextFileID);
	_FrontEndObject->Set_Settings(&_Settings);
	_FrontEndObject->Set_ErrorsOutput(&_Errors);
	_Errors.Remove_Errors();


	auto Item = _FrontEndObject->BuildFile(Text);



	if (Item == nullptr || _Errors.Has_Errors()) { return CompilerRet; }


	Vector<Unique_ptr<FileNode_t>> Files;
	Files.push_back(std::move(Item));
	for (auto& Item : ExternalFiles.Files) {
		Files.push_back(_FrontEndObject->LoadExternFile(Item));
	}

	_FrontEndObject->BuildIR(Files);

	if (_BackEndObject == nullptr 
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
	
	
	if (_BackEndObject->GetOutput().Size()) {
		auto bytes = _BackEndObject->GetOutput();
		CompilerRet.OutFile.Resize(bytes.Size());
		memcpy(CompilerRet.OutFile.Data(), bytes.Data(), bytes.Size());
	}

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
		r.Resize(File.tellg());
		File.seekg(0, std::ios::beg);
		
		File.read((char*)r.Data(), r.Size());

		File.close();
	}
	
	return r;
}
Compiler::CompilerRet Compiler::CompilePathToObj(const Path& path, const Path& OutLib, const ExternalFiles& ExternalFiles)
{
	if (_FrontEndObject == nullptr)
	{
		_FrontEndObject.reset(_FrontEnd());
	}
	else
	{
		_FrontEndObject->Reset();
	}
	_Settings._BackEndInfo = GetOutputBackEndInfo();

	_FrontEndObject->SetSourcePath(path);
	auto Text = GetTextFromFile(path);
	CompilerRet r = CompileText(Text,ExternalFiles);

	if (r._State == CompilerState::Success)
	{
		if (r.OutFile.Size())
		{
			std::ofstream File(OutLib, std::ios::binary);
			if (File.is_open())
			{
				File.write((const char*)r.OutFile.Data(), r.OutFile.Size());
				File.close();
			}
		}
		else 
		{
			UClib::ToFile(r.OutPut, OutLib);
		}
	}

	return r;
}
namespace fs = std::filesystem;
Compiler::CompilerRet Compiler::CompileFiles(const CompilerPathData& Data, const ExternalFiles& ExternalFiles)
{


	//TODO check if File Int  Dir is in Dir
	Vector<Unique_ptr<String>> FilesStrings;
	Vector<BytesPtr> FilesBytes;
	Vector<Unique_ptr<FileNode_t>> Files;

	
	//Refs

	if (_FrontEndObject == nullptr
		|| (_oldFrontEnd != _FrontEnd))
	{
		_FrontEndObject.reset(_FrontEnd());
		_oldFrontEnd = _FrontEnd;

	}
	else
	{
		_FrontEndObject->Reset();
	}
	_Settings._BackEndInfo = GetOutputBackEndInfo();

	const LangDefInfo* Lang = _FrontEndObject->GetInfo();
	_FrontEndObject->Set_Settings(&_Settings);
	_FrontEndObject->Set_ErrorsOutput(&_Errors);
	_Errors.Remove_Errors();


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
		Path RePath = FileHelper::RelativePath(FilePath,Path(Data.FileDir));

		_Errors.FilePath = RePath;

		if (_FrontEndObject == nullptr)
		{
			_FrontEndObject.reset(_FrontEnd());
		}
		else
		{
			_FrontEndObject->Reset();
		}
		_FrontEndObject->Set_FileIDType(FInfo->FileId);

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
			auto Filenode = _FrontEndObject->BuildFile(V.AsSpan());

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
	
		for (auto& Item : ExternalFiles.Files) {
			Files.push_back(_FrontEndObject->LoadExternFile(Item));
		}

		_FrontEndObject->BuildIR(Files);
	

		if (!_Errors.Has_Errors())
		{
			if (_BackEndObject == nullptr || _BackEnd != _oldBackEnd)
			{
				_BackEndObject.reset(_BackEnd());
				_oldBackEnd = _BackEnd;
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

					r.OutPut = &_BackEndObject->Getliboutput();
					if (Output.Size())
					{
						r.OutFile.Resize(Output.Size());
						memcpy(r.OutFile.Data(), Output.Data(), Output.Size());

						std::ofstream File(Data.OutFile, std::ios::binary);
						if (File.is_open())
						{
							File.write((const char*)Output.Data(), Output.Size());
							File.close();
						}
					}
					else
					{
						UClib::ToFile(r.OutPut, Data.OutFile);
					}
				}
			}
		}
	}

	
	r._State = _Errors.Has_Errors() ? CompilerState::Fail : CompilerState::Success;
	return r;
}

UInt64 GetFileHash(BytesView bytes)
{
	String_view Bits = String_view((const char*)bytes.Data(), bytes.Size());

	auto hasher = std::hash<String_view>();
	UInt64 BitsHash = hasher(Bits);

	return BitsHash;
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
		bool HasOpenedFile()
		{
			return OpenedFile.Size() != 0;
		}

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

	const Path DependencyPath = Data.IntDir.native() + Path(DependencyFile::FileName).native();

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
		|| (_oldFrontEnd != _FrontEnd))
	{
		_FrontEndObject.reset(_FrontEnd());
		_oldFrontEnd = _FrontEnd;
		_oldBackEnd = _BackEnd;
	}
	else
	{
		_FrontEndObject->Reset();
	}
	_Settings._BackEndInfo = GetOutputBackEndInfo();
	const LangDefInfo* Lang = _FrontEndObject->GetInfo();
	_FrontEndObject->Set_Settings(&_Settings);
	_FrontEndObject->Set_ErrorsOutput(&_Errors);
	_Errors.Remove_Errors();


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

			auto writetime = fs::last_write_time(path);
			F.FileLastUpdated = *(UInt64*)&writetime;
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
					UInt64 BitsHash = GetFileHash(V.AsSpan());
					
					if (FileInfo->FileHash != BitsHash)
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
			Path  FilePath = FilePath_t;

			Path RePath = FileHelper::RelativePath(FilePath, Data.FileDir);

			_Errors.FilePath = RePath;
			DependencyFile::FileInfo F;
			F.FilePath = RePath;

			auto writetime = fs::last_write_time(FilePath_t);
			F.FileLastUpdated = (UInt64)writetime.time_since_epoch().count();
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
					UInt64 BitsHash = GetFileHash(V.AsSpan());

					
					if (FileInfo->FileHash != BitsHash)
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
				if (!Item->HasOpenedFile())
				{
					Item->_IntFile = _FrontEndObject->LoadExternFile(Item->path);
				}
				else
				{
					Item->_IntFile = _FrontEndObject->LoadExternFile(
						Item->OpenedFile.AsSpan(), Item->path.extension());
				}

				Files.push_back(Item->_IntFile.get());
				Files.back()->FileName = Item->path.filename();
			}
			else
			{
				//if (!Item->HasOpenedFile())
				{
					Item->_IntFile = _FrontEndObject->LoadIntFile(Item->InterPath);
				}
				/*
				else
				{
					Item->_IntFile = _FrontEndObject->LoadIntFile(
						Item->OpenedFile.AsSpan(), Item->InterPath.extension());
				}
				*/
				Files.push_back(Item->_IntFile.get());
				Files.back()->FileName = Item->Repath;
			}
		}
		

		for (auto& Item : ChangedFiles)
		{
			if (Item->IsExternal) 
			{
				if (!Item->HasOpenedFile())
				{
					Item->OpenedFile = GetBytesFromFile(Item->path);
				}
				Item->_IntFile = _FrontEndObject->LoadExternFile(
						Item->OpenedFile.AsSpan(), Item->path.extension());

				Files.push_back(Item->_IntFile.get());
				Files.back()->FileName = Item->path.filename();
				continue;
			}
			if (!Item->HasOpenedFile())
			{
				Item->OpenedFile = OpenFile(Item->_FInfo, Item->path);
			}

			_Errors.FilePath = Item->Repath;
			_FrontEndObject->SetSourcePath(Item->path);

			Item->_File = _FrontEndObject->BuildFile(Item->OpenedFile.AsSpan());
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
				Files.back()->FileName = Item->Repath;
			}
		}

	}



	CompilerRet r;
	r._State = CompilerState::Fail;
	r.OutPut = nullptr;

	bool sholdcompile = Files.size() || !fs::exists(Data.OutFile);
	if (sholdcompile && !_Errors.Has_Errors())
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
				if (_BackEndObject == nullptr
					|| (_oldBackEnd != _BackEnd))
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
						if (Output.Size())
						{
							std::ofstream File(Data.OutFile, std::ios::binary);
							if (File.is_open())
							{
								File.write((const char*)Output.Data(), Output.Size());
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
							if (Item->IsExternal) 
							{ 
								Item->ExternalFileInfo->FileHash= GetFileHash(Item->OpenedFile.AsSpan());
								continue; 
							}


							{
								Path InterDirPath = Item->InterPath.parent_path();
								if (!fs::exists(InterDirPath))
								{
									std::filesystem::create_directories(InterDirPath);
								}
							}

							_FrontEndObject->ToIntFile(Item->_File.get(), Item->InterPath);
							auto FileDeps = _FrontEndObject->Get_DependenciesPostIR(Item->_File.get());


							Item->FileInfo->FileHash = GetFileHash(Item->OpenedFile.AsSpan());
							Item->FileInfo->Dependencies.clear();
							Item->FileInfo->ExternDependencies.clear();
							for (auto& Item2 : FileDeps)
							{

								bool IsExtern = Item->IsExternal;

								MyStruct* depfileinfo = nullptr;
								{
									for (auto& Item : ChangedFiles)
									{
										if (Item->IsExternal != IsExtern) { continue; }

										auto& CmdStr = IsExtern ? Item->path : Item->Repath;

										if (Item->Repath == CmdStr)
										{
											depfileinfo = Item;
											break;
										}
									}
									if (depfileinfo == nullptr) 
									{
										for (auto& Item : UnChangedFiles)
										{
											if (Item->IsExternal != IsExtern) { continue; }

											auto& CmdStr = IsExtern ? Item->path : Item->Repath;

											if (Item->Repath == CmdStr)
											{
												depfileinfo = Item;
											}
										}
									}
								}

								UCodeLangAssert(depfileinfo);
								if (IsExtern)
								{
									Item->FileInfo->ExternDependencies.push_back(depfileinfo->path);
								}
								else
								{
									Item->FileInfo->Dependencies.push_back(depfileinfo->Repath);
								}

							}
						}
					}
				}
			}
		}
	}
	else
	{
		if (!_Errors.Has_Errors()) 
		{
			//TODO return UClib if BackEnd is not UCodeBackEnd.
			auto bytes = GetBytesFromFile(Data.OutFile);
			if (Data.OutFile.extension() == FileExt::LibWithDot)
			{
				thread_local UClib lib;
				UClib::FromBytes(&lib, bytes.AsSpan());
				r.OutPut = &lib;
			}
			else
			{
				r.OutFile = std::move(bytes);
			}
		}
	}

	r._State = _Errors.Has_Errors() ? CompilerState::Fail : CompilerState::Success;

	

	if (r._State == CompilerState::Success && Files.size())
	{
		for (auto& Item : NewFilesInfo)
		{
			NewFile.Files.push_back(std::move(*Item));
		}
		for (auto& Item : UnChangedFiles)
		{
			if (Item->IsExternal)
			{
				NewFile.ExternalFiles.push_back(std::move(*Item->ExternalFileInfo));
			}
			else
			{
				NewFile.Files.push_back(std::move(*Item->FileInfo));
			}
		}
		for (auto& Item : NewExternalInfo)
		{
			NewFile.ExternalFiles.push_back(std::move(*Item));
		}
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
		V.Resize(tep.size());
		memcpy(V.Data(), tep.data(), V.Size());
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