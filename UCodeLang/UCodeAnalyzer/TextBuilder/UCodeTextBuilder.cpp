#include "UCodeTextBuilder.hpp"
UCodeAnalyzerStart


#define ScopeCommonnet \
Str.Str += "\n"; \
AddIndents(Str.Str, Str.IndentLevel); \
Str.Str += (String)"// " + Node.Name + "\n"; \


void UCodeTextBuilder::ToUCodeFile(String& Str) const
{
	BuildState State;

	for (auto Item : _GlobalNamespace._Nodes)
	{
		AddNodeToString(State,*Item);
	}

	Str = State.Str;
}

void UCodeTextBuilder::AddNameSpaceToString(BuildState& Str,const NamespaceNode& Node)
{
	AddIndents(Str.Str,Str.IndentLevel);
	Str.Str += KeyWordAndTokens::NamespaceKeyWord + Node.Name + ": \n";
	Str.IndentLevel += 1;
	
	//
	for (auto Item : Node._Nodes)
	{
		AddNodeToString(Str, *Item);
	}
	//
	AddIndents(Str.Str, Str.IndentLevel);
	
	ScopeCommonnet;
	
	Str.IndentLevel -= 1;
	
}
void UCodeTextBuilder::AddNodeToString(BuildState& Str, const UCodeNode& Node)
{
	switch (Node.Type)
	{
	case UCodeClassType::Namespace:
		AddNameSpaceToString(Str, Node.NamesSpace);
		break;
	case UCodeClassType::Alias:
		AddAlias(Str, Node.Alias);
		break;
	case UCodeClassType::Class:
		AddClassToString(Str, Node.Class);
		break;
	case UCodeClassType::Enum:
		AddEnumToString(Str, Node.Enum);
		break;
	case UCodeClassType::Attribute:
		AddAttributeNodeToString(Str, Node.Attribute);
		break;
	case UCodeClassType::UseingStatement:
		AddUseingToString(Str, Node.Useings);
		break;
	default:
		break;
	}
}
void UCodeTextBuilder::AddClassToString(BuildState& Str, const ClassNode& Node)
{
	AddIndents(Str.Str, Str.IndentLevel);

	AddAttributesToString(Str, Node.attributes);
	Str.Str += KeyWordAndTokens::ClassKeyWord + Node.Name;
	AddGenericsToString(Str.Str,Node.GenericTypes);
	if (Node.MemberVariables.size() == 0 &&
		Node.Methods.size() == 0 &&
		Node._Nodes.size() == 0)
	{
		Str.Str += ";";
		return;
	}
	Str.Str += ": \n";
	
	Str.IndentLevel += 1;
	//
	for (auto& Item : Node.MemberVariables)
	{
		AddIndents(Str.Str, Str.IndentLevel);
		Str.Str += Item.Type + " " + Item.Name;
		if (Item.Expression.size())
		{
			Str.Str += " = " + Item.Expression;
		}
		Str.Str += ";\n";
	}

	for (auto& Item : Node.Methods)
	{
		AddMethodToString(Str,Item);
	}

	for (auto Item : Node._Nodes)
	{
		AddNodeToString(Str, *Item);
	}
	//
	AddIndents(Str.Str, Str.IndentLevel);

	ScopeCommonnet;

	Str.IndentLevel -= 1;
}

