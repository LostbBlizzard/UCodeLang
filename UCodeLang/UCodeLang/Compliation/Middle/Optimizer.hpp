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
		size_t NewIndex = GetNextInsIndex();

		auto& Ins = _Lib->Get_Instructions();
		if (NewIndex < Ins.size())
		{
			return &Ins[NewIndex];
		}
		else
		{
			return nullptr;
		}
	}
	inline Instruction* GetLastIns()
	{
		if (Index == 0) { return nullptr; }
		size_t NewIndex = GetLastInsIndex();

		auto& Ins = _Lib->Get_Instructions();
		if (NewIndex < Ins.size())
		{
			return &Ins[NewIndex];
		}
		else
		{
			return nullptr;
		}
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
		auto& Ins = _Lib->Get_Instructions();
		Ins.erase(Ins.begin() + Index);
	}

	bool UpdatedLib = false;

	
	UAddress GetFuncEnd(UAddress Index);
	void optimiz_FuncForSize(UAddress Index);
};

UCodeLangEnd

