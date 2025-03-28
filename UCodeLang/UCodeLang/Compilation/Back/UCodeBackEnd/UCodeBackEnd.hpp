#pragma once

#include "UCodeLang/LangCore/LangTypes.hpp"
#ifndef UCodeLangNoCompiler
#include "../BackEndInterface.hpp"
#include "RegistersManager.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
#include "UCodeLang/Compilation/CompilationSettings.hpp"
UCodeLangStart

class UCodeBackEndObject : BackEndObject
{

public:
	UCodeBackEndObject();
	~UCodeBackEndObject() override;

	void Reset() override;
	void Build(const IRBuilder* Input) override;

	void AddDebugInfo();


	String GetBackEndName() override { return "UCodeVM"; }
	String GetOutputExtWithDot() override { return FileExt::LibWithDot; }
	void UpdateBackInfo(CompilationBackEndInfo& BackInfo) override
	{
		BackInfo.Output = CompilationBackEndInfo::BackEnd::UCodeVm;
		BackInfo.OutputSet = CompilationBackEndInfo::InsSet::Other;
	}

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


	struct StackPreCall
	{
		size_t Offset = 0;
		StackPreCall(size_t Offset) :Offset(Offset)
		{

		}
	};
	struct StackPostCall
	{
		size_t Offset = 0;
		StackPostCall(size_t Offset) :Offset(Offset)
		{

		}
	};


	struct ParlocData
	{
		const IRPar* Par = nullptr;
		Variant<RegisterID, StackPreCall, StackPostCall> Location;
	};
	struct Funcpos
	{
		UAddress Index;
		IRidentifierID _FuncID;
	};
	struct Optimizations
	{
		bool InlineFunctionCopys = false;
		bool ReOderForCacheHit = false;
	};
	struct UCodeFunc
	{
		IRidentifierID IRName = 0;
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


	struct IRlocData_Register
	{
		RegisterID _Reg;
	};
	struct IRlocData_IRPar
	{
		const IRPar* Par = nullptr;
	};
	struct IRlocData_StackPre
	{
		size_t offset = 0;
		IRlocData_StackPre()
		{

		}
		IRlocData_StackPre(size_t offset) :offset(offset)
		{

		}
	};
	struct IRlocData_StackPost
	{
		size_t offset = 0;
		IRlocData_StackPost()
		{

		}
		IRlocData_StackPost(size_t offset) :offset(offset)
		{

		}
	};
	struct IRlocData_StaticPos
	{
		size_t offset = 0;
		IRlocData_StaticPos()
		{

		}
		IRlocData_StaticPos(size_t offset) :offset(offset)
		{

		}
	};
	struct IRlocData_ThreadPos
	{
		size_t offset = 0;
		IRlocData_ThreadPos()
		{

		}
		IRlocData_ThreadPos(size_t offset) :offset(offset)
		{

		}
	};


	struct Nothing
	{
		Nothing() {}
	};
	struct IRlocData
	{
		IRType ObjectType;
		Variant<Nothing, RegisterID, IRlocData_IRPar, IRlocData_StackPre, IRlocData_StackPost
			, IRlocData_ThreadPos, IRlocData_StaticPos> Info;
	};
	IRlocData To(const ParlocData& Value);

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
	CodeLayer::UCodeByteCode* _OutLayer = nullptr;

	UnorderedMap< const IRBlock*, BlockData> IRToBlockData;


	StaticMemoryManager _StaticMemory;
	StaticMemoryManager _ThreadMemory;

	size_t Index = 0;
	const IRBlock* LookingBlock = nullptr;

	RegisterID _InputPar = RegisterID::StartParameterRegister;
	size_t _CurrentParIndex = 0;

	Vector<Unique_ptr<UCodeFunc>> Funcs;
	UCodeFunc* BuildingFunc = nullptr;
	Optimizations _Optimizations;
	const IRFunc* lookingatfunc = nullptr;
	Vector<size_t> ThrowJumps;
	UnorderedMap< const IRFunc*,size_t> FuncStackSizes;

