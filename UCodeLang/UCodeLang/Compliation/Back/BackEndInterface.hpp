#pragma once

#include "UCodeLang/Compliation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"
UCodeLangStart
class SystematicAnalysis;
class BackEndInterface
{
public:
	//What Your API Sees
	template<typename T> using GenBackEndCallBack = T*(*)();
	template<typename T> using JustObjectCallBack = void (*)(T BackEndObject);
	template<typename T> using ErrorsCallBack = void (*)(T BackEndObject, CompliationErrors* ErrorsOutput);
	template<typename T> using BuildStatementsCallBack = void (*)(T BackEndObject, const Vector<Node*>& nodes);
	template<typename T> using AnalysisCallBack =void (*)(T BackEndObject,SystematicAnalysis* analysis);

	template<typename T> 
	BackEndInterface& Set_GenBackEnd(GenBackEndCallBack<T> Value) { _GenBackEnd = (_GenBackEndCallBack)Value; return *this; }

	template<typename T>
	BackEndInterface& Set_DeleteBackEnd(JustObjectCallBack<T> Value) { _DeleteBackEnd = (_JustObjectCallBack)Value; return *this;}

	template<typename T>
	BackEndInterface& Set_ErrorsOutput(ErrorsCallBack<T> Value) { _SetErrors = (_ErrorsCallBack)Value; return *this; }

	template<typename T>
	BackEndInterface& Set_Analysis(AnalysisCallBack<T> Value) { _Analysis = (_AnalysisCallBack)Value; return *this; }

	template<typename T>
	BackEndInterface& Set_BuildStatements(BuildStatementsCallBack<T> Value) { _BuildStatements = (_BuildStatementsCallBack)Value; return *this; }
	
	
	//Helpers For The API
	using CustomBackendType = void;
	using BackendPtr = CustomBackendType*;
	//Ptr Types
	using _GenBackEndCallBack = BackendPtr (*)();
	using _JustObjectCallBack = void (*)(BackendPtr BackEndObject);
	using _BuildStatementsCallBack = void (*)(BackendPtr BackEndObject,const Vector<Node*>& nodes);
	using _ErrorsCallBack = void (*)(BackendPtr BackEndObject, CompliationErrors* ErrorsOutput);
	using _AnalysisCallBack = void (*)(BackendPtr BackEndObject, SystematicAnalysis* analysis);
	
	//What SystematicAnalysis Sees
	BackendPtr GenNewBackEnd() const { if (_GenBackEnd) { return _GenBackEnd(); }return nullptr; }
	void DeleteBackEnd(BackendPtr BackEndObject) const{if (_DeleteBackEnd) { _DeleteBackEnd(BackEndObject); }}
	void BuildStatements(BackendPtr BackEndObject, const Vector<Node*>& nodes)const { if (_BuildStatements) { _BuildStatements(BackEndObject,nodes); } }
	
	void SetBackEndAnalysis(BackendPtr BackEndObject, SystematicAnalysis* analysis)const { if (_Analysis) { _Analysis(BackEndObject,analysis); } }
	void SetBackEndErrors(BackendPtr BackEndObject, CompliationErrors* ErrorsOutput)const { if (_SetErrors) { _SetErrors(BackEndObject, ErrorsOutput); } }
	//
	_GenBackEndCallBack _GenBackEnd =nullptr;
	_JustObjectCallBack _DeleteBackEnd = nullptr;
	_BuildStatementsCallBack _BuildStatements = nullptr;
	_ErrorsCallBack _SetErrors = nullptr;
	_AnalysisCallBack _Analysis = nullptr;
};
UCodeLangEnd

