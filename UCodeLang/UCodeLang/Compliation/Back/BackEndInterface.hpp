#pragma once

#include "UCodeLang/Compliation/Front/Parser.hpp"

#include "UCodeLang/LangCore/UClib.hpp"

#include <cstring> //memcpy
UCodeLangStart
class IRBuilder;

enum class BackEndObjectOutputType
{
	UCLib,
	Bytes,
	Text,
};

class BackEndObject
{
public:
	virtual void Reset() {}
	virtual void Build(const IRBuilder* Input) {};
	virtual ~BackEndObject() {}

	virtual String GetBackEndName() = 0;
	virtual String GetOutputExtWithDot() = 0;

	inline CompliationErrors& Get_ErrorsOutput()
	{
		return *_ErrorsOutput;
	}
	inline CompliationSettings& Get_Settings()
	{
		return *_Settings;
	}


	UCodeLangForceinline bool HasLibOutput()
	{
		return _OutputLib;
	}
	UCodeLangForceinline UClib& Getliboutput()
	{
		return *_OutputLib;
	}
	UCodeLangForceinline const BytesView GetOutput()
	{
		return _Output.AsSpan();
	}

	inline void Set_ErrorsOutput(CompliationErrors* Errors)
	{
		_ErrorsOutput = Errors;
	}
	inline void Set_Settings(CompliationSettings* setting)
	{
		_Settings = setting;

	}
	inline void Set_OutputLib(UClib* output)
	{
		_OutputLib = output;
	}
protected:
	
	inline void Set_Output(String_view output)
	{
		Set_Output(BytesView((Byte*)output.data(), output.size()));
	}
	inline void Set_Output(const BytesView output)
	{
		_Output.Resize(output.Size());
		memcpy(_Output.Data(), output.Data(), output.Size());
	}
private:
	CompliationErrors* _ErrorsOutput = nullptr;
	CompliationSettings* _Settings = nullptr;
	BytesPtr _Output;
	UClib* _OutputLib;
};

using BackEndObject_Ptr = BackEndObject* (*)();


UCodeLangEnd

