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

	static BackEndObject* MakeObject() { return new UCodeBackEndObject(); }
private:

	//types	
	using UObjectSize_t = UInt64;
	struct BlockData
	{

	};
	struct FuncInsID
	{
		UAddress Index;
		IRidentifierID _FuncID;
	};
	enum class Parloc
	{
		Register,
		StackPreCall,
		StackPostCall,
	};
	struct ParlocData
	{
		const IRPar* Par = nullptr;
		Parloc Type;
		size_t StackOffset = 0;
		Optional<RegisterID> _Reg;
		void SetAsStackPreCall(size_t Offset)
		{
			StackOffset = Offset;
			Type = Parloc::StackPreCall;
		}
		void SetAsStackPostCall(size_t Offset)
		{
			StackOffset = Offset;
			Type = Parloc::StackPostCall;
		}
		void SetAsRegister(RegisterID V)
		{
			_Reg = V;
			Type = Parloc::Register;
		}
	};	
	struct Funcpos
	{
		UAddress Index;
		IRidentifierID _FuncID;
	};
	//Data
	Instruction _Ins;
	RegistersManager _Registers;
	StackInfo _Stack;
	Vector<FuncInsID> FuncsToLink;
	Vector<Funcpos> _Funcpos;
	Vector<ParlocData> ParsPos;
	const IRBuilder* _Input = nullptr;
	UClib* _Output = nullptr;
	BinaryVectorMap< const IRBlock*, BlockData> IRToBlockData;

	RegisterID _InputPar = RegisterID::A;
	StaticMemoryManager _StaticMemory;
	StaticMemoryManager _ThreadMemory;

	size_t Index = 0;
	const IRBlock* LookingBlock = nullptr;
	//code
	

	void OnFunc(const IRFunc* IR);
	void OnBlock(const IRBlock* IR);

	void OnBlockBuildCode(const IRBlock* IR);

	
	BlockData& GetBlockData(const IRBlock* V)
	{
		return IRToBlockData.at(V);
	}
	void BindBlockData(BlockData& Data, const IRBlock* V)
	{
		IRToBlockData[V] = Data;
	}
	UAddress PushIns()
	{
		return	_Output->Add_Instruction(_Ins);
	}

	bool IsReferenceingTheSame(const IROperator& Test,const IROperator& Other)
	{
		if (Test.Type == Other.Type)
		{
			if (Test.identifer == Test.identifer)
			{
				return true;
			}
		}

		return false;
	}

	bool IsReferencedAfterThisIndex(const IRInstruction* Op)
	{
		return  IsReferencedAfterThisIndex(IROperator((IRInstruction*)Op));
	}
	bool IsReferencedAfterThisIndex(const IROperator& Op)
	{
		for (size_t i = Index+1; i < LookingBlock->Instructions.size(); i++)
		{
			auto Item = LookingBlock->Instructions[i].get();

			if (IsOperatorValueInTarget(Item->Type))
			{
				if (IsReferenceingTheSame(Op,Item->Target())) 
				{
					return true;
				}
			}

			if (IsOperatorValueInInput(Item->Type))
			{
				if (IsReferenceingTheSame(Op,Item->Input()))
				{
					return true;
				}
			}
		}
		return false;
	}

	void DropStack();
	void DropPars();

	inline UClib& Get_Output()
	{
		return Getliboutput();
	}
	ParlocData* GetParData(const IRPar* Par)
	{

		for (auto& Item : ParsPos)
		{
			if (Item.Par == Par)
			{
				SynchronizePar(&Item);
				return &Item;
			}
		}
		return nullptr;
	}

	bool IsLookingAtPar(const IRInstruction* IR,const IRPar* Par)
	{
		return IR->Target().Parameter == Par;
	}
	void SynchronizePar(ParlocData* Par);
	void LockRegister(RegisterID ID);


	void RegWillBeUsed(RegisterID Value)
	{
		auto& Info = _Registers.GetInfo(Value);
		
		if (Info.Inuse == RegistersManager::RegisterInUse::InUseSybol && IsReferencedAfterThisIndex(Info.IRField))
		{
			Info.Inuse = RegistersManager::RegisterInUse::NotInUse;
			CopyValueToStack(Info.IRField, GetType(Info.IRField), Value);
		}
	}
	IRType GetType(const IRInstruction* IR)
	{
		return IR->ObjectType;
	}
	void GiveNameToReg(RegisterID Value, const IRInstruction* Name)
	{
		_Registers.WeakLockRegisterValue(Value,Name);
	}
	RegisterID GetRegisterForTep();
	Optional< RegisterID> FindIRInRegister(const IRInstruction* Value)
	{
		return _Registers.GetInfo(Value);
	}

	void UnLockRegister(RegisterID ID)
	{
		_Registers.UnLockRegister(ID);
	}

	RegisterID LoadOp(const IRInstruction& Ins, const IROperator& Op);
	void LoadOpToReg(const IRInstruction& Ins, const IROperator& Op, RegisterID Out);
	void RegToReg(IRTypes Type, RegisterID In, RegisterID Out);
	void PushOpStack(const IRInstruction& Ins, const IROperator& Op);
	RegisterID FindOp(const IRInstruction& Ins, const IROperator& Op);
	void FindOpToReg(const IRInstruction& Ins, const  IROperator& Op, RegisterID Out);
	void LogicalNot(IRTypes Type, RegisterID In, RegisterID Out);
	void DoCPPCall(const IRidentifier& FuncName);
	
	void CopyValueToStack(const IRInstruction* IRName, const IRType& ObjectType, RegisterID Item);

	void StoreValue(const IRInstruction& Ins, const  IROperator& OutputLocationIR,const IROperator& Input);

	void StoreValueInPointer(const IRType& ObjectType, RegisterID Pointer, const  IROperator& Value, IRInstruction& Ins);
	void StoreValueInPointer(const IRType& ObjectType, RegisterID Pointer, RegisterID Value);
	RegisterID ReadValueFromPointer(const IRType& ObjectType, RegisterID Pointer);

	void FuncCallStart();
	void FuncCallRet(const IRType& RetType);
	void FuncCallEnd();
};
UCodeLangEnd

