#include "IntermediateAssembler.hpp"
#include "../NodeTypes/TreeNodeTypes.hpp"
UCodeLangStart

inline void GenInst(Intermediate_Set Inst, const UInt64 Value0, const UInt64 Value1, Instruction& Out)
{
	Out = Instruction();
	return InstructionBuilder::GenInst((InstructionSet_t)Inst, Value0, Value1, Out);
};
inline void GenInst(Intermediate_Set Inst, const void* Value0, const void* Value1, Instruction& Out)
{
	Out = Instruction();
	return InstructionBuilder::GenInst((InstructionSet_t)Inst, (UInt64)Value0, (UInt64)Value1, Out);
};
inline void GenInst(Intermediate_Set Inst, const void* Value0, Instruction& Out)
{
	Out = Instruction();
	return InstructionBuilder::GenInst((InstructionSet_t)Inst, (UInt64)Value0, (UInt64)NullAddress, Out);
};
inline void GenInst(Intermediate_Set Inst, size_t Value0, Instruction& Out)
{
	Out = Instruction();
	return InstructionBuilder::GenInst((InstructionSet_t)Inst, (UInt64)Value0, (UInt64)NullAddress, Out);
};
inline void GenInst(Intermediate_Set Inst,RegisterID Register, size_t Value0, Instruction& Out)
{
	Out = Instruction();
	return InstructionBuilder::GenInst((InstructionSet_t)Inst, (UInt64)Register, (UInt64)Value0, Out);
};
inline void GenInst(Intermediate_Set Inst,Instruction& Out)
{
	Out = Instruction();
	return InstructionBuilder::GenInst((InstructionSet_t)Inst, (UInt64)NullAddress, (UInt64)NullAddress, Out);
};
void IntermediateAssembler::Assemble(SemanticAnalysisRet& Data)
{
	_OutPut.ClearState();
	_StaticVariables.clear();
	ClassAssembly::Move(Data.AssemblyData,_OutPut.Get_Assembly());

	_Data = &Data;
	ThisScope.ThisScope = ScopeHelper::_globalScope;
	_Ins = UCodeLang::Instruction();

	BuildNameSpace(_Data->Tree->As());


	if (_StaticVariables.size() != 0)
	{
		
	}
}
#define GenIns(X) ReSetIns(); X

void IntermediateAssembler::BuildNameSpace(const Node* Tree)
{
	bool IsNameSpace = Tree->_Type == NodeType::NamespaceNode;

	const Vector<Node*>& Nodes = (Tree->_Type == NodeType::NamespaceNode) 
		? NamespaceNode::As(Tree)->_Nodes : FileNode::As(Tree)->_Nodes;

	if (IsNameSpace) 
	{
		auto NameSpace = NamespaceNode::As(Tree);
		NameSpace->NamespaceName.GetScopedName(TepString);

		ThisScope.AddScope(TepString);
	}

	for (const auto& Item : Nodes)
	{
		
		switch (Item->_Type)
		{
		case NodeType::NamespaceNode:
			BuildNameSpace(Item);
			break;
		case NodeType::NameNode:
			BuildClass(*ClassNode::As(Item));
			break;
		case NodeType::UsingNode:
			
			break;
		default:
			#if CompliationTypeSafety
			throw std::exception("Cant UnWap BuildStatement");
			#endif
			break;
		}

	}

	if (IsNameSpace) {
		ThisScope.ReMoveScope();
	}
}
void IntermediateAssembler::BuildStaticVariable(const UCodeLang::Node* node)
{
	_StaticVariables.push_back(node);
}

void IntermediateAssembler::BuildClass(const UCodeLang::ClassNode& node)
{
	
}

void IntermediateAssembler::BuildFunc(const UCodeLang::FuncNode& Node)
{
	
}

void IntermediateAssembler::BuildStatements(const UCodeLang::StatementsNode& BodyStatements)
{
	
}

void IntermediateAssembler::BuildStatement(const UCodeLang::Node* Statement)
{
	
}
void IntermediateAssembler::BuildExpressionType(const UCodeLang::Node* Statement)
{
	
}
void IntermediateAssembler::BuildExpressionValue(const UCodeLang::Node* Item)
{
	
}
void IntermediateAssembler::BuildUnaryExpression(const UCodeLang::Node* Item)
{
}
void IntermediateAssembler::BuildBinaryExpression(const UCodeLang::Node* Item)
{
	
}
void IntermediateAssembler::BuildReturnExpression()
{
	GenInst(Intermediate_Set::Ret, _Ins);
	_OutPut.Add_Instruction(_Ins);
}
void IntermediateAssembler::BuildStoreExpression(const String_view& VarName)
{
	auto _VarPos = AddDebug_String(VarName);
	GenInst(Intermediate_Set::StoreVar, _VarPos, _Ins);
	_OutPut.Add_Instruction(_Ins);
}
void IntermediateAssembler::BuildDeclareVariable(const String_view& VarName, const String_view& Type)
{
	auto _VarPos = AddDebug_String(VarName);
	auto _TypePos = AddDebug_String(Type);
	GenInst(Intermediate_Set::DeclareVar, _TypePos, _VarPos, _Ins);
	_OutPut.Add_Instruction(_Ins);
}


UCodeLangEnd

