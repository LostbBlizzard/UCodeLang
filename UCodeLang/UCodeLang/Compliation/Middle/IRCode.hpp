#pragma once

#include "UCodeLang/Compliation/CompliationSettings.hpp"
#include "SymbolID.hpp"
UCodeLangStart

using IRField = size_t;
constexpr IRField NullIRField = 0;
enum class IROperator : UInt8
{
	Null,
	Assign_Operand0,
	Add8,

	Func,
	Ret,
};

enum class IRFieldInfoType : UInt8
{
	Null,
	Int8,
	Int16,
	Int32,
	Int64,

	IRLocation,
	Var,
	SymbolID,
};
struct IROperand
{
	IRFieldInfoType Type;

	union
	{
		AnyInt64 AnyValue;
		IRField IRLocation;
		SymbolID SymbolId;
	};
	IROperand()
	{
		Type = IRFieldInfoType::Null;
		AnyValue = (UInt64)0;
	}
	UCodeLangForceinline static IROperand AsInt8(UInt8 Value)
	{
		IROperand operand;
		operand.Type = IRFieldInfoType::Int8;
		operand.AnyValue = Value;
		return operand;
	}

	UCodeLangForceinline static IROperand AsSymbol(SymbolID Value)
	{
		IROperand operand;
		operand.Type = IRFieldInfoType::SymbolID;
		operand.SymbolId = Value;
		return operand;
	}
	UCodeLangForceinline static IROperand AsLocation(IRField Value)
	{
		IROperand operand;
		operand.Type = IRFieldInfoType::IRLocation;
		operand.IRLocation = Value;
		return operand;
	}
	UCodeLangForceinline static IROperand AsVarable(SymbolID Value)
	{
		IROperand operand;
		operand.Type = IRFieldInfoType::Var;
		operand.SymbolId = Value;
		return operand;
	}
};
struct IRThreeAddressCode
{
	IROperand Result;

	IROperand Operand0;
	IROperator Operator = IROperator::Null;
	IROperand Operand1;
};

struct IRSeg
{
	size_t Start = 0;
	size_t End = 0;
	UCodeLangForceinline auto GetSize()
	{
		return  End - Start;
	}
};
class IRBuilder
{
public:
	IRBuilder(){}
	~IRBuilder(){}


	
	void MakeOperand( IROperand field, IROperand field2, IROperator Op)
	{
		Code.push_back({});
		auto& V = Code.back();
		V.Result =IROperand::AsLocation(GetLastField());
		V.Operand0 = field;
		V.Operand1 = field;
		V.Operator = Op;

	}
	UCodeLangForceinline void MakeAdd8(IROperand field, IROperand field2)
	{
		MakeOperand(field,field2,IROperator::Add8);
	}

	void Build_Assign(IROperand result, IROperand field)
	{
		Code.push_back({});
		auto& V = Code.back();
		V.Result =result;
		V.Operand0 = field;
		V.Operator = IROperator::Assign_Operand0;
	}
	void Build_Assign(IROperand field)
	{
		Code.push_back({});
		auto& V = Code.back();
		V.Result =IROperand::AsLocation(Code.size() -1);
		V.Operand0 = field;
		V.Operator = IROperator::Assign_Operand0;
	}

	void Build_Func(SymbolID Value)
	{
		Code.push_back({});
		auto& V = Code.back();
		V.Operator = IROperator::Func;
		V.Operand0 = IROperand::AsSymbol(Value);
	}

	void Build_Ret()
	{
		Code.push_back({});
		auto& V = Code.back();
		V.Operator = IROperator::Ret;
	}
	

	void Reset() { Code.clear(); }


	UCodeLangForceinline IRThreeAddressCode& Get_IR(IRField field)
	{
		return Code[(size_t)field];
	}
	IRField GetLastField()
	{
		return Code.size() - 1;
	}
	inline auto& Get_Code()
	{
		return Code;
	}
private:
	Vector<IRThreeAddressCode> Code;
};

UCodeLangEnd