	struct CleanUpVar
	{
		UAddress VarableStart;
		UAddress VarableEnd;
		size_t PostStackOffset;
		IRidentifierID CleanUpFuncion;
	};
	UnorderedMap< const IRFunc*, Vector<CleanUpVar>> FuncCleanUpFuncions;
	//Compile Flags
	bool Flag_NoExceptions = false;

	
	//code
	bool IsPrimitive(const IRType& type)
	{
		return _Input->IsPrimitive(type);
	}
	IRType GetType(const IROperator& Op)
	{
		return _Input->GetType(Op);
	}
	IRType GetType(const IRInstruction* IR)
	{
		return _Input->GetType(IR);
	}
	IRType GetType(const IRInstruction* IR, const IROperator& Op)
	{
		return _Input->GetType(IR, Op);
	}
	struct BinaryOpRet
	{
		RegisterID A;
		RegisterID B;
		RegisterID V;
	};
	BinaryOpRet DoBinaryOpValues(const IRInstruction* IR);


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
	void DoPostOptimizations();
	void LinkFuncs();

	void BuildSymbols();
	void BuildFuncs();

	void OnBlockBuildCode(const IRBlock* IR);

	void GiveFuncReturnName(const IRType& ReturnType, const IRInstruction* Item);


	size_t GetSize(const IRInstruction* Ins)
	{
		return _Input->GetSize(Ins->ObjectType, Get_Settings().PtrSize == IntSizes::Int32);
	}
	size_t GetSize(const IRStruct* Value)
	{
		return _Input->GetSize(Value);
	}
	size_t GetSize(const IRType& Value)
	{
		return _Input->GetSize(Value, Get_Settings().PtrSize == IntSizes::Int32);
	}


