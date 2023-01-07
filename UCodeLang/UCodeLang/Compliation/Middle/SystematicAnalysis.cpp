#include "SystematicAnalysis.hpp"
#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"

#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
#include "UCodeLang/Compliation/Back/UCodeBackEnd/UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Helpers/ParseHelper.hpp"
UCodeLangStart

void SystematicAnalysis::Reset()
{
	_Lib.ClearState();
	passtype = PassType::Null;
	_Builder.Reset();
}

bool SystematicAnalysis::Analyze(const FileNode& File)
{
	Vector< UClib*> Libs;
	Vector<FileNode*> files;
	files.push_back((FileNode*)&File);//shh
	return Analyze(files, Libs);
}
bool SystematicAnalysis::Analyze(const Vector<FileNode*>& Files, const Vector<UClib*>& Libs)
{


	passtype = PassType::Null;
	_Files = &Files;
	_Libs = &Libs;

	passtype = PassType::GetTypes;
	Pass();

	if (_ErrorsOutput->Has_Errors()) { goto EndFunc; };

	LoadLibSymbols();
	passtype = PassType::FixedTypes;
	Pass();


	if (!_ErrorsOutput->Has_Errors()) {
		BuildCode();
	}

EndFunc:
	_Files = nullptr;
	_Libs = nullptr;

	_Lib._LibType = OutputTypeAsLibType();

	return !_ErrorsOutput->Has_Errors();
}
void SystematicAnalysis::BuildCode()
{
	if (BackEnd == nullptr)
	{
		BackEnd = UCodeBackEnd::Get();
	}
	BackEndObject = BackEnd->GenNewBackEnd();
	BackEnd->SetBackEndAnalysis(BackEndObject, this);
	BackEnd->SetBackEndErrors(BackEndObject, _ErrorsOutput);

	passtype = PassType::BuidCode;
	Pass();

	BackEndInput input;
	input._Builder = &_Builder;
	input._Table = &_Table;
	BackEnd->Build(BackEndObject, input);

	BackEnd->DeleteBackEnd(BackEndObject);
	BackEndObject = nullptr;
}
void SystematicAnalysis::Pass()
{
	for (const auto& File : *_Files)
	{
		OnFileNode(File);
	}
	
}
void SystematicAnalysis::OnNamespace(const NamespaceNode& node)
{
	auto UseingIndex = _Table.GetUseingIndex();

	const auto Namespace = GetScopedNameAsString(node.NamespaceName);
	_Table.AddScope(Namespace);
	for (auto node : node._Nodes)
	{
		switch (node->Get_Type())
		{
		case NodeType::NamespaceNode:OnNamespace(*NamespaceNode::As(node)); break;
		case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node)); break;
		case NodeType::ClassNode: OnClassNode(*ClassNode::As(node)); break;
		case NodeType::EnumNode:OnEnum(*EnumNode::As(node)); break;
		case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node)); break;
		case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node)); break;
		default:break;
		}
	}
	_Table.RemoveScope();

	_Table.RemovePopUseing(UseingIndex);
}
void SystematicAnalysis::OnAttributeNode(const AttributeNode& node)
{
	if (passtype == PassType::GetTypes) {
		_TepAttributes.push_back(&node);
	}
}
void SystematicAnalysis::OnNonAttributeable(size_t Line, size_t Pos)
{
	if (passtype == PassType::GetTypes)
	{
		if (_TepAttributes.size())
		{
			_ErrorsOutput->AddError(ErrorCodes::TreeAnalyerError, Line, Pos, "You cant put a Tag on this");
		}
	}
}
void SystematicAnalysis::OnFileNode(UCodeLang::FileNode* const& File)
{
	for (auto node : File->_Nodes)
	{
		switch (node->Get_Type())
		{
		case NodeType::NamespaceNode:OnNamespace(*NamespaceNode::As(node)); break;
		case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node)); break;
		case NodeType::ClassNode: OnClassNode(*ClassNode::As(node)); break;
		case NodeType::EnumNode:OnEnum(*EnumNode::As(node)); break;
		case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node)); break;
		case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node)); break;
		default:break;
		}
	}
	_Table.ClearUseings();
}
void SystematicAnalysis::OnClassNode(const ClassNode& Node)
{
	const auto& ClassName = Node.ClassName.Token->Value._String;


	_Table.AddScope(ClassName);

	
	auto& Class = passtype == PassType::GetTypes ?
		_Lib.Get_Assembly().AddClass(String(ClassName), _Table._Scope.ThisScope)
		: *_Lib.Get_Assembly().Find_Class(_Table._Scope.ThisScope);
	_ClassStack.push(&Class);

	if (passtype == PassType::GetTypes)
	{
		PushTepAttributesInTo(Class._Class.Attributes);
	}

	auto UseingIndex = _Table.GetUseingIndex();

	for (const auto& node : Node._Nodes)
	{
		switch (node->Get_Type())
		{
		case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node)); break;
		case NodeType::ClassNode: OnClassNode(*ClassNode::As(node)); break;
		case NodeType::EnumNode:OnEnum(*EnumNode::As(node)); break;
		case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node)); break;
		case NodeType::FuncNode:OnFuncNode(*FuncNode::As(node)); break;
		case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(node)); break;
		default:break;
		}
	}

	if (passtype == PassType::GetTypes)
	{
		for (auto& node : Class._Class.Fields)
		{
			if (node.FullNameType == Uint8TypeName|| node.FullNameType == Sint8TypeName
				|| node.FullNameType == CharTypeName || node.FullNameType == boolTypeName)
			{
				node.offset = Class._Class.Size;
				Class._Class.Size += sizeof(UInt8);
			}
			else if(node.FullNameType == Uint16TypeName || node.FullNameType == Sint16TypeName)
			{
				node.offset = Class._Class.Size;
				Class._Class.Size += sizeof(UInt16);
			}
			else if (node.FullNameType == Uint32TypeName || node.FullNameType == Sint32TypeName)
			{
				node.offset = Class._Class.Size;
				Class._Class.Size += sizeof(UInt32);
			}
			else if (node.FullNameType == Uint64TypeName || node.FullNameType == Sint64TypeName
				|| node.FullNameType == UintPtrTypeName || node.FullNameType == SintPtrTypeName)
			{
				node.offset = Class._Class.Size;
				Class._Class.Size += sizeof(UInt64);
			}
			else
			{
				node.offset = Class._Class.Size;
				Class._Class.Size += sizeof(UInt64);
			}
		}
	}

	_Table.RemovePopUseing(UseingIndex);


	_ClassStack.pop();
	_Table.RemoveScope();
}
void SystematicAnalysis::OnUseingNode(const UsingNode& node)
{
	auto T = node.ScopedName.ScopedName[0];
	OnNonAttributeable(T->OnLine, T->OnPos);
	const auto UseingString =GetScopedNameAsString(node.ScopedName);
	_Table.AddUseing(UseingString);

}
void SystematicAnalysis::OnFuncNode(const FuncNode& node)
{
	auto FuncName = node.Signature.Name.AsStringView();
	auto NameToken = node.Signature.Name.Token;
	if (NameToken->Type == TokenType::KeyWorld_This)
	{
		FuncName = ClassConstructorfunc;
		if (_ClassStack.empty())
		{
			_ErrorsOutput->AddError(ErrorCodes::InValidType, NameToken->OnLine, NameToken->OnPos, "cant use this here");
		}
	}
	else if (NameToken->Type == TokenType::KeyWorld_Drop)
	{
		FuncName = ClassDestructorFunc;
	}

	
	
	_Table.AddScope(FuncName);
	auto FullName = _Table._Scope.ThisScope;

	ClassData* Ptr = nullptr;
	if (_ClassStack.empty())
	{
		auto& Assembly = _Lib.Get_Assembly();

		auto globalAssemblyObjectName = (String_view)ScopeHelper::_globalAssemblyObject;

		Ptr = Assembly.Find_Class(globalAssemblyObjectName);
		if (Ptr == nullptr)
		{
			Ptr = &_Lib.Get_Assembly().AddClass(String(globalAssemblyObjectName));
		}
	}
	else
	{
		Ptr = _ClassStack.top();
	}

	
	
	Symbol* syb;
	SymbolID sybId = (SymbolID)&node;
	

	auto UseingIndex = _Table.GetUseingIndex();

	if (passtype == PassType::GetTypes)
	{
		ClassMethod V;
		V.FullName = _Table._Scope.ThisScope;
		PushTepAttributesInTo(V.Attributes);


		Ptr->_Class.Methods.push_back(V);

		syb = &_Table.AddSybol(SymbolType::Func, (String)FuncName, FullName);
		_Table.AddSymbolID(*syb, sybId);
	}
	else
	{
		syb = &_Table.GetSymbol(sybId);
	}
	bool buidCode = passtype == PassType::BuidCode;
	
	if (buidCode)
	{
		_Builder.Build_Func(sybId);
	}

	if (node.Body.has_value()) 
	{
		auto& Body = node.Body.value();
		_InStatements = true;
		for (const auto& node2 : Body.Statements._Nodes)
		{
			switch (node2->Get_Type())
			{
			case NodeType::AttributeNode:OnAttributeNode(*AttributeNode::As(node2)); break;
			case NodeType::ClassNode: OnClassNode(*ClassNode::As(node2)); break;
			case NodeType::EnumNode:OnEnum(*EnumNode::As(node2)); break;
			case NodeType::UsingNode: OnUseingNode(*UsingNode::As(node2)); break;
			case NodeType::DeclareVariableNode:OnDeclareVariablenode(*DeclareVariableNode::As(node2)); break;
			default:break;
			}
		}
		_InStatements = false;
	}

	if (buidCode)
	{
		_Builder.Build_Ret();
	}

	_Table.RemovePopUseing(UseingIndex);

	_Table.RemoveScope();
}
void SystematicAnalysis::OnEnum(const EnumNode& node)
{
}

