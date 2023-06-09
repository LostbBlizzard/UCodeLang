#pragma once

#include <mutex>
#include "LanguageSeverNameSpace.h"
#include "JSONstructures.hpp"
#include "JSONstructureSerialization.hpp"
LanguageSeverStart
struct SeverPacket
{
    String _Data;
};


struct ClientPacket
{
    String _Data;
};



class LanguageSeverFuncMap;
class LanguageSever 
{
public:
    friend LanguageSeverFuncMap;
    LanguageSever();
    ~LanguageSever();


    //Is Runing Bool
    bool Step();



    //Thread Safe
    void AddPacket(ClientPacket&& Item)
    {
        _ClientInputLock.lock();
        _ClientPackets.push_back(Item);
        _ClientInputLock.unlock();
    }
    
    //Thread Safe
    Vector<SeverPacket> GetPackets()
    {
        if (_ClientOutputLock.try_lock())
        {
            Vector<SeverPacket> R = std::move(_SeverPackets);
            _ClientOutputLock.unlock();

            return R;
        }
           
        return {};
    }
   
    //Thread Safe
    void SendPacketToClient(SeverPacket&& Item)
    {
        _ClientOutputLock.lock();
        _SeverPackets.push_back(Item);
        _ClientOutputLock.unlock();
    }
  
private:
    std::mutex _ClientInputLock;
    Vector<ClientPacket> _ClientPackets; 
    
    std::mutex _ClientOutputLock;
    Vector<SeverPacket> _SeverPackets;
    //
    Vector<ClientPacket> _ClientPacketsToRun;
    void OnReceivedPacket(const ClientPacket& params);


    // ResponseMessage
    template<typename T>
    void SendResponseMessageToClient(integer requestid,const T& Object)
    {
        json Json;
        {
            Json["jsonrpc"] = "2.0";
            Json["id"] = requestid;
            Json["result"] = Object;
        }


        SeverPacket packet;
        packet._Data = Json.dump();
        SendPacketToClient(std::move(packet));
    }

    // ResponseMessage
    void SendResponseErrorToClient(integer requestid,const ResponseError& Error)
    {
        json Json;
        {
            Json["jsonrpc"] = "2.0";
            Json["id"] = requestid;
            Json["error"] = Error;
        }


        SeverPacket packet;
        packet._Data = Json.dump();
        SendPacketToClient(std::move(packet));
    }
    //
    void Sever_initialize(integer  requestid, const json& params);

    void textDocument_definition(integer requestid, const json& params);
   
    void textDocument_hover(integer requestid, const json& params);

    void textDocument_rename(integer requestid, const json& params);
};

LanguageSeverEnd