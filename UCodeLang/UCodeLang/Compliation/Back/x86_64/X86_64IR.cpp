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
	
	
	//if (Mode != CleanUpMode::None)
	{
		for (auto& Item : Funcs)
		{
			for (size_t i = 0; i < Item.Body.size(); i++)
			{
				auto& Insi = Item.Body[i];
				if (auto Val = Insi.variant.Get_If<Ins::Move>())
				{
					if (auto Val2 = Val->MoveTypes.Get_If<Ins::Move::ConstToReg>())
					{
						if (Val2->Src.Value == 0)
						{
							//X0R is faster then load zero because its an one byte ins,faster decoding.
							auto NewIns = Ins::XOR::RegToReg(Val2->RegSize, Val2->Out, Val2->Out);
							Insi = Ins(Ins::XOR(NewIns));
						}
					}
					else if (auto Val2 = Val->MoveTypes.Get_If<Ins::Move::RegToReg>())
					{
						if (Val2->Out == Val2->Src)
						{
							//Removed unneeded Ins. 
							Insi = Ins(Ins::Removed());
						}
					}
				}

			}

		}
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
	auto& CallConvention = CallingConventions.at(Value.CallConvention);
	for (auto& Item : CallConvention.FuncionProlog)
	{
		State.Gen._Base._Output.ByteOutput.push_back(Item);
	}

	size_t lastepilogue =0;
	for (auto& Item : Value.Body)
	{

		if (Item.variant.Is<Ins::Ret>())
		{
			for (auto& Item : CallConvention.FuncionEpilogue)
			{
				State.Gen._Base._Output.ByteOutput.push_back(Item);
			}
			lastepilogue = State.Gen.Size();
		}
		else 
		{
			Build(Out, State, Item);
		}
	}

	if (Value.Body.size() == 0 || lastepilogue != State.Gen.Size())
	{
		for (auto& Item : CallConvention.FuncionEpilogue)
		{
			State.Gen._Base._Output.ByteOutput.push_back(Item);
		}
	}
}

//#ifdef DEBUG
//constexpr Int8 RelocVal8 = INT8_MAX;
//constexpr Int16 RelocVal16 = INT16_MAX;
//constexpr Int32 RelocVal32 = INT_MAX;
//constexpr Int64 RelocVal64 = INT64_MAX;
//#else
constexpr Int8 RelocVal8 = 0;
constexpr Int16 RelocVal16 = 0;
constexpr Int32 RelocVal32 = 0;
constexpr Int64 RelocVal64 = 0;
//#endif // DEBUG

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
	else if (auto Val = Value.variant.Get_If<Ins::Call>())
	{
		if (auto Val2 = Val->callvariants.Get_If<GReg>())
		{
			State.Gen.call(*Val2);
		}
		else if (auto Val2 = Val->callvariants.Get_If<X86_64IR::Near32>())
		{
			State.Gen.call(*Val2);
		}
		else if (auto Val2 = Val->callvariants.Get_If<X86_64IR::NearRelocation32>())
		{
			State.Gen.call(X86_64IR::Near32(RelocVal32));

			Relocation v;
			v.ByteToUpdateOffset = State.Gen.Size() - 4;
			v.RelocationID = Val2->Value.ID;
			v.Type = RelocationType::Size32;
			Out.Relocations.push_back(std::move(v));
		}
		else
		{
			throw std::exception("bad path");

		}

	}
	else if (auto Val = Value.variant.Get_If<Ins::XOR>())
	{
		if (auto Val2 = Val->Xortypes.Get_If<Ins::XOR::RegToReg>())
		{
			switch (Val2->RegSize)
			{
			case Ins::InsRegSize::bits8:
			{
				State.Gen.XOr8(Val2->Out, Val2->Src);
			}
			break;
			case Ins::InsRegSize::bits16:
			{
				State.Gen.XOr16(Val2->Out, Val2->Src);
			}
			break;
			case Ins::InsRegSize::bits32:
			{
				State.Gen.XOr32(Val2->Out, Val2->Src);
			}
			break;
			case Ins::InsRegSize::bits64:
			{
				State.Gen.XOr64(Val2->Out, Val2->Src);
			}
			break;
			default:
				throw std::exception("bad path");
				break;
			}
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


