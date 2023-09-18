#include "WebAssembly.hpp"
#include "UCodeLang/Compliation/Middle/IR.hpp"
UCodeLangStart
WebAssemblyBackEnd::WebAssemblyBackEnd()
{
}
WebAssemblyBackEnd::~WebAssemblyBackEnd()
{
}

void WebAssemblyBackEnd::Reset()
{
	this->~WebAssemblyBackEnd();
	new (this) WebAssemblyBackEnd;
}

void WebAssemblyBackEnd::Build(const IRBuilder* Input)
{
	_Output += "(module\n";
	_Input = Input;


	{
		OnFuncSyb(&Input->_StaticInit);
		OnFuncSyb(&Input->_threadInit);
		OnFuncSyb(&Input->_threaddeInit);
		OnFuncSyb(&Input->_StaticdeInit);


		for (auto& Item : Input->Funcs)
		{
			OnFuncSyb(Item.get());
		}

	}


	{
		OnFunc(&Input->_StaticInit);
		OnFunc(&Input->_threadInit);
		OnFunc(&Input->_threaddeInit);
		OnFunc(&Input->_StaticdeInit);


		for (auto& Item : Input->Funcs)
		{
			OnFunc(Item.get());
		}


	}
	
	_Output += ")";
	Set_Output(_Output);
}

BackEndObject* WebAssemblyBackEnd::MakeObject()
{
	return new WebAssemblyBackEnd();
}

void WebAssemblyBackEnd::OnFunc(const IRFunc* Func)
{
	auto WebName = ToWebName(Func->identifier);
	_Output += " (func $";
	_Output += WebName;
	_Output += ' ';

	{
		size_t ParCount = 0;
		for (auto& Item : Func->Pars)
		{
			_Output += "(param $";

			_Output += std::to_string(ParCount);

			_Output += " ";
			_Output += ToString(Item.type);
			_Output += ")";

			ParCount++;
		}
	}

	_Output += ")\n";
}

void WebAssemblyBackEnd::OnFuncSyb(const IRFunc* Func)
{
	auto WebName = ToWebName(Func->identifier);
	_Output += " (export ";
	_Output += WebName;
	_Output += " (func $";
	_Output += WebName;
	_Output += "))\n";
}

String WebAssemblyBackEnd::ToWebName(const IRidentifierID Name)
{
	return _Input->FromID(Name);
}

String WebAssemblyBackEnd::ToString(const IRType& Type)
{
	String r;
	switch (Type._Type)
	{
	case IRTypes::i8:
		r += "i32";
		break;
	case IRTypes::i16:
		r += "i32";
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
