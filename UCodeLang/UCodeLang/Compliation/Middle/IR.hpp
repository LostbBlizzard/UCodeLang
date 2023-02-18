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
};

struct IRPar
{
	IRidentifier identifier;
	IRType type;
};


using IRInstructionType_t = UInt8;
enum class IRInstructionType : IRInstructionType_t
{
	None,
	Load,//Load Target
	Call,
	PushParameter,//Push Target
	LoadReturn,//Load Target as Return Value
	Return,//
};

using IROperator_t = UInt8;
enum class IROperatorType :IROperator_t
{
	Null,
	IRInstruction,
	IRidentifier,
	Value,
};

struct IRInstruction;
struct IROperator
{
	IROperatorType Type;
	union 
	{
		IRInstruction* Pointer;
		AnyInt64 Value;
		IRidentifierID identifer;
	};

	IROperator():Type(IROperatorType::Null),Pointer(nullptr){}
	IROperator(IROperatorType type):Type(type), Pointer(nullptr) {}
	
	IROperator(IRInstruction* pointer):Type(IROperatorType::IRInstruction),Pointer(pointer){}
	IROperator(AnyInt64 value) :Type(IROperatorType::Value), Value(value) {}
	IROperator(IRidentifierID Value):Type(IROperatorType::IRidentifier), identifer(Value) {}
};
struct IRInstruction
{
	IRInstruction():Type(IRInstructionType::None){}
	IRInstruction(IRInstructionType type) :Type(type){}
	IRInstruction(IRInstructionType type, IROperator Value):Type(type){Target(Value);}

	IRInstructionType Type;
	IROperator Storage;


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
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::None)).get();
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
		AnyInt64 NewValue = Value;

		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load,IROperator(NewValue))).get();
		V->ObjectType = IRType(IRTypes::i8);
		return V;
	}
	IRInstruction* NewLoad(Int8 Value) { return NewLoad(*(UInt8*)&Value); }
	IRInstruction* NewLoad(bool Value) { return NewLoad(*(UInt8*)&Value); }

	//16bit
	IRInstruction* NewLoad(UInt16 Value)
	{
		AnyInt64 NewValue = Value;

		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(NewValue))).get();
		V->ObjectType = IRType(IRTypes::i16);
		return V;
	}
	IRInstruction* NewLoad(Int8 Value) { return NewLoad(*(UInt8*)&Value); }

	//32bit
	IRInstruction* NewLoad(UInt32 Value)
	{
		AnyInt64 NewValue = Value;

		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(NewValue))).get();
		V->ObjectType = IRType(IRTypes::i32);
		return V;
	}
	IRInstruction* NewLoad(Int32 Value) { return NewLoad(*(UInt32*)&Value); }
	IRInstruction* NewLoad(float32 Value) 
	{
		AnyInt64 NewValue = Value;

		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(NewValue))).get();
		V->ObjectType = IRType(IRTypes::f32);
		return V;
	}
	
	//64bit

	IRInstruction* NewLoad(UInt64 Value)
	{
		AnyInt64 NewValue = Value;

		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(NewValue))).get();
		V->ObjectType = IRType(IRTypes::i64);
		return V;
	}
	IRInstruction* NewLoad(Int64 Value) { return NewLoad(*(UInt64*)&Value); }
	IRInstruction* NewLoad(float64 Value) 
	{
		AnyInt64 NewValue = Value;

		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(NewValue))).get();
		V->ObjectType = IRType(IRTypes::f64);
		return V;
	}

	void NewStore(IRInstruction* Storage, IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load,Value)).get();
		V->Target(Storage);
	}
	
	//math
	IRInstruction* NewAdd(IRInstruction* A, IRInstruction* B)
	{

	}
	IRInstruction* NewSub(IRInstruction* A, IRInstruction* B)
	{

	}
	IRInstruction* NewUMul(IRInstruction* A, IRInstruction* B)
	{

	}
	IRInstruction* NewSMul(IRInstruction* A, IRInstruction* B)
	{

	}
	IRInstruction* NewUDiv(IRInstruction* A, IRInstruction* B)
	{

	}
	IRInstruction* NewSDiv(IRInstruction* A, IRInstruction* B)
	{

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
	inline IRInstruction* Get_LastIR()
	{
		return Instructions.back().get();
	}
};

struct IRFunc
{
	IRidentifierID identifier;
	IRType ReturnType;
	Vector<IRPar> Pars;
	IRFunc()
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

	IRidentifierID ToID(const IRidentifier& Value)
	{
		auto r = std::hash<IRidentifier>()(Value);
		_Map[r] = Value;
		return r;
	}
	IRidentifier FromID(IRidentifierID Value) const
	{
		return _Map.at(Value);
	}	

	Vector<Unique_ptr<IRFunc>> Funcs;
	Unordered_map<IRidentifierID, IRidentifier> _Map;
	void Reset();
};

UCodeLangEnd