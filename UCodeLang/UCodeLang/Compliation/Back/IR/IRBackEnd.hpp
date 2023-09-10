#pragma once

#include "../BackEndInterface.hpp"
UCodeLangStart
class IRBackEnd : BackEndObject
{
public:
	IRBackEnd();
	~IRBackEnd() override;

	void Reset() override;
	void Build(const IRBuilder* Input) override;


	String GetBackEndName() override { return "ULangIR"; }
	String GetOutputExtWithDot() override { return FileExt::IRWithDot; }
	static BackEndObject* MakeObject();
};
UCodeLangEnd

