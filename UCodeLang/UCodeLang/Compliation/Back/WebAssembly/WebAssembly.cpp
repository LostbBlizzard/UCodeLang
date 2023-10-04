#include "WebAssembly.hpp"
#include "UCodeLang/Compliation/Middle/IR.hpp"
#include "UCodeLang/Compliation/CompliationSettings.hpp"
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
	_Input = Input;


	{
		WasmFile::Section V1;
		
		WasmFile::TypeSection V2;


		V1.Type = std::move(V2);
		_Output.section.push_back(std::move(V1));
	}
	{
		WasmFile::Section V1;

		WasmFile::FuncSection V2;


		V1.Type = std::move(V2);
		_Output.section.push_back(std::move(V1));
	}
	{
		WasmFile::Section V1;

		WasmFile::CodeSection V2;


		V1.Type = std::move(V2);
		_Output.section.push_back(std::move(V1));
	}

	_typeSection = &_Output.section[0].Type.Get<WasmFile::TypeSection>();
	_funcSection = &_Output.section[1].Type.Get<WasmFile::FuncSection>();
	_codeSection = &_Output.section[2].Type.Get<WasmFile::CodeSection>();
	
	//OnFunc(&Input->_StaticInit);
	
	//OnFunc(&Input->_threadInit);

	//OnFunc(&Input->_StaticdeInit);

	//OnFunc(&Input->_threadInit);

	for (auto& Item : Input->Funcs)
	{
		OnFunc(Item.get());
	}
	

	auto outbytes = WasmFile::ToBytes(_Output);
	Set_Output(BytesView::Make(outbytes.Data(), outbytes.Size()));
}

BackEndObject* WebAssemblyBackEnd::MakeObject()
{
	return new WebAssemblyBackEnd();
}

void WebAssemblyBackEnd::UpdateBackInfo(CompliationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompliationBackEndInfo::BackEnd::Wasm;
	BackInfo.OutputSet = CompliationBackEndInfo::InsSet::Other;
}

void WebAssemblyBackEnd::OnFunc(const IRFunc* Func)
{
	FuncType func;
	func.Params.resize(Func->Pars.size());
	for (size_t i = 0; i < func.Params.size(); i++)
	{
		func.Params[i] = ToType(Func->Pars[i].type);
	}
	if (Func->ReturnType._Type != IRTypes::Void)
	{
		func.Results.push_back(ToType(Func->ReturnType));
	}

	_codeSection->code.push_back({});
	_funccode = &_codeSection->code.back();

	if (Func->ReturnType._Type != IRTypes::Void)
	{
		_funccode->Add_i32_const(0);
		_funccode->Add_Return();
	}
	//
	
	_funccode->Add_End();

	_funcSection->TypesIndex.push_back(_typeSection->Types.size());
	_typeSection->Types.push_back(std::move(func));
}

WebAssemblyBackEnd::WasmType WebAssemblyBackEnd::ToType(const IRType& Type)
{
	switch (Type._Type)
	{
	case IRTypes::Void:
		return WasmType::i32;
		break;
	case IRTypes::i32:
		return WasmType::i32;
		break;
	case IRTypes::i64:
		return WasmType::i64;
		break;

	case IRTypes::f32:
		return WasmType::f32;
		break;
	case IRTypes::f64:
		return WasmType::f64;
		break;
	default:
		UCodeLangUnreachable();
		break;
	}
}

String WebAssemblyBackEnd::ToWebName(const IRidentifierID Name)
{
	return _Input->FromID(Name);
}
UCodeLangEnd
