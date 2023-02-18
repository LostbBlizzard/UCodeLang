#pragma once
#include "UCodeLang/LangCore/LangTypes.hpp"
UCodeLangStart

using IRidentifier = String;

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
	size_t ID=0;
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

struct IRCall
{
	IRidentifier identifier;
	Vector<Weak_ptr<IRD>> Parameters;
};

struct IRD
{

};

struct IRBlock
{

	IRD* NewLoad()
	{

	}
	void NewStore(IRD* loc, IRD* Value)
	{

	}
	IRD* NewMul(IRD* A, IRD* B)
	{

	}
	IRD* NewAdd(IRD* A, IRD* B)
	{

	}
	IRD* NewCall(const IRidentifier& identifier)
	{

	}
	void NewRet(IRD* Value)
	{

	}
	Vector<Unique_ptr<IRD>> Instructions;
};

struct IRFunc
{
	IRidentifier identifier;
	IRType ReturnType;
	Vector<IRPar> Pars;

	IRBlock* NewBlock(const IRidentifier& identifier)
	{
	
	}
	Vector<Unique_ptr<IRBlock>> Blocks;
};

class IR
{
public:
	IRFunc* NewFunc(const IRidentifier& identifier, IRType ReturnType)
	{

	};

	Vector<Unique_ptr<IRFunc>> Funcs;
};

UCodeLangEnd