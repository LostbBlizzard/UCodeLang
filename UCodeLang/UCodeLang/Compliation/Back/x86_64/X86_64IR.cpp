#include "X86_64IR.hpp"
UCodeLangStart
void X86_64IR::CleanUp(CleanUpMode Mode)
{

	{
		for (auto& Item : Funcs)
		{
			auto& CallConvention = CallingConventions.at(Item.CallConvention);
			for (size_t i = 0; i < Item.Body.size(); i++)
			{
				auto& Insi = Item.Body[i];

				if (auto Val = Insi.variant.Get_If<Ins::Move>())
				{
					if (auto Val2 = Val->MoveTypes.Get_If<Ins::Move::RegToFuncReturn>())
					{
						if (Val2->Src == CallConvention.IntegerReturnValue)
						{
							Insi = Ins(Ins::Removed());
						}
						else {
							auto NewIns = Ins::Move::RegToReg(Val2->RegSize, Val2->Src, CallConvention.IntegerReturnValue);
							Insi = Ins(Ins::Move(NewIns));
						}
					}
				}
			}
		}

	}
	if (Mode != CleanUpMode::None)
	{

	}
}
X86_64IR::BuildInfo X86_64IR::Build() const
{
	BuildInfo r;
	BuildState State;

	r.Funcs.reserve(Funcs.size());
	for (auto& Item : Funcs)
	{
		r.Funcs.push_back({});
		auto& Func = r.Funcs.back();
		Build(Func, State, Item);

		Func.Func = Item.funcID;
		Func.Bytes = std::move(State.Gen._Base._Output.ByteOutput);
	}
	return r;
}
void X86_64IR::Build(BuildInfo::BuildFunc& Out, BuildState& State, const Func& Value) const
{
	for (auto& Item : Value.Body)
	{
		Build(Out, State, Item);
	}
}
void X86_64IR::Build(BuildInfo::BuildFunc& Out, BuildState& State, const Ins& Value) const
{
	if (auto Val =Value.variant.Get_If<Ins::NoOp>())
	{

	}
	else if (auto Val = Value.variant.Get_If<Ins::Move>())
	{
		if (auto Val2 = Val->MoveTypes.Get_If<Ins::Move::RegToReg>())
		{
			switch (Val2->RegSize)
			{	
			case Ins::InsRegSize::bits8:
			{
				State.Gen.mov8(Val2->Out, Val2->Src);
			}
			break;
			case Ins::InsRegSize::bits16:
			{
				State.Gen.mov16(Val2->Out, Val2->Src);
			}
			break;
			case Ins::InsRegSize::bits32:
			{
				State.Gen.mov32(Val2->Out, Val2->Src);
			}
			break;
			case Ins::InsRegSize::bits64:
			{
				State.Gen.mov64(Val2->Out, Val2->Src);
			}
			break;
			default:
				throw std::exception("bad path");
				break;
			}
		}
		else if (auto Val2 = Val->MoveTypes.Get_If<Ins::Move::ConstToReg>())
		{
			switch (Val2->RegSize)
			{
			case Ins::InsRegSize::bits8:
			{
				State.Gen.mov(Val2->Out, Val2->Src.AsInt8);
			}
			break;
			case Ins::InsRegSize::bits16:
			{
				State.Gen.mov(Val2->Out, Val2->Src.AsInt16);
			}
			break;
			case Ins::InsRegSize::bits32:
			{
				State.Gen.mov(Val2->Out, Val2->Src.AsInt32);
			}
			break;
			case Ins::InsRegSize::bits64:
			{
				State.Gen.mov(Val2->Out, Val2->Src.AsInt64);
			}
			break;
			default:
				throw std::exception("bad path");
				break;
			}
		}
		else
		{
			throw std::exception("bad path");

		}
	}
	else if (auto Val = Value.variant.Get_If<Ins::Ret>())
	{
		State.Gen.ret();
	}
	else if (auto Val = Value.variant.Get_If<Ins::Removed>())
	{

	}
	else
	{
		throw std::exception("bad path");
	}
}
UCodeLangEnd


