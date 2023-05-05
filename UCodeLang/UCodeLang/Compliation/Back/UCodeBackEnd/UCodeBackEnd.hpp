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
		Parloc Type;
		union 
		{
			size_t StackOffset = 0;
			RegisterID _Reg;
			const IRPar* Par;
		};
		
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
	struct Optimizations
	{
		bool InlineFuncionCopys = false;
		bool ReOderForCacheHit = false;
	};
	struct UCodeFunc
	{
		IRidentifierID IRName;
		Vector<Instruction> _Ins;

		Optional<size_t> _Hash;
		Optional<UCodeFunc*> _AutoJumpTo;


		Vector<FuncInsID> FuncsToLink;
	};
	struct FuncInsID_UCodeFunc
	{
		UCodeFunc* Func = nullptr;
		UAddress Index;
		IRidentifierID _FuncID;
	};

	enum class IRloc
	{
		Register,
		StackPreCall,
		StackPostCall,
	};

	struct IRlocData
	{
		IRloc Type;
		IRType ObjectType;
		union
		{
			size_t StackOffset = 0;
			const IRPar* Par;
			RegisterID _Reg;
		};
		
		void SetAsStackPreCall(size_t Offset)
		{
			StackOffset = Offset;
			Type = IRloc::StackPreCall;
		}
		void SetAsStackPostCall(size_t Offset)
		{
			StackOffset = Offset;
			Type = IRloc::StackPostCall;
		}
		void SetAsRegister(RegisterID V)
		{
			_Reg = V;
			Type = IRloc::Register;
		}
	};
	
	struct  FuncCallEndData
	{
		Vector<IRPar> Pars;
	};
	struct FindParsLoc
	{
		Vector<ParlocData> ParsPos;
		Vector<size_t> OverflowedPars;
	};
	//Data
	Instruction _Ins;
	RegistersManager _Registers;
	StackInfo _Stack;
	Vector<FuncInsID> FuncsToLink;
	Vector<Funcpos> _Funcpos;
	Vector<ParlocData> CurrentFuncParPos;
	
	const IRBuilder* _Input = nullptr;
	UClib* _Output = nullptr;
	BinaryVectorMap< const IRBlock*, BlockData> IRToBlockData;
	
	
	StaticMemoryManager _StaticMemory;
	StaticMemoryManager _ThreadMemory;

	size_t Index = 0;
	const IRBlock* LookingBlock = nullptr;
	
	RegisterID _InputPar = RegisterID::StartParameterRegister;
	size_t _CurrentParIndex = 0;

	Vector<Unique_ptr<UCodeFunc>> Funcs;
	UCodeFunc* BuildingFunc = nullptr;
	Optimizations _Optimizations;

	//code
	
	UCodeFunc* NewBuildFunc()
	{
		auto Item = new UCodeFunc();
		Funcs.push_back(Unique_ptr<UCodeFunc>(Item));
		return Item;
	}
	void UpdateOptimizations();
	void OnFunc(const IRFunc* IR);
	void OnBlock(const IRBlock* IR);
	void DoOptimizations();
	void LinkFuncs();

	void BuildSymbols();
	void BuildFuncs();

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

	bool IsReferenceingTheSame(const IROperator& Test, const IROperator& Other);

	bool IsReferencedAfterThisIndex(const IRInstruction* Op)
	{
		return  IsReferencedAfterThisIndex(IROperator((IRInstruction*)Op));
	}
	
	using WeightType = int;
	WeightType IsReferencedAfterThisIndexWeighted(const IRInstruction* Op)
	{
		return  IsReferencedAfterThisIndexWeighted(IROperator((IRInstruction*)Op));
	}
	WeightType IsReferencedAfterThisIndexWeighted(const IROperator& Op);

	bool IsReferencedAfterThisIndex(const IROperator& Op);

	void DropStack();
	void DropPars();

	inline UClib& Get_Output()
	{
		return Getliboutput();
	}
	ParlocData* GetParData(const IRPar* Par)
	{

		for (auto& Item : CurrentFuncParPos)
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


	void RegWillBeUsed(RegisterID Value);
	IRType GetType(const IRInstruction* IR)
	{
		return _Input->GetType(IR);
	}
	IRType GetType(const IROperator& IR)
	{
		return _Input->GetType(IR);
	}


	void GiveNameToReg(RegisterID Value, const AnyInt64 Name)
	{
		_Registers.WeakLockRegisterValue(Value, Name);
	}
	void GiveNameToReg(RegisterID Value, const IRInstruction* Name)
	{
		_Registers.WeakLockRegisterValue(Value,Name);
	}
	RegisterID GetRegisterForTep();
	Optional<RegisterID> FindIRInRegister(const IRInstruction* Value);
	Optional<RegisterID> FindValueInRegister(AnyInt64 Value);

	void UnLockRegister(RegisterID ID)
	{
		_Registers.UnLockRegister(ID);
	}

	IRlocData GetIRLocData(const IRInstruction& Ins, const IROperator& Op);
	void CopyValues(const IRlocData& Src, const IRlocData& Out);
	void StoreValueInPointer(const IRType& ObjectType, RegisterID Pointer, const IRlocData& Value);
	RegisterID MakeIntoRegister(const IRlocData& Value, Optional<RegisterID> RegisterToPut = {});
	RegisterID MakeIntoRegister(const IRInstruction& Ins, const IROperator& Op, Optional<RegisterID> RegisterToPut = {})
	{
		auto Value = GetIRLocData(Ins,Op);
		return MakeIntoRegister(Value,RegisterToPut);
	}
	void GiveNameTo(const IRlocData& Value, const IRInstruction& Name);

	IRlocData GetPointerOf(const IRlocData& Value);

	RegisterID LoadOp(const IRInstruction& Ins, const IROperator& Op);
	void LoadOpToReg(const IRInstruction& Ins, const IROperator& Op, RegisterID Out);
	void RegToReg(IRTypes Type, RegisterID In, RegisterID Out);
	void PushOpStack(const IRInstruction& Ins, const IROperator& Op);
	void LogicalNot(IRTypes Type, RegisterID In, RegisterID Out);
	void DoCPPCall(const IRidentifier& FuncName);
	
	void CopyValueToStack(const IRInstruction* IRName, const IRType& ObjectType, RegisterID Item);

	void MoveValueToStack(const IRInstruction* IRName, const IRType& ObjectType, RegisterID Item);

	void StoreValue(const IRInstruction& Ins, const  IROperator& OutputLocationIR,const IROperator& Input);

	void StoreValueInPointer(const IRType& ObjectType, RegisterID Pointer, const  IROperator& Value, IRInstruction& Ins);
	void StoreValueInPointer(const IRType& ObjectType, RegisterID Pointer, RegisterID Value);
	


	RegisterID ReadValueFromPointer(const IRType& ObjectType, RegisterID Pointer);

	void BuildSIntToIntCast(const IRInstruction& Item, const IROperator& Op, size_t IntSize);
	void BuildUIntToIntCast(const IRInstruction& Item, const IROperator& Op, size_t IntSize);



	FuncCallEndData FuncCallStart(const Vector<IRType>& Pars, const IRType& RetType);
	FuncCallEndData FuncCallStart(const Vector<IRPar>& Pars, const IRType& RetType);
	void FuncCallRet(const IRType& RetType);
	void FuncCallEnd(FuncCallEndData& Data);



	FindParsLoc GetParsLoc(const Vector<IRType>& Pars);
	FindParsLoc GetParsLoc(const Vector<IRPar>& Pars);

	AnyInt64 ToAnyInt(const IRType& ObjectType, const  IROperator& Op);

	size_t GetStatckOffset(const ParlocData& ItemStackOffset)
	{
		if (ItemStackOffset.Type == Parloc::StackPostCall)
		{
			return GetPostCallStackOffset(ItemStackOffset.StackOffset);
		}
		else
		{
			return GetPreCallStackOffset(ItemStackOffset.StackOffset);
		}
	}
	size_t GetPostCallStackOffset(size_t ItemStackOffset)
	{
		return ItemStackOffset;
	}
	size_t GetPreCallStackOffset(size_t ItemStackOffset)
	{
		size_t FuncPointerSize = Get_Settings().PtrSize == IntSizes::Int64 ? 8 : 4;
		return  ItemStackOffset + FuncPointerSize + 4;
	}
};
UCodeLangEnd

