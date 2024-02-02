#ifndef UCodeLangNoCompiler

#include "LLVMBackEnd.hpp"
#include "UCodeLang/Compilation/Middle/IR.hpp"
#include "UCodeLang/Compilation/CompilationSettings.hpp"
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

			_OutStr += '%';
			_OutStr += SybName;
			_OutStr += " = type {\n";

			for (size_t i = 0; i < V->Fields.size(); i++)
			{
				auto& Field = V->Fields[i];
				_OutStr += ToString(V->Fields[i].Type);

				if (&Field != &V->Fields.back())
				{
					_OutStr += ",";
				}
				_OutStr += '\n';
			}

			_OutStr += "}\n\n";
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
void LLVMBackEnd::UpdateBackInfo(CompilationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompilationBackEndInfo::BackEnd::C11;
	BackInfo.OutputSet = CompilationBackEndInfo::InsSet::Other;
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

	_OutStr += "}\n";
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
		auto Syb = _Input->GetSymbol(Type._symbol);
		if (Syb)
		{
			r += _Input->FromID(Syb->identifier) + "*";
		}
		else {
			r += "void*";
		}
	}
	break;
	case IRTypes::IRsymbol:
	{
		auto Syb = _Input->GetSymbol(Type._symbol);
		if (Syb)
		{
			r += _Input->FromID(Syb->identifier);
		}
	}
	break;
	default:
		UCodeLangUnreachable();
		break;
	}
	return r;
}
UCodeLangEnd

#endif