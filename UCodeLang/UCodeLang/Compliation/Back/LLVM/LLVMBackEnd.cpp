#include "LLVMBackEnd.hpp"
UCodeLangStart
void LLVMBackEnd::Reset()
{
}

void LLVMBackEnd::Build(const IRBuilder* Input)
{
	_Input = Input;

	for (auto& Item : Input->_Symbols)
	{
		String SybName = Input->FromID(Item->identifier);
		switch (Item->SymType)
		{
		case IRSymbolType::Struct:
		{
			IRStruct* V = Item->Get_ExAs<IRStruct>();

			if (V->IsUnion)
			{
				_OutStr += "union " + SybName + "\n";
			}
			else
			{
				_OutStr += "$" + SybName + "\n";
			}

			for (size_t i = 0; i < V->Fields.size(); i++)
			{
				_OutStr += " " + ToString(V->Fields[i].Type) + " __" + std::to_string(i) + "; \n";
			}

			_OutStr += "\n";
		}
		break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}

	OnFunc(&Input->_StaticInit);
	OnFunc(&Input->_threadInit);
	OnFunc(&Input->_threaddeInit);
	OnFunc(&Input->_StaticdeInit);


	for (auto& Item : Input->Funcs)
	{
		OnFunc(Item.get());
	}

	Set_Output(_OutStr);
}
void LLVMBackEnd::OnFunc(const IRFunc* Func)
{
	_OutStr += "define " + ToString(Func->ReturnType) + " &" + _Input->FromID(Func->identifier);

	_OutStr += "(";
	for (auto& Item : Func->Pars)
	{
		_OutStr += ToString(Item.type);
		_OutStr += " " + _Input->FromID(Item.identifier);

		if (&Item != &Func->Pars.back())
		{
			_OutStr += ",";
		}
	}
	_OutStr += ")";

	_OutStr += " #0";
	_OutStr += "{\n";

	_OutStr += "}";
}
String LLVMBackEnd::ToString(const IRType& Type)
{
	String r;
	switch (Type._Type)
	{
	case IRTypes::i8:
		r += "i8";
		break;
	case IRTypes::i16:
		r += "i8";
		break;
	case IRTypes::i32:
		r += "i32";
		break;
	case IRTypes::i64:
		r += "i64";
		break;

	case IRTypes::f32:
		r += "f32";
		break;
	case IRTypes::f64:
		r += "f64";
		break;

	case IRTypes::Void:
		r += "void";
		break;
	case IRTypes::pointer:
	{
		auto Syb =_Input->GetSymbol(Type._symbol);
		if (Syb)
		{
			r += _Input->FromID(Syb->identifier) + "*";
		}
		r += "void*";

	}
	case IRTypes::IRsymbol:
	{
		auto Syb = _Input->GetSymbol(Type._symbol);
		if (Syb)
		{
			r += _Input->FromID(Syb->identifier);
		}
	}
	default:
		break;
	}
}
UCodeLangEnd