void UCodeTextBuilder::AddMethodToString(BuildState& Str, const Method& Node)
{
	AddIndents(Str.Str, Str.IndentLevel);
	AddAttributesToString(Str, Node.attributes);
	Str.Str += KeyWordAndTokens::FuncKeyWord + Node.Name;
	AddGenericsToString(Str.Str, Node.GenericTypes);
	Str.Str += "[";

	for (size_t i = 0; i < Node.parameters.size(); i++)
	{
		auto& Item = Node.parameters[i];
		Str.Str += Item.Type + " " + Item.Name;

		if (i != Node.parameters.size() - 1)
		{
			Str.Str += ",";
		}
	}

	Str.Str += "] -> " + Node.RetType;
	Str.Str += ";\n";
}
void UCodeTextBuilder::AddAttributesToString(BuildState& Str, const Vector<Attribute>& Node)
{
	constexpr size_t MaxForNextLine = 4;
	size_t AttributesNextLine = MaxForNextLine;
	for (auto& Item : Node)
	{
		AddAttributeToString(Str, Item);

		if (MaxForNextLine == 0)
		{
			Str.Str += "\n";
			AddIndents(Str.Str, Str.IndentLevel);
			AttributesNextLine = AttributesNextLine;
		}
		AttributesNextLine--;
	}
	if (Node.size()) 
	{
		Str.Str += "\n";
		AddIndents(Str.Str, Str.IndentLevel);
	}
}
void UCodeTextBuilder::AddAttributeToString(BuildState& Str, const Attribute& Node)
{
	Str.Str += "[" + Node.Type + "]";
}
void UCodeTextBuilder::AddEnumToString(BuildState& Str, const EnumNode& Node)
{
	AddIndents(Str.Str, Str.IndentLevel);
	AddAttributesToString(Str, Node.attributes);
	Str.Str += KeyWordAndTokens::EnumKeyWord + (String)" " + Node.Name;

	if (Node.BaseType.size())
	{
		Str.Str += "[" + Node.BaseType + "]";
	}

	if (Node.Values.size() == 0)
	{
		Str.Str += ";";
		return;
	}

	Str.Str += ": \n";
	Str.IndentLevel += 1;
	//

	for (auto& Item : Node.Values)
	{
		AddIndents(Str.Str, Str.IndentLevel);
		Str.Str += Item.Name;
		if (Item.Expression.size())
		{
			Str.Str += " = " + Item.Expression;
		}

		if (&Item != &Node.Values.back())
		{
			Str.Str += ",";
		}
		Str.Str += "\n";
	}

	//
	AddIndents(Str.Str, Str.IndentLevel);
	
	ScopeCommonnet;

	Str.IndentLevel -= 1;
}
void UCodeTextBuilder::AddAttributeNodeToString(BuildState& Str, const AttributeNode& Node)
{
	AddIndents(Str.Str, Str.IndentLevel);
	AddAttributesToString(Str, Node.attributes);
	Str.Str += KeyWordAndTokens::TagKeyWord + (String)" " + Node.Name;
	if (0 == 0)//this will be Fixed this soon
	{
		Str.Str += ";";
		return;
	}
	Str.Str += ": \n";
	Str.IndentLevel += 1;
    //


	//
	AddIndents(Str.Str, Str.IndentLevel);
	
	ScopeCommonnet;

	Str.IndentLevel -= 1;
}
void UCodeTextBuilder::AddGenericToString(String& Str, const Generic& Node)
{
	Str += Node.Type;
}
void UCodeTextBuilder::AddGenericsToString(String& Str, const Vector<Generic>& Node)
{
	if (Node.size())
	{
		Str += "<";
		for (size_t i = 0; i < Node.size(); i++)
		{
			auto& Item = Node[i];
			AddGenericToString(Str,Item);

			if (i != Node.size() - 1)
			{
				Str += ",";
			}
		}

		Str += ">";
	}
}
void UCodeTextBuilder::AddAlias(BuildState& Str, const AliasNode& Node)
{
	AddIndents(Str.Str, Str.IndentLevel);
	AddAttributesToString(Str, Node.attributes);
	Str.Str += KeyWordAndTokens::ClassKeyWord + Node.NewName + " = " + Node.OldName;
	AddGenericsToString(Str.Str, Node.GenericTypes);
	Str.Str += ";" + (String)" \n";
}
void UCodeTextBuilder::AddIndents(String& Str, const size_t Indent)
{
	for (size_t i = 0; i < Indent; i++){Str += ' ';}
}
void UCodeTextBuilder::AddUseingToString(BuildState& Str, const UseingStatement& Node)
{
	AddIndents(Str.Str, Str.IndentLevel);
	if (Node.Names.size())
	{
		Str.Str += (String)KeyWordAndTokens::UseKeyWord += " ";

		for (size_t i = 0; i < Node.Names.size(); i++)
		{
			auto& Item = Node.Names[i];
			AddUseingToString(Str.Str, Item);

			if (i != Node.Names.size() - 1)
			{
				Str.Str += KeyWordAndTokens::ScopeResolutionKeyWord;
			}
		}

		Str.Str += ";\n";
	}

}
void UCodeTextBuilder::AddUseingToString(String& Str, const UseingItem& Node)
{
	Str += Node.Name;
	AddGenericsToString(Str, Node.GenericTypes);
}

