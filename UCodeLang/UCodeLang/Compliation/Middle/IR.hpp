#pragma once
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "UCodeLang/LangCore/DataType/BinaryVectorMap.hpp"
#include "UCodeLang/LangCore/BitMaker.hpp"
UCodeLangStart

using IRidentifier = String;
using IRidentifierID = UInt64;
using IRTypes_t = int;
enum class IRTypes :IRTypes_t
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

	IRsymbol,
};
struct IRSymbol
{
	IRidentifierID  ID;
	IRSymbol()
	{
		ID = 0;
	}
	IRSymbol(IRidentifierID V)
	{
		ID = V;
	}
};
struct IRType
{
	IRTypes _Type;
	IRSymbol _symbol;

	IRType():_Type(IRTypes::Null), _symbol(){}
	IRType(IRTypes type) { SetType(type); }
	IRType(IRSymbol symbol)  { SetType(symbol); }

	bool IsType(IRTypes Type)
	{
		return Type == _Type;
	}
	void SetType(IRTypes Value)
	{
		_Type = Value;
	}
	void SetType(IRSymbol Value)
	{
		_Type = IRTypes::IRsymbol;
		_symbol = Value;
	}
	bool IsSame(IRType Value) const
	{
		return _Type == Value._Type && _symbol.ID == Value._symbol.ID;
	}
};

struct IRPar
{
	IRidentifierID identifier=0;
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
	//
	CallFuncPtr,

	//memory
	MallocCall,
	FreeCall,
	Reassign_dereference,//Reassign Target

	//structs
	Member_Access,
	Member_Access_Dereference,

	//casting U
	UIntToUInt8,
	UIntToUInt16,
	UIntToUInt32,
	UIntToUInt64,

	SIntToSInt8,
	SIntToSInt16,
	SIntToSInt32,
	SIntToSInt64,

	UIntToSInt,
	SIntToUInt,
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
		|| Value == IRInstructionType::BitWise_Not
		
		|| Value == IRInstructionType::UIntToUInt8
		|| Value == IRInstructionType::UIntToUInt16
		|| Value == IRInstructionType::UIntToUInt32
		|| Value == IRInstructionType::UIntToUInt64
		
		|| Value == IRInstructionType::SIntToSInt8
		|| Value == IRInstructionType::SIntToSInt16
		|| Value == IRInstructionType::SIntToSInt32
		|| Value == IRInstructionType::SIntToSInt64
		
		|| Value == IRInstructionType::SIntToUInt
		|| Value == IRInstructionType::UIntToSInt;
}
inline bool IsLoadValue(IRInstructionType Value)
{
	return Value == IRInstructionType::Load
		|| Value == IRInstructionType::LoadReturn
		|| Value == IRInstructionType::PushParameter;
}
inline bool IsLoadValueOnInput(IRInstructionType Value)
{
	return Value == IRInstructionType::ConditionalJump;
}

inline bool IsLocation(IRInstructionType Value)
{
	return Value == IRInstructionType::Load
		|| Value == IRInstructionType::LoadNone
		|| Value == IRInstructionType::Call
		|| Value == IRInstructionType::CallFuncPtr
		|| Value == IRInstructionType::MallocCall
		|| IsBinary(Value)
		|| IsUnary(Value);
}
inline bool IsAction(IRInstructionType Value)
{
	return Value == IRInstructionType::Return
		|| Value == IRInstructionType::FreeCall;
}

inline bool IsOperatorValueInTarget(IRInstructionType Value)
{
	return IsBinary(Value)
		|| IsUnary(Value)
		|| IsLoadValue(Value);
}
inline bool IsOperatorValueInInput(IRInstructionType Value)
{
	return IsBinary(Value)
		|| IsLoadValueOnInput(Value);
}



