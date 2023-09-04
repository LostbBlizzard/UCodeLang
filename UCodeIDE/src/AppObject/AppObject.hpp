#pragma once
#include <ULanguageNameSpace.hpp>
#include <memory>
#include <thread>

#include "ImGuiHelpers/TextEditor/TextEditor.h"
#include "LSPSever.hpp"
#include "UCodeLang/UCodeLang.hpp"

#include "ImGuiHelpers/ImguiHelper.hpp"
#include <chrono>
#include <future>
UCodeIDEStart

using SteadyClock = std::chrono::steady_clock;

struct SandBoxLanguageSever
{
	UCodeLanguageSever::LSPSever _Sever;
};
class  AppClientFuncMap;
class AppObject
{
public:
	friend AppClientFuncMap;
	AppObject() {};
	~AppObject() { EndApp(); };

	void Init();
	void OnDraw();

	void ProcessSeverPackets();

	static void ShowInFiles(const Path& path);

	void ShowUCodeVMWindow();



	static size_t GetColumn(const String& text, size_t line, size_t Pos);

	bool IsAppRuning()
	{
		return _IsAppRuning;
	}
	void EndApp()
	{
		if (_IsAppRuning)
		{
			OnAppEnd();
		}
	}
private:
	void OnAppEnd();
	bool _IsAppRuning = false;
	bool _IsLSPRuning = false;
	float DetaTime = 0;
	SteadyClock::time_point LastFrame;

	using CPacket = UCodeLanguageSever::ClientPacket;
	using SPacket = UCodeLanguageSever::SeverPacket;
	using RequestCallBack = std::function<void(SPacket::ResponseMessage_t)>;

	UCodeLanguageSever::integer _RequestID = 1;
	UCodeLanguageSever::integer Get_NewRequestID()
	{
		return _RequestID++;
	}
	void SendPacket(CPacket&& packet)
	{
		if (this->SeverPtr)
		{
			this->SeverPtr->_Sever.AddPacket(std::move(packet));
		}
	}
	void OnSeverPacket(SPacket&& packet);

	struct RequestInfo
	{
		UCodeLanguageSever::integer RequestID;
		AppObject* This = nullptr;

		void SetCallBack(RequestCallBack CallBack)
		{
			This->SetRequestCallBack(RequestID, CallBack);
		}
	};

	template<typename T>
	RequestInfo SendNewRequestMessage(const String& method, const T& params)
	{
		RequestInfo r;
		r.RequestID = Get_NewRequestID();
		r.This = this;
		SendPacket(CPacket::RequestMessage(r.RequestID, method, params));
		return r;
	}

	template<typename T>
	void SendNotificationMessage(const String& method, const T& params)
	{
		SendPacket(CPacket::NotificationMessage(method, params));
	}

	RequestInfo SendInitializeRequest(const UCodeLanguageSever::InitializeParams& Pars)
	{
		return SendNewRequestMessage("initialize", Pars);
	}

	///The shutdown request is sent from the client to the server. 
	//It asks the server to shut down, but to not exit 
	//(otherwise the response might not be delivered correctly to the client).
	RequestInfo SendShutdoneRequest()
	{
		return SendNewRequestMessage("shutdown", UCodeLanguageSever::TsNull());
	}

	//A notification to ask the server to exit its process.
	void SendExitNotification()
	{
		SendNotificationMessage("exit", UCodeLanguageSever::TsNull());
	}
	//The document open notification is sent from the client to the server to signal newly opened text documents. 
	void SendDidOpenTextDocument(const UCodeLanguageSever::DidOpenTextDocumentParams& Pars)
	{
		SendNotificationMessage("textDocument/didOpen", Pars);
	}
	//The document close notification is sent from the client to the server when the document got closed in the client.
	void SendDidCloseTextDocument(const UCodeLanguageSever::DidCloseTextDocumentParams& Pars)
	{
		SendNotificationMessage("textDocument/didClose", Pars);
	}
	//The document change notification is sent from the client to the server to signal changes to a text document.
	void SendDidChangeTextDocument(const UCodeLanguageSever::DidChangeTextDocumentParams& Pars)
	{
		SendNotificationMessage("textDocument/didChange", Pars);
	}
	void OnPublishDiagnostics(const UCodeLanguageSever::json& Params);

	String SeverSideFile;
	size_t FileVersion = 0;

	float LastFileUpdated = 0;
	float AutoCompileTimeOut = 0;
	UCodeLanguageSever::PublishDiagnosticsParams PublishedDiagnostics;

	bool LSPHasNoErrors();

