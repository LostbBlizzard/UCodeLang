#include "UCodeLangCAPI.h"
#include "UCodeLang/UCodeLang.hpp"


//Casts

#ifndef UCodeLangNoCompiler
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
#endif

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

UCodeLang::Jit_Interpreter* As(UCodeLangCAPI_JitInterpreter* Value)
{
	return (UCodeLang::Jit_Interpreter*)Value;
}
const UCodeLang::Jit_Interpreter* As(UCodeLangCAPI_Const_JitInterpreter* Value)
{
	return (const UCodeLang::Jit_Interpreter*)Value;
}
UCodeLangCAPI_JitInterpreter* As(UCodeLang::Jit_Interpreter* Value)
{
	return (UCodeLangCAPI_JitInterpreter*)Value;
}
UCodeLangCAPI_Const_JitInterpreter* As(const UCodeLang::Jit_Interpreter* Value)
{
	return (UCodeLangCAPI_Const_JitInterpreter*)Value;
}

UCodeLang::NativeInterpreter* As(UCodeLangCAPI_NativeInterpreter* Value)
{
	return (UCodeLang::NativeInterpreter*)Value;
}
const UCodeLang::NativeInterpreter* As(UCodeLangCAPI_Const_NativeInterpreter* Value)
{
	return (const UCodeLang::NativeInterpreter*)Value;
}
UCodeLangCAPI_NativeInterpreter* As(UCodeLang::NativeInterpreter* Value)
{
	return (UCodeLangCAPI_NativeInterpreter*)Value;
}
UCodeLangCAPI_Const_NativeInterpreter* As(const UCodeLang::NativeInterpreter* Value)
{
	return (UCodeLangCAPI_Const_NativeInterpreter*)Value;
}

UCodeLang::AnyInterpreter* As(UCodeLangCAPI_AnyInterpreter* Value)
{
	return (UCodeLang::AnyInterpreter*)Value;
}
const UCodeLang::AnyInterpreter* As(UCodeLangCAPI_Const_AnyInterpreter* Value)
{
	return (const UCodeLang::AnyInterpreter*)Value;
}
UCodeLangCAPI_AnyInterpreter* As(UCodeLang::AnyInterpreter* Value)
{
	return (UCodeLangCAPI_AnyInterpreter*)Value;
}
UCodeLangCAPI_Const_AnyInterpreter* As(const UCodeLang::AnyInterpreter* Value)
{
	return (UCodeLangCAPI_Const_AnyInterpreter*)Value;
}


