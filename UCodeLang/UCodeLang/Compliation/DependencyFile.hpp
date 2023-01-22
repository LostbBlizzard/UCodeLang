#pragma once

#include "../LangCore.hpp"

UCodeLangStart
class DependencyFile
{
public:
	inline static const char* FileName = "Dep.data";


	
	
	static BytesPtr ToRawBytes(const DependencyFile* Lib);
	static bool FromBytes(DependencyFile* Lib, const BytesView& Data);

	static bool ToFile(const DependencyFile* Lib, const Path& path);
	static bool FromFile(DependencyFile* Lib, const Path& path);
};
UCodeLangEnd