ClassNode& ClassNode::AddClass(const String& Name)
{
	auto Ptr = new UCodeNode();
	_Nodes.push_back(Ptr);
	UCodeNode::MakeClass(*Ptr, Name);

	return Ptr->Class;
}

AliasNode& ClassNode::AddAlias(const String& Name, const String& oldType)
{
	auto Ptr = new UCodeNode();
	_Nodes.push_back(Ptr);
	UCodeNode::MakeAlias(*Ptr, Name, oldType);

	return Ptr->Alias;
}

EnumNode& ClassNode::AddEnum(const String& Name)
{
	auto Ptr = new UCodeNode();
	_Nodes.push_back(Ptr);
	UCodeNode::MakeEnum(*Ptr, Name);

	return Ptr->Enum;
}

AttributeNode& ClassNode::AddAttributeClass(const String& Name)
{
	auto Ptr = new UCodeNode();
	_Nodes.push_back(Ptr);
	UCodeNode::MakeAttribute(*Ptr, Name);

	return Ptr->Attribute;
}

ClassNode::~ClassNode()
{
	for (auto Item : _Nodes)
	{
		delete Item;
	}
	_Nodes.clear();
}


 NamespaceNode& NamespaceNode::AddNameSpace(const String& Name)
{
	auto Ptr = new UCodeNode();
	_Nodes.push_back(Ptr);
	UCodeNode::MakeNamesSpace(*Ptr, Name);

	return Ptr->NamesSpace;
}

 ClassNode& NamespaceNode::AddClass(const String& Name)
{
	auto Ptr = new UCodeNode();
	_Nodes.push_back(Ptr);
	UCodeNode::MakeClass(*Ptr, Name);

	return Ptr->Class;
}

 AliasNode& NamespaceNode::AddAlias(const String& Name, const String& oldType)
{
	auto Ptr = new UCodeNode();
	_Nodes.push_back(Ptr);
	UCodeNode::MakeAlias(*Ptr, Name, oldType);

	return Ptr->Alias;
}

 EnumNode& NamespaceNode::AddEnum(const String& Name)
{
	auto Ptr = new UCodeNode();
	_Nodes.push_back(Ptr);
	UCodeNode::MakeEnum(*Ptr, Name);

	return Ptr->Enum;
}

AttributeNode& NamespaceNode::AddAttributeClass(const String& Name)
{
	auto Ptr = new UCodeNode();
	_Nodes.push_back(Ptr);
	UCodeNode::MakeAttribute(*Ptr, Name);

	return Ptr->Attribute;
}

UseingStatement& NamespaceNode::AddUseing(const String& Name)
{
	auto Ptr = new UCodeNode();
	_Nodes.push_back(Ptr);
	UCodeNode::MakeUseing(*Ptr, Name);

	return Ptr->Useings;
}

NamespaceNode::~NamespaceNode()
{
	for (auto Item : _Nodes)
	{
		delete Item;
	}
	_Nodes.clear();
}
UCodeAnalyzerEnd