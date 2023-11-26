#ifndef UCodeLangNoCompiler
#include "X86_64UNativeBackEnd.hpp"
#include "IRToX86_64IR.hpp"
#include "UCodeLang/Compilation/Middle/IR.hpp"
#include "UCodeLang/Compilation/CompilationSettings.hpp"
#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart
void X86_64UNativeBackEnd::Reset()
{
	this->~X86_64UNativeBackEnd();
	new (this) X86_64UNativeBackEnd;
}
void X86_64UNativeBackEnd::Build(const IRBuilder* Input)
{
	_Input = Input;
	_Output = &Get_Output();

	size_t GetRetID = 0;
	size_t GetRetParID = 1;


	if (Get_Settings()._Type == OutPutType::IRAndSymbols)
	{
		auto V = _Output->AddLayer(UCode_CodeLayer_IR_Name);
		auto V2 = CodeLayer::JustData();
		V2._Data = Input->ToBytes().MoveToVector();

		V->_Data = std::move(V2);
	}
	else
	{
		auto V = _Output->AddLayer(UCode_CodeLayer_X86_64_UCodeVM_Name);
		V->_Data = CodeLayer::MachineCode();
		CodeLayer::MachineCode* _OutLayer = V->_Data.Get_If<CodeLayer::MachineCode>();

		x8664_ir = IRToX86_64IR::Into(*Input);

		ABIList.push_back({ 2,"Windows" });
		ABIList.push_back({ 4,"Unix" });
		//x8664_ir.AddNewFunc(GetRetID);


		
		MakeWapperFuncion(&_Input->_StaticInit);
		MakeWapperFuncion(&_Input->_threadInit);

		MakeWapperFuncion(&_Input->_threaddeInit);
		MakeWapperFuncion(&_Input->_StaticdeInit);

		for (auto& Item : _Input->Funcs)
		{
			MakeWapperFuncion(Item.get());
		}


		auto Info = x8664_ir.Build();

		size_t MaxBuffersize = 0;
		for (auto& Item : Info.Funcs)
		{
			MaxBuffersize += Item.Bytes.size();

		}
		_OutLayer->_Code.reserve(MaxBuffersize);

		UnorderedMap<IRidentifierID,size_t> Funcoffsets;
		for (auto& Item : Info.Funcs)
		{
			Funcoffsets.AddValue(Item.Func, _OutLayer->_Code.size());

			for (auto& Byte : Item.Bytes)
			{
				_OutLayer->_Code.push_back(Byte);
			}
		}

		for (auto& Item : Funcoffsets)
		{
			_OutLayer->_NameToPtr.AddValue(_Input->_Map.HasValue(Item.first) ?
				"native-" + _Input->FromID(Item.first)
				: newidnames.GetValue(Item.first).substr(4)//remove cpp-
				, Item.second);
		}
	}
}
void X86_64UNativeBackEnd::MakeWapperFuncion(const IRFunc* func)
{
	for (auto& ABI : ABIList) 
	{
		auto newname = "cpp-" + _Input->FromID(func->identifier) + "-[" + ABI.OS + "]";
		auto newnameid = std::hash<String>()(newname);
		auto& Func = x8664_ir.AddNewFunc(newnameid);
		Func.CallConvention = ABI.id;


		auto& Convention = x8664_ir.CallingConventions.GetValue(ABI.id);
		
		newidnames.AddValue(newnameid, newname);
		newids.push_back({ func->identifier,Func.funcID });

		using Ins = X86_64IR::Ins;
		using GReg = X86_64IR::GReg;

		GReg NativeInterpreterThis = Convention.IntegerFunctionArguments[0];
		for (auto& Item : func->Pars)
		{
			auto parsize = _Input->GetSize(Item.type, false);

			Func.Add_Ins(Ins::Move(Ins::Move::ConstToReg(parsize, Convention.IntegerFunctionArguments[1])));
		}

		Func.Add_Ins(Ins::Call(X86_64IR::NearRelocation32(X86_64IR::Relocation32(func->identifier))));

		if (func->ReturnType._Type != IRTypes::Void)
		{
			auto retsize = _Input->GetSize(func->ReturnType, false);
			Func.Add_Ins(Ins::Move(Ins::Move::ConstToReg(retsize, Convention.IntegerReturnValue)));
		}
	}
}
void X86_64UNativeBackEnd::UpdateBackInfo(CompilationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompilationBackEndInfo::BackEnd::UCodeVm;
	BackInfo.OutputSet = CompilationBackEndInfo::InsSet::X86;
}
UCodeLangEnd


#endif