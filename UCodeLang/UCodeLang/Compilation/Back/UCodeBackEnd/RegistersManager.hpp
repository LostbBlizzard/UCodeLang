#pragma once

#ifndef UCodeLangNoCompiler
#include "../../Middle/IR.hpp"
#include "../../Middle/IRBackEndHelper.hpp"
UCodeLangStart

class RegistersManager
{
public:


	struct RegisterInfo
	{
		Optional<Variant<AnyInt64, const IRInstruction*, IROperator, IRAndOperator>> Types;
	};

	RegistersManager();
	~RegistersManager();

	void Reset();

	static constexpr size_t RegisterSize = (RegisterID_t)RegisterID::EndRegister - (RegisterID_t)RegisterID::StartRegister + 1;
	Array<RegisterInfo, RegisterSize> Registers;

	auto& GetInfo(RegisterID id)
	{
		return Registers[(size_t)id];
	}
	bool IsUsed(RegisterID id)
	{
		return GetInfo(id).Types.has_value();
	}

	Optional<RegisterID> GetInfo(const IRInstruction* IRField)
	{
		for (size_t i = 0; i < RegisterSize; i++)
		{
			auto& Info = Registers[i];
			if (Info.Types.has_value())
			{
				auto& Value = Info.Types.value();
				if (auto IR = Value.Get_If<const IRInstruction*>())
				{
					if (*IR == IRField)
					{
						return (RegisterID)i;
					}
				}
			}
		}
		return {};
	}
	Optional<RegisterID> GetInfo(IROperator Value)
	{
		for (size_t i = 0; i < RegisterSize; i++)
		{
			auto& Info = Registers[i];
			if (Info.Types.has_value())
			{
				auto& TypesValue = Info.Types.value();
				if (auto Any = TypesValue.Get_If<IROperator>())
				{
					if (*Any == Value)
					{
						return (RegisterID)i;
					}
				}
			}
		}
		return {};
	}

	Optional<RegisterID> GetValue(AnyInt64 Value)
	{
		for (size_t i = 0; i < RegisterSize; i++)
		{
			auto& Info = Registers[i];
			if (Info.Types.has_value())
			{
				auto& TypesValue = Info.Types.value();
				if (auto Any = TypesValue.Get_If<AnyInt64>())
				{
					if (Any->AsInt64 == Value.AsInt64)
					{
						return (RegisterID)i;
					}
				}
			}
		}
		return {};
	}


	Optional<RegisterID> GetFreeRegister()
	{
		for (size_t i = 0; i < RegisterSize; i++)
		{
			auto& Info = Registers[i];
			if (!Info.Types.has_value())
			{
				return (RegisterID)i;
			}
		}
		return {};
	}


	void SetRegister(RegisterID id, AnyInt64 Value)
	{
		auto& Info = Registers[(size_t)id];
		Info.Types = Value;
	}

	void SetRegister(RegisterID id, const IRInstruction* Value)
	{
		auto& Info = Registers[(size_t)id];
		Info.Types = Value;
	}

	void SetRegister(RegisterID id, const IROperator& Value)
	{
		auto& Info = Registers[(size_t)id];
		Info.Types = Value;
	}
	void FreeRegister(RegisterID id)
	{
		auto& Info = Registers[(size_t)id];
		Info.Types = {};
	}
};

class StaticMemoryManager
{
public:
	struct StaticMemInfo
	{
		size_t Offset = 0;
	};
	UnorderedMap<IRidentifierID, StaticMemInfo> _List;
};
struct StackItem
{
	size_t Offset = 0;
	Variant<const IRInstruction*, IROperator, IRAndOperator> IR;

	StackItem(
		size_t offset, const IRInstruction* ir)
		:Offset(offset), IR(ir)
	{

	}
	StackItem(
		size_t offset, const IRAndOperator& ir)
		:Offset(offset), IR(ir)
	{

	}
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
		Reupdates.clear();
	}
	Vector<Unique_ptr<StackItem>> Items;
	struct StackOffsetReUpdate
	{
		size_t InsIndex = 0;

		bool PostFunc = true;
		size_t StackOffset = 0;
	};
	Vector<StackOffsetReUpdate> Reupdates;
	void AddReUpdatePreFunc(size_t InsIndex, size_t StackOffset)
	{
		Reupdates.push_back({ InsIndex,false,StackOffset });
	}
	void AddReUpdatePostFunc(size_t InsIndex, size_t StackOffset)
	{
		Reupdates.push_back({ InsIndex,true,StackOffset });
	}


	NullablePtr<StackItem> Get(size_t Offset)
	{
		for (auto& Item : Items)
		{
			if (Item->Offset == Offset)
			{
				return Item.get();
			}
		}

		return nullptr;
	}

	NullablePtr<StackItem> Has(const IRInstruction* Value)
	{
		for (auto& Item : Items)
		{
			if (auto Val = Item->IR.Get_If<const IRInstruction*>())
			{
				if (*Val == Value)
				{
					return Item.get();
				}
			}
		}

		return {};
	}
	NullablePtr<StackItem> Has(const IRAndOperator& Value)
	{
		for (auto& Item : Items)
		{
			if (auto Val = Item->IR.Get_If<IRAndOperator>())
			{
				if (*Val == Value)
				{
					return Item.get();
				}
			}
		}

		return nullptr;
	}

	NeverNullPtr<StackItem> Add(const IRInstruction* IR, size_t Offset)
	{
		return Items.emplace_back(std::make_unique<StackItem>(Offset, IR)).get();
	}
	NeverNullPtr<StackItem> Add(const IRAndOperator& IR, size_t Offset)
	{
		return Items.emplace_back(std::make_unique<StackItem>(Offset, IR)).get();
	}
	NeverNullPtr<StackItem> Add(const StackItem& Item)
	{
		return Items.emplace_back(std::make_unique<StackItem>(Item)).get();
	}
	NeverNullPtr<StackItem> AddWithSize(const IRInstruction* IR, size_t ObjectSize)
	{
		auto R = Add(IR, Size);
		Size += ObjectSize;
		return R;
	}
	NeverNullPtr<StackItem> AddWithSize(const IRAndOperator& IR, size_t ObjectSize)
	{
		auto R = Add(IR, Size);
		Size += ObjectSize;
		return R;
	}
};
UCodeLangEnd

#endif