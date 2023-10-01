#pragma once
#include "X86_64IR.hpp"
#include "../../Middle/IR.hpp"

#if UCodeLangDebug
#include <iostream>
#endif // DEBUG

UCodeLangStart
class IRToX86_64IR
{
public:
	IRToX86_64IR(){}
	~IRToX86_64IR() {};
	static X86_64IR Into(const IRBuilder& builder, X86_64IR::CleanUpMode mode = X86_64IR::CleanUpMode::None)
	{
		X86_64IR r;

		IRToX86_64IR c;
		c.Build(&builder,mode);
		
		r = std::move(c.Get_Output());
		return r;
	}
	void Reset();
	void Build(const IRBuilder* builder, X86_64IR::CleanUpMode mode = X86_64IR::CleanUpMode::None);
	X86_64IR& Get_Output()
	{
		return _Out;
	}
private:
	X86_64IR _Out;
	const IRBuilder* _Input =nullptr;

	X86_64IR::CallConventionID _MainCallConvention;
	X86_64IR::CallConventionID _InternalCallConvention;
	Vector<X86_64IR::CallConventionID> OtherCallConventions;

	ImportUseing86x64Gen
	
	struct PostStackOffset
	{
		size_t Offset = 0;
		explicit PostStackOffset(size_t offset = 0):Offset(offset){}
	};
	struct IRLoc
	{
		IRType type;
		Variant<GReg, PostStackOffset> LocTypes;
	};
	struct None{};
	struct RegInfo
	{
		Variant<None, const IRInstruction*,AnyInt64> Type;
	};
	UnorderedMap<GReg, RegInfo> TemVarRegs;
	UnorderedMap<const IRInstruction*, IRLoc> IRLocs;
	X86_64IR::Func* OutFunc=nullptr;
	const IRFunc* OnFunc = nullptr;

	//code
	bool IsPrimitive(IRType& type)
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


	void Build(const IRFunc* func);
	void Build(const IRBlock* func);

	IRLoc GetLoc(const IROperator& ins)
	{
		return {};
	}

	IRLoc GetLoc(const IRInstruction* ins)
	{
		return IRLocs.at(ins);
	}

	Optional<GReg> Get_FreeReg()
	{
		for (auto& Item : TemVarRegs)
		{
			if (Item._Value.Type.Is<None>())
			{
				return Item._Key;
			}
		}
		return {};
	}
	GReg Get_FreeRegNoOp()
	{
		return  Get_FreeReg().value();
	}

	void SetInfo(GReg Reg, const IRInstruction* Value)
	{
		TemVarRegs.at(Reg).Type = Value;
	}
	void SetInfo(GReg Reg, const IROperator& Value)
	{
		if (Value.Type == IROperatorType::Value)
		{
			TemVarRegs.at(Reg).Type = Value.Value;
		}
		else
		{
			UCodeLangUnreachable();
		}
		//TemVarRegs.at(Reg).Type = Value;
	}
	void SetInfo(GReg Reg, const IRLoc& Value)
	{
		//TemVarRegs.at(Reg).Type = Value;

	}

	void MoveToReg(IRLoc& loc, Optional<GReg> Reg = {})
	{

	}
	static X86_64IR::Ins::InsRegSize ToInsRegSize(const IRType& type)
	{
		switch (type._Type)
		{
		case IRTypes::i8:return X86_64IR::Ins::InsRegSize::bits8;
		case IRTypes::i16:return X86_64IR::Ins::InsRegSize::bits16;
		case IRTypes::i32:return X86_64IR::Ins::InsRegSize::bits32;
		case IRTypes::i64:return X86_64IR::Ins::InsRegSize::bits64;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
	IRLoc LoadOp(const IRInstruction* Value, const IROperator& Op);

};

UCodeLangEnd