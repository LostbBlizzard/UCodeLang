#pragma once

#include "UCodeLang/LangCore/LangTypes.hpp"
#include <cstring> //memcpy
UCodeLangStart
class IRBuilder;
class UClib;
class CompilationErrors;
struct CompilationSettings;
struct CompilationBackEndInfo;
struct IRFunc;
struct IRInstruction;
struct IROperator;
struct IRType;
struct IRBufferData;
using IRidentifierID = UInt64;
	
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
	virtual void UpdateBackInfo(CompilationBackEndInfo& BackInfo) = 0;

	inline CompilationErrors& Get_ErrorsOutput()
	{
		return *_ErrorsOutput;
	}
	inline CompilationSettings& Get_Settings()
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

	inline void Set_ErrorsOutput(CompilationErrors* Errors)
	{
		_ErrorsOutput = Errors;
	}
	inline void Set_Settings(CompilationSettings* setting)
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
	CompilationErrors* _ErrorsOutput = nullptr;
	CompilationSettings* _Settings = nullptr;
	BytesPtr _Output;
	UClib* _OutputLib;
};

using BackEndObject_Ptr = BackEndObject* (*)();


UCodeLangEnd

