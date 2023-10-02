#include "UCodeLang/Compliation/Front/SystematicAnalysis.hpp"
UCodeLangFrontStart

void SystematicAnalysis::OnAttributeNode(const AttributeNode& node)
{


	if (_PassType == PassType::GetTypes)
	{

	}
	else if (_PassType == PassType::FixedTypes)
	{
		String V;
		node._ScopedName.GetScopedName(V);

		auto SybID = Symbol_GetSymbolID(node);
		auto& Syb = Symbol_AddSymbol(SymbolType::UsedTag, CompilerGeneratedStart + V + CompilerGeneratedEnd, CompilerGeneratedStart + V + CompilerGeneratedEnd, AccessModifierType::Private);
		_Table.AddSymbolID(Syb, SybID);



		auto AttOp = Symbol_GetSymbol(V, SymbolType::Tag_class);
		if (AttOp)
		{
			auto Att = AttOp.value();
			auto token = NeverNullptr(node._ScopedName._ScopedName.back()._token);
			if (Att->Type == SymbolType::Tag_class)
			{
				auto& Generic = *node._ScopedName._ScopedName.back()._generic;
				if (Generic._Values.size())
				{
					LogError_ExpectedSymbolToBea(token, *Att, SymbolType::Generic_Tag);
				}

			}
			else if (Att->Type == SymbolType::Generic_Tag)
			{
				auto& Generic = *node._ScopedName._ScopedName.back()._generic;
				if (Generic._Values.size())
				{
					auto Info = Att->Get_Info<TagInfo>();
					auto TagNode = TagTypeNode::As(Att->Get_NodeInfo<Node>());

					Att = Generic_InstantiateOrFindGeneric_Tag(token, Att, TagNode->_generic, Info->_GenericData, Generic).value();
				}
				else
				{
					LogError_ExpectedSymbolToBea(token, *Att, SymbolType::Tag_class);
				}
			}
			else
			{
				LogError_ExpectedSymbolToBea(token, *Att, SymbolType::Tag_class);
			}
			Syb.VarType = TypeSymbol(Att->ID);
		}
		else
		{
			auto token = NeverNullptr(node._ScopedName._ScopedName.back()._token);
			LogError_CantFindTypeError(token, V);
		}
	}
	else if (_PassType == PassType::BuidCode)
	{

	}
}
void SystematicAnalysis::OnAttributesNode(const Vector<Unique_ptr<AttributeNode>>& nodes)
{
	for (auto& Item : nodes)
	{
		OnAttributeNode(*Item);
	}
}


UCodeLangFrontEnd