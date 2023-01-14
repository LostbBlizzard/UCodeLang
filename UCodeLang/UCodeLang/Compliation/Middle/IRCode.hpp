#pragma once

#include "UCodeLang/Compliation/CompliationSettings.hpp"
#include "SymbolID.hpp"
UCodeLangStart

using IRField = size_t;
constexpr IRField NullIRField = 0;

#define IROperatorIntSet(Bit) \
Add##Bit,\
Sub##Bit,\


enum class IROperator : UInt8
{
	Null,
	Assign_Operand0,
	Func,
	Ret,
	IROperatorIntSet(8)
	IROperatorIntSet(16)
	IROperatorIntSet(32)
	IROperatorIntSet(64)
	Ret_Value,
	FuncCall,
	Malloc,
	Free,
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
	ReadVar
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
#define IROperand_Set(x) \
	UCodeLangForceinline static IROperand AsInt##x(UInt##x Value)\
	{\
		IROperand operand;\
		operand.Type = IRFieldInfoType::Int##x;\
		operand.AnyValue = Value;\
		return operand;\
	}\

	IROperand_Set(8);
	IROperand_Set(16);
	IROperand_Set(32);
	IROperand_Set(64);

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
	UCodeLangForceinline static IROperand AsReadVarable(SymbolID Value)
	{
		IROperand operand;
		operand.Type = IRFieldInfoType::ReadVar;
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

#define IRBuilder_Set(X) \
	UCodeLangForceinline void MakeAdd##X(IROperand field, IROperand field2) \
	{ \
		MakeOperand(field, field2, IROperator::Add##X);\
	}\
	UCodeLangForceinline void MakeSub##X(IROperand field, IROperand field2)\
	{\
		MakeOperand(field, field2, IROperator::Sub##X);\
	}\

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
		V.Operand1 = field2;
		V.Operator = Op;

	}
	IRBuilder_Set(8);
	IRBuilder_Set(16);
	IRBuilder_Set(32);
	IRBuilder_Set(64);

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

	void Build_AssignRet(IROperand field)
	{
		Code.push_back({});
		auto& V = Code.back();
		V.Result = IROperand::AsLocation(Code.size() - 1);
		V.Operand0 = field;
		V.Operator = IROperator::Ret_Value;
	}

	void Build_Func(SymbolID Value)
	{
		Code.push_back({});
		auto& V = Code.back();
		V.Operator = IROperator::Func;
		V.Operand0 = IROperand::AsSymbol(Value);
	}
	void Build_FuncCall(SymbolID Value)
	{
		Code.push_back({});
		auto& V = Code.back();
		V.Operator = IROperator::FuncCall;
		V.Operand0 = IROperand::AsSymbol(Value);
	}
	
	void Build_Malloc(UAddress Size)
	{
		Build_Assign(IROperand::AsInt64(Size));
		Build_Malloc(IROperand::AsLocation(GetLastField()));
	}
	void Build_Malloc(IROperand Sizefield)
	{
		Code.push_back({});
		auto& V = Code.back();
		V.Result = IROperand::AsLocation(Code.size() - 1);
		V.Operator = IROperator::Malloc;
		V.Operand0 = Sizefield;
	}

	void Build_Free(IROperand field)
	{
		Code.push_back({});
		auto& V = Code.back();
		V.Operator = IROperator::Free;
		V.Operand0 = field;
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