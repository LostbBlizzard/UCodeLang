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
	const IRFunc* _Func = nullptr;
	struct ToStringState
	{
		Unordered_map<IRInstruction*, String> PointerToName;
		Vector<IRInstruction*> TepPushedParameters;

		size_t StrValue = 0;

		String GetName(IRInstruction* Ptr)
		{
			char r = 'A' + (char)StrValue;
			StrValue++;
			auto V = (String)String_view(&r, 1);
			PointerToName[Ptr] = V;
			return V;
		}
	};
	String ToString();

	String ToString(const IRType& Type);

	String ToString(ToStringState& State, IRInstruction& Ins, IROperator& Value);
	String ToStringBinary(ToStringState& State, IRInstruction* Ins, const char* V);
};
UCodeLangEnd
