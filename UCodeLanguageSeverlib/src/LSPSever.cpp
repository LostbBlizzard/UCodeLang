#include "LSPSever.hpp"
#include <unordered_map>
#include <functional>
UCodeLanguageSeverStart

#define InitializeCheck() if (IsInitialized== false){ResponseError e;\
	e.code = (integer)ErrorCodes::ServerNotInitialized;\
	SendResponseErrorToClient(requestid, e); return;}\

#define ShutdownCheck() if (IsShutdown == true) { \
	ResponseError e; \
	e.code = (integer)ErrorCodes::InvalidRequest; \
	SendResponseErrorToClient(requestid, e); return;}\


namespace UA = UCodeAnalyzer;

const char NameCharList[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890,.':/\\";
bool IsInNameCharList(char Value)
{
	for (size_t i = 0; i < sizeof(NameCharList); i++)
	{
		if (Value == NameCharList[i])
		{
			return true;
		}
	}
	return false;
}
Optional<SeverPacket> SeverPacket::Stream(StreamState& State, char Char)
{
	State.Buffer += Char;


	if (State.PacketSize == 0)
	{
		if (State.ReadingPacketSize == false)
		{
			if (State.Buffer == "Content-Length:")
			{
				State.Buffer.clear();
				State.ReadingPacketSize = true;
			}
		}
		else
		{
			bool IsNum = IsInNameCharList(Char);
			if (State.NumberBuffer.size())
			{
				if (!IsNum)
				{
					State.PacketSize = std::stoi(State.NumberBuffer) - 3;//the \n,\r,\n,\r. and this char
					State.Buffer.clear();
					State.NumberBuffer.clear();

					State.Buffer += Char;
				}
				else
				{
					State.NumberBuffer += Char;
				}
			}
			else
			{
				if (IsNum)
				{
					State.NumberBuffer += Char;
				}
			}

		}

	}
	else
	{
		State.PacketSize--;
		if (State.PacketSize == 0)
		{

			UCodeLanguageSever::SeverPacket p;
			p._Data = std::move(State.Buffer);

			State = StreamState();
			return p;
		}

	}
	return {};
}

Optional<SeverPacket::ParsedPacket> SeverPacket::Parse() const
{
	ParsedPacket r;
	json Values = json::parse(_Data);

	if (!Values.contains("jsonrpc")) { return {}; }
	r.jsonrpc = Values["jsonrpc"].get<String>();
	if (r.jsonrpc != "2.0") { return{}; }
	if (Values.contains("id") && (Values.contains("result") || Values.contains("error")))
	{
		ResponseMessage_t v;
		v.id = Values.at("id").get<integer>();

		if (Values.contains("result"))
		{
			v.result = std::move(Values["result"]);
		}
		else
		{
			UCodeLanguageSever::from_json(Values["error"], v.result);
		}

		r.Type = std::move(v);
		return { r };
	}
	else if (Values.contains("id") && Values.contains("method"))
	{
		RequestMessage_t v;
		v.id = Values.at("id").get<integer>();

		v.method = Values.at("method").get<String>();


		v.params = std::move(Values["params"]);


		r.Type = std::move(v);
		return { r };
	}
	else if (Values.contains("method"))
	{
		NotificationMessage_t v;
		v.method = Values.at("method").get<String>();


		v.params = std::move(Values["params"]);


		r.Type = std::move(v);
		return { r };
	}
	return {};
}

struct LanguageSeverFuncMap
{

	using RequestFunc = void(LSPSever::*)(integer  requestid, const json& Params);
	using NotificationFunc = void(LSPSever::*)(const json& Params);
	inline static const std::unordered_map<String, RequestFunc> RequestFuncs
	{
		{"initialize",&LSPSever::Sever_initialize},
		{"shutdown",&LSPSever::Sever_Shutdown},

		{"textDocument/definition",&LSPSever::textDocument_definition},
		{"textDocument/hover",&LSPSever::textDocument_hover},
		{"textDocument/rename",&LSPSever::textDocument_rename},
	};
	inline static const std::unordered_map<String, NotificationFunc> NotificationFuncs
	{
		{"exit",&LSPSever::Sever_Exit},
		{"textDocument/didOpen",&LSPSever::textDocument_didOpen},
		{"textDocument/didClose",&LSPSever::textDocument_didClose},
		{"textDocument/didChange",&LSPSever::textDocument_didChange}
	};
};


LSPSever::LSPSever()
{
	
}
LSPSever::~LSPSever()
{

}
bool LSPSever::Step()
{
	//Io

	if (_ClientInputLock.try_lock())
	{
		Vector<ClientPacket> Tep = std::move(_ClientPackets);
		_ClientInputLock.unlock();

		for (auto& Item : Tep)
		{
			_ClientPacketsToRun.push_back(std::move(Item));
		}
	}

	{
		for (auto& Item : _ClientPacketsToRun)
		{
			OnReceivedPacket(Item);
		}
		_ClientPacketsToRun.clear();
	}

	{
		if (IsInitialized && IsShutdown ==false)
		{
			BaseSever.Step();
		}
	}

	{
		UpdateClientErrorList();
	}
    return Runing;
}
void LSPSever::OnReceivedPacket(const ClientPacket& Item)
{
	auto dataOp = Item.Parse();

	if (dataOp.has_value())
	{
		auto& data = dataOp.value();

		if (auto Val = data.Type.Get_If<ClientPacket::RequestMessage_t>())
		{
			if (LanguageSeverFuncMap::RequestFuncs.count(Val->method))
			{
				auto FuncPtr = LanguageSeverFuncMap::RequestFuncs.at(Val->method);
				(*this.*FuncPtr)(Val->id, Val->params);
			}
			else
			{
				ResponseError err;
				err.code = (integer)ErrorCodes::MethodNotFound;
				SendResponseErrorToClient(Val->id,err);
			}
		}
		else  if (auto Val = data.Type.Get_If<ClientPacket::NotificationMessage_t>())
		{
			if (LanguageSeverFuncMap::NotificationFuncs.count(Val->method))
			{
				auto FuncPtr = LanguageSeverFuncMap::NotificationFuncs.at(Val->method);
				(*this.*FuncPtr)(Val->params);
			}
		}
		else if (auto Val = data.Type.Get_If<ClientPacket::ResponseMessage_t>())
		{

		}


		
	}

}

void LSPSever::Sever_Shutdown(integer requestid, const json& params)
{
	IsShutdown = true;
	 

	SendResponseMessageToClient(requestid, TsNull());


	BaseSever.deinit();
}
void LSPSever::Sever_Exit(const json& params)
{
	StopRuning();
	ProcessExitCode = 0;

}
void LSPSever::textDocument_didOpen(const json& Params)
{
	DidOpenTextDocumentParams params;
	UCodeLanguageSever::from_json(Params,params);


	if (params.textDocument.languageId == UCodeLangLanguageId) {
		UA::UCFile newfile;
		newfile._Fileidentifier = CastToFileId(params.textDocument.uri);
		newfile.FileName = params.textDocument.uri;
		newfile.filetext = params.textDocument.text;
		newfile.oldfile = newfile.filetext;

		BaseSever.AddFile(std::move(newfile));
	}
}
void LSPSever::textDocument_didClose(const json& Params)
{
	DidCloseTextDocumentParams params;
	UCodeLanguageSever::from_json(Params, params);

	BaseSever.RemoveFile(CastToFileId(params.textDocument.uri));
}
void LSPSever::textDocument_didChange(const json& Params)
{
	DidChangeTextDocumentParams params;
	UCodeLanguageSever::from_json(Params, params);

	auto& Ufile = BaseSever.GetFile(CastToFileId(params.textDocument.uri));
	

	for (auto& Item : params.contentChanges)
	{
		if (auto val = Item.Get_If< TextDocumentContentChangeEventFilePart>())
		{

		}
		else if (auto val = Item.Get_If< TextDocumentContentChangeEventFullFile>())
		{
			Ufile.filetext = val->text;
		}
	}	
	Ufile.UpdatedFileText();
}
void LSPSever::textDocument_definition(integer  requestid,const json& Params)
{
	InitializeCheck(); ShutdownCheck();
}
void LSPSever::textDocument_hover(integer  requestid, const json& params)
{

	InitializeCheck(); ShutdownCheck();
}
void LSPSever::textDocument_rename(integer  requestid, const json& params)
{

	InitializeCheck(); ShutdownCheck();
	
}

//

void LSPSever::window_logMessage(MessageType Type, String MSg)
{
	LogMessageParams V;
	V.type = Type;
	V.message = MSg;

	SendMethodToClient("window/logMessage", V);
}
Position LSPSever::GetPosition(StringView text, size_t CharIndex, size_t Line)
{
	Position r;


	size_t lineoffset = 0;
	size_t linecount = 0;
	for (size_t i = 0; i < text.size(); i++)
	{
		if (text[i] == '\n')
		{
			linecount++;
		}

		if (Line == linecount)
		{
			lineoffset++;
		}

		if (i == CharIndex)
		{
			break;
		}
	}
	r.character = lineoffset;
	r.line = linecount;

	return r;
}
UCodeLanguageSever::DocumentUri LSPSever::CastToUri(const UCodeAnalyzer::Fileidentifier& Item)
{
	return UCodeAnalyzer::Path(Item).generic_string();
}
UCodeAnalyzer::Fileidentifier LSPSever::CastToFileId(const UCodeLanguageSever::DocumentUri& Item)
{
	return UCodeAnalyzer::Path(Item).native();
}
void LSPSever::UpdateClientErrorList()
{
	bool IsSame = _ClientSideErrorsList.size() == BaseSever.ErrorList.size();

	if (IsSame)
	{
		for (size_t i = 0; i < BaseSever.ErrorList.size(); i++)
		{
			const auto& ErrorItem = BaseSever.ErrorList[i];
			const auto& ClientErrorItem = _ClientSideErrorsList[i];

			if (ErrorItem != ClientErrorItem)
			{
				IsSame = false;
				break;
			}
		}
	}

	if (!IsSame)
	{
		_ClientSideErrorsList = BaseSever.ErrorList;

		std::sort(_ClientSideErrorsList.begin(), _ClientSideErrorsList.end(), [](const UCodeLang::CompilationErrors::Error& A
			, const UCodeLang::CompilationErrors::Error& B)
			{
				return A.File.native().size() > B.File.native().size();
			});

		Optional<URI> LastSet;
		PublishDiagnosticsParams p;

		if (_ClientSideErrorsList.size() == 0)
		{
			auto& files = BaseSever.GetFiles();
			for (auto& file : files)
			{
				p.uri = CastToUri(file.first);
				Send_PublishDiagnostics_ToClient(std::move(p));
			}
		}


		for (size_t i = 0; i < _ClientSideErrorsList.size(); i++)
		{
			auto& Item = _ClientSideErrorsList[i];

			URI ItemUrl = CastToUri(Item.File);

			UCodeAnalyzer::UCFile& f = BaseSever.GetFile(Item.File);

			LastSet = ItemUrl;
			Diagnostic V;
			V.source = "UCodeLang";
			V.range.start = GetPosition(f.filetext, Item.Pos, Item.Line);
			V.range.end = V.range.start;
			V.message = Item._Msg;

			if (UCodeLang::CompilationErrors::IsError(Item._Code)) {
				V.severity = DiagnosticSeverity::Error;
			}
			else if (UCodeLang::CompilationErrors::IsWarning(Item._Code)) {
				V.severity = DiagnosticSeverity::Warning;
			}
			else if (UCodeLang::CompilationErrors::IsInfo(Item._Code)) {
				V.severity = DiagnosticSeverity::Information;
			}
			else if (UCodeLang::CompilationErrors::IsHint(Item._Code)) {
				V.severity = DiagnosticSeverity::Hint;
			}

			p.diagnostics.push_back(std::move(V));

			UCodeLang::CompilationErrors::Error* NextErr = nullptr;
			bool IsLast = &Item == &_ClientSideErrorsList.back();

			if (!IsLast)
			{
				NextErr = &_ClientSideErrorsList[i + 1];
			}

			if (IsLast ||
				(NextErr && ItemUrl != CastToUri(NextErr->File)))
			{

				p.uri = LastSet.value();
				Send_PublishDiagnostics_ToClient(std::move(p));

				p = PublishDiagnosticsParams();

			}
		}

	}
}

void LSPSever::Sever_initialize(integer requestid, const json& Params)
{
	InitializeParams params;
	UCodeLanguageSever::from_json(Params, params);
	
	InitializeResult V;
	V.capabilities.positionEncoding = PositionEncodingkind::PositionEncodingKind8;
	V.capabilities.hoverProvider = true;
	V.capabilities.textDocumentSync = TextDocumentSyncKind::Incremental;

	
	
	window_logMessage(MessageType::Log, "Hello World Sever Side");
	SendResponseMessageToClient(requestid,V);

	IsInitialized = true;

	BaseSever.init();
}
UCodeLanguageSeverEnd