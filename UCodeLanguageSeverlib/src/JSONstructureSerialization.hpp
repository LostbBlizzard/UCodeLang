#pragma once
#include "JSONstructures.hpp"

namespace ns 
{
	using json = nlohmann::json;
	#define UCL UCodeLanguageSever


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
		if (Object.has_value())
		{
			Json = Object.value();
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
}