

#ifndef UCodeLangNoCompiler
#include "WasmBackEnd.hpp"
#include "UCodeLang/Compilation/Middle/IR.hpp"
#include "UCodeLang/Compilation/CompilationSettings.hpp"

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

		WasmFile::MemSection V2;
		auto& lim = V2.limits.emplace_back();

		lim.hasmax = MemSection::Limits::HasMax::minAmax;
		lim.min = 1;
		lim.max = 2;


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
	_memSection = &_Output.section[2].Type.Get<WasmFile::MemSection>();
	_exportSection = &_Output.section[3].Type.Get<WasmFile::ExportSection>();
	_codeSection = &_Output.section[4].Type.Get<WasmFile::CodeSection>();


	OnFunc(&Input->_StaticInit);

	OnFunc(&Input->_threadInit);

	OnFunc(&Input->_threaddeInit);

	OnFunc(&Input->_StaticdeInit);


	for (auto& Item : Input->Funcs)
	{
		OnFunc(Item.get());
	}

	for (auto& Item : InsToUpdate)
	{
		auto& Ins = _codeSection->code[Item.FuncIndex].Ins[Item.InsIndex];
		if (Ins.InsType == WasmFile::Expr::Ins::Call)
		{
			Ins.call(FuncToIndex.GetValue(Item.lookingforfunc));
		}
		else
		{
			UCodeLangUnreachable();
		}
	}

	auto outbytes = WasmFile::ToBytes(_Output);
	Set_Output(BytesView::Make(outbytes.Data(), outbytes.Size()));

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
	_func = Func;
	Position.clear();


	FuncType func;
	func.Params.resize(Func->Pars.size());
	for (size_t i = 0; i < func.Params.size(); i++)
	{
		func.Params[i] = ToType(Func->Pars[i].type);
	}
	if (Func->ReturnType._Type != IRTypes::Void)
	{
		if (Func->ReturnType._Type == IRTypes::IRsymbol)
		{
			auto sym = _Input->GetSymbol(Func->ReturnType._symbol);
			if (sym->SymType == IRSymbolType::FuncPtr)
			{
				func.Results.push_back(ToType(Func->ReturnType));
			}
			else if (sym->SymType == IRSymbolType::StaticArray)
			{
				func.Params.push_back(ToType(IRTypes::pointer));
			}
			else if (sym->SymType == IRSymbolType::Struct)
			{
				func.Params.push_back(ToType(IRTypes::pointer));
			}
			else
			{
				UCodeLangUnreachable();
			}
		}
		else
		{
			func.Results.push_back(ToType(Func->ReturnType));
		}
	}

	_codeSection->code.push_back({});
	_funccode = &_codeSection->code.back();

	FuncToIndex.AddValue(Func->identifier, _codeSection->code.size() - 1);


	if (Func->ReturnType._Type != IRTypes::Void)
	{
		auto& Block = Func->Blocks.front();
		for (auto& ItemV : Block->Instructions)
		{
			auto Item = ItemV.get();
			if (Item->Type == IRInstructionType::Load)
			{
				auto v = StartSave(Item);

				LoadOp(Item, Item->Target());

				EndSave(v);
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
				LoadOp(Item, Item->Target());//Push Value on stack
			}
			else if (Item->Type == IRInstructionType::Return)
			{
				_funccode->Push_Return();
			}
			else if (Item->Type == IRInstructionType::None)
			{

			}
			else if (Item->Type == IRInstructionType::PushParameter)
			{
				LoadOp(Item, Item->Target());
			}
			else if (Item->Type == IRInstructionType::Call)
			{
				auto functocall = Item->Target().identifier;


				auto v = StartSave(Item);

				_funccode->Push_call(0);


				InsToUpdateMap V;
				V.FuncIndex = _codeSection->code.size() - 1;
				V.InsIndex = _funccode->Ins.size() - 1;
				V.lookingforfunc = functocall;
				InsToUpdate.push_back(V);


				EndSave(v);
			}
			else if (Item->Type == IRInstructionType::Add)
			{

				auto v = StartSave(Item);

				LoadOp(Item, Item->Target());
				LoadOp(Item, Item->Input());

				switch (Item->ObjectType._Type)
				{
				case IRTypes::i32:
					_funccode->Push_i32_Add();
					break;
				case IRTypes::i64:
					_funccode->Push_i64_Add();
					break;
				case IRTypes::f32:
					_funccode->Push_f32_Add();
					break;
				case IRTypes::f64:
					_funccode->Push_f64_Add();
					break;
				default:
					UCodeLangUnreachable();
					break;
				}

				EndSave(v);
			}
			else if (Item->Type == IRInstructionType::Sub)
			{

				auto v = StartSave(Item);

				LoadOp(Item, Item->Target());
				LoadOp(Item, Item->Input());

				switch (Item->ObjectType._Type)
				{
				case IRTypes::i32:
					_funccode->Push_i32_Sub();
					break;
				case IRTypes::i64:
					_funccode->Push_i64_Sub();
					break;
				case IRTypes::f32:
					_funccode->Push_f32_Sub();
					break;
				case IRTypes::f64:
					_funccode->Push_f64_Sub();
					break;
				default:
					UCodeLangUnreachable();
					break;
				}

				EndSave(v);
			}
			else if (Item->Type == IRInstructionType::Reassign)
			{
				size_t MyPos = Position.GetValue(Item->Target().Pointer);

				_funccode->Push_i32_const((UInt32)MyPos);

				LoadOp(Item, Item->Input());//Push Value on stack


				_funccode->Push_i32_store();
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
	case IRTypes::pointer:
		if (Get_Settings().PtrSize == IntSizes::Int32)
		{
			return WasmType::i32;
		}
		else
		{
			return WasmType::i64;
		}
		break;
	case IRTypes::IRsymbol:
	{
		auto sym = _Input->GetSymbol(Type._symbol);
		if (sym->SymType == IRSymbolType::FuncPtr)
		{
			return ToType(IRTypes::pointer);
		}
		else if (sym->SymType == IRSymbolType::StaticArray)
		{//pass by pointer
			return ToType(IRTypes::pointer);
		}
		else if (sym->SymType == IRSymbolType::Struct)
		{//pass by pointer
			return ToType(IRTypes::pointer);
		}
		else
		{
			UCodeLangUnreachable();
		}
	}
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
	else if (Op.Type == IROperatorType::IRParameter)
	{
		size_t parindex = Op.Parameter - (&_func->Pars.front());

		_funccode->Push_local_get(parindex);
	}
	else
	{
		UCodeLangUnreachable();
	}
}
WasmBackEnd::SaveInfo WasmBackEnd::StartSave(const IRInstruction* ir)
{
	if (ir->ObjectType != IRTypes::Void) {
		size_t MyPos = Position.size() * 4;

		// the offset in memory where to store the number
		_funccode->Push_i32_const((UInt32)MyPos);

	}
	WasmBackEnd::SaveInfo r;
	r.ir = ir;
	return r;
}
void WasmBackEnd::EndSave(SaveInfo& info)
{
	if (info.ir->ObjectType != IRTypes::Void) {
		Position.AddValue(info.ir, Position.size() * 4);

		switch (info.ir->ObjectType._Type)
		{
		case IRTypes::i32:
			_funccode->Push_i32_store();
			break;
		case IRTypes::i64:
			_funccode->Push_i64_store();
			break;
		default:
			UCodeLangUnreachable();
			break;
		}
	}
}
UCodeLangEnd

#endif