

#ifndef UCodeLangNoCompiler
#include "WasmBackEnd.hpp"
#include "UCodeLang/Compilation/Middle/IR.hpp"
#include "UCodeLang/Compilation/CompilationSettings.hpp"

#include "Source.hpp"
UCodeLangStart
WasmBackEnd::WasmBackEnd()
{
}
WasmBackEnd::~WasmBackEnd()
{
}

void WasmBackEnd::Reset()
{
	this->~WasmBackEnd();
	new (this) WasmBackEnd;
}

void WasmBackEnd::Build(const IRBuilder* Input)
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

		WasmFile::ExportSection V2;


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
	_exportSection = &_Output.section[2].Type.Get<WasmFile::ExportSection>();
	_codeSection = &_Output.section[3].Type.Get<WasmFile::CodeSection>();
	
	OnFunc(&Input->_StaticInit);
	
	OnFunc(&Input->_threadInit);

	OnFunc(&Input->_threaddeInit);

	OnFunc(&Input->_StaticdeInit);


	for (auto& Item : Input->Funcs)
	{
		OnFunc(Item.get());
	}
	

	auto outbytes = WasmFile::ToBytes(_Output);
	Set_Output(BytesView::Make(outbytes.Data(), outbytes.Size()));

	/*
	{
		struct Code : wasmblr::CodeGenerator {
			Code() : wasmblr::CodeGenerator() {
				auto add_func = function({}, { i32 }, [&]() {
					i32.const_(2);
					//i32.const_(5);
					});
				export_(add_func, "main");
			}
		};


		Code c;
		auto bytes = c.emit();

		Vector<Byte> outbytelist;

		for (auto& Item : outbytes)
		{
			outbytelist.push_back(Item);
		}

		
		UCodeLangAssert(outbytelist.size() == bytes.size());
		for (size_t i = 0; i < outbytelist.size(); i++)
		{
			if (outbytelist[i] != bytes[i])
			{
				UCodeLangBreakPoint();
			}
		}
		

		int a = 0;
		//Set_Output(BytesView::Make(bytes.data(), bytes.size()));
	}
	*/
}

BackEndObject* WasmBackEnd::MakeObject()
{
	return new WasmBackEnd();
}

void WasmBackEnd::UpdateBackInfo(CompilationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompilationBackEndInfo::BackEnd::Wasm;
	BackInfo.OutputSet = CompilationBackEndInfo::InsSet::Other;
}

void WasmBackEnd::OnFunc(const IRFunc* Func)
{
	Position.clear();

	
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
		auto& Block = Func->Blocks.front();
		for (auto& ItemV : Block->Instructions)
		{
			auto Item = ItemV.get();
			if (Item->Type == IRInstructionType::Load)
			{
				size_t MyPos = Position.size() * 4 ;
				//_funccode->Push_i32_const((UInt32)MyPos);//the offset in memory where to store the number

				LoadOp(Item, Item->Target());
				
				//_funccode->Push_i32_store();

				Position.AddValue(Item, MyPos);

			}
			else if (Item->Type == IRInstructionType::LoadNone)
			{
				size_t MyPos = Position.size();
				Position.AddValue(Item, MyPos);
			}
			else if (Item->Type == IRInstructionType::Unreachable)
			{
				_funccode->Push_Unreachable();
			}
			else if (Item->Type == IRInstructionType::LoadReturn)
			{
				//LoadOp(Item, Item->Target());//Push Value on stack
			}
			else if (Item->Type == IRInstructionType::Return)
			{
				_funccode->Push_Return();
			}
			else if (Item->Type == IRInstructionType::None)
			{

			}
			else
			{
				UCodeLangUnreachable();
			}	
		}
	}
	
	_funccode->Push_End();
	

	_funcSection->TypesIndex.push_back(_typeSection->Types.size());
	_typeSection->Types.push_back(std::move(func));

	Export V;
	V.Name = ToWebName(Func->identifier);
	V.Tag = WasmFile::ExportTag::Func;
	V.Index = _codeSection->code.size() - 1;
	_exportSection->Exports.push_back(std::move(V));
}

WasmBackEnd::WasmType WasmBackEnd::ToType(const IRType& Type)
{
	switch (Type._Type)
	{
	case IRTypes::i8:
	case IRTypes::i16:
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


String WasmBackEnd::ToWebName(const String& IRName)
{
	return IRName;
}
String WasmBackEnd::ToWebName(const IRidentifierID Name)
{
	return ToWebName(_Input->FromID(Name));
}
void WasmBackEnd::LoadOp(const IRInstruction* ir, const IROperator& Op)
{
	if (Op.Type == IROperatorType::Value)
	{
		switch (ir->ObjectType._Type)
		{
		case IRTypes::f32:
			_funccode->Push_f32_const(Op.Value.Asfloat32);
			break;

		case IRTypes::i8:
			_funccode->Push_i32_const((Int32)Op.Value.AsInt8);
			break;
		case IRTypes::i16:
			_funccode->Push_i32_const((Int32)Op.Value.AsInt16);
			break;

		case IRTypes::i32:
			_funccode->Push_i32_const(Op.Value.AsInt32);
			break;

		case IRTypes::i64:
			_funccode->Push_i64_const(Op.Value.AsInt64);
			break;

		case IRTypes::f64:
			_funccode->Push_f64_const(Op.Value.Asfloat64);
			break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
	else if (Op.Type == IROperatorType::IRInstruction)
	{
		auto pos = Position.GetValue(Op.Pointer);

		_funccode->Push_i32_const((UInt32)pos);//the offset from where to load the number

		_funccode->Push_i32_load();

	}
	else
	{
		UCodeLangUnreachable();
	}
}
UCodeLangEnd

#endif