#pragma once
#include "JSONstructures.hpp"

UCodeLanguageSeverStart

using json = nlohmann::json;
#define UCL UCodeLanguageSever

	

	template<typename T>
	inline void to_json(json& Json,const T Object)
	{
		Json = json(Object);
	}
	template<typename T>
	inline void from_json(const json& Json, T& Object)
	{
		Object = Json.get<T>();
	}



	template<typename T>
	inline void to_jsonOp(const UCL::TsOptional<T>& value,const char* FieldName, json& Json)
	{
		if (value.has_value())
		{
			to_json(Json[FieldName], value.value());
		}
	}
	
	template<typename... T>
	inline void to_jsonPredi(const UCL::TypePredicates<T...>& value, const char* FieldName, json& Jsontoadd)
	{
		
	}

	template<typename T>
	inline void from_json(const json& Json, UCL::TsOptional<T>& Object)
	{

	}
	template<typename T>
	inline void from_json(const json& Json,const char* field, UCL::TsOptional<T>& Object)
	{
		if (Json.contains(field))
		{
			from_json(Json[field], Object);
		}
	}
	template<typename T>
	inline void to_json(json& Json, const UCL::TsOptional<T>& Object)
	{
		if (Object.has_value())
		{
			Json = Object.value();
		}
	}
	template<typename... T>
	inline void from_json(const json& Json, UCL::TypePredicates<T...>& Object)
	{

	}
	template<typename... T>
	inline void to_json(json& Json, const UCL::TypePredicates<T...>& Object)
	{
		
	}

	template<typename T>
	inline void from_json(const json& Json, UCL::TsArray<T>& Object)
	{
		size_t size = Json.end() - Json.begin();
		Object.resize(size);
		for (size_t i = 0; i < size; i++)
		{
			from_json(Json.at(i), Object[i]);
		}
	}
	template<typename T>
	inline void to_json(json& Json, const UCL::TsArray<T>& Object)
	{
		Json = json::array();

		for (auto& Item : Object) 
		{
			json tep;
			to_json(tep,Item);
			Json.push_back(std::move(tep));
		}
	}


	inline void from_json(const json& Json, UCL::TsNull& Object)
	{

	}
	inline void to_json(json& Json, const UCL::TsNull& Object)
	{
		Json = "null";
	}

	inline void from_json(const json& Json,UCL::ServerCapabilities& Object)
	{
		
	}
	inline void to_json(json& Json, const UCL::ServerCapabilities& Object)
	{
		to_jsonOp(Object.positionEncoding, "positionEncoding", Json);
		to_jsonOp(Object.hoverProvider, "hoverProvider", Json);
	}


	inline void from_json(const json& Json, UCL::ResponseError& Object)
	{

	}
	inline void to_json(json& Json, const UCL::ResponseError& Object)
	{
		Json["code"] = Object.code;

		Json["message"] = Object.message;


		to_jsonOp(Object.data, "data", Json);
	}
	inline void from_json(const json& Json, UCL::InitializeResult::Struct& Object)
	{
		
	}
	inline void to_json(json& Json, const UCL::InitializeResult::Struct& Object)
	{
		to_json(Json["name"], Object.name);

		to_jsonOp(Object.version, "version", Json);
	}

	inline void from_json(const json& Json, UCL::InitializeResult& Object)
	{
		
	}
	inline void to_json(json& Json, const UCL::InitializeResult& Object)
	{

		to_json(Json["capabilities"], Object.capabilities);


		to_jsonOp(Object.serverInfo, "serverInfo", Json);
	}

	inline void from_json(const json& Json, UCL::LogMessageParams& Object)
	{

	}
	inline void to_json(json& Json, const UCL::LogMessageParams& Object)
	{
		to_json(Json["type"], Object.type);
		Json["message"] = Object.message;
	}

	inline void from_json(const json& Json, UCL::MessageType& Object)
	{

	}
	inline void to_json(json& Json, const UCL::MessageType& Object)
	{
		Json = (UCL::integer)Object;
	}

	inline void from_json(const json& Json, UCL::InitializeParams& Object)
	{

	}
	inline void to_json(json& Json, const UCL::InitializeParams& Object)
	{
		to_jsonPredi(Object.processId, "processId",Json);
	}

	inline void from_json(const json& Json, UCL::TextDocumentItem& Object)
	{
		Object.uri = Json["uri"].get<UCL::DocumentUri>();

		Object.languageId =Json["languageId"].get<UCL::string>();

		Object.version = Json["version"].get<UCL::integer>();

		Object.text= Json["text"].get<UCL::string>();
	}
	inline void to_json(json& Json, const UCL::TextDocumentItem& Object)
	{
		Json["uri"] = Object.uri;
		
		Json["languageId"] = Object.languageId;

		Json["version"] = Object.version;

		Json["text"] = Object.text;
	}

	inline void from_json(const json& Json, UCL::DidOpenTextDocumentParams& Object)
	{
		from_json(Json["textDocument"], Object.textDocument);
	}
	inline void to_json(json& Json, const UCL::DidOpenTextDocumentParams& Object)
	{
		to_json(Json["textDocument"],Object.textDocument);
	}

	inline void from_json(const json& Json, UCL::TextDocumentIdentifier& Object)
	{
		from_json(Json["uri"], Object.uri);
	}
	inline void to_json(json& Json, const UCL::TextDocumentIdentifier& Object)
	{
		to_json(Json["uri"], Object.uri);
	}

	inline void from_json(const json& Json, UCL::DidCloseTextDocumentParams& Object)
	{
		from_json(Json["textDocument"], Object.textDocument);
	}
	inline void to_json(json& Json, const UCL::DidCloseTextDocumentParams& Object)
	{
		to_json(Json["textDocument"], Object.textDocument);
	}

	inline void from_json(const json& Json, UCL::Position& Object)
	{
		from_json(Json["line"], Object.line);
		from_json(Json["character"], Object.character);
	}
	inline void to_json(json& Json, const UCL::Position& Object)
	{
		to_json(Json["line"], Object.line);
		to_json(Json["character"], Object.character);
	}

	inline void from_json(const json& Json, UCL::Range& Object)
	{
		from_json(Json["start"], Object.start);
		from_json(Json["end"], Object.end);
	}
	inline void to_json(json& Json, const UCL::Range& Object)
	{
		to_json(Json["start"], Object.start);
		to_json(Json["end"], Object.end);
	}


	inline void from_json(const json& Json, UCL::TextDocumentContentChangeEventFullFile& Object)
	{
		from_json(Json["text"], Object.text);
	}
	inline void to_json(json& Json, const UCL::TextDocumentContentChangeEventFullFile& Object)
	{
		to_json(Json["text"], Object.text);
	}

	inline void from_json(const json& Json, UCL::TextDocumentContentChangeEventFilePart& Object)
	{
		from_json(Json["range"],Object.range);
		from_json(Json,"rangeLength",Object.rangeLength);
		from_json(Json["text"], Object.text);
	}
	inline void to_json(json& Json, const UCL::TextDocumentContentChangeEventFilePart& Object)
	{
		to_json(Json["range"], Object.range);
		to_jsonOp(Object.rangeLength,"rangeLength", Json);
		to_json(Json["text"], Object.text);
	}

	inline void from_json(const json& Json, UCL::TextDocumentContentChangeEvent& Object)
	{
		if (Json.contains("range") || Json.contains("rangeLength"))
		{
			UCL::TextDocumentContentChangeEventFilePart v;
			from_json(Json, v);
			Object = std::move(v);
		}
		else if (Json.contains("text"))
		{
			UCL::TextDocumentContentChangeEventFullFile v;
			from_json(Json, v);
			Object = std::move(v);
		}
	}
	inline void to_json(json& Json, const UCL::TextDocumentContentChangeEvent& Object)
	{
		if (auto val = Object.Get_If<UCL::TextDocumentContentChangeEventFilePart>())
		{
			to_json(Json, *val);
		}
		else if (auto val = Object.Get_If<UCL::TextDocumentContentChangeEventFullFile>())
		{
			to_json(Json, *val);
		}
	}

	inline void from_json(const json& Json, UCL::VersionedTextDocumentIdentifier& Object)
	{
		from_json(Json["uri"], Object.uri);
		from_json(Json["version"], Object.version);
	}
	inline void to_json(json& Json, const UCL::VersionedTextDocumentIdentifier& Object)
	{
		to_json(Json["uri"], Object.uri);
		to_json(Json["version"], Object.version);
	}

	inline void from_json(const json& Json, UCL::DidChangeTextDocumentParams& Object)
	{
		from_json(Json["textDocument"], Object.textDocument);

		from_json(Json["contentChanges"],Object.contentChanges);
	}
	inline void to_json(json& Json, const UCL::DidChangeTextDocumentParams& Object)
	{
		to_json(Json["textDocument"], Object.textDocument);

		to_json(Json["contentChanges"], Object.contentChanges);
	}

	inline void from_json(const json& Json, UCL::DiagnosticSeverity& Object)
	{
		
		auto V = Json.get<UCL::String>();
		if (V == "Error")
		{
			Object = UCL::DiagnosticSeverity::Error;
		}
		else if (V == "Warning")
		{
			Object = UCL::DiagnosticSeverity::Warning;
		}
		else if (V == "Information")
		{
			Object = UCL::DiagnosticSeverity::Information;
		}
		else if (V == "Hint")
		{
			Object = UCL::DiagnosticSeverity::Hint;
		}
	}
	inline void to_json(json& Json, const UCL::DiagnosticSeverity& Object)
	{
		switch (Object)
		{
		case UCL::DiagnosticSeverity::Error:
			Json = "Error";
			break;
		case UCL::DiagnosticSeverity::Warning:
			Json = "Warning";
			break;
		case UCL::DiagnosticSeverity::Information:
			Json = "Information";
			break;
		case UCL::DiagnosticSeverity::Hint:
			Json = "Hint";
			break;
		default:
			break;
		}
	}

	inline void from_json(const json& Json, UCL::Diagnostic& Object)
	{
		from_json(Json["range"], Object.range);

		//from_json(Json, "severity", Object.severity);

		from_json(Json,"code", Object.code);

		//from_json(Json, "codeDescription", Object.codeDescription);

		//from_json(Json, "source", Object.source);
		
		from_json(Json["message"], Object.message);

		//from_json(Json,"tags", Object.tags);

		//from_json(Json, "relatedInformation", Object.relatedInformation);

		//from_json(Json,"data", Object.data);
	}
	inline void to_json(json& Json, const UCL::Diagnostic& Object)
	{
		to_json(Json["range"], Object.range);

		//to_jsonOp(Object.severity,"severity", Json);

		to_jsonOp(Object.code, "code", Json);

		//to_jsonOp(Object.codeDescription, "codeDescription", Json);

		//to_jsonOp(Object.source, "source", Json);

		to_json(Json["message"],Object.message);

		//to_jsonOp(Object.tags, "tags", Json);

		//to_jsonOp(Object.relatedInformation, "relatedInformation", Json);

		//to_jsonOp(Object.data, "data", Json);
	}

	inline void from_json(const json& Json, UCL::PublishDiagnosticsParams &Object)
	{
		from_json(Json["uri"], Object.uri);

		from_json(Json, "version", Object.version);

		from_json(Json["diagnostics"], Object.diagnostics);
	}
	inline void to_json(json& Json, const UCL::PublishDiagnosticsParams& Object)
	{
		to_json(Json["uri"], Object.uri);

		to_jsonOp(Object.version, "version", Json);

		to_json(Json["diagnostics"], Object.diagnostics);
	}
UCodeLanguageSeverEnd