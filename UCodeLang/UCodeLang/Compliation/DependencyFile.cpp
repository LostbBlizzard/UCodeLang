#include "DependencyFile.hpp"
UCodeLangStart
BytesPtr DependencyFile::ToRawBytes(const DependencyFile* Lib)
{
    return BytesPtr();
}

bool DependencyFile::FromBytes(DependencyFile* Lib, const BytesView& Data)
{
    return false;
}

bool DependencyFile::ToFile(const DependencyFile* Lib, const Path& path)
{
    return false;
}

bool DependencyFile::FromFile(DependencyFile* Lib, const Path& path)
{
    return false;
}


UCodeLangEnd