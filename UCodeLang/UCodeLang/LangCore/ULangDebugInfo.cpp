#include "ULangDebugInfo.hpp"
#include "UClib.hpp"
UCodeLangStart

void ULangDebugInfo::ToBytes(BitMaker& bit, const ULangDebugInfo& Value)
{
	return;
	bit.WriteType((BitMaker::SizeAsBits)Value.DebugInfo.size());
	for (auto& Item : Value.DebugInfo)
	{
		ToBytes(bit, Item);
	}

	bit.WriteType((BitMaker::SizeAsBits)Value.VarablesInfo.size());
	for (auto& Item : Value.VarablesInfo)
	{
		bit.WriteType(Item._Key);
		ToBytes(bit, Item._Value);
	}
}

bool ULangDebugInfo::FromBytes(BitReader& bit, ULangDebugInfo& Value)
{
	return false;
	BitMaker::SizeAsBits V;
	bit.ReadType(V, V);
	Value.DebugInfo.resize(V);
	for (size_t i = 0; i < (size_t)V; i++)
	{
		FromBytes(bit, Value.DebugInfo[i]);
	}
	bit.ReadType(V, V);
	Value.VarablesInfo.reserve(V);
	for (size_t i = 0; i < (size_t)V; i++)
	{
		String key;
		bit.ReadType(key, key);

		VarableInfo Var;
		FromBytes(bit, Var);

		Value.VarablesInfo.AddValue(std::move(key), std::move(Var));
	}
	return true;
}
void ULangDebugInfo::ToBytes(BitMaker& bit, const UDebugIns& Value)
{
	bit.WriteType((UDebugIns::Type_t)Value.Get_Type());
	if (auto V = Value.Debug.Get_If<UDebugIns::None>())
	{

	}
	else if (auto V = Value.Debug.Get_If<UDebugSetFile>())
	{
		bit.WriteType(V->FileName);
		bit.WriteType((BitMaker::SizeAsBits)V->ForIns);
	}
	else if (auto V = Value.Debug.Get_If<UDebugSetLineNumber>())
	{
		bit.WriteType(V->LineNumber);
		bit.WriteType((BitMaker::SizeAsBits)V->ForIns);
	}
	else if (auto V = Value.Debug.Get_If<UDebugSetVarableLoc>())
	{
		bit.WriteType(V->VarableFullName);
		bit.WriteType((BitMaker::SizeAsBits)V->ForIns);
		bit.WriteType((UDebugSetVarableLoc::TypeV_t)V->Get_Type());

		if (auto V1 = V->Type.Get_If<UDebugSetVarableLoc::UMaped>())
		{

		}
		else if (auto V1 = V->Type.Get_If<RegisterID>())
		{
			bit.WriteType((RegisterID_t)*V1);
		}
		else if (auto V1 = V->Type.Get_If<UDebugSetVarableLoc::StackSub>())
		{
			bit.WriteType(V1->offset);
		}
		else if (auto V1 = V->Type.Get_If<UDebugSetVarableLoc::StackAdd>())
		{
			bit.WriteType(V1->offset);
		}
		else if (auto V1 = V->Type.Get_If<UDebugSetVarableLoc::Static>())
		{
			bit.WriteType(V1->offset);
		}
		else if (auto V1 = V->Type.Get_If<UDebugSetVarableLoc::Thread>())
		{
			bit.WriteType(V1->offset);
		}

	}
	else
	{
		throw std::exception("bad path");
	}
}

void ULangDebugInfo::ToBytes(BitMaker& bit, const VarableInfo& Value)
{
	bit.WriteType(Value.FileDeclaredIn);
	bit.WriteType((BitMaker::SizeAsBits)Value.DeclaredLine);
	bit.WriteType((BitMaker::SizeAsBits)Value.DeclaredPos);
	UClib::ToBytes(bit, Value.ReflectionType);

	bit.WriteType((VarableInfoType_t)Value.VarableType);


	bit.WriteType((VarableInfo::Typeloc_t)Value.Get_Type());
	switch (Value.Get_Type())
	{
	case VarableInfo::Typeloc::None:
		break;
	case VarableInfo::Typeloc::Static:
		bit.WriteType((BitMaker::SizeAsBits)Value.TypeLoc.Get<VarableInfo::Static>().offset);
		break;
	case VarableInfo::Typeloc::Thread:
		bit.WriteType((BitMaker::SizeAsBits)Value.TypeLoc.Get<VarableInfo::Thread>().offset);
		break;
	default:
		throw std::exception("bad path");
		break;
	} 
}
bool ULangDebugInfo::FromBytes(BitReader& bit, VarableInfo& Value)
{
	bit.ReadType(Value.FileDeclaredIn, Value.FileDeclaredIn);
	
	BitMaker::SizeAsBits V =0;
	bit.ReadType(V, V);
	Value.DeclaredLine = V;

	bit.ReadType(V, V);
	Value.DeclaredPos = V;

	UClib::FromBytes(bit, Value.ReflectionType);

	bit.ReadType(*(VarableInfoType_t*)&Value.VarableType, *(VarableInfoType_t*)&Value.VarableType);

	VarableInfo::Typeloc Type= VarableInfo::Typeloc::None;
	bit.ReadType(*(VarableInfo::Typeloc_t*)&Type, *(VarableInfo::Typeloc_t*)&Type);

	switch (Type)
	{
	case VarableInfo::Typeloc::Static:	
	{
		BitMaker::SizeAsBits V = 0;
		bit.ReadType(V, V);

		VarableInfo::Static G;
		G.offset = V;
		Value.TypeLoc = std::move(G);	
	}
	break;
	case VarableInfo::Typeloc::Thread:
	{
		BitMaker::SizeAsBits V = 0;
		bit.ReadType(V, V);

		VarableInfo::Thread G;
		G.offset = V;
		Value.TypeLoc = std::move(G);
	}break;
	default:
		break;
	}

	return true;
}
UCodeLangEnd