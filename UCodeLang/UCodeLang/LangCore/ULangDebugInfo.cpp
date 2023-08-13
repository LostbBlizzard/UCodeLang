#include "ULangDebugInfo.hpp"
#include "UClib.hpp"
UCodeLangStart

void ULangDebugInfo::ToBytes(BitMaker& bit, const ULangDebugInfo& Value)
{
	//return;
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
	//return false;
	
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
		bit.WriteType((BitMaker::SizeAsBits)V->LineNumber);
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
			bit.WriteType((BitMaker::SizeAsBits)V1->offset);
		}
		else if (auto V1 = V->Type.Get_If<UDebugSetVarableLoc::StackAdd>())
		{
			bit.WriteType((BitMaker::SizeAsBits)V1->offset);
		}
		else if (auto V1 = V->Type.Get_If<UDebugSetVarableLoc::Static>())
		{
			bit.WriteType((BitMaker::SizeAsBits)V1->offset);
		}
		else if (auto V1 = V->Type.Get_If<UDebugSetVarableLoc::Thread>())
		{
			bit.WriteType((BitMaker::SizeAsBits)V1->offset);
		}

	}
	else if (auto V = Value.Debug.Get_If<UDebugSetFuncStackFrameSize>())
	{
		bit.WriteType((BitMaker::SizeAsBits)V->StackFrameSize);
		bit.WriteType((BitMaker::SizeAsBits)V->ForIns);
	}
	else
	{
		UCodeLangThrowException("bad path");
	}
}

bool ULangDebugInfo::FromBytes(BitReader& bit, UDebugIns& Value)
{
	UDebugIns::Type V = UDebugIns::Type::None;
	bit.ReadType(*(UDebugIns::Type_t*)&V, *(UDebugIns::Type_t*)&V);
	switch (V)
	{
	case UDebugIns::Type::None:
	{
		UDebugIns::None V2 = UDebugIns::None();

	

		Value.Debug = std::move(V2);
	}
	break;
	case UDebugIns::Type::UDebugSetFile:
	{
		UDebugSetFile V2 = UDebugSetFile();

		bit.ReadType(V2.FileName);

		BitMaker::SizeAsBits Item;
		bit.ReadType(Item);
		V2.ForIns = Item;

		Value.Debug = std::move(V2);
	}
	break;
	case UDebugIns::Type::UDebugSetLineNumber:
	{
		UDebugSetLineNumber V2 = UDebugSetLineNumber();

		{
			BitMaker::SizeAsBits Item;
			bit.ReadType(Item);
			V2.LineNumber = Item;
		}

		{
			BitMaker::SizeAsBits Item;
			bit.ReadType(Item);
			V2.ForIns = Item;
		}

		Value.Debug = std::move(V2);
	}
	break;
	case UDebugIns::Type::UDebugSetVarableLoc:
	{
		UDebugSetVarableLoc V2 = UDebugSetVarableLoc();

		bit.ReadType(V2.VarableFullName);


		BitMaker::SizeAsBits Tep;
		bit.ReadType(Tep);
		V2.ForIns = Tep;

		UDebugSetVarableLoc::TypeV Type;
		bit.ReadType(*(UDebugSetVarableLoc::TypeV_t*)&Type);

		switch (Type)
		{
		case UCodeLang::UDebugSetVarableLoc::TypeV::UMaped:
		{

		}
		break;
		case UCodeLang::UDebugSetVarableLoc::TypeV::RegisterID:
		{
			RegisterID V3;
			bit.ReadType(*(RegisterID_t*)&V3);

			V2.Type = std::move(V3);
		}
		break;
		case UCodeLang::UDebugSetVarableLoc::TypeV::StackSub:
		{
			UDebugSetVarableLoc::StackSub V3;

			BitMaker::SizeAsBits Tep;
			bit.ReadType(Tep);

			V3.offset = Tep;

			V2.Type = std::move(V3);
		}
		break;
		case UCodeLang::UDebugSetVarableLoc::TypeV::StackAdd:
		{
			UDebugSetVarableLoc::StackAdd V3;

			BitMaker::SizeAsBits Tep;
			bit.ReadType(Tep);

			V3.offset = Tep;

			V2.Type = std::move(V3);
		}
		break;
		case UCodeLang::UDebugSetVarableLoc::TypeV::Static:
		{
			UDebugSetVarableLoc::Static V3;

			BitMaker::SizeAsBits Tep;
			bit.ReadType(Tep);

			V3.offset = Tep;

			V2.Type = std::move(V3);
		}
		break;
		case UCodeLang::UDebugSetVarableLoc::TypeV::Thread:
		{
			UDebugSetVarableLoc::Thread V3;

			BitMaker::SizeAsBits Tep;
			bit.ReadType(Tep);

			V3.offset = Tep;

			V2.Type = std::move(V3);
		}
		break;
		default:
			UCodeLangThrowException("bad path");
			break;
		}

		Value.Debug = std::move(V2);
	}
	break;
	case UDebugIns::Type::UDebugSetFuncStackFrameSize:
	{
		UDebugSetFuncStackFrameSize V2 = UDebugSetFuncStackFrameSize();

		{
			BitMaker::SizeAsBits Item;
			bit.ReadType(Item);
			V2.StackFrameSize = Item;
		}

		{
			BitMaker::SizeAsBits Item;
			bit.ReadType(Item);
			V2.ForIns = Item;
		}

		Value.Debug = std::move(V2);
	}
	break;
	default:
		UCodeLangThrowException("bad path");
		break;
	}

	return true;
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
		UCodeLangThrowException("bad path");
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