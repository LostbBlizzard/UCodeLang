#include "TreeNode.hpp"

#include "UCodeLang/Compilation/Front/Lexer.hpp"
#include "UCodeLang/Compilation/Front/Parser.hpp"
UCodeLangStart

LibImportNode::LibImportNode(LibImportNode&& source) : Node(NodeType::LibImportNode), LIb(std::move(source.LIb))
{
	/*
	FrontEnd::Lexer _Lexer;
	FrontEnd::Parser _Parser;
	for (auto& Item : LIb.Get_Assembly().Classes)
	{
		String_view Text;
		_Lexer.Lex(Text);

		_Parser.Parse(Text, _Lexer.Get_Tokens());
	
		auto& list = _Parser.Get_Tree()._Nodes;
		for (auto& Item2 : list)
		{
			_Nodes.push_back(std::move(Item2));
		
		}
	}
	*/
}
UCodeLangEnd
