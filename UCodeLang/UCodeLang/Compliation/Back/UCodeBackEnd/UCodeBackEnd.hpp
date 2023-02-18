#pragma once
#include "../BackEndInterface.hpp"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"
#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
#include "RegistersManager.hpp"
UCodeLangStart

class UCodeBackEndObject : BackEndObject
{
	
public:
	UCodeBackEndObject();
	~UCodeBackEndObject() override;

	void Reset() override;
	void Build(const IRBuilder* Input) override;
	
	static BackEndObject* MakeObject(){return new UCodeBackEndObject();}
private:
	using UObjectSize_t = UInt64;
	const IRBuilder* _Input=nullptr;
	UClib* _Output=nullptr;


	//
	struct BlockData
	{

	};
	Unordered_map< const IRBlock*, BlockData> IRToBlockData;
	BlockData& GetBlockData(const IRBlock* V)
	{
		return IRToBlockData.at(V);
	}
	void BindBlockData(BlockData& Data,const IRBlock* V)
	{
		IRToBlockData[V] = Data;
	}
	//

	void OnFunc(const IRFunc* IR);
	void OnBlock(const IRBlock* IR);

	void OnBlockBuildCode(const UCodeLang::IRBlock* IR);

	inline UClib& Get_Output()
	{
		return Getliboutput();
	}

	//
	Instruction Ins;
	UAddress PushIns()
	{
	 return	_Output->Add_Instruction(Ins);
	}

};
UCodeLangEnd