String SystematicAnalysis::GetScopedNameAsString(const ScopedNameNode& node)
{
	String Text;
	for (const auto& Item : node.ScopedName)
	{
		Text += Item->Value._String;
	}
	return Text;
}
void SystematicAnalysis::OnDeclareVariablenode(const DeclareVariableNode& node)
{
	auto& StrVarName = node.Name.AsString();
	auto FullName = _Table._Scope.ThisScope + ScopeHelper::_ScopeSep + StrVarName;

	SymbolID sybId = (SymbolID)&node;
	Symbol* syb;


	if (passtype == PassType::GetTypes)
	{
		syb = &_Table.AddSybol(SymbolType::StackVarable, StrVarName, FullName);
		_Table.AddSymbolID(*syb, sybId);

		if (_ClassStack.size() && _InStatements == false)
		{
			ClassField V;
			auto& Class = *_ClassStack.top();
			V.FullNameType = node.Type.AsString();

			V.Name = node.Name.AsString();

			Class._Class.Fields.push_back(V);
		}
	}
	else
	{
		syb = &_Table.GetSymbol(sybId);
	}

	if (node.Expression.Value)
	{
		OnExpressionTypeNode(node.Expression.Value);
	}

	if (passtype == PassType::BuidCode)
	{
		auto Op = IROperand::AsLocation(_LastExpressionField);
		auto NewOp = IROperand::AsVarable(sybId);
		_Builder.Build_Assign(NewOp, Op);	
	}
}
void SystematicAnalysis::OnExpressionTypeNode(const Node* node)
{
	switch (node->Get_Type())
	{
	case NodeType::BinaryExpressionNode:OnExpressionNode(*BinaryExpressionNode::As(node));break;
	case NodeType::ValueExpressionNode:OnExpressionNode(*ValueExpressionNode::As(node)); break;
	default:
		break;
	}
	

}
void SystematicAnalysis::OnExpressionNode(const ValueExpressionNode& node)
{
	
	if (passtype == PassType::BuidCode)
	{
		
		switch (node.Value->Get_Type())
		{
		case NodeType::NumberliteralNode:
		{
			NumberliteralNode* num = NumberliteralNode::As(node.Value);
			auto& Str = num->Token->Value._String;
			UInt8 V;
			ParseHelper::ParseStringToUInt8(Str, V);


			_Builder.Build_Assign(IROperand::AsInt8(V));
			_LastExpressionField = _Builder.GetLastField();
		}
		break;
		case NodeType::ReadVariableNode:
		{
			ReadVariableNode* nod = ReadVariableNode::As(node.Value);
			auto Str = GetScopedNameAsString(nod->VariableName);

			auto& Symbols = _Table.GetSymbolsWithName(Str);
			auto Symbol = Symbols[0];
			SymbolID sybId = Symbol->ID;
				
			_Builder.Build_Assign(IROperand::AsReadVarable(sybId));
			_LastExpressionField = _Builder.GetLastField();
		}
		break;
		default:
			break;
		}
	}
}
void SystematicAnalysis::OnExpressionNode(const BinaryExpressionNode& node)
{
	OnExpressionTypeNode(node.Value1.Value);
	auto Ex0 = _LastExpressionField;
	OnExpressionTypeNode(node.Value0.Value);
	auto Ex1 = _LastExpressionField;

	if (passtype == PassType::BuidCode)
	{
		auto Op0 = IROperand::AsLocation(Ex0);
			auto Op1 = IROperand::AsLocation(Ex1);

			switch (node.BinaryOp->Type)
			{
			case TokenType::plus:_Builder.MakeAdd8(Op0, Op1);
				break;
			case TokenType::minus:_Builder.MakeSub8(Op0, Op1);
				break;
			default:
				break;
			}
		
		
		_LastExpressionField = _Builder.GetLastField();
	}
}
void SystematicAnalysis::CheckBackEnd()
{
	if (BackEnd == nullptr)
	{
		_ErrorsOutput->AddError(ErrorCodes::BackEndError, 0, 0,"There is not backend to compile to");

		return;
	}
}
void SystematicAnalysis::PushTepAttributesInTo(Vector<AttributeData>& Input)
{
	for (auto& Item : _TepAttributes)
	{
		AttributeData Data;
		Item->ScopedName.GetScopedName(Data.Name);
		Input.push_back(Data);
	}
	_TepAttributes.clear();
}
void SystematicAnalysis::LoadLibSymbols()
{
	for (auto Item : *_Libs)
	{
		LoadLibSymbols(*Item);
	}
}
void SystematicAnalysis::LoadLibSymbols(const UClib& lib)
{
	auto OutputType = OutputTypeAsLibType();
	auto libType = lib._LibType;
	
	LibType ImportType;
	if (OutputType == LibType::Lib)
	{
		ImportType = libType;
	}
	else
	{
		ImportType = LibType::Dll;
	}

	for (auto Item : lib.Get_NameToPtr())
	{
		const auto Offset = Item.second;
		const auto& FuncStr = Item.first;
		

		auto SymbolType = ImportType == LibType::Dll ? SymbolType::ImportedDllFunc : SymbolType::ImportedLibFunc;
		
		auto& Syb = _Table.AddSybol(SymbolType,FuncStr, ScopeHelper::GetNameFromFullName(FuncStr));
		
		
	}

	for (auto Item : lib.Get_Assembly().Classes)
	{
		switch (Item->Type)
		{
		case ClassType::Class:
		{

		}
			break;
		case ClassType::Alias:
			break;
		case ClassType::Enum:
			break;
		default:
			break;
		}
	}
}
UCodeLangEnd