inline IRInstructionType GetInverse(IRInstructionType Value)
{
	switch (Value)
	{
	case IRInstructionType::BitWise_ShiftL:return IRInstructionType::BitWise_ShiftR;
	case IRInstructionType::BitWise_ShiftR:return IRInstructionType::BitWise_ShiftL;
	case IRInstructionType::Logical_Not:return IRInstructionType::Logical_Not;
	case IRInstructionType::EqualTo:return IRInstructionType::NotEqualTo;
	case IRInstructionType::NotEqualTo:return IRInstructionType::EqualTo;
	case IRInstructionType::UIntToSInt:return IRInstructionType::SIntToUInt;
	case IRInstructionType::SIntToUInt:return IRInstructionType::UIntToSInt;
	default:return IRInstructionType::None;
	}
}
inline bool HasInverse(IRInstructionType Value)
{
	return GetInverse(Value) != IRInstructionType::None;
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
	Get_PointerOf_IRParameter,
	Get_PointerOf_IRidentifier,//like static objects

	DereferenceOf_IRInstruction,
	DereferenceOf_IRParameter,

	Get_Func_Pointer,
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
	IROperator(IROperatorType type, IRPar* Value) :Type(type), Parameter(Value) {}
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
	const auto& Target()const
	{
		return A;
	}
	void Target(const IROperator& Value)
	{
		A = Value;
	}

	auto& Input(){return B;}
	const auto& Input()const { return B; }
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

//
using IRCallConvention_t = int;
enum class IRCallConvention :IRCallConvention_t
{
	BackEndChoice,
	Cdecl,
	StdCall,
	Fastcall,
	UCCall = Fastcall,
};
struct IRSymbol_Ex
{
	virtual ~IRSymbol_Ex() {}
};


struct IRStructField
{
	IRType Type;
	Optional<size_t> Offset;
};

struct IRStruct : IRSymbol_Ex
{
	Vector<IRStructField> Fields;
	size_t ObjectSize = 0;
	bool IsUnion = false;

	void AddField(IRType Type, size_t ObjectSize)
	{
		IRStructField V;
		V.Type = Type;
		V.Offset = ObjectSize;
		Fields.emplace_back(V);
		this->ObjectSize += ObjectSize;
	};
};



struct IRFuncPtr : IRSymbol_Ex
{
	IRCallConvention CallConvention = IRCallConvention::BackEndChoice;
	Vector<IRType> Pars;
	IRType Ret;
};


struct IRStaticArray : IRSymbol_Ex
{
	IRType Type;
	size_t Count=0;
};


struct IRBufferData : IRSymbol_Ex
{
	Vector<Byte> Bytes;
	bool IsInitialized = false;
};

//

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
	IRInstruction* NewLoad_IRID(const IRidentifierID& Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(Value))).get();
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
	IRInstruction* NewLoad(char Value) { return NewLoad(*(UInt8*)&Value); }

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
	IRInstruction* NewLoad(IRPar* Value)
	{
		return Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(Value))).get();
	}

	IRInstruction* NewLoadPtr(IRInstruction* Value)//get the pointer of the Value
	{
		return Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(IROperatorType::Get_PointerOf_IRInstruction,Value))).get();
	}
	IRInstruction* NewLoadPtr(IRPar* Value)//get the pointer of the Value
	{
		return Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(IROperatorType::Get_PointerOf_IRParameter, Value))).get();
	}

	IRInstruction* NewLoadPtr(IRidentifierID Value)//get the pointer of the static object or similar.
	{
		return Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(IROperatorType::Get_PointerOf_IRidentifier, Value))).get();
	}

	void NewStore(IRPar* Storage, IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Reassign, Storage)).get();
		V->Input(IROperator(Value));
	}

	void NewStore(IRInstruction* Storage, IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Reassign, Storage)).get();
		V->Input(IROperator(Value));
	}

	void NewStore(IRidentifierID Storage, IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Reassign, Storage)).get();
		V->Input(IROperator(Value));
	}
	
	void NewStore(IROperator Storage, IRInstruction* Value)
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
	//funcptr
	IRInstruction* NewLoadFuncPtr(IRidentifierID identifier)
	{
		return Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(IROperatorType::Get_Func_Pointer,identifier))).get();
	}
	IRInstruction* NewCallFuncPtr(IRInstruction* FuncPtr)
	{
		return Instructions.emplace_back(new IRInstruction(IRInstructionType::CallFuncPtr, IROperator(FuncPtr))).get();
	}
	IRInstruction* NewCallFuncPtr(IRPar* FuncPtr)
	{
		return Instructions.emplace_back(new IRInstruction(IRInstructionType::CallFuncPtr, IROperator(FuncPtr))).get();
	}
	
	//mem
	IRInstruction* NewMallocCall(IRInstruction* Size)
	{
		return  Instructions.emplace_back(new IRInstruction(IRInstructionType::MallocCall, IROperator(Size))).get();
	}
	void NewFreeCall(IRInstruction* Ptr)
	{
		Instructions.emplace_back(new IRInstruction(IRInstructionType::FreeCall, IROperator(Ptr)));
	}

	void NewDereferenc_Store(IRInstruction* Ptr,IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Reassign_dereference, Ptr)).get();
		V->Input(IROperator(Value));
	}
	void NewDereferenc_Store(IRPar* Ptr, IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Reassign_dereference, Ptr)).get();
		V->Input(IROperator(Value));
	}
	void NewDereferenc_Store(IRidentifierID Ptr, IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Reassign_dereference, Ptr)).get();
		V->Input(IROperator(Value));
	}
	void NewDereferenc_Store(IROperator Ptr, IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Reassign_dereference, Ptr)).get();
		V->Input(IROperator(Value));
	}

	IRInstruction* NewLoad_Dereferenc(IRInstruction* Ptr, IRType Type)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load,IROperator(IROperatorType::DereferenceOf_IRInstruction,Ptr))).get();
		V->ObjectType = Type;
		return V;
	}
	IRInstruction* NewLoad_Dereferenc(IRPar* Ptr, IRType Type)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Load, IROperator(IROperatorType::DereferenceOf_IRParameter, Ptr))).get();
		V->ObjectType = Type;
		return V;
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
	IRInstruction* New_Vetor_Index(IRInstruction* Ptr, IRInstruction* Index,UInt32 ObjectSize)
	{
		auto Tep = NewLoad(ObjectSize);
		return New_Index_Vetor(Ptr,Index, Tep);
	}
	IRInstruction* New_Vetor_Index(IRInstruction* Ptr, IRInstruction* Index, UInt64 ObjectSize)
	{
		auto Tep = NewLoad(ObjectSize);
		return New_Index_Vetor(Ptr, Index, Tep);
	}
	IRInstruction* New_Index_Vetor(IRInstruction* Ptr, IRInstruction* Index,IRInstruction* ObjectSize)
	{
		auto Offset = NewUMul(Index, ObjectSize);
		return NewAdd(Ptr, Offset);
	}
	//
	void New_Increment(IRInstruction* Object)
	{
		NewStore(Object,NewAdd(Object, NewLoad(1)));
	}
	void New_Decrement(IRInstruction* Object)
	{
		NewStore(Object, NewSub(Object, NewLoad(1)));
	}

	void New_Increment(IRInstruction* Object, IRInstruction* ValueToAdd)
	{
		NewStore(Object, NewAdd(Object, ValueToAdd));
	}
	void New_Decrement(IRInstruction* Object, IRInstruction* ValueToAdd)
	{
		NewStore(Object, NewSub(Object, ValueToAdd));
	}
	//casting
	IRInstruction* New_UIntToUInt8(IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::UIntToUInt8, IROperator(Value))).get();
		V->ObjectType = IRTypes::i8;
		return V;
	}
	IRInstruction* New_UIntToUInt16(IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::UIntToUInt16, IROperator(Value))).get();
		V->ObjectType = IRTypes::i16;
		return V;
	}
	IRInstruction* New_UIntToUInt32(IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::UIntToUInt32, IROperator(Value))).get();
		V->ObjectType = IRTypes::i32;
		return V;
	}
	IRInstruction* New_UIntToUInt64(IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::UIntToUInt64, IROperator(Value))).get();
		V->ObjectType = IRTypes::i64;
		return V;
	}

	IRInstruction* New_SIntToSInt8(IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::SIntToSInt8, IROperator(Value))).get();
		V->ObjectType = IRTypes::i8;
		return V;
	}
	IRInstruction* New_SIntToSInt16(IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::SIntToSInt16, IROperator(Value))).get();
		V->ObjectType = IRTypes::i16;
		return V;
	}
	IRInstruction* New_SIntToSInt32(IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::SIntToSInt32, IROperator(Value))).get();
		V->ObjectType = IRTypes::i32;
		return V;
	}
	IRInstruction* New_SIntToSInt64(IRInstruction* Value)
	{
		auto V = Instructions.emplace_back(new IRInstruction(IRInstructionType::SIntToSInt64, IROperator(Value))).get();
		V->ObjectType = IRTypes::i64;
		return V;
	}


	IRInstruction* New_UIntToSInt(IRInstruction* Value)
	{
		return Instructions.emplace_back(new IRInstruction(IRInstructionType::UIntToSInt, IROperator(Value))).get();
	}
	IRInstruction* New_SIntToUInt(IRInstruction* Value)
	{
		return Instructions.emplace_back(new IRInstruction(IRInstructionType::SIntToUInt, IROperator(Value))).get();
	}


	//struct
	IRInstruction* New_Member_Access(IRInstruction* ObjectSrc,const IRStruct* ObjectTypeofSrc,size_t MemberIndex)
	{
		auto& V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Member_Access,IROperator(ObjectSrc)));
		V->Input(MemberIndex);

		V->ObjectType = ObjectTypeofSrc->Fields[MemberIndex].Type;

		return V.get();
	}
	IRInstruction* New_Member_Access(IRPar* ObjectSrc, const IRStruct* ObjectTypeofSrc, size_t MemberIndex)
	{
		auto& V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Member_Access, IROperator(ObjectSrc)));
		V->Input(MemberIndex);

		V->ObjectType = ObjectTypeofSrc->Fields[MemberIndex].Type;
		return V.get();
	}

	IRInstruction* New_Member_Dereference(IRInstruction* ObjectSrc, const IRType Type, size_t MemberIndex)
	{
		auto& V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Member_Access_Dereference, IROperator(ObjectSrc)));
		V->Input(MemberIndex);

		V->ObjectType = Type;
		return V.get();
	}
	IRInstruction* New_Member_Dereference(IRPar* ObjectSrc, const IRType Type, size_t MemberIndex)
	{
		auto& V = Instructions.emplace_back(new IRInstruction(IRInstructionType::Member_Access_Dereference, IROperator(ObjectSrc)));
		V->Input(MemberIndex);

		V->ObjectType = Type;
		return V.get();
	}

	Vector<Unique_ptr<IRInstruction>> Instructions;
	size_t GetIndex()
	{
		return Instructions.size()-1;
	}
};


