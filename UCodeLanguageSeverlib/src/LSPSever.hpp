#pragma once

#include <functional>
#include <mutex>
#include "LanguageSeverNameSpace.h"
#include "JSONstructures.hpp"
#include "JSONstructureSerialization.hpp"
#include "UCodeAnalyzer/LanguageServer.hpp"
#include "UCodeLang/LangCore/LangTypes.hpp"

UCodeLanguageSeverStart
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


    template<typename T>
    static String RequestMessageStr(integer id, const String& method, const T& params)
    {
        json v;
        v["jsonrpc"] = "2.0";
        v["id"] = id;
        v["method"] = method;
        UCodeLanguageSever::to_json(v["params"], params);
        return v.dump();
    }
    template<typename T>
    static String NotificationMessageStr(const String& method, const T& params)
    {
        json v;
        v["jsonrpc"] = "2.0";
        v["method"] = method;
        UCodeLanguageSever::to_json(v["params"], params);
        return v.dump();
    }
    template<typename T>
    static String ResponseMessageStr(integer requestid, const T& Object)
    {
        json Json;
        {
            Json["jsonrpc"] = "2.0";
            Json["id"] = requestid;
            UCodeLanguageSever::to_json(Json["result"], Object);
        }
        return Json.dump();
    }
    static String ResponseErrorStr(integer requestid, const ResponseError& Error)
    {
        json Json;
        {
            Json["jsonrpc"] = "2.0";
            Json["id"] = requestid;
            UCodeLanguageSever::to_json(Json["error"], Error);
        }
        return Json.dump();
    }
    //
    template<typename T>
    static SeverPacket RequestMessage(integer id, const String& method, const T& params)
    {
        SeverPacket r;
        r._Data = RequestMessageStr(id, method, params);
        return r;
    }

    template<typename T>
    static SeverPacket ResponseMessage(integer requestid, const T& Object)
    {
        SeverPacket r;
        r._Data = ResponseMessageStr(requestid, Object);
        return r;
    }

    template<typename T>
    static SeverPacket NotificationMessage(const String& method, const T& Object)
    {
        SeverPacket r;
        r._Data = NotificationMessageStr(method,Object);
        return r;
    }
  
    
    static SeverPacket ResponseError(integer requestid, const ResponseError& Error)
    {
        SeverPacket r;
        r._Data = ResponseErrorStr(requestid,Error);
        return r;
    }

    struct NotificationMessage_t
    {
        /**
      * The method to be invoked.
      */
        String method;

        /**
     * The method's params.
     */
        json params;
    };
    struct RequestMessage_t
    {
        /**
     * The request id.
     */
        integer id;
        /**
      * The method to be invoked.
      */
        String method;

        /**
     * The method's params.
     */
        json params;
    };
    struct ResponseMessage_t
    {/**
	 * The request id.
	 */
        integer id;

        /**
     * The result of a request. This member is REQUIRED on success.
     * This member MUST NOT exist if there was an error invoking the method.
     */
        Optional<json> result;

        /**
     * The error object in case a request fails.
     */
        Optional<UCodeLanguageSever::ResponseError> error;
    };

    struct ParsedPacket
    {
        String jsonrpc;
        Variant<NotificationMessage_t, RequestMessage_t, ResponseMessage_t> Type;
    };
    Optional<ParsedPacket> Parse() const;
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

    template<typename T>
    static String RequestMessageStr(integer id, const String& method, const T& params)
    {
        return SeverPacket::RequestMessageStr(id, method, params);
    }
    template<typename T>
    static String NotificationMessageStr(const String& method, const T& params)
    {
        return SeverPacket::NotificationMessageStr(method, params);
    }

    template<typename T>
    static ClientPacket RequestMessage(integer id,const String& method,const T& params)
    {
     
        ClientPacket r;
        r._Data = RequestMessageStr(id, method, params);
        return r;
    }

    template<typename T>
    static ClientPacket NotificationMessage(const String& method, const T& params)
    {
        ClientPacket r;
        r._Data = NotificationMessageStr(method, params);
        return r;
    }
    using NotificationMessage_t = SeverPacket::NotificationMessage_t;
    using RequestMessage_t = SeverPacket::RequestMessage_t;
    using ResponseMessage_t = SeverPacket::ResponseMessage_t;
    using ParsedPacket = SeverPacket::ParsedPacket;
    Optional<ParsedPacket> Parse() const
    {
        SeverPacket p;
        p._Data = _Data;
        return p.Parse();
    }
};



struct LanguageSeverFuncMap;
class LSPSever 
{
public:
    friend LanguageSeverFuncMap;
    LSPSever();
    ~LSPSever();


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
    size_t PacketCount() { return _SeverPackets.size(); }
   
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
    Optional<std::function<void(StringView Msg)>> LspDebugLogCallback;
    int ProcessExitCode = 2;
private:
    bool Runing = true;
    bool IsInitialized = false;
    bool IsShutdown = false;
    std::mutex _ClientInputLock;
    Vector<ClientPacket> _ClientPackets; 
    
    std::mutex _ClientOutputLock;
    Vector<SeverPacket> _SeverPackets;
    //
    Vector<ClientPacket> _ClientPacketsToRun;
    void OnReceivedPacket(const ClientPacket& params);

    void LspDebugLog(StringView Msg)
    {
        if (LspDebugLogCallback.has_value())
        {
            LspDebugLogCallback.value()(Msg);
        }
    }

    // ResponseMessage
    template<typename T>
    void SendResponseMessageToClient(integer requestid, const T& Object)
    {
        SendPacketToClient(SeverPacket::ResponseMessage(requestid,Object));
    }

    // ResponseMessage
    void SendResponseErrorToClient(integer requestid,const ResponseError& Error)
    {
        SendPacketToClient(SeverPacket::ResponseError(requestid, Error));
    }

    template<typename T>
    void SendMethodToClient(const String& method, const T& params)
    {
        SendPacketToClient(SeverPacket::RequestMessage(Test++,method,params));
    }

    template<typename T>
    void SendNotificationMessageToClient(const String& method, const T& params)
    {
        SendPacketToClient(SeverPacket::NotificationMessage(method, params));
    }

    //https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#initialize
    void Sever_initialize(integer  requestid, const json& params);

    //https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#shutdown
    void Sever_Shutdown(integer  requestid, const json& params);

    //https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#exit
    void Sever_Exit(const json& params);

    //https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#initialize
    void Sever_initialized(const json& params);
    
    void textDocument_didOpen(const json& params);
    void textDocument_didClose(const json& params);
    void textDocument_didChange(const json& params);

    void textDocument_definition(integer requestid, const json& params);
   
    void textDocument_hover(integer requestid, const json& params);

    void textDocument_rename(integer requestid, const json& params);

    
    //Send
    void Send_PublishDiagnostics_ToClient(const PublishDiagnosticsParams& params)
    {
        SendNotificationMessageToClient("textDocument/publishDiagnostics", params);
    }
    void window_logMessage(MessageType Type, String MSg);

    static Position GetPosition(StringView text, size_t CharIndex, size_t Line);
    integer Test = 1;

    UCodeAnalyzer::LanguageServer BaseSever;
    UCodeAnalyzer::Fileidentifier CastToFileId(const  UCodeLanguageSever::DocumentUri& Item);
    UCodeLanguageSever::DocumentUri CastToUri(const  UCodeAnalyzer::Fileidentifier& Item);

    UCodeLang::Vector<UCodeLang::CompilationErrors::Error> _ClientSideErrorsList;

    void UpdateClientErrorList();
};

UCodeLanguageSeverEnd
