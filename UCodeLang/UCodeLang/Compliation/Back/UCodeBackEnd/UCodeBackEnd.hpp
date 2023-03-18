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

	
	struct StackItem
	{
		size_t Offset=0;
		IRInstruction* IR=nullptr;
	};
	struct StackInfo
	{
		size_t Size = 0;
		size_t PushedOffset = 0;


		void Reset()
		{
			Size = 0;
			PushedOffset = 0;
			Items.clear();
		}
		Vector<StackItem> Items;

		StackItem* Has(IRInstruction* Value)
		{
			for (auto& Item : Items)
			{
				if (Item.IR == Value) 
				{
					return &Item;
				}
			}

			return nullptr;
		}
	};
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

	void LockRegister(RegisterID ID);

	void DropStack();
	void DropPars();

	inline UClib& Get_Output()
	{
		return Getliboutput();
	}

	//
	Instruction _Ins;
	UAddress PushIns()
	{
	 return	_Output->Add_Instruction(_Ins);
	}

	RegistersManager _Registers;
	StackInfo _Stack;

	struct FuncInsID
	{
		UAddress Index;
		IRidentifierID _FuncID;
	};
	Vector<FuncInsID> FuncsToLink;
	
	struct Funcpos
	{
		UAddress Index;
		IRidentifierID _FuncID;
	};
	Vector<Funcpos> _Funcpos;

	RegisterID _InputPar= RegisterID::NullRegister;
	

	enum class Parloc
	{
		Register,
		Stack,
	};

	struct ParlocData
	{
		const IRPar* Par = nullptr;
		
		Parloc Type;

		size_t StackOffset =0;
		RegisterID _Reg = RegisterID::NullRegister;


	};
	Vector<ParlocData> ParsPos;
	ParlocData* GetParData(const IRPar* Par)
	{
		for (auto& Item : ParsPos)
		{
			if (Item.Par == Par)
			{
				return &Item;
			}
		}
		return nullptr;
	}
	

	RegisterID LoadOp(IRInstruction& Ins, IROperator Op);
	void LoadOpToReg(IRInstruction& Ins, IROperator Op,RegisterID Out);
	void RegToReg(IRTypes Type, RegisterID In, RegisterID Out);
	void PushOpStack(IRInstruction& Ins, IROperator Op);
	RegisterID FindOp(IRInstruction& Ins, IROperator Op);
	void FindOpToReg(IRInstruction& Ins, IROperator Op, RegisterID Out);
	void LogicalNot(IRTypes Type, RegisterID In, RegisterID Out);
	void DoCPPCall(const IRidentifier& FuncName);
};
UCodeLangEnd

