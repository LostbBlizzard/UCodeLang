#pragma once

#include "../LangCore.hpp"
#include "../LangCore/BitMaker.hpp"

UCodeLangStart
class DependencyFile
{
public:
	inline static const char* FileName = "Dep.data";

	struct FileInfo
	{
		Path FilePath;


		UInt64 FileLastUpdated = 0;
		UInt64 FileSize = 0;
		UInt64 FileHash = 0;
		
		Vector<Path> Dependencies;
		Vector<Path> ExternDependencies;
		void ToBytes(BitMaker& Output) const;
		static void FromBytes(BitReader& Input, FileInfo& Out);
		bool HasDependence(const Path& path) const
		{
			for (auto& Item : Dependencies)
			{
				if (Item == path) 
				{
					return true;
				}
			}
			return false;
		}
		bool HasDependence(const Vector<Path>& paths) const
		{
			for (auto& Item : paths)
			{
				if (HasDependence(Item))
				{
					return true;
				}
			}
			return false;
		}

		bool HasExternDependence(const Path& path) const
		{
			for (auto& Item : ExternDependencies)
			{
				if (Item == path)
				{
					return true;
				}
			}
			return false;
		}
		bool HasExternDependence(const Vector<Path>& paths) const
		{
			for (auto& Item : paths)
			{
				if (HasExternDependence(Item))
				{
					return true;
				}
			}
			return false;
		}
	};
	struct  ExternalFileInfo
	{
		Path FullFilePath;


		UInt64 FileLastUpdated = 0;
		UInt64 FileSize = 0;
		UInt64 FileHash = 0;

		void ToBytes(BitMaker& Output) const;
		static void FromBytes(BitReader& Input, ExternalFileInfo& Out);
	};
	Vector<FileInfo> Files;
	Vector<ExternalFileInfo> ExternalFiles;


	FileInfo* Get_Info(const Path& Path);
	ExternalFileInfo* Get_ExternalFile_Info(const Path& Path);

	
	static BytesPtr ToRawBytes(const DependencyFile* Lib);
	static bool FromBytes(DependencyFile* Lib, const BytesView& Data);

	static bool ToFile(const DependencyFile* Lib, const Path& path);
	static bool FromFile(DependencyFile* Lib, const Path& path);
};
UCodeLangEnd