UCodeLang::AnyInterpreter* As(UCodeLangCAPI_AnyInterpreterPtr* Value)
{
	return (UCodeLang::AnyInterpreter*)Value;
}
const UCodeLang::AnyInterpreterPtr* As(UCodeLangCAPI_Const_AnyInterpreterPtr* Value)
{
	return (const UCodeLang::AnyInterpreterPtr*)Value;
}
UCodeLangCAPI_AnyInterpreterPtr* As(UCodeLang::AnyInterpreterPtr* Value)
{
	return (UCodeLangCAPI_AnyInterpreterPtr*)Value;
}
UCodeLangCAPI_Const_AnyInterpreterPtr* As(const UCodeLang::AnyInterpreterPtr* Value)
{
	return (UCodeLangCAPI_AnyInterpreterPtr*)Value;
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

UCodeLang::ClassMethod* As(UCodeLangCAPI_ClassMethod* Value)
{
	return (UCodeLang::ClassMethod*)Value;
}
UCodeLangCAPI_ClassMethod* As(UCodeLang::ClassMethod* Value)
{
	return (UCodeLangCAPI_ClassMethod*)Value;
}


const UCodeLang::ClassMethod* As(UCodeLangCAPI_Const_ClassMethod* Value)
{
	return (UCodeLang::ClassMethod*)Value;
}
UCodeLangCAPI_Const_ClassMethod* As(const UCodeLang::ClassMethod* Value)
{
	return (UCodeLangCAPI_ClassMethod*)Value;
}

//Funcs

#ifndef UCodeLangNoCompiler
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
#endif

UCodeLangCAPI_RunTimeLangState* UCodeLangAPIExport UCodeLangCAPI_New_RunTimeState()
{
	return As(new UCodeLang::RunTimeLangState());
}

void UCodeLangAPIExport UCodeLangCAPI_Destroy_RunTimeState(UCodeLangCAPI_RunTimeLangState* Value)
{
	delete As(Value);
}

//Interpreter
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
UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_Interpreter_CallA(UCodeLangCStruct UCodeLangCAPI_Interpreter* This, UCodeLangCAPI_UAddress address)
{
	return  As(As(This)->Call(address));
}
UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_Interpreter_CallM(UCodeLangCStruct UCodeLangCAPI_Interpreter* This, UCodeLangCAPI_Const_ClassMethod* method)
{
	return  As(As(This)->Call(As(method)));
}

void UCodeLangAPIExport UCodeLangCAPI_Interpreter_GetReturn(UCodeLangCAPI_Interpreter* This, void* Output, size_t OutputSize)
{
	return  As(This)->Get_Return(Output,OutputSize);
}

void UCodeLangAPIExport UCodeLangCAPI_Interpreter_PushParameter(UCodeLangCAPI_Interpreter* This, const void* object, size_t objectsize)
{
	As(This)->PushParameter(object, objectsize);
}
void UCodeLangCAPI_Interpreter_Init(UCodeLangCStruct UCodeLangCAPI_Interpreter* This, UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* runtime)
{
	As(This)->Init(As(runtime));
}
void UCodeLangCAPI_Interpreter_DeInit(UCodeLangCStruct UCodeLangCAPI_Interpreter* This)
{
	As(This)->UnLoad();
}
//Jit
UCodeLangCAPI_JitInterpreter* UCodeLangAPIExport UCodeLangCAPI_New_JitInterpreter()
{
	return As(new UCodeLang::Jit_Interpreter());
}

void UCodeLangAPIExport UCodeLangCAPI_Destroy_JitInterpreter(UCodeLangCAPI_JitInterpreter* Value)
{
	delete As(Value);
}

UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_JitInterpreter_Call(UCodeLangCAPI_JitInterpreter* This, UCodeLangCAPI_CharSpan* FuncName)
{
	return As(As(This)->Call(UCodeLang::String(As(FuncName))));
}
UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_JitInterpreter_CallA(UCodeLangCStruct UCodeLangCAPI_JitInterpreter* This, UCodeLangCAPI_UAddress address)
{
	return  As(As(This)->Call(address));
}
UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_JitInterpreter_CallM(UCodeLangCStruct UCodeLangCAPI_JitInterpreter* This, UCodeLangCAPI_Const_ClassMethod* method)
{
	return  As(As(This)->Call(As(method)));
}

void UCodeLangAPIExport UCodeLangCAPI_JitInterpreter_GetReturn(UCodeLangCAPI_JitInterpreter* This, void* Output, size_t OutputSize)
{
	return  As(This)->Get_Return(Output, OutputSize);
}

void UCodeLangAPIExport UCodeLangCAPI_JitInterpreter_PushParameter(UCodeLangCAPI_JitInterpreter* This, const void* object, size_t objectsize)
{
	As(This)->PushParameter(object, objectsize);
}
void UCodeLangCAPI_JitInterpreter_Init(UCodeLangCStruct UCodeLangCAPI_JitInterpreter* This, UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* runtime)
{
	As(This)->Init(As(runtime));
}
void UCodeLangCAPI_JitInterpreter_DeInit(UCodeLangCStruct UCodeLangCAPI_JitInterpreter* This)
{
	As(This)->UnLoad();
}
//Native
UCodeLangCAPI_NativeInterpreter* UCodeLangAPIExport UCodeLangCAPI_New_NativeInterpreter()
{
	return As(new UCodeLang::NativeInterpreter());
}

void UCodeLangAPIExport UCodeLangCAPI_Destroy_NativeInterpreter(UCodeLangCAPI_NativeInterpreter* Value)
{
	delete As(Value);
}

UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_NativeInterpreter_Call(UCodeLangCAPI_NativeInterpreter* This, UCodeLangCAPI_CharSpan* FuncName)
{
	return As(As(This)->Call(UCodeLang::String(As(FuncName))));
}
UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_NativeInterpreter_CallA(UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* This, UCodeLangCAPI_UAddress address)
{
	return  As(As(This)->Call(address));
}
UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_NativeInterpreter_CallM(UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* This, UCodeLangCAPI_Const_ClassMethod* method)
{
	return  As(As(This)->Call(As(method)));
}

void UCodeLangAPIExport UCodeLangCAPI_NativeInterpreter_GetReturn(UCodeLangCAPI_NativeInterpreter* This, void* Output, size_t OutputSize)
{
	return  As(This)->Get_Return(Output, OutputSize);
}

void UCodeLangAPIExport UCodeLangCAPI_NativeInterpreter_PushParameter(UCodeLangCAPI_NativeInterpreter* This, const void* object, size_t objectsize)
{
	As(This)->PushParameter(object, objectsize);
}
void UCodeLangCAPI_NativeInterpreter_Init(UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* This, UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* runtime)
{
	As(This)->Init(As(runtime));
}
void UCodeLangCAPI_NativeInterpreter_DeInit(UCodeLangCStruct UCodeLangCAPI_NativeInterpreter* This)
{
	As(This)->UnLoad();
}
//Any Interpreter

UCodeLangCAPI_AnyInterpreter* UCodeLangAPIExport UCodeLangCAPI_New_AnyInterpreter()
{
	return As(new UCodeLang::AnyInterpreter());
}

void UCodeLangAPIExport UCodeLangCAPI_Destroy_AnyInterpreter(UCodeLangCAPI_AnyInterpreter* Value)
{
	delete As(Value);
}

UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_AnyInterpreter_Call(UCodeLangCAPI_AnyInterpreter* This, UCodeLangCAPI_CharSpan* FuncName)
{
	return As(As(This)->Call(UCodeLang::String(As(FuncName))));
}
UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_AnyInterpreter_CallA(UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* This, UCodeLangCAPI_UAddress address)
{
	return  As(As(This)->Call(address));
}
UCodeLangCAPI_Interpreter_Return_t UCodeLangAPIExport UCodeLangCAPI_AnyInterpreter_CallM(UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* This, UCodeLangCAPI_Const_ClassMethod* method)
{
	return  As(As(This)->Call(As(method)));
}

void UCodeLangAPIExport UCodeLangCAPI_AnyInterpreter_GetReturn(UCodeLangCAPI_AnyInterpreter* This, void* Output, size_t OutputSize)
{
	return  As(This)->Get_Return(Output, OutputSize);
}

void UCodeLangAPIExport UCodeLangCAPI_AnyInterpreter_PushParameter(UCodeLangCAPI_AnyInterpreter* This, const void* object, size_t objectsize)
{
	As(This)->PushParameter(object, objectsize);
}
void UCodeLangCAPI_AnyInterpreter_Init(UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* This, UCodeLangCStruct UCodeLangCAPI_RunTimeLangState* runtime)
{
	As(This)->Init(As(runtime));
}
void UCodeLangCAPI_AnyInterpreter_DeInit(UCodeLangCStruct UCodeLangCAPI_AnyInterpreter* This)
{
	As(This)->UnLoad();
}