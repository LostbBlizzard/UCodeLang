#pragma once
#ifndef UCodeLangNoCompiler

#include "../BackEndInterface.hpp"
#include "UCodeLang/LangCore/DataType/UnorderedMap.hpp"
UCodeLangStart
class C11Backend : BackEndObject
{
public:
	C11Backend();
	~C11Backend() override;

	void Reset() override;
	void Build(const IRBuilder* Input) override;

	void AddTextSignature();

	void AddBaseTypes();


	String GetBackEndName()override { return "CLang11"; };
	String GetOutputExtWithDot()override { return ".c"; }
	static BackEndObject* MakeObject() { return new C11Backend(); }

	static String UpdateToCindentifier(const String& Value);
	void UpdateBackInfo(CompilationBackEndInfo& BackInfo) override;
private:
	String OutBuffer;
	const IRBuilder* _Input = nullptr;
	const IRFunc* _Func = nullptr;
	struct ToStringState
	{
		UnorderedMap<IRInstruction*, String> PointerToName;
		Vector<IRInstruction*> TepPushedParameters;


		size_t Num = 0;

		String GetName(IRInstruction* Ptr);
	};
	IRType* OutType;
	//Compile Flags
	bool Flag_NoExceptions = false;


	String ToString();

	void AddSybToString(UCodeLang::String& r);

	void UpdateCppLinks(UCodeLang::String& r, UCodeLang::IRBufferData* V);

	void ToString(String& r, const IRFunc* Item, C11Backend::ToStringState& State, bool OutputBody = false);

	String ToString(const IRType& Type, bool IsUnsigned = false);

	String ToString(ToStringState& State, IRInstruction& Ins, IROperator& Value);
	String ToStringBinary(ToStringState& State, IRInstruction* Ins, const char* V);

	String FromIDToCindentifier(IRidentifierID Value);

};
UCodeLangEnd

#endif