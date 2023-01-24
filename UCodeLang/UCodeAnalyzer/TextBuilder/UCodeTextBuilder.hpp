#pragma once
#include "../UCodeAnalyzer.hpp"
#include "UCodeLang/Compliation/Front/Parser.hpp"
#include "Typedef.hpp"
#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"
UCodeAnalyzerStart

#define defaultAliasValue Uint8TypeName

class KeyWordAndTokens
{
public:
	constexpr static const char* NamespaceKeyWord = UCodeLang::StringHelper::ToString(UCodeLang::TokenType::Namespace);
	constexpr static const char* ClassKeyWord = UCodeLang::StringHelper::ToString(UCodeLang::TokenType::Class);
	constexpr static const char* EnumKeyWord = UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_Enum);
	constexpr static const char* TagKeyWord = UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_Tag);
	constexpr static const char* VarKeyWord = UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_var);
	constexpr static const char* VoidKeyWord = UCodeLang::StringHelper::ToString(UCodeLang::TokenType::Void);
	constexpr static const char* FuncKeyWord = UCodeLang::StringHelper::ToString(UCodeLang::TokenType::bitwise_or);
	constexpr static const char* AsmKeyWord = UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_asm);

	constexpr static const char* TrueKeyWord = UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_True);
	constexpr static const char* FalseKeyWord = UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_False);
	constexpr static const char* UseKeyWord = UCodeLang::StringHelper::ToString(UCodeLang::TokenType::KeyWorld_use);
	constexpr static const char* ScopeResolutionKeyWord = UCodeLang::StringHelper::ToString(UCodeLang::TokenType::ScopeResolution);
};

enum class UCodeClassType :UInt8
{
	Namespace,
	Class,
	Alias,
	Enum,
	Attribute,
	UseingStatement,
};

class parameter
{
public:
	String Name;
	String Type;
};
class Valueparameter
{
public:
	String Expression;
};
class Generic
{
public:
	String Type;
};
class Attribute
{
public:
	String Type;
	Vector<Valueparameter> parameters;
};
class UseingItem
{
public:
	String Name;
	Vector<Generic> GenericTypes;
};
class UseingStatement
{
public:
	Vector<UseingItem> Names;
	UCodeLangForceinline UseingItem& AddName(const String& name)
	{
		Names.push_back({});
		auto& ret = Names.back();
		ret.Name = name;
		return ret;
	}
};
class Method
{
private:
	
public:
	String Name;
	String RetType;
	bool IsMember = false;
	Vector<parameter> parameters;
	Vector<Attribute> attributes;
	Vector<Generic> GenericTypes;
	Attribute& AddAttribute(const String& Name)
	{
		attributes.push_back({ Name});

		return attributes.back();
	}

	inline parameter& AddParameter(const String& Name, const String& type)
	{
		parameters.push_back({ Name,type });

		return parameters.back();
	}
	inline Generic& AddGeneric(const String& Name)
	{
		GenericTypes.push_back({ Name });
		return GenericTypes.back();
	}
	
	parameter& AddParameterUInt8(const String& Name)
	{
		parameters.push_back({ Name,Uint8TypeName });

		return parameters.back();
	}
	parameter& AddParameterInt8(const String& Name)
	{
		parameters.push_back({ Name,Sint8TypeName });

		return parameters.back();
	}
	parameter& AddParameterbool(const String& Name)
	{
		parameters.push_back({ Name,boolTypeName });

		return parameters.back();
	}
	parameter& AddParameterUintptr(const String& Name)
	{
		parameters.push_back({ Name,UintPtrTypeName });

		return parameters.back();
	}
	parameter& AddParameterChar(const String& Name)
	{
		parameters.push_back({ Name,CharTypeName });

		return parameters.back();
	}
	
private:
	
};
class UCodeNode;
class AliasNode;
class EnumNode;
class AttributeNode;
class ClassNode
{
public:
	class MemberVariable
	{
	public:
		String Name;
		String Type;
		String Expression;//Size == 0 for default
	};

	String Name;
	Vector<MemberVariable> MemberVariables;
	Vector<Method> Methods;
	Vector<Attribute> attributes;
	Vector<Generic> GenericTypes;
	Vector<Unique_ptr<UCodeNode>> _Nodes;

	
	Attribute& AddAttribute(const String& Name)
	{
		attributes.push_back({ Name });

		return attributes.back();
	}
	inline MemberVariable& AddMember(const String& Name, const String& Type, const String& expression = "")
	{
		MemberVariables.push_back({ Name,Type,expression });


		return MemberVariables.back();
	}
	inline MemberVariable& AddMember_TypeVar(const String& Name, const String& expression = "")
	{
		return AddMember(Name, KeyWordAndTokens::VarKeyWord,expression);
	}
	inline Method& AddMethod(const String& Name, const String& retrype = KeyWordAndTokens::VoidKeyWord)
	{
		Methods.push_back({ Name,retrype });

		return Methods.back();
	}
	inline Generic& AddGeneric(const String& Name)
	{
		GenericTypes.push_back({ Name });
		return GenericTypes.back();
	}

	ClassNode& AddClass(const String& Name);
	AliasNode& AddAlias(const String& Name, const String& oldType = defaultAliasValue);
	EnumNode& AddEnum(const String& Name);
	AttributeNode& AddAttributeClass(const String& Name);

	~ClassNode();
};
class AliasNode
{
public:
	String OldName;
	String NewName;

	Vector<Attribute> attributes;
	Vector<Generic> GenericTypes;

