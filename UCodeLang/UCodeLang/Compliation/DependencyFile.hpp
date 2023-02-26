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
	};
	
	Vector< FileInfo> Files;
	FileInfo* Get_Info(const Path& Path);

	
	static BytesPtr ToRawBytes(const DependencyFile* Lib);
	static bool FromBytes(DependencyFile* Lib, const BytesView& Data);

	static bool ToFile(const DependencyFile* Lib, const Path& path);
	static bool FromFile(DependencyFile* Lib, const Path& path);
};
UCodeLangEnd