using IRFuncLink_t = int;
enum class IRFuncLink :IRFuncLink_t
{
	StaticLink,
	StaticExternalLink,
	DynamicExternalLink,
	
	StaticExport,
	DynamicExport,
};
struct IRFunc
{
	IRidentifierID identifier;
	IRCallConvention CallConvention = IRCallConvention::BackEndChoice;
	IRFuncLink Linkage = IRFuncLink::StaticLink;

	IRType ReturnType;
	Vector<IRPar> Pars;
	Vector<Unique_ptr<IRBlock>> Blocks;
	IRFunc() : identifier(0)
	{

	}
	IRFunc(IRidentifierID Identifier) : identifier(Identifier)
	{

	}
	IRPar* GetPar(IRidentifierID Id)
	{
		for (auto& Syb : Pars)
		{
			if (Syb.identifier == Id)
			{
				return &Syb;
			}
		}
		return nullptr;
	}

	const	IRPar* GetPar(IRidentifierID Id) const
	{
		for (auto& Syb : Pars)
		{
			if (Syb.identifier == Id)
			{
				return &Syb;
			}
		}
		return nullptr;
	}

	IRBlock* NewBlock(const IRidentifier& identifier)
	{
		return Blocks.emplace_back(new IRBlock()).get();
	}

	size_t InstructionCount()const
	{
		size_t I = 0;
		for (auto& Item : Blocks)
		{
			I += Item->Instructions.size();
		}
		return I;
	}

