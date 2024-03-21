#ifndef UCodeLangNoCompiler
#include "UCodeLang/Compilation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnSizeofNode(const SizeofExpresionNode* nod)
{
	auto& lookT = Type_Get_LookingForType();
	TypeSymbol Type;

	if (_PassType == PassType::FixedTypes || _PassType == PassType::BuidCode)
	{
		switch (lookT._Type)
		{
		case TypesEnum::sInt8:
		case TypesEnum::uInt8:
			Type.SetType(TypesEnum::uInt8);
			break;
		case TypesEnum::sInt16:
		case TypesEnum::uInt16:
			Type.SetType(TypesEnum::uInt16);
			break;
		case TypesEnum::sInt32:
		case TypesEnum::uInt32:
			Type.SetType(TypesEnum::uInt32);
			break;
		case TypesEnum::sInt64:
		case TypesEnum::uInt64:
			Type.SetType(TypesEnum::uInt64);
			break;
		default:
			Type.SetType(TypesEnum::uIntPtr);
			break;
		}
		TypeSymbol Info;
		Type_ConvertAndValidateType(nod->_Type, Info, NodeSyb_t::Any); 
		
		if (!Info.IsBadType())
		{
			UAddress TypeSize;

			if (!Type_GetSize(Info, TypeSize))
			{
				String Msg = "Unable to GetSize of '";
				Msg += ToString(Info);
				Msg += "'. it may not be a vaild runtime type";

				LogError(ErrorCodes::InValidType, Msg,NeverNullptr(nod->_Type._name._ScopedName.back()._token));
			}
		}
	}

	if (_PassType == PassType::BuidCode)
	{
		TypeSymbol Info;
		Type_ConvertAndValidateType(nod->_Type, Info, NodeSyb_t::Any);
		UAddress TypeSize;
		Type_GetSize(Info, TypeSize);
		switch (lookT._Type)
		{
		case TypesEnum::sInt8:
		case TypesEnum::uInt8:
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt8)TypeSize);
			break;
		case TypesEnum::sInt16:
		case TypesEnum::uInt16:
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt16)TypeSize);
			break;
		case TypesEnum::sInt32:
		case TypesEnum::uInt32:
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt32)TypeSize);
			break;
		case TypesEnum::sInt64:
		case TypesEnum::uInt64:
			_IR_LastExpressionField = _IR_LookingAtIRBlock->NewLoad((UInt64)TypeSize);
			break;
		default:
			Type.SetType(TypesEnum::uIntPtr);
			_IR_LastExpressionField = IR_Load_UIntptr(TypeSize);
			break;
		}
	}

	_LastExpressionType = Type;
}

UCodeLangFrontEnd

#endif