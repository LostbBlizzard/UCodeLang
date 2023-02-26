#pragma once
#include "UCodeLang/LangCore/LangTypes.hpp"
UCodeLangStart

using IRidentifier = String;
using IRidentifierID = size_t;
enum class IRTypes
{
	Null,
	Void,
	i8,
	i16,
	i32,
	i64,
	f32,
	f64,
	
	pointer,

	iRsymbol,
};
struct IRSymbol
{
	IRidentifier  ID;
};
struct IRType
{
	IRTypes _Type;
	IRSymbol _symbol;

	IRType():_Type(IRTypes::Null), _symbol(){}
	IRType(IRTypes type) :_Type(type), _symbol(){}
	IRType(IRSymbol symbol) :_Type(IRTypes::iRsymbol), _symbol(symbol){}

	bool IsType(IRTypes Type)
	{
		return Type == _Type;
	}
};

struct IRPar
{
	IRidentifierID identifier;
	IRType type;
};


using IRInstructionType_t = UInt8;
enum class IRInstructionType : IRInstructionType_t
{
	None,
	

	LoadNone,//just a valid location
	Load,//Load Target
	Reassign,//Reassign Target

	Call,//jump to Target 
	PushParameter,//Push Target
	LoadReturn,//Load Target as Return Value
	
	Return,//return out of func

	//Math
	Add,
	Sub,
	UMult,
	SMult,
	UDiv,
	SDiv,
	//bitwise
	BitWise_And,
	BitWise_Or,
	BitWise_XOr,
	BitWise_ShiftL,
	BitWise_ShiftR,
	BitWise_Not,
	//Logical
	Logical_And,
	Logical_Or,
	Logical_Not,
	//Comparison
	EqualTo,
	NotEqualTo,
	ULessThan,
	UGreaterThan,
	ULessThanOrEqual,
	UGreaterThanOrEqual,
	
	SLessThan,
	SGreaterThan,
	SLessThanOrEqual,
	SGreaterThanOrEqual,

	//
	Jump,
	ConditionalJump,
	//memory

	//internal stuff
};

inline bool IsCommutative(IRInstructionType Value)
{
	return Value == IRInstructionType::Add
		|| Value == IRInstructionType::UMult
		|| Value == IRInstructionType::SMult

		|| Value == IRInstructionType::EqualTo
		|| Value == IRInstructionType::NotEqualTo

		|| Value == IRInstructionType::Logical_And
		|| Value == IRInstructionType::Logical_Or;
}

inline bool IsBinary(IRInstructionType Value)
{
	return Value == IRInstructionType::Add
		|| Value == IRInstructionType::Sub
		|| Value == IRInstructionType::UMult
		|| Value == IRInstructionType::SMult
		|| Value == IRInstructionType::UDiv
		|| Value == IRInstructionType::SDiv
		
		|| Value == IRInstructionType::EqualTo
		|| Value == IRInstructionType::NotEqualTo

		|| Value == IRInstructionType::UGreaterThan
		|| Value == IRInstructionType::ULessThan
		|| Value == IRInstructionType::UGreaterThanOrEqual
		|| Value == IRInstructionType::ULessThanOrEqual

		|| Value == IRInstructionType::SGreaterThan
		|| Value == IRInstructionType::SLessThan
		|| Value == IRInstructionType::SGreaterThanOrEqual
		|| Value == IRInstructionType::SLessThanOrEqual

		|| Value == IRInstructionType::Logical_And
		|| Value == IRInstructionType::Logical_Or;
}
inline bool IsUnary(IRInstructionType Value)
{
	return Value == IRInstructionType::Logical_Not
		|| Value == IRInstructionType::BitWise_Not;
}
inline bool IsLoadValue(IRInstructionType Value)
{
	return Value == IRInstructionType::Load
		|| Value == IRInstructionType::LoadReturn
		|| Value == IRInstructionType::PushParameter
		|| Value == IRInstructionType::ConditionalJump;
}

using IROperator_t = UInt8;
enum class IROperatorType :IROperator_t
{
	Null,
	IRInstruction,
	IRidentifier,
	Value,
	IRParameter,

	Get_PointerOf_IRInstruction,//Gets the from the IRInstruction Pointer
};

struct IRPar;
struct IRInstruction;
struct IROperator
{
	IROperatorType Type;
	union 
	{
		IRInstruction* Pointer;
		AnyInt64 Value;
		IRidentifierID identifer;
		IRPar* Parameter;
	};

	IROperator():Type(IROperatorType::Null),Pointer(nullptr){}
	IROperator(IROperatorType type):Type(type), Pointer(nullptr) {}
	
	

