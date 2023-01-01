#pragma once

#include "UCodeLang/Compliation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart
class BackEndInterface
{
public:
	//What Your API Sees
	template<typename T> using GenBackEndCallBack = T*(*)();
	template<typename T> using JustObjectCallBack = void (*)(T BackEndObject);


	template<typename T> 
	BackEndInterface& Set_GenBackEnd(GenBackEndCallBack<T> Value) { _GenBackEnd = (_GenBackEndCallBack)Value; return *this; }

	template<typename T>
	BackEndInterface& Set_DeleteBackEnd(JustObjectCallBack<T> Value) { _DeleteBackEnd = (_JustObjectCallBack)Value; return *this;}

	
	
	
	//Helpers For The API
	using CustomBackendType = void;
	using BackendPtr = CustomBackendType*;
	//Ptr Types
	using _GenBackEndCallBack = BackendPtr (*)();
	using _JustObjectCallBack = void (*)(BackendPtr BackEndObject);
	
	//What SystematicAnalysis Sees
	BackendPtr GenNewBackEnd() const{if (_GenBackEnd) { _GenBackEnd(); }}
	void DeleteBackEnd(BackendPtr BackEndObject) const{if (_DeleteBackEnd) { _DeleteBackEnd(BackEndObject); }}

	
	//
	_GenBackEndCallBack _GenBackEnd =nullptr;
	_JustObjectCallBack _DeleteBackEnd = nullptr;

};
UCodeLangEnd

