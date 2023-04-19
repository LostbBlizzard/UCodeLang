#pragma once
#include "../Typedef.hpp"
UCodeAnalyzerStart


using Language_ID = UInt32;
using OutputPacketType_t = Language_ID;
enum class OutputPacketType :OutputPacketType_t
{
	Return_FileText,
	_Max,
};

using InputPacketType_t = Language_ID;
enum class InputPacketType : InputPacketType_t
{
	Openfile,

	_Max,
};


using FileID = Language_ID;
using ListenerID = Language_ID;

UCodeAnalyzerEnd