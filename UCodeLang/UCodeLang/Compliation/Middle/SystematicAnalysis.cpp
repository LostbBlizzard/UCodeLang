#include "SystematicAnalysis.hpp"


UCodeLangStart

void SystematicAnalysis::Reset()
{
	_Lib.ClearState();
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

	_Files = &Files;
	_Libs = &Libs;
	TypePass();


	_Files = nullptr;
	_Libs = nullptr;
	return !_ErrorsOutput->Has_Errors();
}
void SystematicAnalysis::TypePass()
{
	for (const auto& Files : *_Files)
	{
		for (auto node : Files->_Nodes)
		{
			switch (node->Get_Type())
			{
			case NodeType::ClassNode: TypePass_ClassNode(*ClassNode::As(node)); break;
			default:break;
			}
		}
	}
	
}
void SystematicAnalysis::TypePass_ClassNode(const ClassNode& node)
{
}
void SystematicAnalysis::CheckBackEnd()
{
	if (BackEnd == nullptr)
	{
		_ErrorsOutput->AddError(ErrorCodes::BackEndError, 0, 0,"There is not backend to compile to");

		return;
	}
}
UCodeLangEnd


