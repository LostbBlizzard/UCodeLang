#ifndef UCodeLangNoCompiler
#include "FrontEndObject.hpp"
#include <fstream>
UCodeLangStart

Unique_ptr<FileNode_t> FrontEndObject::LoadExternFile(const Path& path)
{

	BytesPtr Bytes;
	std::ifstream File(path, std::ios::binary);
	if (File.is_open())
	{
		File.seekg(0, std::ios::end);
		Bytes.Resize(File.tellg());
		File.seekg(0, std::ios::beg);

		File.read((char*)Bytes.Data(), Bytes.Size());

		File.close();
	}
	return LoadExternFile(Bytes.AsSpan(), path.extension());
}

Unique_ptr<FileNode_t> FrontEndObject::LoadIntFile(const Path& path)
{
	BytesPtr Bytes;
	std::ifstream File(path, std::ios::binary);
	if (File.is_open())
	{
		File.seekg(0, std::ios::end);
		Bytes.Resize(File.tellg());
		File.seekg(0, std::ios::beg);

		File.read((char*)Bytes.Data(), Bytes.Size());

		File.close();
	}
	return  LoadIntFile(Bytes.AsSpan(), path.extension());
}

UCodeLangEnd

#endif