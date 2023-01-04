#include "SystematicAnalysis.hpp"
#include "UCodeLang/Compliation/Helpers/KeyWords.hpp"

#include "UCodeLang/Compliation/Helpers/InstructionBuilder.hpp"
UCodeLangStart

void SystematicAnalysis::Reset()
{
	_Lib.ClearState();
	passtype = PassType::Null;
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


	BackEndObject = BackEnd->GenNewBackEnd();
	BackEnd->SetBackEndAnalysis(BackEndObject, this);

	passtype = PassType::FixedTypes;
	Pass();

	_Files = nullptr;
	_Libs = nullptr;

	BackEnd->DeleteBackEnd(BackEndObject);
	BackEndObject = nullptr;

	return !_ErrorsOutput->Has_Errors();
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
	auto UseingIndex = _Sc.GetUseingIndex();

	const auto Namespace = GetScopedNameAsString(node.NamespaceName);
	_Sc.AddScope(Namespace);
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
	_Sc.RemoveScope();

	_Sc.RemovePopUseing(UseingIndex);
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
	_Sc.ClearUseings();
}
void SystematicAnalysis::OnClassNode(const ClassNode& Node)
{
	const auto& ClassName = Node.ClassName.Token->Value._String;


	_Sc.AddScope(ClassName);

	
	auto& Class = passtype == PassType::GetTypes ?
		_Lib.Get_Assembly().AddClass(String(ClassName), _Sc._Scope.ThisScope)
		: *_Lib.Get_Assembly().Find_Class(_Sc._Scope.ThisScope);
	_ClassStack.push(&Class);

	if (passtype == PassType::GetTypes)
	{
		PushTepAttributesInTo(Class._Class.Attributes);
	}

	auto UseingIndex = _Sc.GetUseingIndex();

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

	_Sc.RemovePopUseing(UseingIndex);


	_ClassStack.pop();
	_Sc.RemoveScope();
}
void SystematicAnalysis::OnUseingNode(const UsingNode& node)
{
	auto T = node.ScopedName.ScopedName[0];
	OnNonAttributeable(T->OnLine, T->OnPos);
	const auto UseingString =GetScopedNameAsString(node.ScopedName);
	_Sc.AddUseing(UseingString);

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

	_Sc.AddScope(FuncName);

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

	
	

	auto UseingIndex = _Sc.GetUseingIndex();

	if (passtype == PassType::GetTypes)
	{
		ClassMethod V;
		V.FullName = _Sc._Scope.ThisScope;		
		PushTepAttributesInTo(V.Attributes);
		
		
		Ptr->_Class.Methods.push_back(V);

		//Testing
		if (node.Body.has_value())
		{
			auto& Body = node.Body.value();
			BackEnd->BuildStatements(BackEndObject, Body.Statements._Nodes);
		}
		else
		{
			Instruction _ins;
			if (_Lib.Get_Instructions().size() == 0)
			{
				InstructionBuilder::Exit(ExitState::Failure, _ins);
				_Lib.Add_Instruction(_ins);
			}

			InstructionBuilder::Return(ExitState::Success, _ins);
			auto pos = _Lib.Add_Instruction(_ins);

			_Lib.Add_NameToLastInstruction(V.FullName);
		}
	}

	if (node.Body.has_value()) 
	{
		auto& Body = node.Body.value();
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
	}

	_Sc.RemovePopUseing(UseingIndex);

	_Sc.RemoveScope();
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
	auto& Class = *_ClassStack.top();

	if (passtype == PassType::GetTypes) {
		ClassField V;

		V.FullNameType = node.Type.AsString();

		V.Name = node.Name.AsString();

		Class._Class.Fields.push_back(V);
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
UCodeLangEnd


