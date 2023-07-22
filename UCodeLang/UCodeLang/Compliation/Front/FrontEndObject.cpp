
#include "FrontEndObject.hpp"
#include "../Compiler.hpp"
UCodeLangStart

Unique_ptr<FileNode_t> FrontEndObject::LoadExternFile(const Path& path)
{
	auto Bytes = Compiler::GetBytesFromFile(path);
	return LoadExternFile(Bytes.AsSpan(), path.extension());
}

UCodeLangEnd