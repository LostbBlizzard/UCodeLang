#include "X86_64UNativeBackEnd.hpp"
#include "IRToX86_64IR.hpp"
#include "UCodeLang/Compliation/Middle/IR.hpp"
#include "UCodeLang/Compliation/CompliationSettings.hpp"
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


	if (Get_Settings()._Type == OutPutType::IRAndSymbols)
	{
		auto V = _Output->AddLayer(UCode_CodeLayer_IR_Name);
		auto V2 = CodeLayer::JustData();
		V2._Data = Input->ToBytes().MoveToVector();

		V->_Data = std::move(V2);
	}
	else
	{
		auto V = _Output->AddLayer(UCode_CodeLayer_X86_UCodeVM_Name);
		V->_Data = CodeLayer::MachineCode();
		CodeLayer::MachineCode* _OutLayer = V->_Data.Get_If<CodeLayer::MachineCode>();

		auto x8664_ir = IRToX86_64IR::Into(*Input);

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
			_OutLayer->_NameToPtr.AddValue(_Input->FromID(Item.first), Item.second);
		}
	}
}
void X86_64UNativeBackEnd::UpdateBackInfo(CompliationBackEndInfo& BackInfo)
{
	BackInfo.Output = CompliationBackEndInfo::BackEnd::UCodeVm;
	BackInfo.OutputSet = CompliationBackEndInfo::InsSet::X86;
}
UCodeLangEnd