	IROperator(IRInstruction* pointer):Type(IROperatorType::IRInstruction),Pointer(pointer){}
	IROperator(AnyInt64 value) :Type(IROperatorType::Value), Value(value) {}
	IROperator(IRidentifierID Value):Type(IROperatorType::IRidentifier), identifer(Value) {}
	IROperator(IRPar* Value) :Type(IROperatorType::IRParameter), Parameter(Value) {}
	
	IROperator(IROperatorType type,IRInstruction* pointer) :Type(type), Pointer(pointer) {}
	IROperator(IROperatorType type, IRidentifierID Value) :Type(type), identifer(Value) {}
};
struct IRInstruction
{
	IRInstruction():Type(IRInstructionType::None){}
	IRInstruction(IRInstructionType type) :Type(type){}
	IRInstruction(IRInstructionType type, IROperator Value):Type(type){Target(Value);}
	IRInstruction(IRInstructionType type, IROperator a, IROperator b) :Type(type), A(a),B(b){}
	IRInstructionType Type;


	IROperator A;
	IROperator B;

	IRType ObjectType;

	auto& Target()
	{
		return A;
	}
	void Target(const IROperator& Value)
	{
		A = Value;
	}

	auto& Input(){return B;}
	void Input(const IROperator& Value){B = Value;}

	void SetAsNone()
	{
		*this = IRInstruction();
	}
};
struct IRCall
{
	IRidentifier identifier;
	Vector<IRInstruction*> Parameters;
};


struct IRBlock
{

	//Loading Values
	IRInstruction* NewLoad(IRType Type)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::LoadNone)).get();
		V->ObjectType = Type;
		return V;
	}
	IRInstruction* NewLoad(IRType Type,const IROperator& Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, Value)).get();
		V->ObjectType = Type;
		return V;
	}

	
	//8bit
	IRInstruction* NewLoad(UInt8 Value)
	{
		AnyInt64 NewValue; NewValue = Value;

		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load,IROperator(NewValue))).get();
		V->ObjectType = IRType(IRTypes::i8);
		return V;
	}
	IRInstruction* NewLoad(Int8 Value) { return NewLoad(*(UInt8*)&Value); }
	IRInstruction* NewLoad(bool Value) { return NewLoad(*(UInt8*)&Value); }

	//16bit
	IRInstruction* NewLoad(UInt16 Value)
	{
		AnyInt64 NewValue; NewValue = Value;

		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(NewValue))).get();
		V->ObjectType = IRType(IRTypes::i16);
		return V;
	}
	IRInstruction* NewLoad(Int16 Value) { return NewLoad(*(UInt8*)&Value); }

	//32bit
	IRInstruction* NewLoad(UInt32 Value)
	{
		AnyInt64 NewValue; NewValue = Value;

		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(NewValue))).get();
		V->ObjectType = IRType(IRTypes::i32);
		return V;
	}
	IRInstruction* NewLoad(Int32 Value) { return NewLoad(*(UInt32*)&Value); }
	IRInstruction* NewLoad(float32 Value) 
	{
		AnyInt64 NewValue; NewValue = Value;

		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(NewValue))).get();
		V->ObjectType = IRType(IRTypes::f32);
		return V;
	}
	
	//64bit

	IRInstruction* NewLoad(UInt64 Value)
	{
		AnyInt64 NewValue; NewValue = Value;

		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(NewValue))).get();
		V->ObjectType = IRType(IRTypes::i64);
		return V;
	}
	IRInstruction* NewLoad(Int64 Value) { return NewLoad(*(UInt64*)&Value); }
	IRInstruction* NewLoad(float64 Value) 
	{
		AnyInt64 NewValue; NewValue = Value;

		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(NewValue))).get();
		V->ObjectType = IRType(IRTypes::f64);
		return V;
	}

	IRInstruction* NewLoad(IRInstruction* Value)
	{
		return Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(Value))).get();
	}


	IRInstruction* NewLoadPtr(IRInstruction* Value)//get the pointer of the Value
	{
		return Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(IROperatorType::Get_PointerOf_IRInstruction,Value))).get();
	}


	void NewStore(IRInstruction* Storage, IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Reassign, Storage)).get();
		V->Input(IROperator(Value));
	}
	
	void NewStorePtr(IRInstruction* Storage, IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Reassign, Storage)).get();
		V->Input(IROperator(IROperatorType::Get_PointerOf_IRInstruction,Value));
	}
	
	//math
	IRInstruction* NewAdd(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::Add,IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewSub(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::Sub, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewUMul(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::UMult, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewSMul(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::SMult, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewUDiv(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::UDiv, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewSDiv(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::SDiv, IROperator(A), IROperator(B))).get();
	}

	//comparison operators
	IRInstruction* NewC_Equalto(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::EqualTo, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewC_NotEqualto(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::NotEqualTo, IROperator(A), IROperator(B))).get();
	}

	IRInstruction* NewULessThan(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::ULessThan, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewUGreaterThan(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::UGreaterThan, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewULessThanOrEqual(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::ULessThanOrEqual, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewUGreaterThanOrEqual(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::UGreaterThanOrEqual, IROperator(A), IROperator(B))).get();
	}


	IRInstruction* NewSLessThan(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::SLessThan, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewSGreaterThan(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::SGreaterThan, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewSLessThanOrEqual(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::SLessThanOrEqual, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewSGreaterThanOrEqual(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::SGreaterThanOrEqual, IROperator(A), IROperator(B))).get();
	}
	//logical operators

	IRInstruction* NewlogicalAnd(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::Logical_And, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewlogicalOr(IRInstruction* A, IRInstruction* B)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::Logical_Or, IROperator(A), IROperator(B))).get();
	}
	IRInstruction* NewlogicalNot(IRInstruction* Value)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::Logical_Not, IROperator(Value))).get();
	}
	//control flow
	
	IRInstruction* NewJump(size_t JumpTo =0)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::Jump, IROperator(JumpTo))).get();
	}
	IRInstruction* NewConditionalJump(IRInstruction* Conditional, size_t JumpTo=0)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::ConditionalJump, IROperator(JumpTo), IROperator(Conditional))).get();
	}

	struct NewConditionalFalseJump_t
	{
		IRInstruction* logicalNot;
		IRInstruction* ConditionalJump;
	};

	NewConditionalFalseJump_t NewConditionalFalseJump(IRInstruction* Conditional, size_t JumpTo=0)
	{
		auto r = NewlogicalNot(Conditional);
		return { r, NewConditionalJump(r, JumpTo) };
	}

	void UpdateJump(IRInstruction* This, size_t JumpTo)
	{
		*This = IRInstruction(IRInstructionType::Jump, IROperator(JumpTo));
	}
	void UpdateConditionaJump(IRInstruction* This, IRInstruction* Conditional, size_t JumpTo)
	{
		*This = IRInstruction(IRInstructionType::ConditionalJump, IROperator(JumpTo), IROperator(Conditional));
	}

	//call func
	void NewPushParameter(IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::PushParameter, IROperator(Value))).get();
	}
	IRInstruction* NewCall(IRidentifierID identifier)
	{
		return Instructions.emplace_back(new IRInstruction(IRInstructionType::Call,IROperator(identifier))).get();
	}
	void NewRetValue(IRInstruction* Value)
	{
		auto& V = Instructions.emplace_back(new IRInstruction(IRInstructionType::LoadReturn, { Value }));	
	}
	void NewRet()
	{
		auto& V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Return));
	}
	//vetor

	Vector<Unique_ptr<IRInstruction>> Instructions;
	size_t GetIndex()
	{
		return Instructions.size()-1;
	}
};

