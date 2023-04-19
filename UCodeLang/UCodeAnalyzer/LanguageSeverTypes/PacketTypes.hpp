#pragma once
#include "BasicType.hpp"
UCodeAnalyzerStart




struct OpenFile_Packet
{
	Path FilePath;
	FileID ClientSideID;
};

struct Query_FileText_Packet
{
	FileID ClientSideID;
};

struct Return_FileText_Packet
{
	FileID ClientSideID;
	String FileText;
};

UCodeAnalyzerEnd