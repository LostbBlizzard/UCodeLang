#pragma once
#include "../BackEndInterface.hpp"
#include "UCodeLang/LangCore/DataType/BinaryVectorMap.hpp"
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

	static String UpdateToCindentifier(const String& Value);
	void UpdateBackInfo(CompliationBackEndInfo& BackInfo) override;
private:
	String OutBuffer;
	const IRBuilder* _Input =nullptr;
	const IRFunc* _Func = nullptr;
	struct ToStringState
	{
		BinaryVectorMap<IRInstruction*, String> PointerToName;
		Vector<IRInstruction*> TepPushedParameters;

		
		String Val;

		String GetName(IRInstruction* Ptr);
	};
	IRType* OutType;
	String ToString();

	void AddSybToString(UCodeLang::String& r);

	void UpdateCppLinks(UCodeLang::String& r, UCodeLang::IRBufferData* V);

	void ToString(String& r, const IRFunc* Item, C89Backend::ToStringState& State,bool OutputBody = false);

	String ToString(const IRType& Type,bool IsUnsigned = false);

	String ToString(ToStringState& State, IRInstruction& Ins, IROperator& Value);
	String ToStringBinary(ToStringState& State, IRInstruction* Ins, const char* V);
	
	String FromIDToCindentifier(IRidentifierID Value);

};
UCodeLangEnd
