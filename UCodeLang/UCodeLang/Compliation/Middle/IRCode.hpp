#pragma once

#include "UCodeLang/Compliation/CompliationSettings.hpp"
#include "SymbolID.hpp"
UCodeLangStart

using IRField = size_t;
constexpr IRField NullIRField = 0;

#define IROperatorIntSet(Bit) \
Add##Bit,\
Sub##Bit,\
MultU##Bit,\
MultS##Bit,\
DivU##Bit,\
DivS##Bit,\



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
	Func_Parameter,
	DLLJump,
	PassParameter,
	IfFalseJump,
	Jump,
	Assign_OperandOnPointer
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
	ReadVar,
	AsPointer,
	ReadPointer,
	Nothing,
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
	UCodeLangForceinline static IROperand AsInt##x(Int##x Value)\
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


	UCodeLangForceinline static IROperand AsPointer(SymbolID Value)
	{
		IROperand operand;
		operand.Type = IRFieldInfoType::AsPointer;
		operand.SymbolId = Value;
		return operand;
	}


	UCodeLangForceinline static IROperand AsReadPointer(SymbolID Value)
	{
		IROperand operand;
		operand.Type = IRFieldInfoType::ReadPointer;
		operand.SymbolId = Value;
		return operand;
	}

	UCodeLangForceinline static IROperand AsNothing()
	{
		IROperand operand;
		operand.Type = IRFieldInfoType::Nothing;
		return operand;
	}
};


struct IRCode
{
	struct TypeSybol
	{
		size_t TypeSize=0;
		String_view Name;
	};
	IROperand Result;

	IROperand Operand0;
	IROperator Operator = IROperator::Null;
	IROperand Operand1;