	bool HasInstructions() const
	{
		return InstructionCount() != 0;
	}
};


using IRSymbolType_t = int;
enum class IRSymbolType : IRSymbolType_t
{
	Null,
	StaticVarable,
	ThreadLocalVarable,
	Struct,
	FuncPtr,
	StaticArray,
};


struct IRSymbolData
{
	IRidentifierID identifier{};
	IRSymbolType SymType =IRSymbolType::Null;
	IRType Type;
	Unique_ptr<IRSymbol_Ex> Ex;
	template<typename T> T* Get_ExAs()
	{
		return (T*)Ex.get();
	}
	 template<typename T>const T* Get_ExAs() const
	{
		return (T*)Ex.get();
	}
};


class IRBuilder
{
public:
	IRFunc _StaticInit;
	IRFunc _StaticdeInit;
	IRFunc _threadInit;
	IRFunc _threaddeInit;
	Vector<Unique_ptr<IRFunc>> Funcs;
	Vector<Unique_ptr<IRSymbolData>> _Symbols;
	BinaryVectorMap<IRidentifierID, IRidentifier> _Map;
	VectorMap<String, IRidentifierID> ConstStaticStrings;



	struct ExternalVarable
	{
		IRBufferData* Pointer;
		IRidentifierID identifier;
	};

