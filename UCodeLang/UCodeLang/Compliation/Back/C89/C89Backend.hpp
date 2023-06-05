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


	String GetBackEndName()override { return "CLang89"; };
	String GetOutputExtWithDot()override { return ".c"; }
	static BackEndObject* MakeObject() { return new C89Backend(); }
private:
	String OutBuffer;
	const IRBuilder* _Input =nullptr;
	const IRFunc* _Func = nullptr;
	struct ToStringState
	{
		BinaryVectorMap<IRInstruction*, String> PointerToName;
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

	void AddSybToString(UCodeLang::String& r);

	void NewFunction(UCodeLang::String& r, UCodeLang::IRBufferData* V);

	void ToString(String& r, const IRFunc* Item, C89Backend::ToStringState& State);

	String ToString(const IRType& Type);

	String ToString(ToStringState& State, IRInstruction& Ins, IROperator& Value);
	String ToStringBinary(ToStringState& State, IRInstruction* Ins, const char* V);
	String UpdateToCindentifier(const 	String& Value);
	String FromIDToCindentifier(IRidentifierID Value);
};
UCodeLangEnd
