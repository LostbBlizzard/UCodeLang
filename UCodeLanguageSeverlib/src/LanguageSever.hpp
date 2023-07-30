#pragma once

#include <mutex>
#include "LanguageSeverNameSpace.h"
#include "JSONstructures.hpp"
#include "JSONstructureSerialization.hpp"
LanguageSeverStart
struct SeverPacket
{
    String _Data;

    String ToLanguageServerString() const
    {
        String r;
        r += "Content-Length: ";
        r += std::to_string(_Data.size());
        r += "\r\n\r\n";
        r += _Data;

        return r;
    }
    struct StreamState
    {
        bool ReadingPacketSize = false;
        String Buffer;
        String NumberBuffer;
        size_t PacketSize = 0;
    };

    

    static Optional<SeverPacket> Stream(StreamState& State, char Char);
    static Optional<SeverPacket> Parse(const StringView str)
    {
        StreamState state;
        for (size_t i = 0; i < str.size(); i++)
        {
            auto V = Stream(state, str[i]);
            if (V.has_value())
            {
                return V.value();
            }
        }
        return {};
    }
};


struct ClientPacket
{
    String _Data;

    String ToLanguageServerString() const
    {
        String r;
        r += "Content-Length: ";
        r += std::to_string(_Data.size());
        r += "\r\n\r\n";
        r += _Data;

        return r;
    }
    using StreamState = SeverPacket::StreamState;
    static Optional<ClientPacket> Stream(StreamState& State,char Char)
    {
        auto Val = SeverPacket::Stream(State,Char);
        if (Val.has_value())
        {
            ClientPacket r;
            r._Data = std::move(Val.value()._Data);

            return r;
        }
        return {};
    }
    static Optional<ClientPacket> Parse(const StringView str)
    {
        auto Val = ClientPacket::Parse(str);
        if (Val.has_value())
        {
            ClientPacket r;
            r._Data = std::move(Val.value()._Data);

            return r;
        }
        return {};
    }
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
    void StopRuning()
    {
        Runing = false;
    }
  
private:
    bool Runing = true;
    std::mutex _ClientInputLock;
    Vector<ClientPacket> _ClientPackets; 
    
    std::mutex _ClientOutputLock;
    Vector<SeverPacket> _SeverPackets;
    //
    Vector<ClientPacket> _ClientPacketsToRun;
    void OnReceivedPacket(const ClientPacket& params);


    // ResponseMessage
    template<typename T>
    void SendResponseMessageToClient(const T& Object)
    {
        json Json;
        {
            Json["jsonrpc"] = "2.0";
            ns::to_json(Json["result"],Object);
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
            ns::to_json(Json["error"],Error);
        }


        SeverPacket packet;
        packet._Data = Json.dump();
        SendPacketToClient(std::move(packet));
    }

    template<typename T>
    void SendMethodToClient(const String& method, const T& params)
    {
        json Json;
        {
            Json["jsonrpc"] = "2.0";
            Json["id"] = Test++;
            Json["method"] = method;
            ns::to_json(Json["params"], params);
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

    //
    void window_logMessage(MessageType Type, String MSg);
    size_t Test = 1;
};

LanguageSeverEnd