	TextEditor _Editor;
	String GetTextEditorString()
	{
		return _Editor.GetText();
	}
	struct OnGoingRequest
	{
		UCodeLanguageSever::integer RequestID;
		RequestCallBack CallBack;
	};
	UCodeLang::BinaryVectorMap<UCodeLanguageSever::integer, OnGoingRequest> RequestCallBacks;
	void SetRequestCallBack(UCodeLanguageSever::integer RequestID, RequestCallBack CallBack);

	//SandBox
	SandBoxLanguageSever* SeverPtr = nullptr;
	std::unique_ptr<std::thread> _LangSeverThread;
	//Compiler
	String _CompilerStr;
	UCodeLang::Compiler _Compiler;
	UCodeLang::UClib _CompiledLib;
	String _LibInfoString;
	struct Error
	{
		UCodeLang::CompliationErrors::Error _Error;
		bool IsSelected = false;
	};
	void OnErrorListUpdated();

	void CompileText(const UCodeAnalyzer::String& String);

	void OnDoneCompileing(UCodeLang::Compiler::CompilerRet& Val, const UCodeAnalyzer::Path& tepoutpath);

	Vector<Error> Errors;
	//RunTime
	String _RunTimeStr;
	UCodeLang::RunTimeLangState _RunTimeState;
	UCodeLang::AnyInterpreter _AnyInterpreter;
	UCodeLang::RunTimeLib _RuntimeLib;
	//Windows

	enum class BackEndType
	{
		UCodeVM,
		C89,
		IR,
		LLVM,
		WebAssembly,

		WindowsExecutable,
		LinuxExecutable,
	};

	enum class NativeSet
	{
		x86,
		x86_64,
		Amd,
		Amd64,

#if UCodeLang_CPUIs_x86
		Native = x86,
#elif UCodeLang_CPUIs_x86_64
		Native = x86_64,
#elif UCodeLang_CPUIs_Arm
		Native = Amd,
#elif UCodeLang_CPUIs_Arm64
		Native = Amd64,
#else
		Native = x86_64,
#endif
	};
	struct OutputWindowData
	{
		BackEndType Type = BackEndType::UCodeVM;

		bool AutoCompile = true;
		bool AutoReload = true;
		bool AutoHotReload = true;

		UCodeLang::OptimizationFlags Flags = UCodeLang::OptimizationFlags::NoOptimization;
		bool InDebug = true;

		NativeSet OldNativeCpuType = NativeSet::x86;
	};
	OutputWindowData OutputWindow;

	enum class UCodeVMType
	{
		Interpreter,
		Jit_Interpreter,
		Native_Interpreter,
	};
	struct UCodeVMWindow
	{
		UCodeVMType VMType = UCodeVMType::Interpreter;
		NativeSet NativeCpuType = NativeSet::Native;
		bool ShowRegisters = false;
		bool ShowStack = false;
		bool ShowStaticMemory = false;
		bool ShowThreadMemory = false;
		bool ShowHeapMemory = false;

		bool CallStaticVarOnReload = true;
		struct InsData
		{
			UCodeLang::UAddress InsAddress = UCodeLang::NullAddress;
			UCodeLang::Instruction _Ins;
			String StringValue;
		};
		Vector<InsData> InsInfo;
		InsData& GetIns(UCodeLang::UAddress address)
		{
			return InsInfo[address];
		}
		const InsData& GetIns(UCodeLang::UAddress address) const
		{
			return InsInfo[address];
		}
	};
	UCodeVMWindow windowdata;
	void UpdateInsData(UCodeVMWindow& windowdata);

	void ShowDebugerMenu(UCodeVMWindow& windowdata);
	void OnRuntimeUpdated();

	struct CallFuncContext
	{
		const UCodeLang::ClassMethod* current_method = nullptr;
		Vector<BytesPtr> Args;

		UCodeLang::ReflectionTypeInfo _LastRetType;
		BytesPtr _LastRet;

		bool CallStaticAndThreadInit = true;
		bool CallStaticAndThreadDeInit = true;
	};
	CallFuncContext callFuncContext;
	void FullReloadRunTime();
	void HotReloadRunTime();
	void OpenOnWeb(const String& WebLink);


	bool IsRuningCompiler = false;
	std::future<UCodeLang::Compiler::CompilerRet> _RuningCompiler;
	UCodeLang::Compiler::CompilerPathData _RuningPaths;


	template<typename T, typename... Pars> std::future<T>
		SendTaskToWorkerThread(std::function<T(Pars...)> Func, Pars... pars)
		{
			return std::async(Func, pars...);
		}

	void DrawTestMenu();

	void DrawPerformanceMenu();
};

UCodeIDEEnd