	Attribute& AddAttribute(const String& Name)
	{
		attributes.push_back({ Name });

		return attributes.back();
	}
	inline Generic& AddGeneric(const String& Name)
	{
		GenericTypes.push_back({ Name });
		return GenericTypes.back();
	}
};

class EnumNode
{
public:
	class EnumNodeValue
	{
	public:
		String Name;
		String Expression;//Size == 0 for default
	};
	String Name;
	String BaseType;//Size == 0 for default
	Vector<EnumNodeValue> Values;
	inline EnumNodeValue& AddValue(const String& Name, const String& expression = "")
	{
		Values.push_back({ Name,expression });


		return Values.back();
	}

	Vector<Attribute> attributes;

	Attribute& AddAttribute(const String& Name)
	{
		attributes.push_back({ Name });

		return attributes.back();
	}
};
class AttributeNode
{
public:
	String Name;
	Vector<Attribute> attributes;

	Attribute& AddAttribute(const String& Name)
	{
		attributes.push_back({ Name });

		return attributes.back();
	}
};

class UCodeNode;

class NamespaceNode
{
public:
	String Name;

	Vector<Unique_ptr<UCodeNode>> _Nodes;
	NamespaceNode()
	{

	}

	NamespaceNode& AddNameSpace(const String& Name);
	ClassNode& AddClass(const String& Name);
	AliasNode& AddAlias(const String& Name, const String& oldType = defaultAliasValue);
	EnumNode& AddEnum(const String& Name);
	AttributeNode& AddAttributeClass(const String& Name);
	UseingStatement& AddUseing(const String& Name = "");

	~NamespaceNode();
};
class UCodeNode
{
public:
	UCodeClassType Type;
	//union 
	//{
	NamespaceNode NamesSpace;
	ClassNode Class;
	AliasNode Alias;
	EnumNode Enum;
	AttributeNode Attribute;
	UseingStatement	Useings;
	//};
	
	UCodeNode()
	{
		Type = UCodeClassType::Namespace;
	}
	~UCodeNode()
	{

	}
	
	static UCodeLangForceinline void MakeNamesSpace(UCodeNode& nullNode, const String& Name)
	{
		nullNode.Type = UCodeClassType::Namespace;
		nullNode.NamesSpace.Name = Name;
	}
	static UCodeLangForceinline void MakeClass(UCodeNode& nullNode, const String& Name)
	{
		nullNode.Type = UCodeClassType::Class;
		nullNode.Class.Name = Name;
	}
	static UCodeLangForceinline void MakeAlias(UCodeNode& nullNode, const String& Name,const String& oldType = defaultAliasValue)
	{
		nullNode.Type = UCodeClassType::Alias;
		nullNode.Alias.NewName = Name;
		nullNode.Alias.OldName = oldType;
	}
	static UCodeLangForceinline void MakeEnum(UCodeNode& nullNode, const String& Name)
	{
		nullNode.Type = UCodeClassType::Enum;
		nullNode.Enum.Name = Name;
	}
	static UCodeLangForceinline void MakeUseing(UCodeNode& nullNode, const String& Name)
	{
		nullNode.Type = UCodeClassType::UseingStatement;
		nullNode.Useings.AddName(Name);
	}
	static UCodeLangForceinline void MakeAttribute(UCodeNode& nullNode, const String& Name)
	{
		nullNode.Type = UCodeClassType::Attribute;
		nullNode.Attribute.Name = Name;
	}
};
struct BuildState
{
	String Str;
	size_t IndentLevel = 0;
};
class UCodeTextBuilder
{
public:

	void ToUCodeFile(String& Str) const;
	static void AddNameSpaceToString(BuildState& Str, const NamespaceNode& Node);
	static void AddNodeToString(BuildState& Str, const UCodeNode& Node);
	static void AddClassToString(BuildState& Str, const ClassNode& Node);
	static void AddAlias(BuildState& Str, const AliasNode& Node);
	static void AddEnumToString(BuildState& Str, const EnumNode& Node);
	static void AddMethodToString(BuildState& Str, const Method& Node);
	static void AddIndents(String& Str, const size_t Indent);
	static void AddGenericsToString(String& Str, const Vector<Generic>& Node);
	static void AddUseingToString(BuildState& Str, const UseingStatement& Node);
	static void AddUseingToString(String& Str, const UseingItem& Node);
	static void AddGenericToString(String& Str, const Generic& Node);
	static void AddAttributesToString(BuildState& Str, const Vector<Attribute>& Node);
	static void AddAttributeToString(BuildState& Str, const Attribute& Node);

	static void AddAttributeNodeToString(BuildState& Str, const AttributeNode& Node);

	UCodeLangForceinline NamespaceNode& AddNameSpace(const String& Name)
	{
		return _GlobalNamespace.AddNameSpace(Name);
	}
	UCodeLangForceinline ClassNode& AddClass(const String& Name)
	{
		return _GlobalNamespace.AddClass(Name);
	}
	UCodeLangForceinline AliasNode& AddAlias(const String& Name, const String& oldType = defaultAliasValue)
	{
		return _GlobalNamespace.AddAlias(Name, oldType);
	}
	UCodeLangForceinline EnumNode& AddEnum(const String& Name)
	{
		return _GlobalNamespace.AddEnum(Name);
	}
	UCodeLangForceinline AttributeNode& AddAttributeClass(const String& Name)
	{
		return _GlobalNamespace.AddAttributeClass(Name);
	}
	UCodeLangForceinline UseingStatement& AddUseing(const String& Name)
	{
		return _GlobalNamespace.AddUseing(Name);
	}
private:
	NamespaceNode _GlobalNamespace;
};
UCodeAnalyzerEnd

