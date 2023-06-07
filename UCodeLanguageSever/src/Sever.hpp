#pragma once
#include "LanguageSeverNameSpace.h"
#include "UCodeAnalyzer/Typedef.hpp"
#include "jsonrpccxx/server.hpp"
#include <thread>
#include "httplib.h"
#include <jsonrpccxx/iclientconnector.hpp>
#include <jsonrpccxx/server.hpp>
#include <string>



class CppHttpLibServerConnector {
public:
    explicit CppHttpLibServerConnector(jsonrpccxx::JsonRpcServer& server) :
        thread(),
        server(server),
        httpServer(),
        port(0) {
        httpServer.Post("/jsonrpc",
            [this](const httplib::Request& req, httplib::Response& res) {
                this->PostAction(req, res);
            });
    }

    virtual ~CppHttpLibServerConnector() { StopListening(); }

    bool StartListening(const std::string& Ip = "localhost", int port = 80) {
        if (httpServer.is_running())
            return false;
        this->thread = std::thread([this,Ip,port]() { this->httpServer.listen(Ip.c_str(), port); });
        return true;
    }

    void StopListening() {
        if (httpServer.is_running()) {
            httpServer.stop();
            this->thread.join();
        }
    }

private:
    std::thread thread;
    jsonrpccxx::JsonRpcServer& server;
    httplib::Server httpServer;
    int port;

    void PostAction(const httplib::Request& req,
        httplib::Response& res) {
        res.status = 200;
        res.set_content(this->server.HandleRequest(req.body), "application/json");
    }
};

LanguageSeverStart
using namespace UCodeAnalyzer;
using json = nlohmann::json;
class Sever 
{
public:
    Sever();
    ~Sever();


    //Is Runing Bool
    bool Step();

	void RunOnSever(const String& IP, int Port);
private:
	jsonrpccxx::JsonRpc2Server rpcServer;
    CppHttpLibServerConnector httpServer;

    void textDocument_definition(const json& Value);
};

LanguageSeverEnd