#pragma once
#include "JSONstructures.hpp"
namespace nlohmann
{
	using namespace UCodeLanguageSever;
	inline void from_json(const json& Json,ServerCapabilities& Object)
	{
		
	}
	inline void to_json(json& Json, const ServerCapabilities& Object) 
	{
		if (Object.positionEncoding.has_value())
		{
			Json["positionEncoding"] = Object.positionEncoding.value();
		}
	}


	inline void from_json(const json& Json, ResponseError& Object)
	{

	}
	inline void to_json(json& Json, const ResponseError& Object)
	{
	
	}
}