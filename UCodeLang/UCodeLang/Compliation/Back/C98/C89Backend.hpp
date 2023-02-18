#pragma once
#include "../BackEndInterface.hpp"
UCodeLangStart
class C89Backend : BackEndObject
{
public:
	C89Backend();
	~C89Backend() override;

	void Reset() override;
	void Build(const IRBuilder* Input) override;

	void AddTextSignature();

	void AddBaseTypes();

	static BackEndObject* MakeObject() { return new C89Backend(); }
private:
	String OutBuffer;
	const IRBuilder* _Input =nullptr;
};
UCodeLangEnd
