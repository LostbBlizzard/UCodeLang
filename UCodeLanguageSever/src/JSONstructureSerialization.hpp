#pragma once
#include "JSONstructures.hpp"

namespace ns 
{
	using json = nlohmann::json;
	#define UCL UCodeLanguageSever


	inline void from_json(const json& Json,UCL::ServerCapabilities& Object)
	{
		
	}
	inline void to_json(json& Json, const UCL::ServerCapabilities& Object)
	{
		if (Object.positionEncoding.has_value())
		{
			Json["positionEncoding"] = Object.positionEncoding.value();
		}
		if (Object.hoverProvider.has_value())
		{
			Json["hoverProvider"] = Object.hoverProvider.value();
		}
	}


	inline void from_json(const json& Json, UCL::ResponseError& Object)
	{

	}
	inline void to_json(json& Json, const UCL::ResponseError& Object)
	{
	
	}
	inline void from_json(const json& Json, UCL::InitializeResult::Struct& Object)
	{
		
	}
	inline void to_json(json& Json, const UCL::InitializeResult::Struct& Object)
	{
		to_json(Json["name"], Object.name);


		if (Object.version.has_value())
		{
			to_json(Json["version"], Object.version.value());
		}
	}

	inline void from_json(const json& Json, UCL::InitializeResult& Object)
	{
		
	}
	inline void to_json(json& Json, const UCL::InitializeResult& Object)
	{

		to_json(Json["capabilities"], Object.capabilities);


		if (Object.serverInfo)
		{
			to_json(Json["serverInfo"], Object.serverInfo.value());
		}
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
}