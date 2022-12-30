#include "FileHelper.hpp"
UCodeLangStart
String FileHelper::RelativePath(const String& SubPath, const String& Path)
{
    return SubPath.substr(Path.size());
}

Path FileHelper::RelativePath(const Path& SubPath, const Path& path)
{
	return SubPath.native().substr(path.native().size());
}

void FileHelper::ChangeExt(String& Path, const String& NewExt)
{
	size_t ExtIndex = 0;
	for (size_t i = Path.size() - 1; i > 0; i--)
	{
		char Char = Path[i];
		if (Char == '.')
		{
			ExtIndex = i;
		}

	}

	Path = Path.substr(0, ExtIndex);
	Path += NewExt;
}

UCodeLangEnd