	ExternalVarable NewThreadLocalVarable(IRidentifierID identifier, IRType Type)
	{
		IRBufferData* V = new IRBufferData();

		IRSymbolData* r = new IRSymbolData();
		r->SymType = IRSymbolType::ThreadLocalVarable;
		r->identifier = identifier;
		r->Type = Type;
		r->Ex.reset(V);

		_Symbols.emplace_back(r);
		return { V, identifier };
	}
	ExternalVarable NewStaticVarable(IRidentifierID identifier, IRType Type)
	{
		IRBufferData* V = new IRBufferData();

		IRSymbolData* r = new IRSymbolData();
		r->SymType = IRSymbolType::StaticVarable;
		r->identifier = identifier;
		r->Type = Type;
		r->Ex.reset(V);

		_Symbols.emplace_back(r);
		return { V, identifier };
	}


	//note will not add a null char at the end.
	IRidentifierID FindOrAddConstStrings(String_view Buffer)
	{
		String VKey = (String)Buffer;
		if (ConstStaticStrings.HasValue(VKey))
		{
			return ConstStaticStrings.at(VKey);
		}
		IRidentifierID identifier = ToID(".Const.String:" + (String)Buffer);
		auto V = NewStaticVarable(identifier, IRType(IRTypes::i8));

		V.Pointer->IsInitialized = true;
		V.Pointer->Bytes.resize(Buffer.size());
		memcpy(V.Pointer->Bytes.data(), Buffer.data(), Buffer.size());

		ConstStaticStrings.AddValue(VKey, identifier);

		return identifier;
	}



	IRStruct* NewStruct(IRidentifierID identifier)
	{
		IRStruct* V = new IRStruct();
		IRSymbolData* r = new IRSymbolData();

		r->identifier = identifier;
		r->SymType = IRSymbolType::Struct;
		r->Ex.reset(V);
		r->Type.SetType({ identifier });

		_Symbols.emplace_back(r);
		return V;
	}
	IRStruct* NewUnion(IRidentifierID identifier)
	{
		auto r = NewStruct(identifier);
		r->IsUnion = true;
		return r;
	}

	IRFuncPtr* NewFuncPtr(IRidentifierID identifier, IRType ReturnType)
	{
		IRFuncPtr* V = new IRFuncPtr();
		V->Ret = ReturnType;
		IRSymbolData* r = new IRSymbolData();

		r->identifier = identifier;
		r->SymType = IRSymbolType::FuncPtr;
		r->Ex.reset(V);
		r->Type.SetType({ identifier });

		_Symbols.emplace_back(r);
		return V;
	}

	IRStaticArray* NewStaticArray(IRidentifierID identifier, IRType Type, size_t Count)
	{
		IRStaticArray* V = new IRStaticArray();
		V->Count = Count;
		V->Type = Type;
		IRSymbolData* r = new IRSymbolData();

		r->identifier = identifier;
		r->SymType = IRSymbolType::StaticArray;
		r->Ex.reset(V);
		r->Type.SetType({ identifier });

		_Symbols.emplace_back(r);
		return V;
	}


