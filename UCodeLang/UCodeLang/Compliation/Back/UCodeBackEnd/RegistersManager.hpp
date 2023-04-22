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
		Locked,
	};
	struct RegisterInfo 
	{
		RegisterInUse Inuse = RegisterInUse::NotInUse;
		const IRInstruction* IRField =nullptr;
		AnyInt64 BitValue;
	};
	
	RegistersManager();
	~RegistersManager();

	void Reset();

	static constexpr size_t RegisterSize = (RegisterID_t)RegisterID::EndRegister - (RegisterID_t)RegisterID::StartRegister+1;
	Array<RegisterInfo, RegisterSize> Registers;

	auto& GetInfo(RegisterID id)
	{
		return Registers[(size_t)id];
	}
	void LockRegister(RegisterID id)
	{
		GetInfo(id).Inuse = RegisterInUse::Locked;
	}
	void UnLockRegister(RegisterID id)
	{
		auto& Info = GetInfo(id);
		Info.Inuse = RegisterInUse::NotInUse;
		Info.BitValue = AnyInt64();
		Info.IRField = nullptr;
	}
	bool IsLocked(RegisterID id)
	{
		return GetInfo(id).Inuse != RegisterInUse::NotInUse;
	}

	Optional<RegisterID> GetInfo(const IRInstruction* IRField)
	{
		for (size_t i = 0; i < RegisterSize; i++)
		{
			auto& Info = Registers[i];
			if (Info.Inuse == RegisterInUse::InUseSybol && Info.IRField == IRField)
			{
				return (RegisterID)i;
			}
		}
		return {};
	}
	Optional<RegisterID> GetValue(AnyInt64 Value)
	{
		for (size_t i = 0; i < RegisterSize; i++)
		{
			auto& Info = Registers[i];
			if (Info.Inuse == RegisterInUse::HasBitValue && Info.BitValue.Value==Value.Value)
			{
				return (RegisterID)i;
			}
		}
		return {};
	}

	Optional<RegisterID> GetFreeRegister()
	{
		for (size_t i = 0; i < RegisterSize; i++)
		{
			auto& Info = Registers[i];
			if (!IsLocked((RegisterID)i))
			{
				return (RegisterID)i;
			}
		}
		
		//Reset();
		return {};
	}

	Optional< RegisterID> GetFreeRegisterAndWeakLock()
	{
		auto r = GetFreeRegister();
		if (r.has_value()) 
		{
			WeakLockRegister(r.value());
		}
		return r;
	}

	void WeakLockRegisterValue(RegisterID id,AnyInt64 Value)
	{
		auto& Info = Registers[(size_t)id];
		Info.Inuse = RegisterInUse::HasBitValue;
		Info.BitValue = Value;
	}

	void WeakLockRegisterValue(RegisterID id,const IRInstruction* Value)
	{
		auto& Info = Registers[(size_t)id];
		Info.Inuse = RegisterInUse::InUseSybol;
		Info.IRField = Value;
	}

	void WeakLockRegister(RegisterID id)
	{
		auto& Info = Registers[(size_t)id];
		Info.Inuse = RegisterInUse::InUseSybol;
	}
	void UnLockWeakRegister(RegisterID id)
	{
		auto& Info = Registers[(size_t)id];
		Info.Inuse = RegisterInUse::NotInUse;
	}
};

class StaticMemoryManager
{
public:
	struct StaticMemInfo
	{
		size_t Offset=0;
	};
	BinaryVectorMap<IRidentifierID, StaticMemInfo> _List;
};
struct StackItem
{
	size_t Offset = 0;
	const IRInstruction* IR = nullptr;
};
struct StackInfo
{
	size_t Size = 0;
	size_t PushedOffset = 0;


	void Reset()
	{
		Size = 0;
		PushedOffset = 0;
		Items.clear();
	}
	Vector<StackItem> Items;

	StackItem* Has(IRInstruction* Value)
	{
		for (auto& Item : Items)
		{
			if (Item.IR == Value)
			{
				return &Item;
			}
		}

		return nullptr;
	}
};
UCodeLangEnd