	BlockData& GetBlockData(const IRBlock* V)
	{
		return IRToBlockData.GetValue(V);
	}
	void BindBlockData(BlockData& Data, const IRBlock* V)
	{
		IRToBlockData.AddValue(V, Data);
	}
	UAddress PushIns()
	{
		return	_OutLayer->Add_Instruction(_Ins);
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

	bool IsLookingAtPar(const IRInstruction* IR, const IRPar* Par)
	{
		return IR->Target().Parameter == Par;
	}
	void SynchronizePar(ParlocData* Par);



	void RegWillBeUsed(RegisterID Value, Vector<RegisterID> BanRegisters = {});

	void SetRegister(RegisterID Value, const AnyInt64 Name)
	{
		_Registers.SetRegister(Value, Name);
	}
	void SetRegister(RegisterID Value, const IRInstruction* Name)
	{
		_Registers.SetRegister(Value, Name);
	}
	void SetRegister(RegisterID Value, const IROperator& Name)
	{
		_Registers.SetRegister(Value, Name);
	}
	void SetRegister(RegisterID Value, const IRPar* Name)
	{
		SetRegister(Value, IROperator((IRPar*)Name));
	}
	void SetRegister(RegisterID Value, const IRlocData& Name)
	{
		SetRegister(Value, IROperator());
	}


	RegisterID GetRegisterForTep();


	RegisterID GetRegisterForTep(const AnyInt64 Value)
	{
		auto R = GetRegisterForTep();
		SetRegister(R, Value);
		return R;
	}
	RegisterID GetRegisterForTep(const IRInstruction* Value)
	{
		auto R = GetRegisterForTep();
		RegWillBeUsed(R);
		SetRegister(R, Value);
		return R;
	}
	RegisterID GetRegisterForTep(const IROperator& Value)
	{
		auto R = GetRegisterForTep();
		RegWillBeUsed(R);
		SetRegister(R, Value);
		return R;
	}
	RegisterID GetRegisterForTep(const IRlocData& Value)
	{
		auto R = GetRegisterForTep();
		RegWillBeUsed(R);
		SetRegister(R, Value);
		return R;
	}

	Optional<RegisterID> FindIRInRegister(const IRInstruction* Value);
	Optional<RegisterID> FindValueInRegister(AnyInt64 Value);

	void FreeRegister(RegisterID ID)
	{
		_Registers.FreeRegister(ID);
	}

	IRlocData GetIRLocData(const IRInstruction* Ins, bool GetAddress = false);
	IRlocData GetIRLocData(const IRInstruction* Ins, const IROperator& Op, bool GetAddress = false);
	IRlocData GetIRLocData(const IROperator& Op, bool GetAddress = false);
	void CopyValues(const IRlocData& Src, const IRlocData& Out, bool DerefSrc = false, bool DerefOut = false);

	RegisterID MakeIntoRegister(const IRlocData& Value, Optional<RegisterID> RegisterToPut = {});
	RegisterID MakeIntoRegister(const IRInstruction* Ins, const IROperator& Op, Optional<RegisterID> RegisterToPut = {})
	{
		auto Value = GetIRLocData(Ins, Op);
		return MakeIntoRegister(Value, RegisterToPut);
	}
	void GiveNameTo(const IRlocData& Value, const IRInstruction* Name);

	IRlocData GetPointerOf(const IRlocData& Value);

	RegisterID LoadOp(const IRInstruction* Ins, const IROperator& Op);
	void LoadOpToReg(const IRInstruction* Ins, const IROperator& Op, RegisterID Out);
	void RegToReg(IRTypes Type, RegisterID In, RegisterID Out, bool IsCopy)
	{
		IRType v;
		v._Type = Type;
		UCodeLangAssert(Type != IRTypes::IRsymbol);
		RegToReg(v, In, Out, IsCopy);
	}
	void RegToReg(IRType Type, RegisterID In, RegisterID Out, bool IsCopy);
	void PushOpStack(const IRInstruction* Ins, const IROperator& Op);
	void LogicalNot(IRTypes Type, RegisterID In, RegisterID Out);
	void BuildLink(const IRidentifier& FuncName, IRFuncLink LinkType);

	void CopyValueToStack(const IRInstruction* IRName, const IRType& ObjectType, RegisterID Item);

	void MoveValueToStack(const IRInstruction* IRName, const IRType& ObjectType, RegisterID Item);

	struct MemberAccessOffsetInfo
	{
		// Base->X.Y
		// ^
		IROperator Base;
		size_t Offset = 0;
		IRType FieldType;
	};
	Optional<MemberAccessOffsetInfo> GetMemberAccessDereferenceOffset(const IRInstruction* VIns);

	IRlocData GetIRLocData(MemberAccessOffsetInfo info, bool GetAddress);

	void StoreValue(const IRInstruction* Ins, const  IROperator& OutputLocationIR, const IROperator& Input);

	void StoreValueInPointer(const IRType& ObjectType, RegisterID Pointer, const  IROperator& Value, IRInstruction* Ins);
	void StoreValueInPointer(const IRType& ObjectType, RegisterID Pointer, RegisterID Value);
	void StoreValueInPointer(RegisterID Pointer, size_t Pointerofset, const IRlocData& Value);
	void StoreValueInPointer(RegisterID Pointer, const IRlocData& Value)
	{
		StoreValueInPointer(Pointer, 0, Value);
	}

	void MoveRegInValue(RegisterID Value, const IRlocData& To, size_t Offset);
	void MoveValueInReg(const IRlocData& Value, size_t Offset, RegisterID To);

	RegisterID ReadValueFromPointer(const IRType& ObjectType, RegisterID Pointer);

	void ReadValueFromPointer(RegisterID Pointer, size_t Pointerofset, const IRlocData& Out);

	void BuildSIntToIntCast(const IRInstruction* Item, const IROperator& Op, size_t IntSize);
	void BuildUIntToIntCast(const IRInstruction* Item, const IROperator& Op, size_t IntSize);

	RegisterID GetRegisterOut(const IRInstruction* Item);


	FuncCallEndData FuncCallStart(const Vector<IRType>& Pars, const IRType& RetType);
	FuncCallEndData FuncCallStart(const Vector<IRPar>& Pars, const IRType& RetType);
	void FuncCallRet(const IRType& RetType);
	void FuncCallEnd(FuncCallEndData& Data);

	void AddOffset(IRlocData& Pos, size_t Offset);

	FindParsLoc GetParsLoc(const Vector<IRType>& Pars, bool SetReg = false);
	FindParsLoc GetParsLoc(const Vector<IRPar>& Pars, bool SetReg = false);

	AnyInt64 ToAnyInt(const IRType& ObjectType, const  IROperator& Op);

	size_t GetStackOffset(const ParlocData& ItemStackOffset)
	{
		if (ItemStackOffset.Location.Is<StackPostCall>())
		{
			return GetPostCallStackOffset(ItemStackOffset.Location.Get<StackPostCall>().Offset);
		}
		else
		{
			return GetPreCallStackOffset(ItemStackOffset.Location.Get<StackPreCall>().Offset);
		}
	}
	size_t GetPostCallStackOffset(size_t ItemStackOffset)
	{
		return ItemStackOffset;
	}

	size_t PointerSize()
	{
		return Get_Settings().PtrSize == IntSizes::Int64 ? 8 : 4;
	}

	size_t GetPreCallStackOffset(size_t ItemStackOffset)
	{
		size_t FuncPointerSize = Get_Settings().PtrSize == IntSizes::Int64 ? 8 : 4;
		return  ItemStackOffset + FuncPointerSize + 4;
	}
	IRlocData_StackPost GetFreeStackPos(IRType V);
	IRlocData GetFreeStackLoc(IRType V);

	IRType GetType(StackItem& item)
	{
		return GetType(item.IR.Get<const IRInstruction*>());
	}

	NeverNullPtr<ParlocData> GetPreCallPar(size_t offset)
	{
		for (auto& Item : CurrentFuncParPos)
		{
			if (auto val = Item.Location.Get_If<StackPreCall>())
			{
				auto size = GetSize(Item.Par->type);
				if (offset >= val->Offset && offset < val->Offset + size)
				{
					return NeverNullptr(&Item);
				}
			}
		}
		UCodeLangUnreachable();
	}

	size_t GetPreCallStackOffset2(size_t ItemStackOffset)
	{
		auto  par = GetPreCallPar(ItemStackOffset);

		auto mainobjsize = GetSize(par->Par->type);

		size_t paroffset = par->Location.Get<StackPreCall>().Offset;
		size_t FeildOffset = ItemStackOffset - paroffset;
		//return paroffset + mainobjsize - FeildOffset + _Stack.PushedOffset;
		return paroffset + mainobjsize - FeildOffset;
	
	}

	//AddDebuginfo
	UCodeLang::ULangDebugInfo _DebugInfo;
	bool IsDebugMode()
	{
		return (OptimizationFlags_t)Get_Settings()._Flags & (OptimizationFlags_t)OptimizationFlags::Debug;
	}

	void Add_SetFile(const String& file, size_t InsInBlock)
	{
		if (IsDebugMode())
		{
			_DebugInfo.Add_SetFile(file, InsInBlock);
		}
	}
	void Add_SetLineNumber(size_t LineNumber, size_t InsInBlock)
	{
		if (IsDebugMode())
		{
			_DebugInfo.Add_SetLineNumber(LineNumber, InsInBlock);
		}
	}
	void Add_SetVarableName(const String& Name, VarableInfo& Info)
	{
		if (IsDebugMode())
		{
			_DebugInfo.Add_SetVarableName(Name, std::move(Info));
		}
	}


	RegistersManager _OldVarableLocRegisters;
	void ClearVarableLocs();
	void UpdateVarableLocs();

	void MoveValuesToState(const RegistersManager& state);
	RegistersManager SaveState();

	void SynchronizePars();
};
UCodeLangEnd

#endif