struct IRFunc
{
	IRidentifierID identifier;
	IRType ReturnType;
	Vector<IRPar> Pars;
	IRFunc() : identifier(0)
	{

	}
	IRFunc(IRidentifierID Identifier) : identifier(Identifier)
	{

	}

	IRBlock* NewBlock(const IRidentifier& identifier)
	{
		return Blocks.emplace_back(new IRBlock()).get();
	}
	Vector<Unique_ptr<IRBlock>> Blocks;
};

class IRBuilder
{
public:
	
	IRFunc* NewFunc(IRidentifierID identifier, IRType ReturnType)
	{
		return Funcs.emplace_back(new IRFunc(identifier)).get();
	};
	IRFunc* NewFunc(const IRidentifier& identifier, IRType ReturnType)
	{
		return NewFunc(ToID(identifier), ReturnType);
	};

	IRidentifierID ToID(const IRidentifier& Value);
	IRidentifier FromID(IRidentifierID Value) const
	{
		return _Map.at(Value);
	}	

	Vector<Unique_ptr<IRFunc>> Funcs;
	Unordered_map<IRidentifierID, IRidentifier> _Map;
	void Reset();

	struct ToStringState
	{
		Unordered_map<IRInstruction*, String> PointerToName;
		Vector<IRInstruction*> TepPushedParameters;
		
		size_t StrValue=0;

		String GetName(IRInstruction* Ptr)
		{
			char r = 'A' + (char)StrValue;
			StrValue++;
			auto V = (String)String_view(&r, 1);
			PointerToName[Ptr] = V;
			return V;
		}
	};
	size_t GetSize(const IRType& Type);

	//uses UCodeLang syntax
	String ToString();
	String ToString(const IRType& Type);
	String ToString(ToStringState& State,IRInstruction& Ins, IROperator& Value);
	String ToStringBinary(ToStringState& State, IRInstruction* Ins, const char* V);
};

UCodeLangEnd