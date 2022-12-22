#pragma once
#include "UCodeLang/LangCore/UClib.hpp"
#include "../Helpers/CompliationErrors.hpp"
#include "../CompliationSettings.hpp"
UCodeLangStart
class Optimizer
{
public:
	Optimizer() {}
	~Optimizer(){}
	void optimiz(UClib& Lib);

	void Reset();
	UCodeLangForceinline void Set_ErrorsOutput(CompliationErrors* V) { _ErrorsOutput = V; }
	UCodeLangForceinline void Set_Settings(CompliationSettings* V) { _Settings = V; }
private:
	static constexpr InstructionSet_t ReMoveIns = (InstructionSet_t)InstructionSet::MAXVALUE + 1;
	UClib* _Lib = nullptr;

	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;


	UAddress Index = 0;
	inline Instruction* GetNextIns()
	{
		return nullptr;
	}
	inline Instruction* GetLastIns()
	{
		if (Index == 0) { return nullptr; }
		size_t NewIndex = 0;


		return nullptr;
	}

	inline UAddress GetNextInsIndex()
	{
		return Index + 1;
	}
	inline UAddress GetLastInsIndex()
	{
		return Index - 1;
	}
	inline void RemoveIns(UAddress Index)
	{
		UpdatedLib = true;
	}

	bool UpdatedLib = false;

	
	UAddress GetFuncEnd(UAddress Index);
	void optimiz_FuncForSize(UAddress Index);
};

UCodeLangEnd

