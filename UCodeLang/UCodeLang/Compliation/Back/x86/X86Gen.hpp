#pragma once
#include "../BackEndInterface.hpp"
UCodeLangStart
class X86Gen
{
public:
	X86Gen() {}
	~X86Gen() {}

	void Reset()
	{

	}
	void Build(const IRBuilder* Input);
	void OnFunc(const IRFunc* IR);
	void OnBlock(const IRBlock* IR);

	const IRBuilder* _Input;
	Vector<Byte> InstructionsOutput;
	void PushInstruction(Byte Value)
	{
		InstructionsOutput.push_back(Value);
	}
};
UCodeLangEnd