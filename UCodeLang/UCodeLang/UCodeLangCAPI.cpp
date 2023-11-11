#include "UCodeLangCAPI.h"
#include "UCodeLang/UCodeLang.hpp"


//Casts
UCodeLang::Compiler* As(UCodeLangCAPI_Compiler* Value)
{
	return (UCodeLang::Compiler*)Value;
}
const UCodeLang::Compiler* As(UCodeLangCAPI_Const_Compiler* Value)
{
	return (const UCodeLang::Compiler*)Value;
}
UCodeLangCAPI_Compiler* As(UCodeLang::Compiler* Value)
{
	return (UCodeLangCAPI_Compiler*)Value;
}
const UCodeLangCAPI_Const_Compiler* As(const UCodeLang::Compiler* Value)
{
	return (UCodeLangCAPI_Const_Compiler*)Value;
}

UCodeLang::Interpreter* As(UCodeLangCAPI_Interpreter* Value)
{
	return (UCodeLang::Interpreter*)Value;
}
const UCodeLang::Interpreter* As(UCodeLangCAPI_Const_Interpreter* Value)
{
	return (const UCodeLang::Interpreter*)Value;
}
UCodeLangCAPI_Interpreter* As(UCodeLang::Interpreter* Value)
{
	return (UCodeLangCAPI_Interpreter*)Value;
}
UCodeLangCAPI_Const_Interpreter* As(const UCodeLang::Interpreter* Value)
{
	return (UCodeLangCAPI_Const_Interpreter*)Value;
}


UCodeLang::RunTimeLangState* As(UCodeLangCAPI_RunTimeLangState* Value)
{
	return (UCodeLang::RunTimeLangState*)Value;
}
const UCodeLang::RunTimeLangState* As(UCodeLangCAPI_Const_RunTimeLangState* Value)
{
	return (const UCodeLang::RunTimeLangState*)Value;
}
UCodeLangCAPI_RunTimeLangState* As(UCodeLang::RunTimeLangState* Value)
{
	return (UCodeLangCAPI_RunTimeLangState*)Value;
}
UCodeLangCAPI_Const_RunTimeLangState* As(const UCodeLang::RunTimeLangState* Value)
{
	return (UCodeLangCAPI_Const_RunTimeLangState*)Value;
}

UCodeLang::String_view As(UCodeLangCAPI_CharSpan* Value)
{
	return UCodeLang::String_view(Value->pointer, Value->size);
}
const UCodeLang::String_view As(UCodeLangCAPI_Const_CharSpan* Value)
{
	return UCodeLang::String_view(Value->pointer, Value->size);
}
UCodeLang::String_view As(UCodeLangCAPI_PathSpan* Value)
{
	return UCodeLang::String_view(Value->pointer, Value->size);
}
const UCodeLang::String_view As(UCodeLangCAPI_Const_PathSpan* Value)
{
	return UCodeLang::String_view(Value->pointer, Value->size);
}
UCodeLang::BytesView As(UCodeLangCAPI_ByteSpan* Value)
{
	return UCodeLang::BytesView::Make(Value->pointer, Value->size);
}
const UCodeLang::BytesView As(UCodeLangCAPI_Const_ByteSpan* Value)
{
	return UCodeLang::BytesView::Make(Value->pointer, Value->size);
}

UCodeLangCAPI_AnyInt64 As(UCodeLang::AnyInt64 Value)
{
	return { Value.Value };
}
UCodeLangCAPI_Interpreter_Register As(UCodeLang::Interpreter::Register Value)
{
	return { As(Value.Value) };
}


UCodeLangCAPI_Interpreter_RetState As(UCodeLang::Interpreter::RetState Value)
{
	switch (Value)
	{
	case UCodeLang::Interpreter::RetState::Null:return { UCodeLangCAPI_Interpreter_RetState::Null };
	case UCodeLang::Interpreter::RetState::Success:return { UCodeLangCAPI_Interpreter_RetState::Success };
	case UCodeLang::Interpreter::RetState::Error:return {UCodeLangCAPI_Interpreter_RetState::Error};
	case UCodeLang::Interpreter::RetState::Error_Function_doesnt_exist:return { UCodeLangCAPI_Interpreter_RetState::Null };
	default:return { UCodeLangCAPI_Interpreter_RetState::Null };
	}
}

UCodeLangCAPI_Interpreter_Return_t As(UCodeLang::Interpreter::Return_t Value)
{
	return {As(Value._Succeed),As(Value.ReturnValue)};
}
void UCodeLangAPIExport UCodeLangCAPI_Init()
{
	//make all static varables
}

void UCodeLangAPIExport UCodeLangCAPI_DeInit()
{
	//free all static varables
}

//Funcs
UCodeLangCAPI_Compiler* UCodeLangAPIExport UCodeLangCAPI_New_Compiler()
{
	return As(new UCodeLang::Compiler());
}

void  UCodeLangAPIExport UCodeLangCAPI_Destroy_Compiler(UCodeLangCAPI_Compiler* Value)
{
	delete As(Value);
}

void UCodeLangAPIExport UCodeLangCAPI_Compiler_BuildTxt(UCodeLangCAPI_Compiler* This, UCodeLangCAPI_CharSpan String, UCodeLangCAPI_CompilerOutput* Output)
{
	auto r = As(This)->CompileText(As(&String));
	Output->Success = r.IsError();
}

UCodeLangCAPI_RunTimeLangState* UCodeLangAPIExport UCodeLangCAPI_New_RunTimeState()
{
	return As(new UCodeLang::RunTimeLangState());
}

void UCodeLangAPIExport UCodeLangCAPI_Destroy_RunTimeState(UCodeLangCAPI_RunTimeLangState* Value)
{
	delete As(Value);
}

UCodeLangCAPI_Interpreter* UCodeLangAPIExport UCodeLangCAPI_New_Interpreter()
{
	return As(new UCodeLang::Interpreter());
}

void UCodeLangAPIExport UCodeLangCAPI_Destroy_Interpreter(UCodeLangCAPI_Interpreter* Value)
{
	delete As(Value);
}

UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_Interpreter_Call(UCodeLangCAPI_Interpreter* This, UCodeLangCAPI_CharSpan* FuncName)
{
	return As(As(This)->Call(UCodeLang::String(As(FuncName))));
}
