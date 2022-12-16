#include "SemanticAnalysis.hpp"
#include "../Helpers/KeyWords.hpp"
UCodeLangStart

SemanticAnalysis::SemanticAnalysis():_Success(false), _ErrorsOutput(nullptr), _Settings(nullptr)
{
}
SemanticAnalysis::~SemanticAnalysis()
{

}
void SemanticAnalysis::DoAnalysis(FileNode& Tree)
{
	Value.Tree = &Tree;
}
UCodeLangEnd