	IRFunc* NewFunc(IRidentifierID identifier, IRType ReturnType)
	{
		auto Item = Funcs.emplace_back(new IRFunc(identifier)).get();
		Item->ReturnType = ReturnType;
		return Item;
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

	IRSymbolData* GetSymbol(IRidentifierID Value)
	{
		for (auto& Item : _Symbols)
		{
			if (Value == Item->identifier)
			{
				return Item.get();
			}
		}
		return nullptr;
	}
	IRSymbolData* GetSymbol(IRSymbol Value)
	{
		return GetSymbol(Value.ID);
	}

	const IRSymbolData* GetSymbol(IRidentifierID Value) const
	{
		for (auto& Item : _Symbols)
		{
			if (Value == Item->identifier)
			{
				return Item.get();
			}
		}
		return nullptr;
	}
	const IRSymbolData* GetSymbol(IRSymbol Value) const
	{
		return GetSymbol(Value.ID);
	}
	const IRFunc* GetFunc(IRidentifierID Value) const
	{
		for (auto& Item : Funcs)
		{
			if (Value == Item->identifier)
			{
				return Item.get();
			}
		}
		return nullptr;
	}
	IRFunc* GetFunc(IRidentifierID Value)
	{
		for (auto& Item : Funcs)
		{
			if (Value == Item->identifier)
			{
				return Item.get();
			}
		}
		return nullptr;
	}

	void Reset();

	struct ToStringState
	{
		BinaryVectorMap<IRInstruction*, String> PointerToName;
		Vector<IRInstruction*> TepPushedParameters;

		size_t StrValue = 0;
		IRFunc* _Func = nullptr;

		String GetName(IRInstruction* Ptr)
		{
			char r = 'A' + (char)StrValue;
			StrValue++;
			auto V = (String)String_view(&r, 1);
			PointerToName[Ptr] = V;
			return V;
		}
	};


	//Helpers

	void AddField(IRStruct* V, IRType Type)
	{
		V->AddField(Type, GetSize(Type));
	}
	//uses UCodeLang syntax
	String ToString();
	void ToString(ToStringState& State, IRFunc* Item, String& r);
	String ToString(const IRType& Type);
	String ToString(ToStringState& State, IRInstruction& Ins, IROperator& Value);
	String ToStringBinary(ToStringState& State, IRInstruction* Ins, const char* V);
	//pre backends
	void Fix_Size(IRStruct* Struct);
	//for backends

	size_t GetSize(const IRType& Type)const;
	size_t GetSize(const IRStruct* Struct) const;
	size_t GetOffset(const IRStruct* Struct, size_t Index) const;

	IRType GetType(const IRInstruction* IR) const
	{
		return IR->ObjectType;
	}
	IRType GetType(const IROperator& IR) const
	{
		switch (IR.Type)
		{
		case IROperatorType::IRInstruction:
			return GetType(IR.Pointer);
		default:
			return {};
			break;
		}
	}

	bool IsTheSame(const IRType& Type0, const IRType& Type1)const
	{
		return Type0.IsSame(Type1);
	}
	bool IsTheSame(const IRFuncPtr* Func, const IRFuncPtr* Func2) const;
	bool IsTheSame(const IRFuncPtr* Func, const IRFunc* Func2)const;
	bool IsTheSame(const IRFunc* Func, const IRFunc* Func2)const;

	bool IsPrimitive(const IRType& Type) const;

	//
	IRBuilder();


	BytesPtr ToBytes() const;
	static bool FromBytes(IRBuilder& Out, const BytesView Bytes);
	static bool ToFile(const Path& path, const IRBuilder& Value);
	static bool FromFile(IRBuilder& Out, const Path& path);


	static void ToBytes(BitMaker& Out, const IRFunc& Value);
	static void FromBytes(BitReader& Out, IRFunc& Value);

	static void ToBytes(BitMaker& Out, const IRSymbolData& Value);
	static void FromBytes(BitReader& Out, IRSymbolData& Value);

	static void ToBytes(BitMaker& Out, const IRStruct& Value);
	static void FromBytes(BitReader& Out, IRStruct& Value);

	static void ToBytes(BitMaker& Out, const IRStaticArray& Value);
	static void FromBytes(BitReader& Out, IRStaticArray& Value);

	static void ToBytes(BitMaker& Out, const IRFuncPtr& Value);
	static void FromBytes(BitReader& Out, IRFuncPtr& Value);

	static void ToBytes(BitMaker& Out, const IRSymbol& Value);
	static void FromBytes(BitReader& Out, IRSymbol& Value);

	static void ToBytes(BitMaker& Out, const IRType& Value);
	static void FromBytes(BitReader& Out, IRType& Value);

	static void ToBytes(BitMaker& Out, const IRPar& Value);
	static void FromBytes(BitReader& Out, IRPar& Value);

	static void ToBytes(BitMaker& Out, const IRBlock& Value);
	static void FromBytes(BitReader& Out, IRBlock& Value);

	static void ToBytes(BitMaker& Out, const IRInstruction& Value, const IRBlock& MyBlock);
	static void FromBytes(BitReader& Out, IRInstruction& Value, const IRBlock& MyBlock);

	static void ToBytes(BitMaker& Out, const IROperator& Value, const IRType& Type, const IRBlock& MyBlock);
	static void FromBytes(BitReader& Out, IROperator& Value, const IRType& Type, const IRBlock& MyBlock);


	//very slow
	static size_t GetImplementationHash(const IRFunc* Func);

	//very slow
	static size_t GetImplementationHash(BitMaker BitsOutput, const IRFunc* Func);

	//very slow
	static size_t GetImplementationHash(const IRBlock* Func);

	//very slow
	static size_t GetImplementationHash(BitMaker BitsOutput, const IRBlock* Func);

	//
	void CombineWith(const IRBuilder& Other);
	void CombineWith(IRBuilder&& Other);
};




UCodeLangEnd