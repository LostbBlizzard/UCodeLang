#pragma once

#include "UCodeLang/Compliation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart
class SystematicAnalysis;
class IRBuilder;
class SymbolTable;

struct BackEndInput
{
	IRBuilder* _Builder = nullptr;
	SymbolTable* _Table = nullptr;
};

class BackEndInterface
{
public:
	//What Your API Sees
	template<typename T> using GenBackEndCallBack = T*(*)();
	template<typename T> using JustObjectCallBack = void (*)(T BackEndObject);
	template<typename T> using ErrorsCallBack = void (*)(T BackEndObject, CompliationErrors* ErrorsOutput);
	template<typename T> using AnalysisCallBack =void (*)(T BackEndObject,SystematicAnalysis* analysis);
	template<typename T> using BuildInputCallBack = void (*)(T BackEndObject, BackEndInput& Input);

	template<typename T> 
	BackEndInterface& Set_GenBackEnd(GenBackEndCallBack<T> Value) { _GenBackEnd = (_GenBackEndCallBack)Value; return *this; }

	template<typename T>
	BackEndInterface& Set_DeleteBackEnd(JustObjectCallBack<T> Value) { _DeleteBackEnd = (_JustObjectCallBack)Value; return *this;}

	template<typename T>
	BackEndInterface& Set_ErrorsOutput(ErrorsCallBack<T> Value) { _SetErrors = (_ErrorsCallBack)Value; return *this; }

	template<typename T>
	BackEndInterface& Set_Analysis(AnalysisCallBack<T> Value) { _Analysis = (_AnalysisCallBack)Value; return *this; }

	template<typename T>
	BackEndInterface& Set_BuildInput(BuildInputCallBack<T> Value) { _BuildBackEnd = (_BuildInputCallBack)Value; return *this; }

	//Helpers For The API
	using CustomBackendType = void;
	using BackendPtr = CustomBackendType*;
	//Ptr Types
	using _GenBackEndCallBack = BackendPtr (*)();
	using _JustObjectCallBack = void (*)(BackendPtr BackEndObject);
	using _ErrorsCallBack = void (*)(BackendPtr BackEndObject, CompliationErrors* ErrorsOutput);
	using _AnalysisCallBack = void (*)(BackendPtr BackEndObject, SystematicAnalysis* analysis);
	using _BuildInputCallBack = void (*)(BackendPtr BackEndObject, BackEndInput& Input);
	
	//What SystematicAnalysis Sees
	BackendPtr GenNewBackEnd() const { if (_GenBackEnd) { return _GenBackEnd(); }return nullptr; }
	void DeleteBackEnd(BackendPtr BackEndObject) const{if (_DeleteBackEnd) { _DeleteBackEnd(BackEndObject); }}
	
	void SetBackEndAnalysis(BackendPtr BackEndObject, SystematicAnalysis* analysis)const { if (_Analysis) { _Analysis(BackEndObject,analysis); } }
	void SetBackEndErrors(BackendPtr BackEndObject, CompliationErrors* ErrorsOutput)const { if (_SetErrors) { _SetErrors(BackEndObject, ErrorsOutput); } }
	
	void Build(BackendPtr BackEndObject, BackEndInput& Input)const{ if (_BuildBackEnd) { _BuildBackEnd(BackEndObject, Input); } }
	//
	_GenBackEndCallBack _GenBackEnd =nullptr;
	_JustObjectCallBack _DeleteBackEnd = nullptr;
	_ErrorsCallBack _SetErrors = nullptr;
	_AnalysisCallBack _Analysis = nullptr;
	_BuildInputCallBack _BuildBackEnd = nullptr;
};
UCodeLangEnd

