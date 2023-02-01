#pragma once
#include "../BackEndInterface.hpp"
#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"

UCodeLangStart
class RegistersManager
{
public:
	enum class RegisterInUse :UInt8
	{
		NotInUse,
		InUseSybol,
		HasBitValue,
	};
	struct RegisterInfo 
	{
		RegisterInUse Inuse = RegisterInUse::NotInUse;
		IRField IRField =0;
		AnyInt64 BitValue;
	};
	
	RegistersManager();
	~RegistersManager();

	void Reset();

	static constexpr size_t RegisterSize = (RegisterID_t)RegisterID::EndRegister - (RegisterID_t)RegisterID::StartRegister;
	RegisterInfo Registers[RegisterSize];

	auto& GetInfo(RegisterID id)
	{
		return Registers[(size_t)id];
	}


	RegisterID GetInfo(IRField IRField)
	{
		for (size_t i = 0; i < RegisterSize; i++)
		{
			auto& Info = Registers[i];
			if (Info.Inuse == RegisterInUse::InUseSybol && Info.IRField == IRField)
			{
				return (RegisterID)i;
			}
		}
		return RegisterID::NullRegister;
	}
	RegisterID GetValue(AnyInt64 Value)
	{
		for (size_t i = 0; i < RegisterSize; i++)
		{
			auto& Info = Registers[i];
			if (Info.Inuse == RegisterInUse::HasBitValue && Info.BitValue.Value==Value.Value)
			{
				return (RegisterID)i;
			}
		}
		return RegisterID::NullRegister;
	}

	RegisterID GetFreeRegister()
	{
		for (size_t i = 0; i < RegisterSize; i++)
		{
			auto& Info = Registers[i];
			if (Info.Inuse == RegisterInUse::NotInUse)
			{
				return (RegisterID)i;
			}
		}
		
		Reset();
		return RegisterID::A;
	}

	RegisterID GetFreeRegisterAndWeakLock()
	{
		RegisterID r = GetFreeRegister();
		WeakLockRegister(r);
		return r;
	}

	void WeakLockRegisterValue(RegisterID id,AnyInt64 Value)
	{
		if (id == RegisterID::NullRegister) { throw std::exception("Bad Register"); }
		auto& Info = Registers[(size_t)id];
		Info.Inuse = RegisterInUse::HasBitValue;
		Info.BitValue = Value;
	}

	void WeakLockRegister(RegisterID id)
	{
		if (id == RegisterID::NullRegister) { throw std::exception("Bad Register"); }
		auto& Info = Registers[(size_t)id];
		Info.Inuse = RegisterInUse::InUseSybol;
	}
	void UnLockWeakRegister(RegisterID id)
	{
		if (id == RegisterID::NullRegister) { throw std::exception("Bad Register"); }
		auto& Info = Registers[(size_t)id];
		Info.Inuse = RegisterInUse::NotInUse;
	}
};
UCodeLangEnd