	TypeSybol InfoType;
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
		auto& V2 = GetLast_IR();	\
		V2.InfoType.TypeSize =sizeof(Int##X);\
	}\
	UCodeLangForceinline void MakeSub##X(IROperand field, IROperand field2)\
	{\
		MakeOperand(field, field2, IROperator::Sub##X);\
		auto& V2 = GetLast_IR();	\
		V2.InfoType.TypeSize =sizeof(Int##X);\
	}\
	UCodeLangForceinline void MakeUMult##X(IROperand field, IROperand field2)\
	{\
		MakeOperand(field, field2, IROperator::MultU##X);\
		auto& V2 = GetLast_IR();	\
		V2.InfoType.TypeSize =sizeof(Int##X);\
	}\
	UCodeLangForceinline void MakeSMult##X(IROperand field, IROperand field2)\
	{\
		MakeOperand(field, field2, IROperator::MultS##X);\
		auto& V2 = GetLast_IR();	\
		V2.InfoType.TypeSize =sizeof(Int##X);\
	}\
	UCodeLangForceinline void MakeUDiv##X(IROperand field, IROperand field2)\
	{\
		MakeOperand(field, field2, IROperator::DivU##X);\
		auto& V2 =GetLast_IR();	\
		V2.InfoType.TypeSize =sizeof(Int##X);\
	}\
	UCodeLangForceinline void MakeSDiv##X(IROperand field, IROperand field2)\
	{\
		MakeOperand(field, field2, IROperator::DivS##X);\
		auto& V2 = GetLast_IR();	\
		V2.InfoType.TypeSize =sizeof(Int##X);\
	}\
	UCodeLangForceinline void Build_Increment##X(IROperand field,Int##X Value)\
	{\
		Build_Assign(IROperand::AsInt##X(Value));\
		MakeAdd##X(field, IROperand::AsLocation(GetLastField()));\
	}\
	UCodeLangForceinline void Build_Increment##X(IROperand field,UInt##X Value)\
	{\
		Build_Assign(IROperand::AsInt##X(Value));\
		MakeAdd##X(field, IROperand::AsLocation(GetLastField()));\
	}\
	UCodeLangForceinline void Build_Increment##X(UInt##X Value)\
	{\
		Build_Increment##X(IROperand::AsLocation(GetLastField()),Value);\
	}\
	UCodeLangForceinline void Build_Increment##X(Int##X Value)\
	{\
		Build_Increment##X( IROperand::AsLocation(GetLastField()),Value);\
	}\
	UCodeLangForceinline void Build_Decrement##X(IROperand field, UInt##X Value)\
	{\
		Build_Assign(IROperand::AsInt##X(Value));\
		MakeSub##X(field, IROperand::AsLocation(GetLastField()));\
	}\
	UCodeLangForceinline void Build_Decrement##X(IROperand field, Int##X Value)\
	{\
		Build_Assign(IROperand::AsInt##X(Value));\
		MakeSub##X(field, IROperand::AsLocation(GetLastField()));\
	}\
	UCodeLangForceinline void Build_Decrement##X(UInt##X Value)\
	{\
		Build_Decrement##X(IROperand::AsLocation(GetLastField()),Value);\
	}\
	UCodeLangForceinline void Build_Decrement##X(Int##X Value)\
	{\
		Build_Decrement##X(IROperand::AsLocation(GetLastField()),Value);\
	}\

class IRBuilder
{
public:
	IRBuilder(){}
	~IRBuilder(){}
	
	void MakeOperand( IROperand field, IROperand field2, IROperator Op)
	{
		auto& V = Code.emplace_back();
		V.Result =IROperand::AsLocation(GetLastField());
		V.Operand0 = field;
		V.Operand1 = field2;
		V.Operator = Op;

	}
	IRBuilder_Set(8);
	IRBuilder_Set(16);
	IRBuilder_Set(32);
	IRBuilder_Set(64);

	void Build_Assign(IROperand result, IROperand field, UAddress offset = 0)
	{
		auto& V = Code.emplace_back();
		V.Result =result;
		V.Operand0 = field;
		V.Operand1 = IROperand::AsInt64(offset);
		V.Operator = IROperator::Assign_Operand0;
	}
	void Build_Assign(IROperand field,UAddress offset = 0)
	{
		Build_Assign(IROperand::AsLocation(Code.size()), field,offset);
	}

	void Build_AssignOnPointer(IROperand pointer, IROperand field, UAddress pointeroffset = 0)
	{
		auto& V = Code.emplace_back();
		V.Result = pointer;
		V.Operand0 = field;
		V.Operand1 = IROperand::AsInt64(pointeroffset);
		V.Operator = IROperator::Assign_OperandOnPointer;
	}

	void Build_AssignRet(IROperand field)
	{
		auto& V = Code.emplace_back();
		V.Result = IROperand::AsLocation(GetLastField());
		V.Operand0 = field;
		V.Operator = IROperator::Ret_Value;
	}

	void Build_Func(SymbolID Value,String_view FuncName)
	{
		auto& V = Code.emplace_back();
		V.Operator = IROperator::Func;
		V.Operand0 = IROperand::AsSymbol(Value);
		V.InfoType.Name = FuncName;
	}
	void Build_Parameter(SymbolID Value)
	{
		auto& V = Code.emplace_back();
		V.Operator = IROperator::Func_Parameter;
		V.Operand0 = IROperand::AsSymbol(Value);
	}

	void Build_PassParameter(IROperand field)
	{
		auto& V = Code.emplace_back();
		V.Operator = IROperator::PassParameter;
		V.Operand0 = field;
	}
	void Build_PassLastAsParameter()
	{
		Build_PassParameter(IROperand::AsLocation(GetLastField()));
	}

	void Build_FuncCall(SymbolID Value)
	{
		auto& V = Code.emplace_back();
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
		auto& V = Code.emplace_back();
		V.Result = IROperand::AsLocation(GetLastField());
		V.Operator = IROperator::Malloc;
		V.Operand0 = Sizefield;
	}

	void Build_Free(IROperand field)
	{
		auto& V = Code.emplace_back();
		V.Operator = IROperator::Free;
		V.Operand0 = field;
	}

	void Build_IfFalseJump(IROperand Boolfield,IRField JumpToLoc)
	{
		auto& V = Code.emplace_back();
		V.Operator = IROperator::IfFalseJump;
		V.Operand0 = IROperand::AsLocation(JumpToLoc);
		V.Operand1 = Boolfield;
	}
	
	void Update_IfFalseJump(IRCode& V, IROperand Boolfield, IRField JumpToLoc)
	{
		V.Operator = IROperator::IfFalseJump;
		V.Operand0 = IROperand::AsLocation(JumpToLoc);
		V.Operand1 = Boolfield;
	}

	void Build_Jump(IRField JumpToLoc)
	{
		auto& V = Code.emplace_back();
		V.Operator = IROperator::Jump;
		V.Operand0 = IROperand::AsLocation(JumpToLoc);
	}

	void Update_Jump(IRCode& V, IRField JumpToLoc)
	{
		V.Operator = IROperator::Jump;
		V.Operand0 = IROperand::AsLocation(JumpToLoc);
	}



	void Build_Ret()
	{
		auto& V = Code.emplace_back();
		V.Operator = IROperator::Ret;
	}
	

	void Build_DLLJump(String_view FuncName)
	{
		Code.push_back({});
		auto& V = Code.back();
		V.Operator = IROperator::DLLJump;
		V.Operand0.AnyValue.AsPtr = (void*)FuncName.data();
		V.Operand1.AnyValue = FuncName.size();
	}

	void Reset() { Code.clear(); }


	UCodeLangForceinline IRCode& Get_IR(IRField field)
	{
		return Code[(size_t)field];
	}
	UCodeLangForceinline IRCode& GetLast_IR()
	{
		return Get_IR(GetLastField());
	}
	IRField GetLastField()
	{
		return Code.size() - 1;
	}
	IRField GetNextField()
	{
		return GetLastField() + 1;
	}
	inline auto& Get_Code()
	{
		return Code;
	}
	inline const auto& Get_Code() const
	{
		return Code;
	}

	String ToString();
private:
	Vector<IRCode> Code;
};

UCodeLangEnd