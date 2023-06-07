#include "Sever.hpp"
LanguageSeverStart

Sever::Sever() : httpServer(rpcServer)
{
	rpcServer.Add((String)"textDocument/definition", [this](const json& Value) {textDocument_definition(Value); });
}
Sever::~Sever()
{

}
bool Sever::Step()
{
    return true;
}
void Sever::RunOnSever(const String& IP, int Port)
{
	std::cout << "Starting UCodeLanguageServer on IP:" << IP << " Port:" << Port << "\n";
	httpServer.StartListening(IP, Port);

}
void Sever::textDocument_definition(const json& Value)
{
	std::cout << "Got Document_definition" << "\n";
}
LanguageSeverEnd