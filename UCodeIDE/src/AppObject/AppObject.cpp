#include "AppObject.hpp"
#include "imgui/imgui.h"
#include "ImGuiHelpers/ImguiHelper.hpp"
#include "AppAPILink.hpp"

#include "imgui/misc/cpp/imgui_stdlib.h"
#include "UCodeLang/Compilation/UAssembly/UAssembly.hpp"

#include "UCodeLang/Compilation/ModuleFile.hpp"


#include "UCodeLang/LangCore/Version.hpp"

#include "UCodeLang/Compilation/Back/UCodeBackEnd/UCodeBackEnd.hpp"
#include "UCodeLang/Compilation/Back/C11/C11Backend.hpp"
#include "UCodeLang/Compilation/Back/IR/IRBackEnd.hpp"
#include "UCodeLang/Compilation/Back/LLVM/LLVMBackEnd.hpp"
#include "UCodeLang/Compilation/Back/WebAssembly/WasmBackEnd.hpp"
#include "UCodeLang/Compilation/Back/Windows/WindowsBackEnd.hpp"
#include "UCodeLang/Compilation/Back/Linux/LinuxBackEnd.hpp"
#include "UCodeLang/Compilation/Back/MacOs/MacOSBackEnd.hpp"

#include "UCodeLang/RunTime/ProfilerDebuger.hpp"

#include "UCodeAnalyzer/SyntaxHelper.hpp"

#include "UCodeLang/Compilation/Back/Windows/PE_File.hpp"
#include "UCodeLang/Compilation/Back/Linux/ELF_File.hpp"
#include "UCodeLang/Compilation/Back/MacOs/MachO.hpp"



#include "UCodeLang/Compilation/Back/x86_64/X86_64UNativeBackEnd.hpp"
#include "UCodeLang/Compilation/Back/x86_64/X86_64JitCompiler.hpp"

#include "ECSExample.hpp"
#include <fstream>
#include <filesystem>
#include "ImGuiHelpers/imgui_memory_editor/imgui_memory_editor.h"
#include <tests/Test.hpp>
#include <future>
#include <sstream>
#if UCodeLang_Platform_Windows
#include <Windows.h>
#elif UCodeLang_Platform_Posix
#include <dlfcn.h>
#else

#endif 

UCodeIDEStart

//I dont think threads work in wasm

#if __EMSCRIPTEN__
#define NoUseThread 1
#else
#define NoUseThread 0
#endif

static UCodeLang::ProfilerDebuger Debuger;

void UCodeIDEStyle(ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	
	style->FrameBorderSize = 1.0f;
	style->FrameRounding = 0.0f;

   // ImVec4 BackRoundColor = {};
   // ImVec4 MainColor = {};
   // ImVec4 SecondaryColor = {};

    //colors[ImGuiCol_::ImGuiCol_FrameBg] = BackRoundColor;
    //colors[ImGuiCol_::ImGuiCol_Button] = MainColor;

    ImVec4* colors = style->Colors;
    colors[ImGuiCol_Border] = ImVec4(0.97f, 1.00f, 0.00f, 0.11f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.08f, 0.29f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.08f, 0.12f, 0.29f, 0.54f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.08f, 0.49f, 0.29f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.08f, 0.49f, 0.29f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.49f, 0.29f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.16f, 0.08f, 0.29f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.08f, 0.12f, 0.29f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.08f, 0.29f, 0.22f, 0.54f);
    colors[ImGuiCol_Header] = ImVec4(0.08f, 0.49f, 0.29f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.08f, 0.29f, 0.14f, 0.54f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.08f, 0.29f, 0.22f, 0.54f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.08f, 0.29f, 0.22f, 0.54f);
    colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.49f, 0.29f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.08f, 0.29f, 0.14f, 0.54f);
    colors[ImGuiCol_TabActive] = ImVec4(0.16f, 0.94f, 0.67f, 0.54f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.16f, 0.08f, 0.29f, 0.54f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.08f, 0.21f, 0.29f, 0.54f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.08f, 0.49f, 0.29f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);


}

namespace LS = UCodeLanguageSever;
const char* EditorTextURL = "src.uc";




void AppObject::Init()
{
    if (!_IsAppRuning) {
        _IsAppRuning = true;

#if !NoUseThread
        /*
        _LangSeverThread = std::make_unique<std::thread>([this]()
            {
                SandBoxLanguageSever SandBox;
                this->SeverPtr = &SandBox;
                while (SandBox._Sever.Step());
                this->SeverPtr = nullptr;
            });
        */
#endif


        {
            TextEditor::LanguageDefinition Def = Def.CPlusPlus();
            Def.mName = "UCodeLang";
            auto keywords = SyntaxHelper::GetKeyWords();

            Def.mKeywords.clear();
            for (auto& Item : keywords)
            {
                Def.mKeywords.insert(Item);
            }
            /*
            Def.mTokenize = [](const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end, TextEditor::PaletteIndex& paletteIndex)
            {


                return false;
            };
            */

            _Editor.SetShowWhitespaces(false);
            _Editor.SetLanguageDefinition(Def);
        }
        /*
        r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += "await::new(" + ToString(State, *I, I->Target()) + ")";
        */
      
        UCodeIDEStyle(nullptr);
        
            
        auto str2 = UCodeLang::Compiler::GetTextFromFile("src/AppObject/test2.uc");
        String str;
        if (str2 == "*null")
        {
            auto str = UCodeLang::Compiler::GetTextFromFile("src/AppObject/test.uc");
            if (str == "*null")
            {
                str = "|main[] => 0;";
            }
        }
        else
        {
            str = str2;
        }
       
        _Editor.SetText(str);
        
        if (UCodeLang::StringHelper::Contains(str,"//Wasm"))
        {
            OutputWindow.Type = BackEndType::WebAssembly;
        }
        if (UCodeLang::StringHelper::Contains(str, "//C"))
        {
            OutputWindow.Type = BackEndType::C89;
        }
        
        UpdateBackEnd();
        CompileText(GetTextEditorString());


        _AnyInterpreter.SetAsInterpreter();
        _AnyInterpreter.Init(&_RunTimeState);
        ImguiHelper::_Ptr = _AnyInterpreter.GetPtr();


        {
            LS::InitializeParams p;
            p.processId = LS::TsNull();
            SendInitializeRequest(p)
                .SetCallBack([this](SPacket::ResponseMessage_t info)
                    {
                        _IsLSPRuning = true;

                        LS::DidOpenTextDocumentParams p;
                        p.textDocument.text = GetTextEditorString();
                        p.textDocument.uri = EditorTextURL;
                        p.textDocument.version = FileVersion;
                        p.textDocument.languageId = UCodeLangLanguageId;

                        SeverSideFile = p.textDocument.text;
                        SendDidOpenTextDocument(p);
                    });
        }
    }
}
void BeginDockSpace(bool* p_open)
{
    // If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
    // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
    // In this specific demo, we are not using DockSpaceOverViewport() because:
    // - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
    // - we allow the host window to have padding (when opt_padding == true)
    // - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
    // TL;DR; this demo is more complicated than what you would normally use.
    // If we removed all the options we are showcasing, this demo would become:
    //     void ShowExampleAppDockSpace()
    //     {
    //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    //     }

    bool opt_fullscreen = true;
    bool opt_padding =false;
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_::ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    //ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("IDE", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("IDE");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
}

#if UCodeLangDebug
inline const UCodeLang::String UCodeLang_SoultionDir = "../";

inline const UCodeLang::String UCodeLang_UCAppDir = UCodeLang_SoultionDir + "UCApp/";
inline const UCodeLang::String UCodeLang_UCAppDir_ScrDir = UCodeLang_UCAppDir + "src/";
inline const UCodeLang::String UCodeLang_UCAppDir_TestDir = UCodeLang_UCAppDir + "tests/";
inline const UCodeLang::String UCodeLang_UCAppDir_Test_UCodeFiles = UCodeLang_UCAppDir_TestDir + "UCodeFiles/Files/";
inline const UCodeLang::String UCodeLang_UCAppDir_Test_OutputFiles = UCodeLang_UCAppDir_TestDir + "UCodeFiles/Output/";
#endif // DEBUG

struct AppClientFuncMap
{

    using RequestFunc = void(AppObject::*)(LS::integer  requestid, const LS::json& Params);
    using NotificationFunc = void(AppObject::*)(const LS::json& Params);
    inline static const std::unordered_map<String, RequestFunc> RequestFuncs
    {
        
    };
    inline static const std::unordered_map<String, NotificationFunc> NotificationFuncs
    {
        {"textDocument/publishDiagnostics",&AppObject::OnPublishDiagnostics},
    };
};

void EndDockSpace()
{
    ImGui::End();//EndDockSpace
}
void AppObject::OpenOnWeb(const String& WebLink)
{
    #if UCodeLang_Platform_Windows
    ShellExecuteA(0, 0, WebLink.c_str(), 0, 0, SW_SHOW);
    #elif UCodeLang_Platform_Posix
    auto cmd =String("open ") + WebLink;
    system(cmd.c_str());
    #else //MacOS

    #endif 
}
void AppObject::DrawTestMenu()
{
#if UCodeLangDebug   

    static constexpr size_t TestCount = ULangTest::Tests.size();

    using TestMode = ULangTest::TestMode;
    struct TestInfo
    {
       TestMode Testmode = TestMode::UCodeLangBackEnd;
       size_t MinTestIndex = 75;
       size_t MaxTestCount = 85;//;//ULangTest::Tests.size();
       //size_t MinTestIndex = 0;
       //size_t MaxTestCount = 15;//;//ULangTest::Tests.size();


        size_t ModuleIndex = 0;
        size_t ModuleTestCount = 1;//;//ULangTest::Tests.size();
        bool TestAsRan = false;

        size_t StandardLibraryTestIndex = 16;
        size_t StandardLibraryTestCount = 20;//;//ULangTest::Tests.size();


        enum class TestState
        {
            Null,
            Passed,
            Fail,
            Exception
        };
        using SuccessCondition = ULangTest::SuccessCondition;


        struct TestData
        {
            TestState State = TestState::Null;

            String Logs;

            std::unique_ptr<Byte[]> RetValue;
            String OutputBytesToString(Byte* Bytes, size_t Size)
            {
                std::stringstream stream;
                for (size_t i = 0; i < Size; i++)
                {
                    if (i == 0)
                    {
                        //stream << "0x";
                    }
                    stream << std::to_string(Bytes[i]);
                    if (i + 1 != Size)
                    {
                        stream << ",";
                    }
                }
                stream << '\0';
                return stream.str();
            }
            bool LogErrors(std::ostream& out, UCodeLang::Compiler& _Compiler)
            {
                out << "[\n";
                auto& Errors = _Compiler.Get_Errors().Get_Errors();
                for (auto& Item : Errors)
                {
                    out << Item.ToString() << std::endl;
                }
                out << "]\n";
                return _Compiler.Get_Errors().Has_Errors();
            }
            bool RunTestForFlag(const ULangTest::TestInfo& Test, UCodeLang::OptimizationFlags flag,TestMode Testmod)
            {

                Logs.clear();

                using namespace UCodeLang;
                using namespace ULangTest;

                Compiler::CompilerPathData paths;


                Compiler Com;
                if (Testmod == TestMode::CLang89BackEnd)
                {
                    Com.Set_BackEnd(C11Backend::MakeObject);
                }
                else if (Testmod == TestMode::WasmBackEnd)
                {
                    Com.Set_BackEnd(WasmBackEnd::MakeObject);
                }

                Com.Get_Settings()._Flags = flag;
                Com.Get_Settings().PtrSize = IntSizes::Native;

                Compiler::CompilerRet Com_r = NeverNullptr(&Com.Get_Errors());
                std::string InputFilesPath = UCodeLang_UCAppDir_Test_UCodeFiles + Test.InputFilesOrDir;
                std::string OutFileDir = UCodeLang_UCAppDir_Test_OutputFiles + Test.TestName;
                std::filesystem::path p = OutFileDir;
                OutFileDir = p.parent_path().generic_string() + "/" + +Test.TestName + "/";

                std::filesystem::create_directories(OutFileDir);
                std::string OutFilePath = OutFileDir + Test.TestName + ULangTest::ModeType(flag) + ".ulibtest" + Com.GetOutputExtWithDot();



                paths.FileDir = InputFilesPath;
                paths.OutFile = OutFilePath;



                try
                {
                    if (std::filesystem::is_directory(paths.FileDir))
                    {
                        Com_r = Com.CompileFiles(paths);
                    }
                    else
                    {
                        Com_r = Com.CompilePathToObj(paths.FileDir, paths.OutFile);
                    }

                }
                catch (const std::exception& ex)
                {
                    State = TestState::Exception;
                    Logs += "fail from Compile [exception] '" + (String)ex.what() + "' : " + "'" + Test.TestName + "'" + '\n';
                    //ErrStream << "fail from Compile [exception] '" << ex.what() << "' : " << "'" << Test.TestName << "'" << std::endl;
                    return false;
                }


                if (Test.Condition == SuccessCondition::Compilation
                    || Test.Condition == SuccessCondition::CompilationFail)
                {
                    if (
                        (Com_r.IsValue() && Test.Condition == SuccessCondition::Compilation)
                        ||
                        (Com_r.IsError() && Test.Condition == SuccessCondition::CompilationFail)
                        )
                    {
                        Logs += "Success from test '" + (String)Test.TestName + ModeType(flag) + "'" + '\n';
                        State = TestState::Passed;
                        return true;
                    }
                    else
                    {
                        Logs += "fail from test '" + (String)Test.TestName + "'" + '\n';

                        State = TestState::Fail;

                        std::stringstream errs;
                        LogErrors(errs, Com);
                        Logs += errs.str();
                        return false;
                    }

                }

                if (Com_r.IsError())
                {
                    Logs += (String)"fail from test [Cant Compile File/Files] '" + String(Test.TestName) + ModeType(flag) + "'" + '\n';

                    std::stringstream errs;
                    LogErrors(errs, Com);
                    Logs += errs.str();
                    State = TestState::Fail;
                    return false;
                }

                if (Testmod == TestMode::UCodeLangBackEnd)
                {
                    UClib lib;
                    if (!UClib::FromFile(&lib, OutFilePath))
                    {
                        State = TestState::Fail;

                        Logs += (String)"fail from test [Cant Open ULib File] '" + Test.TestName + ModeType(flag) + "'" + '\n';
                        return false;
                    }

                    RunTimeLangState state;
                    RunTimeLib rLib;
                    rLib.Init(&lib);
                    state.AddLib(&rLib);
                    state.LinkLibs();
                    {
                        Interpreter RunTime;
                        RunTime.Init(&state);

                        Interpreter::Return_t r;
                        try
                        {
                            RunTime.Call(StaticVariablesInitializeFunc);
                            RunTime.Call(ThreadVariablesInitializeFunc);

                            r = RunTime.Call(Test.FuncToCall);

                            RunTime.Call(ThreadVariablesUnLoadFunc);
                            RunTime.Call(StaticVariablesUnLoadFunc);
                        }
                        catch (const std::exception& ex)
                        {
                            State = TestState::Exception;

                            Logs += (String)"fail from test [exception] '" + ex.what() + "' : " + "'" + Test.TestName + "'" + ModeType(flag) + '\n';
                            return false;
                        }

                        if (Test.Condition == SuccessCondition::RunTimeValue)
                        {
                            RetValue = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
                            RunTime.Get_Return(RetValue.get(), Test.RunTimeSuccessSize);

                            String Type = "Interpreter";

                            bool IsSame = true;
                            for (size_t i = 0; i < Test.RunTimeSuccessSize; i++)
                            {
                                if (RetValue[i] != Test.RunTimeSuccess[i])
                                {
                                    IsSame = false;
                                    break;
                                }
                            }
                            if (IsSame)
                            {
                                Logs += "Success from test '" + (String)Test.TestName + "'" + ModeType(flag) + " " + Type + '\n';
                            }
                            else
                            {
                                State = TestState::Fail;

                                Logs += "fail from got value '";
                                Logs += OutputBytesToString(RetValue.get(), Test.RunTimeSuccessSize);

                                Logs += "' but expecting '";
                                Logs += OutputBytesToString(Test.RunTimeSuccess.get(), Test.RunTimeSuccessSize);
                                Logs += ": '" + Type + "," + ModeType(flag) + "'" + Type + '\n';
                                return false;
                            }
                        }
                        RunTime.UnLoad();
                    }
                    State = TestState::Passed;
                    return true;
                }
                else if (Testmod == TestMode::CLang89BackEnd)
                {
                    UClib& ulib = *Com_r.GetValue().OutPut;
                    auto ufunc = ulib.Get_Assembly().Find_Func(Test.FuncToCall);
                    UCodeLangAssert(ufunc);
                    {
                        // String filetxt = Compiler::GetTextFromFile(OutFilePath);

                        Path  dllfile = OutFilePath + ".lib";
                        Path Testablefile = OutFilePath;
                        UCodeLangAssert(CompileC89ToLib(Testablefile, dllfile));
                        

                        auto& Assembly = ulib.Get_Assembly();
                        auto cfuncname = C11Backend::UpdateToCindentifier(ufunc->DecorationName);
                        
                        auto staticinitname = C11Backend::UpdateToCindentifier(StaticVariablesInitializeFunc);
                        auto threadinitname = C11Backend::UpdateToCindentifier(ThreadVariablesInitializeFunc);

                        auto staticdeinitname = C11Backend::UpdateToCindentifier(StaticVariablesUnLoadFunc);
                        auto threaddeinitname = C11Backend::UpdateToCindentifier(ThreadVariablesUnLoadFunc);

                        #if UCodeLang_Platform_Windows
                        auto lib = LoadLibrary(dllfile.c_str());
                        UCodeLangAssert(lib);
                        UCodeLangDefer(FreeLibrary(lib));
                        auto staticinittocall = GetProcAddress(lib,staticinitname.c_str());
                        auto threadinittocall = GetProcAddress(lib,threadinitname.c_str());
                        auto staticdeinittocall = GetProcAddress(lib,staticdeinitname.c_str());
                        auto threaddeinittocall = GetProcAddress(lib,threaddeinitname.c_str());

                        auto functocall = GetProcAddress(lib, cfuncname.c_str());
                        #elif UCodeLang_Platform_Posix
                        auto lib = dlopen(dllfile.c_str(), RTLD_NOW);
                        UCodeLangAssert(lib);
                        UCodeLangDefer(dlclose(lib));

                        auto staticinittocall = dlsym(lib,staticinitname.c_str());
                        auto threadinittocall = dlsym(lib,threadinitname.c_str());
                        auto staticdeinittocall = dlsym(lib,staticdeinitname.c_str());
                        auto threaddeinittocall = dlsym(lib,threaddeinitname.c_str());
                        
                        auto functocall = dlsym(lib,cfuncname.c_str());
                        #endif       
                        
                        UCodeLangAssert(functocall);

                        bool hasautocall = cfuncname == "main";
                        if (!hasautocall)
                        {
                            using Func = void(*)();
                            ((Func)staticinittocall)();
                            ((Func)threadinittocall)();
                        }

                        auto rettype = ufunc->RetType;
                        if (auto val = Assembly.Find_Node(rettype))
                        {
                            if (val->Get_Type() == ClassType::Enum)
                            {
                                auto& Enum = val->Get_EnumData();
                                rettype = Enum.BaseType;
                            }
                        }

                        RetValue = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
                        {
                            if (rettype._Type == ReflectionTypes::Bool)
                            {
                                using GetValueFunc = bool(*)();
                                auto val = ((GetValueFunc)functocall)();

                                UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
                                memcpy(RetValue.get(), &val, sizeof(val));
                            }
                            else if (rettype._Type == ReflectionTypes::sInt8
                                || rettype._Type == ReflectionTypes::uInt8
                                || rettype._Type == ReflectionTypes::Char)
                            {
                                using GetValueFunc = UInt8(*)();
                                auto val = ((GetValueFunc)functocall)();

                                UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
                                memcpy(RetValue.get(), &val, sizeof(val));
                            }
                            else  if (rettype._Type == ReflectionTypes::uInt16
                                || rettype._Type == ReflectionTypes::sInt16)
                            {
                                using GetValueFunc = Int16(*)();
                                auto val = ((GetValueFunc)functocall)();

                                UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
                                memcpy(RetValue.get(), &val, sizeof(val));
                            }
                            else  if (rettype._Type == ReflectionTypes::uInt32
                                || rettype._Type == ReflectionTypes::sInt32)
                            {
                                using GetValueFunc =Int32(*)();
                                auto val = ((GetValueFunc)functocall)();

                                UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
                                memcpy(RetValue.get(), &val, sizeof(val));
                            }
                            else  if (rettype._Type == ReflectionTypes::float32)
                            {
                                using GetValueFunc = float32(*)();
                                auto val = ((GetValueFunc)functocall)();

                                UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
                                memcpy(RetValue.get(), &val, sizeof(val));
                            }
                            else  if (rettype._Type == ReflectionTypes::float64)
                            {
                                using GetValueFunc = float64(*)();
                                auto val = ((GetValueFunc)functocall)();

                                UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
                                memcpy(RetValue.get(), &val, sizeof(val));
                            }
                            else  if (rettype._Type == ReflectionTypes::uIntPtr
                                || rettype._Type == ReflectionTypes::sIntPtr)
                            {
                                using GetValueFunc = uintptr_t(*)();
                                auto val = ((GetValueFunc)functocall)();

                                UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
                                memcpy(RetValue.get(), &val, sizeof(val));
                            }
                            else if (auto typenod = Assembly.Find_Node(rettype))
                            {
                                if (StringHelper::StartWith(typenod->FullName, "Vec2")
                                    || StringHelper::StartWith(typenod->FullName, "vec2"))
                                {
                                   // using GetValueFunc = Vec2(*)();//I have no idea why this does not work
                                   // auto val = ((GetValueFunc)functocall)();



                                    using GetValueFunc2 = Int64(*)();
                                    auto val2 = ((GetValueFunc2)functocall)();

                                    Vec2& val = *(Vec2*)&val2;

                                    UCodeLangAssert(Test.RunTimeSuccessSize == sizeof(val));
                                    memcpy(RetValue.get(), &val, sizeof(val));
                                }
                                else
                                {
                                    UCodeLangUnreachable();
                                }
                            }
                            else
                            {
                                UCodeLangUnreachable();
                            }
                        }

                        if (!hasautocall)
                        {
                            using Func = void(*)();
                            ((Func)staticdeinittocall)();
                            ((Func)threaddeinittocall)();
                        }

                        String Type = "NativeC";

                        bool IsSame = true;
                        for (size_t i = 0; i < Test.RunTimeSuccessSize; i++)
                        {
                            if (RetValue[i] != Test.RunTimeSuccess[i])
                            {
                                IsSame = false;
                                break;
                            }
                        }
                        if (IsSame)
                        {
                            Logs += "Success from test '" + (String)Test.TestName + "'" + ModeType(flag) + " " + Type + '\n';
                            State = TestState::Passed;
                            return true;
                        }
                        else
                        {
                            State = TestState::Fail;

                            Logs += "fail from got value '";
                            Logs += OutputBytesToString(RetValue.get(), Test.RunTimeSuccessSize);

                            Logs += "' but expecting '";
                            Logs += OutputBytesToString(Test.RunTimeSuccess.get(), Test.RunTimeSuccessSize);
                            Logs += ": '" + Type + "," + ModeType(flag) + "'" + Type + '\n';
                            return false;
                        }
                    }
                    return true;
                }
                else if (Testmod == TestMode::WasmBackEnd)
                {
                    UClib& ulib = *Com_r.GetValue().OutPut;
                    auto& outfile = Com_r.GetValue().OutFile.value();

                    auto ufunc = ulib.Get_Assembly().Find_Func(Test.FuncToCall);
                    UCodeLangAssert(ufunc);

                    String JsString = "const wasm = new Uint8Array([";

                    std::stringstream ss;
                    ss << "const wasm = new Uint8Array([";
                    for (const auto& b : outfile) {
                        ss << "0x" << std::hex << static_cast<int>(b) << ", ";
                    }
                    ss << "]);\n";
                    ss << "const m = new WebAssembly.Module(wasm);\n";
                    ss << "const instance = new WebAssembly.Instance(m, {});\n";
                    ss << "console.log(instance.exports.";
                    ss << WasmBackEnd::ToWebName(ufunc->DecorationName);
                    ss << "());";

                    Path node_file = paths.OutFile.native() + Path("test.js").native();
                    Path out_file = paths.OutFile.native() + Path("test.js.out").native();


                    std::ofstream nf(node_file);
                    nf << ss.str();
                    nf << std::flush;

                    String expected;
                    auto rettype = ufunc->RetType;

                    if (rettype._Type == ReflectionTypes::sInt32)
                    {
                        expected += std::to_string(*(int*)Test.RunTimeSuccess.get());
                    }
                    else if (rettype._Type == ReflectionTypes::sInt16)
                    {
                        expected += std::to_string(*(Int16*)Test.RunTimeSuccess.get());
                    }
                    else if (rettype._Type == ReflectionTypes::sInt8)
                    {
                        expected += std::to_string(*(Int8*)Test.RunTimeSuccess.get());
                    }
                    else if (rettype._Type == ReflectionTypes::uInt16)
                    {
                        expected += std::to_string(*(UInt16*)Test.RunTimeSuccess.get());
                    }
                    else if (rettype._Type == ReflectionTypes::uInt8)
                    {
                        expected += std::to_string(*(UInt8*)Test.RunTimeSuccess.get());
                    }
                    else if (rettype._Type == ReflectionTypes::Char)
                    {
                        expected += std::to_string(*(Int8*)Test.RunTimeSuccess.get());
                    }
                    else if (rettype._Type == ReflectionTypes::float32)
                    {
                        expected += std::to_string(*(float32*)Test.RunTimeSuccess.get());
                    }
                    else if (rettype._Type == ReflectionTypes::float64)
                    {
                        expected += std::to_string(*(float64*)Test.RunTimeSuccess.get());
                    }
                    else
                    {
                        UCodeLangUnreachable();
                    }


                    expected += '\n';

                    {
                        std::system(("node " + node_file.generic_string() + " > " + out_file.generic_string()).c_str());
                    }

                    std::stringstream ss_out;
                    ss_out << std::ifstream(out_file).rdbuf();
                    auto outstr = ss_out.str();

                    {
                        //C++ adds trailing zeros but node.js does not
                        if (rettype._Type == ReflectionTypes::float32)
                        {
                            float newfloat = std::stof(outstr.substr(0, outstr.size() - 1));//-1 to remove /n
                            outstr = std::to_string(newfloat);
                            outstr += '\n';
                        }
                        else if (rettype._Type == ReflectionTypes::float64)
                        {
                            float64 newfloat = std::stof(outstr.substr(0, outstr.size() - 1));//-1 to remove /n
                            outstr = std::to_string(newfloat);
                            outstr += '\n';
                        }

                    }

                    if (outstr != expected) {
                        std::cerr << "got: " << ss_out.str();
                        std::cerr << "expected: " << expected;
                        State = TestState::Fail;
                        return false;
                    }
                    State = TestState::Passed;
                    return true;
                }
                else
                {
                    UCodeLangUnreachable();
                }
            }
        };

        UCodeLang::Array<TestData, TestCount> Testinfo;
        UCodeLang::Array<UCodeLang::Unique_ptr<std::future<bool>>, TestCount> Threads;

        TestState SortBy = TestState::Null;
        String SortByName;
        bool IncludeOptimization = false;

        bool IncludeJitInterpreter = false;

        bool IncludeNativeInterpreter = false;

        static String GetToString(TestState Value)
        {
            switch (Value)
            {
            case TestInfo::TestState::Null:return "null";
            case TestInfo::TestState::Passed:return "passed";
            case TestInfo::TestState::Fail:return "fail";
            case TestInfo::TestState::Exception:return "exception";
            default:
                UCodeLangUnreachable();
                break;
            }
        }
        static String GetToString(SuccessCondition Value)
        {
            switch (Value)
            {
            case SuccessCondition::Compilation:return "Compilation";
            case SuccessCondition::CompilationFail:return "CompilationFail";
            case SuccessCondition::RunTimeValue:return "RunTimeValue";
            default:
                UCodeLangUnreachable();
                break;
            }
        }
        bool DebugMode = true;
        UCodeLang::OptimizationFlags Flags;


        struct ModuleTestData
        {
            TestState State = TestState::Null;

            String Logs;
        };

        UCodeLang::Array<ModuleTestData, ULangTest::ModuleTests.size()> ModuleTestinfo;
        UCodeLang::Array<UCodeLang::Unique_ptr<std::future<bool>>, ULangTest::ModuleTests.size()> ModuleThreads;
    };
    static TestInfo TestWindowData;
    bool SetTextFocus = false;

   

    if (ImGui::Begin("Tests"))
    {
        static const Vector<ImguiHelper::EnumValue<TestMode>> TestModeList =
        {
            { "UCodeLang",TestMode::UCodeLangBackEnd},
            { "C89",TestMode::CLang89BackEnd},
            { "Wasm",TestMode::WasmBackEnd},
        };


        const  ImVec4 ColorRed = { 0.839, 0.129, 0.051,1 };
        const  ImVec4 ColorGreen = { 0.267, 0.788, 0.2,1 };
        const  ImVec4 ColorGray = { 0.431, 0.427, 0.365,1 };
        const  ImVec4 Colorlightgrey = { 0.675, 0.702, 0.675,1 };
        const  ImVec4 ColorOrange = { 0.929, 0.329, 0.09,1 };
        const  ImVec4 ColorBlue = { 0.396, 0.722, 0.769,1 };
        const  ImVec4 ColorYellow = {1, 1, 0,1 };
        {

            {
                int v = (int)TestWindowData.MinTestIndex;
                if (ImGui::SliderInt("MinShowTests", &v, 0, TestWindowData.MaxTestCount - 1))
                {
                    TestWindowData.MinTestIndex = v;
                }
            }
            {
                int v = (int)TestWindowData.MaxTestCount;
                if (ImGui::SliderInt("MaxShowTests", &v, TestWindowData.MinTestIndex + 1, ULangTest::Tests.size()))
                {
                    TestWindowData.MaxTestCount = v;
                }
            }
            UCodeLang::OptimizationFlags flags = TestWindowData.Flags;
            bool FlagsWasUpdated = false;

            if (ImguiHelper::EnumField("BackEnd", TestWindowData.Testmode, TestModeList))
            {
                FlagsWasUpdated = true;
            }

            if (TestWindowData.DebugMode)
            {
                *(UCodeLang::OptimizationFlags_t*)&flags |= (UCodeLang::OptimizationFlags_t)UCodeLang::OptimizationFlags::Debug;
            }
            {
                if (ImGui::Button("Run Tests And skip"))
                {
                    TestWindowData.MinTestIndex = 0;
                    TestWindowData.MaxTestCount = ULangTest::Tests.size();

                    TestWindowData.TestAsRan = true;
                    const auto& Tests = ULangTest::Tests;
                    for (size_t i = 0; i < TestWindowData.MaxTestCount; i++)
                    {
                        auto& ItemTest = ULangTest::Tests[i];
                        auto& ItemTestOut = TestWindowData.Testinfo[i];
                        auto& Thread = TestWindowData.Threads[i];

                        if (!ULangTest::ShouldSkipTests(i, TestWindowData.Testmode)) 
                        {
                            Thread = std::make_unique< std::future<bool>>(std::async(std::launch::async, [i, flags, testmod = TestWindowData.Testmode]
                                {
                                    auto& ItemTest = ULangTest::Tests[i];
                                    auto& ItemTestOut = TestWindowData.Testinfo[i];

                                    ItemTestOut.State = TestInfo::TestState::Exception;
                                    ItemTestOut.RunTestForFlag(ItemTest, flags, testmod);
                                    return false;
                                }));
                        }
                    }
                }
            }
           


            thread_local UCodeLang::UnorderedMap<String, String> Openedfiles;
            thread_local UCodeLang::UnorderedMap<String, UCodeLang::Optional<std::shared_ptr<UCodeLang::UClib>>> Outputfiles;
            thread_local UCodeLang::UnorderedMap<String, String> OutputIRStr;
            thread_local UCodeLang::UnorderedMap<String, String> OutputLibStr;
            if (ImGui::Button("Run Tests"))
            {
                Openedfiles.clear();
                Outputfiles.clear();
                OutputIRStr.clear();
                OutputLibStr.clear();

                TestWindowData.TestAsRan = true;
                const auto& Tests = ULangTest::Tests;
                for (size_t i = 0; i < TestWindowData.MaxTestCount; i++)
                {
                    auto& ItemTest = ULangTest::Tests[i];
                    auto& ItemTestOut = TestWindowData.Testinfo[i];
                    auto& Thread = TestWindowData.Threads[i];


                    Thread = std::make_unique< std::future<bool>>(std::async(std::launch::async, [i, flags,testmod = TestWindowData.Testmode]
                        {
                            auto& ItemTest = ULangTest::Tests[i];
                            auto& ItemTestOut = TestWindowData.Testinfo[i];

                            ItemTestOut.State = TestInfo::TestState::Exception;
                            ItemTestOut.RunTestForFlag(ItemTest, flags,testmod);
                            return false;
                        }));
                }
            }
           
            if (ImguiHelper::BoolEnumField("DebugFlag", TestWindowData.DebugMode))
            {

                FlagsWasUpdated = true;
            }

            static const Vector<ImguiHelper::EnumValue<UCodeLang::OptimizationFlags>> OptimizationList =
            {
                { "NoOptimization",UCodeLang::OptimizationFlags::NoOptimization },
                { "ForSize",UCodeLang::OptimizationFlags::ForSize },
                { "ForSpeed",UCodeLang::OptimizationFlags::ForSpeed },
                { "SizeAndSpeed",UCodeLang::OptimizationFlags::ForMaxSpeed },
            };

            ImGui::SameLine();
            if (ImguiHelper::EnumField("Optimization", TestWindowData.Flags, OptimizationList))
            {
                FlagsWasUpdated = true;
            }


            if (FlagsWasUpdated) {
                Openedfiles.clear();
                Outputfiles.clear();
                OutputIRStr.clear();
                OutputLibStr.clear();
            }
            //ImguiHelper::BoolEnumField("Include O1,02,03 Optimizations", TestWindowData.IncludeOptimization);
            ImguiHelper::BoolEnumField("Include JitInterpreter", TestWindowData.IncludeJitInterpreter);
            ImguiHelper::BoolEnumField("Include NativeInterpreter", TestWindowData.IncludeNativeInterpreter);

            size_t TestPassedCount = 0;
            size_t TestRuningCount = 0;
            for (size_t i = TestWindowData.MinTestIndex; i < TestWindowData.MaxTestCount; i++)
            {
                auto& ItemTest = ULangTest::Tests[i];
                auto& ItemTestOut = TestWindowData.Testinfo[i];
                auto& Thread = TestWindowData.Threads[i];

                if (ItemTestOut.State == TestInfo::TestState::Passed)
                {
                    TestPassedCount++;
                }
                if (Thread.get() && Thread->valid())
                {
                    TestRuningCount++;
                }
            }


            {
                String info;
                info += "TestPassed:";
                info += std::to_string(TestPassedCount);
                info += "/";
                info += std::to_string(TestWindowData.MaxTestCount - TestWindowData.MinTestIndex);
                info += " :" + std::to_string((int)(((float)TestPassedCount / (float)(TestWindowData.MaxTestCount - TestWindowData.MinTestIndex)) * 100));
                info += " percent";
                ImGui::Text(info.c_str());
            }
            {
                String info;
                info += "TestRuning:";
                info += std::to_string(TestRuningCount);
                info += "/";
                info += std::to_string(TestWindowData.MaxTestCount - TestWindowData.MinTestIndex);
                info += " :" + std::to_string((int)(((float)TestRuningCount / (float)(TestWindowData.MaxTestCount - TestWindowData.MinTestIndex) * 100)));
                info += " percent";
                ImGui::Text(info.c_str());
            }
            static const Vector<ImguiHelper::EnumValue<TestInfo::TestState>> List =
            {
                { "Null ",TestInfo::TestState::Null },
                { "Passed",TestInfo::TestState::Passed },
                { "Fail",TestInfo::TestState::Fail },
                { "Exception",TestInfo::TestState::Exception },
            };

            ImguiHelper::InputText("Sort:", TestWindowData.SortByName);
            ImGui::SameLine();
            ImguiHelper::EnumField("Type:", TestWindowData.SortBy, List);

            ImGui::Separator();

            for (size_t i = TestWindowData.MinTestIndex; i < TestWindowData.MaxTestCount; i++)
            {
                auto& ItemTest = ULangTest::Tests[i];
                auto& ItemTestOut = TestWindowData.Testinfo[i];
                auto& Thread = TestWindowData.Threads[i];

                bool CanBeShowed = false;
                if (TestWindowData.SortBy == TestInfo::TestState::Null)
                {
                    CanBeShowed = true;
                }
                else
                {
                    CanBeShowed = TestWindowData.SortBy == ItemTestOut.State;
                }

                if (CanBeShowed)
                {
                    ImVec4 buttioncolor;


                    bool isworking = false;
                    if (Thread.get())
                    {
                        if (Thread->valid())
                        {
                            if (Thread->wait_for(std::chrono::seconds(0)) != std::future_status::ready)
                            {
                                isworking = true;
                            }
                        }
                    }

                    if (isworking)
                    {
                        buttioncolor = ColorYellow;//gray
                    }
                    else
                    {
                        if (ItemTestOut.State == TestInfo::TestState::Passed)
                        {
                            buttioncolor = ColorGreen;//green
                        }
                        else if (ItemTestOut.State == TestInfo::TestState::Null)
                        {
                            buttioncolor = Colorlightgrey;
                        }
                        else if (ItemTestOut.State == TestInfo::TestState::Fail)
                        {
                            buttioncolor = ColorOrange;
                        }
                        else
                        {
                            buttioncolor = ColorRed;//red
                        }
                    }

                    String TestV = "Test:";
                    TestV += ItemTest.TestName;
                    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, buttioncolor);
                    bool isopen = ImGui::TreeNode(TestV.c_str());
                    ImGui::PopStyleColor();

                    if (isopen)
                    {
                        bool IsWorking = isworking;

                        {
                            String txt = "State:" + TestInfo::GetToString(ItemTestOut.State);
                            ImGui::Text(txt.c_str());
                        }
                        ImGui::BeginDisabled();

                        String tepstr1 = ItemTest.TestName;
                        ImguiHelper::InputText("TestName", tepstr1);

                        String tepstr2 = ItemTest.InputFilesOrDir;
                        ImguiHelper::InputText("TestPath", tepstr2);

                        String tepstr3 = TestInfo::GetToString(ItemTest.Condition);
                        ImguiHelper::InputText("Success Condition", tepstr3);

                        if (ItemTest.Condition == ULangTest::SuccessCondition::RunTimeValue)
                        {
                            auto valsize = ItemTest.RunTimeSuccessSize;
                            auto val = ItemTest.RunTimeSuccess.get();

                            if (Outputfiles.HasValue(ItemTest.TestName))
                            {

                                if (Outputfiles.GetValue(ItemTest.TestName).has_value())
                                {
                                    const auto& lib = Outputfiles.GetValue(ItemTest.TestName).value();
                                    UCodeLang::ReflectionTypeInfo valtype = lib->Get_Assembly().Find_Func(ItemTest.FuncToCall)->RetType;

                                    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ColorBlue);
                                    ImguiHelper::UCodeObjectField("Expected", val, valtype, lib->Get_Assembly());
                                    ImGui::PopStyleColor();

                                    if (ItemTestOut.RetValue.get())
                                    {

                                        ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text,
                                            ItemTestOut.State == TestInfo::TestState::Passed ? ColorGreen : ColorRed);

                                        ImguiHelper::UCodeObjectField("Returned", ItemTestOut.RetValue.get(), valtype, lib->Get_Assembly());
                                        ImGui::PopStyleColor();


                                    }
                                }
                            }
                        }
                        ImGui::EndDisabled();

                        if (ImGui::TreeNode("Logs")) {
                            ImGui::BeginDisabled();
                            ImGui::InputTextMultiline("Logs", &ItemTestOut.Logs);
                            ImGui::EndDisabled();
                            ImGui::TreePop();
                        }
                        //ImGui::SameLine();



                        if (ImGui::Button("Show in files"))
                        {
                            ShowInFiles(UCodeLang_UCAppDir_Test_UCodeFiles + ItemTest.InputFilesOrDir);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Show Output in files"))
                        {
                            ShowInFiles(UCodeLang_UCAppDir_Test_OutputFiles + ItemTest.TestName);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Set file.uc"))
                        {
                            if (Openedfiles.HasValue(ItemTest.TestName))
                            {
                                _Editor.SetText(Openedfiles.GetValue(ItemTest.TestName));
                                SetTextFocus = true;
                            }
                        }
                        ImGui::SameLine();


                        ImGui::BeginDisabled(IsWorking);
                        if (ImGui::Button("Run Test"))
                        {
                            if (!IsWorking)
                            {

                                auto& ItemTest = ULangTest::Tests[i];
                                auto& ItemTestOut = TestWindowData.Testinfo[i];
                                auto& Thread = TestWindowData.Threads[i];


                                Thread = std::make_unique< std::future<bool>>(std::async(std::launch::async, [i, flags,testmod =TestWindowData.Testmode]
                                    {
                                        auto& ItemTest = ULangTest::Tests[i];
                                        auto& ItemTestOut = TestWindowData.Testinfo[i];

                                        ItemTestOut.State = TestInfo::TestState::Exception;
                                        auto V = ItemTestOut.RunTestForFlag(ItemTest, flags,testmod);
                                        return V;
                                    }));
                            }
                        }
                        ImGui::EndDisabled();

                        ImGui::BeginDisabled();

                        ImGui::PushID(&ItemTest.TestName);
                        {


                            String filetxt = "file...";
                            if (Openedfiles.HasValue(ItemTest.TestName))
                            {
                                filetxt = Openedfiles.GetValue(ItemTest.TestName);
                            }
                            else
                            {
                                filetxt = UCodeLang::Compiler::GetTextFromFile(UCodeLang_UCAppDir_Test_UCodeFiles + ItemTest.InputFilesOrDir);
                                Openedfiles.AddValue(ItemTest.TestName, filetxt);
                            }


                            ImGui::InputTextMultiline("src", &filetxt);
                        }
                        ImGui::PopID();

                        std::string OutFileDir = UCodeLang_UCAppDir_Test_OutputFiles + ItemTest.TestName;
                        std::string OutFilePath = OutFileDir + "/" + ItemTest.TestName + ULangTest::ModeType(flags) + ".ulibtest";


                        if (!Outputfiles.HasValue(ItemTest.TestName))
                        {
                            UCodeLang::UClib lib;
                            if (UCodeLang::UClib::FromFile(&lib, OutFilePath))
                            {
                                Outputfiles.AddValue(ItemTest.TestName, std::make_shared<UCodeLang::UClib>(std::move(lib)));
                            }
                            else
                            {
                                Outputfiles.AddValue(ItemTest.TestName, {});
                            }

                        }

                        ImGui::PushID(&ItemTest.RunTimeSuccess);
                        {
                            String filetxt = "file...";
                            if (Outputfiles.HasValue(ItemTest.TestName))
                            {
                                auto& v = Outputfiles.GetValue(ItemTest.TestName);
                                if (!OutputIRStr.HasValue(ItemTest.TestName))
                                {
                                    String str;
                                    if (v.has_value())
                                    {
                                        auto IRLayer = v.value()->GetLayer(UCode_CodeLayer_IR_Name);
                                        if (IRLayer && IRLayer->_Data.Is<UCodeLang::CodeLayer::JustData>())
                                        {
                                            auto& Data = IRLayer->_Data.Get<UCodeLang::CodeLayer::JustData>();
                                            UCodeLang::IRBuilder tep;
                                            if (UCodeLang::IRBuilder::FromBytes(tep, BytesView::Make(Data._Data.data(), Data._Data.size())))
                                            {
                                                str = tep.ToString();
                                            }
                                        }

                                    }
                                    OutputIRStr.AddValue(ItemTest.TestName, str);
                                }
                                else
                                {
                                    filetxt = OutputIRStr.GetValue(ItemTest.TestName);
                                }
                            }
                            ImGui::InputTextMultiline("ir", &filetxt);
                        }
                        ImGui::PopID();

                        ImGui::PushID(&ItemTest.FuncToCall);
                        {
                            String filetxt = "out...";
                            if (Outputfiles.HasValue(ItemTest.TestName))
                            {
                                if (!OutputLibStr.HasValue(ItemTest.TestName))
                                {

                                    auto& v = Outputfiles.GetValue(ItemTest.TestName);
                                    if (v.has_value())
                                    {
                                        filetxt = UCodeLang::UAssembly::UAssembly::ToString(v.value().get());
                                    }
                                    OutputLibStr.AddValue(ItemTest.TestName, filetxt);
                                }
                                else
                                {
                                    filetxt = OutputLibStr.GetValue(ItemTest.TestName);
                                }
                            }
                            ImGui::InputTextMultiline("out", &filetxt);
                        }
                        ImGui::PopID();

                        if (IsWorking)
                        {
                            ImGui::Text("Working");
                        }
                        else
                        {

                        }
                        ImGui::EndDisabled();

                        if (!IsWorking)
                        {
                            if (ImGui::Button("Show in files"))
                            {
                                ShowInFiles(UCodeLang_UCAppDir_Test_UCodeFiles + ItemTest.InputFilesOrDir);
                            }
                            ImGui::SameLine();
                        }

                        ImGui::TreePop();
                    }
                }

            }

        }

        {
            ImGui::Separator();
            ImGui::Text("ModuleTests");
            {
                int v = (int)TestWindowData.ModuleIndex;
                if (ImGui::SliderInt("MinShowTests", &v, 0, TestWindowData.ModuleTestCount - 1))
                {
                    TestWindowData.ModuleIndex = v;
                }
            }
            {
                int v = (int)TestWindowData.ModuleTestCount;
                if (ImGui::SliderInt("MaxShowTests", &v, TestWindowData.ModuleIndex + 1, ULangTest::ModuleTests.size() - 1))
                {
                    TestWindowData.ModuleTestCount = v;
                }
            }

          
            if (ImGui::Button("Run Module Tests"))
            {
                for (size_t i = TestWindowData.ModuleIndex; i < TestWindowData.ModuleTestCount; i++)
                {
                    auto& ItemTest = ULangTest::ModuleTests[i];
                    auto& ItemTestOut = TestWindowData.ModuleTestinfo[i];
                    auto& Thread = TestWindowData.ModuleThreads[i];

                    bool IsWorking = false;

                    if (Thread.get())
                    {
                        if (Thread->valid())
                        {
                            if (Thread->wait_for(std::chrono::seconds(0)) != std::future_status::ready)
                            {
                                IsWorking = true;
                            }
                        }
                    }
                    if (!IsWorking)
                    {

                        Thread = std::make_unique< std::future<bool>>(std::async(std::launch::async, [i]
                            {
                                auto& ItemTest = ULangTest::ModuleTests[i];
                                auto& ItemOut = TestWindowData.ModuleTestinfo[i];
                                UCodeLang::ModuleFile file;

                                Path pathdir = UCodeLang_UCAppDir_Test_UCodeFiles + ItemTest.Modulefile;
                                Path modulefilepath = pathdir / Path(UCodeLang::ModuleFile::FileNameWithExt);
                                bool r = false;
                                if (file.FromFile(&file, modulefilepath))
                                {
                                    UCodeLang::Compiler compiler;
                                    UCodeLang::ModuleIndex LangIndex = UCodeLang::ModuleIndex::GetModuleIndex();

                                    try
                                    {
                                        auto ret = file.BuildModule(compiler, LangIndex);
                                        if (ret.CompilerRet.IsValue())
                                        {
                                            ItemOut.State = TestInfo::TestState::Passed;
                                            r = true;
                                        }
                                        else
                                        {
                                            ItemOut.State = TestInfo::TestState::Fail;
                                        }
                                    }
                                    catch (const std::exception&)
                                    {
                                        ItemOut.State = TestInfo::TestState::Exception;
                                    }
                                   

                                }

                                return r;
                            }));
                    }
                }
            }

            for (size_t i = TestWindowData.ModuleIndex; i < TestWindowData.ModuleTestCount; i++)
            {
                auto& ItemTest = ULangTest::ModuleTests[i];
                auto& ItemTestOut = TestWindowData.ModuleTestinfo[i];
                auto& Thread = TestWindowData.ModuleThreads[i];


                ImVec4 buttioncolor;
                if (ItemTestOut.State == TestInfo::TestState::Passed)
                {
                    buttioncolor = ColorGreen;//green
                }
                else if (ItemTestOut.State == TestInfo::TestState::Null)
                {
                    buttioncolor = Colorlightgrey;
                }
                else if (ItemTestOut.State == TestInfo::TestState::Fail)
                {
                    buttioncolor = ColorOrange;
                }
                else
                {
                    buttioncolor = ColorRed;//red
                }


                String TestV = "Test:";
                TestV += ItemTest.TestName;
                ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, buttioncolor);
                bool isopen = ImGui::TreeNode(TestV.c_str());
                ImGui::PopStyleColor();

                if (isopen)
                {
                    bool IsWorking = false;

                    if (Thread.get())
                    {
                        if (!Thread->valid())
                        {
                            IsWorking = true;
                        }
                    }

                    if (ImGui::Button("Show in files"))
                    {
                        ShowInFiles(UCodeLang_UCAppDir_Test_UCodeFiles + ItemTest.Modulefile);
                    }
                    ImGui::SameLine();

                    ImGui::BeginDisabled(IsWorking);
                    if (ImGui::Button("Run Test"))
                    {
                        if (!IsWorking)
                        {

                            Thread = std::make_unique< std::future<bool>>(std::async(std::launch::async, [i]
                                {
                                    auto& ItemTest = ULangTest::ModuleTests[i];
                                    auto& ItemOut = TestWindowData.ModuleTestinfo[i];
                                    UCodeLang::ModuleFile file;

                                    Path pathdir = UCodeLang_UCAppDir_Test_UCodeFiles + ItemTest.Modulefile;
                                    Path modulefilepath = pathdir / Path(UCodeLang::ModuleFile::FileNameWithExt);
                                    bool r = false;
                                    if (file.FromFile(&file, modulefilepath))
                                    {
                                        UCodeLang::Compiler compiler;
                                        UCodeLang::ModuleIndex LangIndex = UCodeLang::ModuleIndex::GetModuleIndex();

                                        try
                                        {
                                            auto ret = file.BuildModule(compiler, LangIndex);
                                            if (ret.CompilerRet.IsValue())
                                            {

                                                ItemOut.State = TestInfo::TestState::Passed;
                                                r = true;
                                            }
                                            else
                                            {
                                                ItemOut.State = TestInfo::TestState::Fail;
                                            }
                                        }
                                        catch (const std::exception&)
                                        {
                                            ItemOut.State = TestInfo::TestState::Exception;
                                        }


                                    }

                                    return r;
                                }));
                        }
                    }
                    ImGui::EndDisabled();

                    ImGui::TreePop();
                }

            }
        }
        {
            ImGui::Separator();
            ImGui::Text("StandardLibrary Tests");

            struct StandardLibTest
            {
                bool TestRan = false;
                bool TestPassing = false;
            };
            static Vector<StandardLibTest> StandardLibraryTestInfo;
            static UCodeLang::UClib TestLib;
            static UCodeLang::Path Outpath;
            static TestMode mode= TestMode::Max;
            static Vector<const UCodeLang::ClassMethod*> StandardLibrarytests;
            static bool check = false;
            static UCodeLang::Compiler _TestCompiler;


            {
                int v = (int)TestWindowData.StandardLibraryTestIndex;
                if (ImGui::SliderInt("MinShowTests 2", &v, 0, TestWindowData.StandardLibraryTestCount - 1))
                {
                    TestWindowData.StandardLibraryTestIndex = v;
                }
            }
            {
                int v = (int)TestWindowData.StandardLibraryTestCount;
                if (ImGui::SliderInt("MaxShowTests 2", &v, TestWindowData.StandardLibraryTestIndex + 1, StandardLibrarytests.size() - 1))
                {
                    TestWindowData.StandardLibraryTestCount = v;
                }
            }


            Path StandardLibrarydir = "../UCodeAPI/StandardLibrary";

            if (mode != TestWindowData.Testmode)
            {
                check = false;
            }

            if (check == false)
            {
                check = true;
                UCodeLang::ModuleFile f;
                f.FromFile(&f, StandardLibrarydir / UCodeLang::ModuleFile::FileNameWithExt);
                if (TestWindowData.Testmode == TestMode::UCodeLangBackEnd)
                {
                    _TestCompiler.Set_BackEnd(UCodeLang::UCodeBackEndObject::MakeObject);
                }
                else if (TestWindowData.Testmode == TestMode::CLang89BackEnd)
                {
                    _TestCompiler.Set_BackEnd(UCodeLang::C11Backend::MakeObject);
                }
                else if (TestWindowData.Testmode == TestMode::WasmBackEnd)
                {
                    _TestCompiler.Set_BackEnd(UCodeLang::WasmBackEnd::MakeObject);
                }
                /*
                auto out = f.BuildModule(_TestCompiler, UCodeLang::ModuleIndex::GetModuleIndex());

                if (out.CompilerRet.IsValue()) 
                {
                    TestLib = std::move(*out.CompilerRet.GetValue().OutPut);

                    StandardLibrarytests = UCodeLang::TestRuner::GetTests(TestLib.Get_Assembly());

                    StandardLibraryTestInfo.resize(StandardLibrarytests.size());

                    Outpath = out.OutputItemPath;
                    mode = TestWindowData.Testmode;
                }
                */
            }

            if (ImGui::Button("Build StandardLibrary"))
            {
                UCodeLang::ModuleFile f;
                f.FromFile(&f, StandardLibrarydir / UCodeLang::ModuleFile::FileNameWithExt);

                if (TestWindowData.Testmode == TestMode::UCodeLangBackEnd)
                {
                    _TestCompiler.Set_BackEnd(UCodeLang::UCodeBackEndObject::MakeObject);
                }
                else if (TestWindowData.Testmode == TestMode::CLang89BackEnd)
                {
                    _TestCompiler.Set_BackEnd(UCodeLang::C11Backend::MakeObject);
                }
                else if (TestWindowData.Testmode == TestMode::WasmBackEnd)
                {
                    _TestCompiler.Set_BackEnd(UCodeLang::WasmBackEnd::MakeObject);
                }

                auto out = f.BuildModule(_TestCompiler, UCodeLang::ModuleIndex::GetModuleIndex());

                TestLib = std::move(*out.CompilerRet.GetValue().OutPut);


                StandardLibrarytests = UCodeLang::TestRuner::GetTests(TestLib.Get_Assembly());

                StandardLibraryTestInfo.resize(StandardLibrarytests.size());

                Outpath = out.OutputItemPath;

                mode = TestWindowData.Testmode;
            }
            if (ImGui::Button("Clean StandardLibrary"))
            {
                std::filesystem::remove_all(StandardLibrarydir / "int");
                std::filesystem::remove_all(StandardLibrarydir / "out");
            }

            if (ImGui::Button("Run All StandardLibrary Tests"))
            {

                UCodeLang::ModuleFile f;
                f.FromFile(&f, StandardLibrarydir / UCodeLang::ModuleFile::FileNameWithExt);

                if (TestWindowData.Testmode == TestMode::UCodeLangBackEnd)
                {
                    _TestCompiler.Set_BackEnd(UCodeLang::UCodeBackEndObject::MakeObject);
                }
                else if (TestWindowData.Testmode == TestMode::CLang89BackEnd)
                {
                    _TestCompiler.Set_BackEnd(UCodeLang::C11Backend::MakeObject);
                }
                else if (TestWindowData.Testmode == TestMode::WasmBackEnd)
                {
                    _TestCompiler.Set_BackEnd(UCodeLang::WasmBackEnd::MakeObject);
                }
                auto out = f.BuildModule(_TestCompiler, UCodeLang::ModuleIndex::GetModuleIndex());

                TestLib = std::move(*out.CompilerRet.GetValue().OutPut);


                Outpath = out.OutputItemPath;
                mode = TestWindowData.Testmode;
                StandardLibrarytests = UCodeLang::TestRuner::GetTests(TestLib.Get_Assembly());

                StandardLibraryTestInfo.resize(StandardLibrarytests.size());

                UCodeLang::TestRuner::InterpreterType interpreter;
                
                if (windowdata.VMType == UCodeVMType::Native_Interpreter)
                {
                    interpreter = UCodeLang::TestRuner::InterpreterType::NativeInterpreter;
                }
                else if (windowdata.VMType ==UCodeVMType::Jit_Interpreter)
                {
                    interpreter = UCodeLang::TestRuner::InterpreterType::JitInterpreter;
                }
                else
                {
                    interpreter = UCodeLang::TestRuner::InterpreterType::Interpreter;
                }

                UCodeLang::TestRuner runer;
                if (TestWindowData.Testmode == TestMode::UCodeLangBackEnd)
                {
                    auto info = runer.RunTests(TestLib, interpreter);

                    for (size_t i = 0; i < info.Tests.size(); i++)
                    {
                        StandardLibraryTestInfo[i].TestPassing = info.Tests[i].Passed;
                        StandardLibraryTestInfo[i].TestRan = true;
                    }
                }
                else  if (TestWindowData.Testmode == TestMode::CLang89BackEnd) 
                {
                    Path dllfile = "out.lib";
                    auto cfilepath = out.OutputItemPath.native();

                    UCodeLangAssert(CompileC89ToLib(cfilepath, dllfile));

                    using namespace UCodeLang;

                    auto staticinitname =C11Backend::UpdateToCindentifier(StaticVariablesInitializeFunc);
                    auto threadinitname = C11Backend::UpdateToCindentifier(ThreadVariablesInitializeFunc);

                    auto staticdeinitname = C11Backend::UpdateToCindentifier(StaticVariablesUnLoadFunc);
                    auto threaddeinitname = C11Backend::UpdateToCindentifier(ThreadVariablesUnLoadFunc);

                    auto functocallStr = "main";
                    #if UCodeLang_Platform_Windows
                    auto lib = LoadLibrary(dllfile.c_str());
                    UCodeLangDefer(FreeLibrary(lib));
                    auto staticinittocall = GetProcAddress(lib, staticinitname.c_str());
                    auto threadinittocall = GetProcAddress(lib, threadinitname.c_str());
                    auto staticdeinittocall = GetProcAddress(lib, staticdeinitname.c_str());
                    auto threaddeinittocall = GetProcAddress(lib, threaddeinitname.c_str());
                    #elif UCodeLang_Platform_Posix
                    auto lib = dlopen(dllfile.c_str(), RTLD_NOW);
                    UCodeLangDefer(dlclose(lib));

                    auto staticinittocall = dlsym(lib, staticinitname.c_str());
                    auto threadinittocall = dlsym(lib, threadinitname.c_str());
                    auto staticdeinittocall = dlsym(lib, staticdeinitname.c_str());
                    auto threaddeinittocall = dlsym(lib, threaddeinitname.c_str());
                    #endif

                    for (size_t i = 0; i < StandardLibraryTestInfo.size(); i++)
                    {
                        auto& func = StandardLibrarytests[i];
                        auto functocallStr = C11Backend::UpdateToCindentifier(func->DecorationName);
                        #if UCodeLang_Platform_Windows
                        auto functocall = GetProcAddress(lib, functocallStr.c_str());
                        #elif UCodeLang_Platform_Posix
                        auto functocall = dlsym(lib, functocallStr.c_str());
                        #endif

                        bool testpassed = false;
                        if (func->RetType._Type == ReflectionTypes::Bool)
                        {
                            using Func = bool(*)();
                            testpassed = ((Func)functocall)();
                        }
                        else
                        {
                            testpassed = true;
                        }

                        StandardLibraryTestInfo[i].TestPassing = testpassed;
                        StandardLibraryTestInfo[i].TestRan = true;
                    }
                }
                else
                {
                    UCodeLangUnreachable();
                }
            }
            if (ImGui::Button("Run StandardLibrary Tests"))
            {

                UCodeLang::ModuleFile f;
                f.FromFile(&f, StandardLibrarydir / UCodeLang::ModuleFile::FileNameWithExt);

                if (TestWindowData.Testmode == TestMode::UCodeLangBackEnd)
                {
                    _TestCompiler.Set_BackEnd(UCodeLang::UCodeBackEndObject::MakeObject);
                }
                else if (TestWindowData.Testmode == TestMode::CLang89BackEnd)
                {
                    _TestCompiler.Set_BackEnd(UCodeLang::C11Backend::MakeObject);
                }
                else if (TestWindowData.Testmode == TestMode::WasmBackEnd)
                {
                    _TestCompiler.Set_BackEnd(UCodeLang::WasmBackEnd::MakeObject);
                }
                auto out = f.BuildModule(_TestCompiler, UCodeLang::ModuleIndex::GetModuleIndex());

                Outpath = out.OutputItemPath;

                mode = TestWindowData.Testmode;
                TestLib = std::move(*out.CompilerRet.GetValue().OutPut);


                StandardLibrarytests = UCodeLang::TestRuner::GetTests(TestLib.Get_Assembly());

                StandardLibraryTestInfo.resize(StandardLibrarytests.size());

                UCodeLang::TestRuner::InterpreterType interpreter;
                if (windowdata.VMType == UCodeVMType::Native_Interpreter)
                {
                    interpreter = UCodeLang::TestRuner::InterpreterType::NativeInterpreter;
                }
                else if (windowdata.VMType == UCodeVMType::Jit_Interpreter)
                {
                    interpreter = UCodeLang::TestRuner::InterpreterType::JitInterpreter;
                }
                else
                {
                    interpreter = UCodeLang::TestRuner::InterpreterType::Interpreter;
                }

                UCodeLang::TestRuner runer;

                for (size_t i = TestWindowData.StandardLibraryTestIndex; i < TestWindowData.StandardLibraryTestCount; i++)
                {
                    bool v = false;
                    
                    
                    if (TestWindowData.Testmode == TestMode::UCodeLangBackEnd)
                    {
                        v = runer.RunTest(TestLib, interpreter, StandardLibrarytests[i]);
                    }
                    else  if (TestWindowData.Testmode == TestMode::CLang89BackEnd)
                    {

                    }
                    else
                    {
                        UCodeLangUnreachable();
                    }
                    StandardLibraryTestInfo[i].TestPassing = v;
                    StandardLibraryTestInfo[i].TestRan = true;
                }
            }

            for (size_t i = TestWindowData.StandardLibraryTestIndex; i < TestWindowData.StandardLibraryTestCount; i++)
            {
                if (StandardLibrarytests.size() < i) { continue; };
                auto& ItemTest = StandardLibrarytests[i];
                auto& ItemOut = StandardLibraryTestInfo[i];

                ImVec4 buttioncolor;

                if (!ItemOut.TestRan)
                {
                    buttioncolor = Colorlightgrey;//gray
                }
                else
                {
                    if (ItemOut.TestPassing)
                    {
                        buttioncolor = ColorGreen;//green
                    }
                    else
                    {
                        buttioncolor = ColorRed;//red
                    }
                }

                String TestV = "Test:";
                TestV += ItemTest->FullName;
                ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, buttioncolor);
                bool isopen = ImGui::TreeNode(TestV.c_str());
                ImGui::PopStyleColor();

                if (isopen)
                {
                    if (ImGui::Button("Run Test"))
                    {
                        UCodeLang::TestRuner::InterpreterType interpreter;
                        /*
                        if (usenative)
                        {
                            interpreter = TestRuner::InterpreterType::NativeInterpreter;
                        }
                        else if (usejit)
                        {
                            interpreter = TestRuner::InterpreterType::JitInterpreter;
                        }
                        else
                        */
                        {
                            interpreter = UCodeLang::TestRuner::InterpreterType::Interpreter;
                        }

                        UCodeLang::TestRuner runer;
                        bool v = false;

                        if (TestWindowData.Testmode == TestMode::UCodeLangBackEnd)
                        {
                            v = runer.RunTest(TestLib, interpreter, ItemTest);
                        }
                        else  if (TestWindowData.Testmode == TestMode::CLang89BackEnd)
                        {
                            Path dllfile = "out.lib";
                            auto cfilepath = Outpath.native();

                            UCodeLangAssert(CompileC89ToLib(cfilepath, dllfile));

                            using namespace UCodeLang;

                            auto staticinitname = C11Backend::UpdateToCindentifier(StaticVariablesInitializeFunc);
                            auto threadinitname = C11Backend::UpdateToCindentifier(ThreadVariablesInitializeFunc);

                            auto staticdeinitname = C11Backend::UpdateToCindentifier(StaticVariablesUnLoadFunc);
                            auto threaddeinitname = C11Backend::UpdateToCindentifier(ThreadVariablesUnLoadFunc);

                            #if UCodeLang_Platform_Windows
                            auto lib = LoadLibrary(dllfile.c_str());
                            UCodeLangDefer(FreeLibrary(lib));
                            auto staticinittocall = GetProcAddress(lib, staticinitname.c_str());
                            auto threadinittocall = GetProcAddress(lib, threadinitname.c_str());
                            auto staticdeinittocall = GetProcAddress(lib, staticdeinitname.c_str());
                            auto threaddeinittocall = GetProcAddress(lib, threaddeinitname.c_str());
                            #elif UCodeLang_Platform_Posix
                            auto lib = dlopen(dllfile.c_str(), RTLD_NOW);
                            UCodeLangDefer(dlclose(lib));

                            auto staticinittocall = dlsym(lib, staticinitname.c_str());
                            auto threadinittocall = dlsym(lib, threadinitname.c_str());
                            auto staticdeinittocall = dlsym(lib, staticdeinitname.c_str());
                            auto threaddeinittocall = dlsym(lib, threaddeinitname.c_str());
                            #endif


                            auto & func = StandardLibrarytests[i];
                            auto functocallStr = C11Backend::UpdateToCindentifier(func->DecorationName);
                            #if UCodeLang_Platform_Windows
                            auto functocall = GetProcAddress(lib, functocallStr.c_str());
                            #elif UCodeLang_Platform_Posix
                            auto functocall = dlsym(lib, functocallStr.c_str());
                            #endif

                            bool testpassed = false;
                            if (func->RetType._Type == ReflectionTypes::Bool)
                            {
                                using Func = bool(*)();
                                testpassed = ((Func)functocall)();
                            }
                            else
                            {
                                testpassed = true;
                            }

                            StandardLibraryTestInfo[i].TestPassing = testpassed;
                            StandardLibraryTestInfo[i].TestRan = true;

                        }
                        else
                        {
                            UCodeLangUnreachable();
                        }
                        StandardLibraryTestInfo[i].TestPassing = v;
                        StandardLibraryTestInfo[i].TestRan = true;
                    }
                    ImGui::TreePop();
                }
            }
        }
    }ImGui::End();
#endif // DEBUG
}
void AppObject::DrawPerformanceMenu()
{
    if (ImGui::Begin("Performance"))
    {
        {
            String Cpuinfo;
            #if UCodeLang_Platform_Windows 
            int CPUInfo[4] = { -1 };
            unsigned   nExIds, i = 0;
            char CPUBrandString[0x40];
            // Get the information associated with each extended ID.
            __cpuid(CPUInfo, 0x80000000);
            nExIds = CPUInfo[0];
            for (i = 0x80000000; i <= nExIds; ++i)
            {
                __cpuid(CPUInfo, i);
                // Interpret CPU brand string
                if (i == 0x80000002)
                    memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
                else if (i == 0x80000003)
                    memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
                else if (i == 0x80000004)
                    memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
            }
            //string includes manufacturer, model and clockspeed
            Cpuinfo = CPUBrandString;
            #endif // 
            ImGui::BeginDisabled();
            ImguiHelper::InputText("CPU Info", Cpuinfo);
            String OSInfo;
            #if UCodeLang_Platform_Windows
            OSInfo += "Windows";
            #endif
            #if UCodeLang_Platform_Linux
            OSInfo += "Linux";
            #endif
   
            #if UCodeLang_Platform_IPHONE
            OSInfo += "IPHONE";
            #endif

            #if UCodeLang_Platform_MacOS
            OSInfo += "MacOS";
            #endif

            #if UCodeLang_Platform_ANDROID
            OSInfo += "ANDROID";
            #endif

            #if UCodeLang_Platform_Wasm 
            OSInfo += "Wasm ";
            #endif

            #if UCodeLang_32BitSytem
            OSInfo += "- 32 bit";
            #else
            OSInfo += "- 64 bit";
            #endif
            ImguiHelper::InputText("OS Info", OSInfo);

            String UCodeLangInfo;
            UCodeLangInfo += std::to_string(UCodeLangMajorVersion);
            UCodeLangInfo += "," + std::to_string(UCodeLangMinorVersion);
            UCodeLangInfo += "," + std::to_string(UCodeLangPatchVersion);

            UCodeLangInfo += " ";

            #if UCodeLangMSVC
            UCodeLangInfo += "MSVC";
            #endif
            #if UCodeLangGNUC
            UCodeLangInfo += "GNUC";
            #endif
            #if UCodeLangClang
            UCodeLangInfo += "Clang";
            #endif


            #if UCodeLangDebug
            UCodeLangInfo += " Debug";
            #endif
            #if Release
            UCodeLangInfo += " Release";
            #endif
            #if Release
            UCodeLangInfo += " Published";
            #endif

            ImguiHelper::InputText("UCodeLang Info", UCodeLangInfo);

            ImGui::EndDisabled();
        }


        if (ImGui::TreeNode("ULang Tests"))
        {
            if (ImGui::Button("RunTests"))
            {

            }

            ImGui::Text("Report");
            float ULangInterpreterRelativeSpeedC89 = 0;
            float ULangJitInterpreterRelativeSpeedC89 = 0;
            float ULangNativeInterpreterRelativeSpeedC89 = 0;

            
            ImGui::BeginDisabled();

            ImguiHelper::float32Field("UCode Interpreter Relative C89", ULangInterpreterRelativeSpeedC89);
            ImguiHelper::float32Field("UCode Jit-Interpreter Relative C89", ULangJitInterpreterRelativeSpeedC89);
            ImguiHelper::float32Field("UCode Native-Interpreter Relative To C89", ULangNativeInterpreterRelativeSpeedC89);

            ImGui::EndDisabled();

            ImGui::TreePop();
        }
        if (ImGui::TreeNode("ULang PerformanceTests"))
        {
            struct TestInfo
            {
                String TestName;
                Path DirPath;
                 
            };
            static Vector<TestInfo> Tests;

            if (Tests.size()==0)
            {
                for (auto& dirEntry : std::filesystem::directory_iterator(UCodeLang_UCAppDir_TestPerformanceTestsOut)) 
                {
                    if (dirEntry.is_directory())
                    {
                        TestInfo f;
                        f.DirPath = dirEntry.path();
                        f.TestName = dirEntry.path().filename().generic_string();
                        Tests.push_back(std::move(f));
                    }

                }
            }

            if (ImGui::Button("RunTests"))
            {
              
            }

            ImGui::Text("Report");
            float ULangInterpreterRelativeSpeedC89 = 0;
            float ULangJitInterpreterRelativeSpeedC89 = 0;
            float ULangNativeInterpreterRelativeSpeedC89 = 0;


            ImGui::BeginDisabled();

            ImguiHelper::float32Field("UCode Interpreter Relative C89", ULangInterpreterRelativeSpeedC89);
            ImguiHelper::float32Field("UCode Jit-Interpreter Relative C89", ULangJitInterpreterRelativeSpeedC89);
            ImguiHelper::float32Field("UCode Native-Interpreter Relative To C89", ULangNativeInterpreterRelativeSpeedC89);

            ImGui::EndDisabled();

            ImGui::Text("Indivual Report");
            for (auto& Item : Tests)
            {
                if (ImGui::TreeNode(Item.TestName.c_str()))
                {
                    float ULangInterpreterRelativeSpeedC89 = 0;
                    float ULangJitInterpreterRelativeSpeedC89 = 0;
                    float ULangNativeInterpreterRelativeSpeedC89 = 0;


                    ImGui::BeginDisabled();

                    ImguiHelper::float32Field("UCode Interpreter Relative C89", ULangInterpreterRelativeSpeedC89);
                    ImguiHelper::float32Field("UCode Jit-Interpreter Relative C89", ULangJitInterpreterRelativeSpeedC89);
                    ImguiHelper::float32Field("UCode Native-Interpreter Relative To C89", ULangNativeInterpreterRelativeSpeedC89);

                    ImGui::EndDisabled();
                    ImGui::TreePop();
                }
            }

            ImGui::TreePop();
        }
        
    }ImGui::End();

 
}
bool AppObject::CompileC89ToLib(const Path& Cfile, const Path& Outdllfile)
{
    return ULangTest::CompileC89ToLib(Cfile, Outdllfile);
}
void AppObject::OnDraw()
{
    bool SetTextFocus = false;
    {
        auto now = SteadyClock::now();

        auto TimePassed =  now - LastFrame;

        DetaTime = std::chrono::duration_cast<std::chrono::duration<float>>(TimePassed).count();


        LastFrame = now;
    }
    
    ProcessSeverPackets();


    {
        #if NoUseThread
        if (IsRuningCompiler == false && NoThreadRetChecked == false)
        {
            auto& compilerret =_NoThreadRuningCompiler;
            OnDoneCompileing(compilerret, _RuningPaths.OutFile);
        }
        #else 
        if (IsRuningCompiler == false && _RuningCompiler.valid())
        {
            auto compilerret =_RuningCompiler.get();
            OnDoneCompileing(compilerret, _RuningPaths.OutFile);
        }
        #endif
    }

    bool Doc = true;
    BeginDockSpace(&Doc);

    //ImGui::ShowDemoWindow();
    if (ImGui::Begin("ECE Example"))
    {
        ECSExample::_Context.ImguiDraw();

    }  ImGui::End();

    if (ImGui::Begin("ShowStyleEditor"))
    {
        ImGui::ShowStyleEditor();

    }  ImGui::End();

    if (ImGui::Begin("VM Draw Window"))
    {
        if (!Debuger.IsinFunc()) {
            if (windowdata.CallFrame) {


                _AnyInterpreter.Call("frame");

            }
        }
        else
        {
            ImGui::Text("Cant Draw while Debuging");
        }
    }
    ImGui::End();

    if (ImGui::Begin("Files"))
    {

    } ImGui::End();
    if (ImGui::Begin("File.uc"))
    {
        if (SetTextFocus)
        {
            ImGui::SetWindowFocus();
            SetTextFocus = false;
        }
     

        _Editor.Render("File.uc");

        static TextEditor::Breakpoints old;
        auto& Break = _Editor.Get_Breakpoints();
        if (old.size() != Break.size())
        {
            Debuger.ClearBreakPoints();
        }

    } ImGui::End();

    if (ImGui::Begin("Error List"))
    {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);
        //ImGui::ListBoxHeader("Errors");  
        
        for (auto& item : PublishedDiagnostics.diagnostics)
        {
            String label = "LSP:" + item.message;
            if (ImGui::Selectable(label.c_str(), false))
            {
                auto text = GetTextEditorString();


                _Editor.SetCursorPosition(TextEditor::Coordinates(item.range.start.line, item.range.start.character));
                // handle selection
            }
        }
        for (auto& item : Errors)
        {
            String label = "Compier:" + item._Error._Msg;
            if (ImGui::Selectable(label.c_str(), item.IsSelected))
            {
                auto text = GetTextEditorString();


                _Editor.SetCursorPosition(TextEditor::Coordinates((int)item._Error.Line, (int)GetColumn(text, item._Error.Line, item._Error.Pos)));
                // handle selection
            }
        }
      
        //ImGui::EndListBox();


        ImGui::PopItemWidth();
    } ImGui::End();

    DrawTestMenu();
    DrawPerformanceMenu();

    if (ImGui::Begin("Output"))
    {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);
        bool IsCodeUpdated = GetTextEditorString() != _CompilerStr;
        bool IsCompiledDiff = GetTextEditorString() != _RunTimeStr;

        static const Vector<ImguiHelper::EnumValue<BackEndType>> List =
        {
            {"UCodeVM",BackEndType::UCodeVM},
            {"IR",BackEndType::IR},
            {"C89",BackEndType::C89},
            {"LLVM",BackEndType::LLVM},
            {"WebAssembly",BackEndType::WebAssembly},

            {"WindowsExecutable",BackEndType::WindowsExecutable},
            {"LinuxExecutable",BackEndType::LinuxExecutable},
        };

        bool UpdateLib = false;
        if (windowdata.VMType == UCodeVMType::Native_Interpreter)
        {
            UpdateLib = OutputWindow.OldNativeCpuType != windowdata.NativeCpuType;
        }
       

        if (ImguiHelper::EnumField("Type", OutputWindow.Type, List) || UpdateLib)
        {
            OutputWindow.OldNativeCpuType = windowdata.NativeCpuType;
            _CompilerStr = "";
            _RunTimeStr = "";
            _LibInfoString = "";
           

            UpdateBackEnd();

            CompileText(GetTextEditorString());
        }


        ImguiHelper::BoolEnumField("Auto Compile", OutputWindow.AutoCompile);
        //ImGui::SameLine();
        ImguiHelper::BoolEnumField("Auto Reload", OutputWindow.AutoReload);

        if (OutputWindow.AutoReload) {
            // ImGui::SameLine();
            ImguiHelper::BoolEnumField("Auto Hot Reload", OutputWindow.AutoHotReload);
        }

        const float MaxWaitTimeForAutoCompile = 1;
        if (OutputWindow.AutoCompile == false)
        {
            ImGui::BeginDisabled(!IsCodeUpdated);
            if (ImGui::Button("Compile"))
            {
                CompileText(GetTextEditorString());
            }
            ImGui::EndDisabled();
        }
        else
        {
            if (AutoCompileTimeOut >= 0)
            {
                AutoCompileTimeOut -= DetaTime;
            }
            if (IsCodeUpdated)
            {
                LastFileUpdated += DetaTime;
            }
            else
            {
                LastFileUpdated = 0;
            }

            if (IsCodeUpdated && LSPHasNoErrors() && AutoCompileTimeOut <= 0
                && LastFileUpdated >= MaxWaitTimeForAutoCompile)
            {
                CompileText(GetTextEditorString());

                
                   

            }
        }
        if (OutputWindow.AutoReload == false)
        {
            ImGui::BeginDisabled(!IsCompiledDiff);
            if (ImGui::Button("Reload RunTime"))
            {
                _RunTimeStr = GetTextEditorString();


                FullReloadRunTime();
            }
            if (OutputWindow.AutoHotReload == false)
            {
                ImGui::BeginDisabled(!_RuntimeLib.Get_Lib());
                if (ImGui::Button("Hot Reload"))
                {
                    _RunTimeStr = GetTextEditorString();
                    HotReloadRunTime();
                }
                ImGui::EndDisabled();
            }
            ImGui::EndDisabled();
        }
       


      
        ImGui::Separator();

        static const Vector <ImguiHelper::EnumValue<UCodeLang::OptimizationFlags>> OpflagList =
        {
            {"None",UCodeLang::OptimizationFlags::NoOptimization},
            {"0_1",UCodeLang::OptimizationFlags::ForSize},
            {"0_2",UCodeLang::OptimizationFlags::ForSpeed},
            {"0_3",UCodeLang::OptimizationFlags::ForMaxSpeed},
        };
        bool UpdatedCompileInfo = false;

       
        if (ImguiHelper::EnumField("Optimizations", OutputWindow.Flags, OpflagList))
        {
            UpdatedCompileInfo = true;
        }
        if (ImguiHelper::BoolEnumField("In Debug Mode", OutputWindow.InDebug))
        {
            UpdatedCompileInfo = true;
        }
        if (ImguiHelper::BoolEnumField("With StandardLibrary", OutputWindow.ImportStandardLibrary))
        {
            UpdatedCompileInfo = true;
        }


        if (UpdatedCompileInfo)
        {
            CompileText(GetTextEditorString());
        }
        
        if (ImGui::Button("Copy To Clip Board"))
        {
            ImGui::LogToClipboard();
            ImGui::LogText(_LibInfoString.c_str());
            ImGui::LogFinish();
        }ImGui::SameLine();


        {
            String Txt;
            if (OutputWindow.Type == BackEndType::LLVM)
            {
                Txt += "Open in Compiler Explorer [llvm]";
            }
            else
            {
                Txt += "Open in Compiler Explorer [C/C++]";
            }
            if (ImGui::Button(Txt.c_str()))
            {
                if (OutputWindow.Type == BackEndType::LLVM) 
                {
                    String path = "https://godbolt.org/noscript/llvm";
                    OpenOnWeb(path);
                }
                else
                {
                    String path = "https://godbolt.org/";
                    OpenOnWeb(path);
                }
            }

        }
        ImGui::BeginDisabled();

        bool ShowLibInfo=true;
        if (IsRuningCompiler)
        {
            String tepstr = "[Compileing]";
            ImGui::PushID(&_LibInfoString);

            ImGui::InputTextMultiline("", &tepstr, ImGui::GetContentRegionAvail());

            ImGui::PopID();
        }
        else
        {
            if (OutputWindow.AutoCompile)
            {
                bool CanAutoCompile = IsCodeUpdated && LSPHasNoErrors();

                if (CanAutoCompile && LastFileUpdated < MaxWaitTimeForAutoCompile)
                {
                    ShowLibInfo = false;

                    String tepstr = "[Waiting for no more Input]";
                    ImGui::PushID(&_LibInfoString);

                    ImGui::InputTextMultiline("", &tepstr, ImGui::GetContentRegionAvail());

                    ImGui::PopID();
                }
            }
            else
            {
                ShowLibInfo = true;
            }

            if (ShowLibInfo)
            {
                ImGui::PushID(&_LibInfoString);

                ImGui::InputTextMultiline("", &_LibInfoString, ImGui::GetContentRegionAvail());

                ImGui::PopID();
            }
        }
        ImGui::EndDisabled();


        ImGui::PopItemWidth();
    }  ImGui::End();
    if (ImGui::Begin("UCode-VM"))
    {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

        ShowUCodeVMWindow();

        ImGui::PopItemWidth();

    } ImGui::End();

    if (ImGui::BeginMainMenuBar())
    {

        if (ImGui::BeginMenu("File"))
        {


            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {


            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Git"))
        {


            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Build"))
        {


            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Settings"))
        {


            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {


            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    EndDockSpace();

   
}

void AppObject::UpdateBackEnd()
{
    UCodeLang::BackEndObject_Ptr _BackEnd;
    switch (OutputWindow.Type)
    {
    case BackEndType::UCodeVM:
        _BackEnd = UCodeLang::UCodeBackEndObject::MakeObject;

        if (windowdata.VMType == UCodeVMType::Native_Interpreter)
        {
            switch (OutputWindow.OldNativeCpuType)
            {
            case NativeSet::x86:
            {
                UCodeLangUnreachable();
            }
            break;
            case NativeSet::x86_64:
            {
                _BackEnd = UCodeLang::X86_64UNativeBackEnd::MakeObject;
            }
            break;
            default:
                UCodeLangUnreachable();
                break;
            }
        }
        break;
    case BackEndType::C89:
        _BackEnd = UCodeLang::C11Backend::MakeObject;
        break;
    case BackEndType::IR:
        _BackEnd = UCodeLang::IRBackEnd::MakeObject;
        break;
    case BackEndType::LLVM:
        _BackEnd = UCodeLang::LLVMBackEnd::MakeObject;
        break;
    case BackEndType::WebAssembly:
        _BackEnd = UCodeLang::WasmBackEnd::MakeObject;
        break;
    case BackEndType::WindowsExecutable:
        _BackEnd = UCodeLang::WindowsBackEnd::MakeObject;
        break;
    case BackEndType::LinuxExecutable:
        _BackEnd = UCodeLang::LinuxBackEnd::MakeObject;
        break;
    default:
        UCodeLangUnreachable();
        break;
    }
    _Compiler.Set_BackEnd(_BackEnd);
}

void AppObject::ProcessSeverPackets()
{
    if (this->SeverPtr)
    {
        auto Packets = this->SeverPtr->_Sever.GetPackets();
        for (auto& Item : Packets)
        {
            OnSeverPacket(std::move(Item));
        }

        if (_IsLSPRuning) 
        {
            String mystr = GetTextEditorString();
            if (mystr != SeverSideFile)
            {
                FileVersion++;

                LS::DidChangeTextDocumentParams p;
                p.textDocument.uri = EditorTextURL;
                p.textDocument.version = FileVersion;


                {//Update this to use parts and not full file.
                    LS::TextDocumentContentChangeEventFullFile p2;
                    p2.text = mystr;
                    p.contentChanges.push_back(std::move(p2));
                }

                SeverSideFile = mystr;

                SendDidChangeTextDocument(p);

            }

        }
    }
}

void AppObject::ShowInFiles(const Path& path)
{
    Path copy;
    if (std::filesystem::is_directory(path) && path.native().back() != Path::preferred_separator)
    {
        copy = path / "";
    }
    else if (std::filesystem::is_regular_file(path))
    {
        copy = path.parent_path();
        copy /= "";
    }
    else
    {
        copy = path;
    }

    #if UCodeLang_Platform_Windows
    ShellExecute(NULL, L"open", copy.c_str(), NULL, NULL, SW_SHOWDEFAULT);
    #elif UCodeLang_Platform_Posix
    auto cmd =(String("open ") + path.generic_string());
    system(cmd.c_str());
    #else //MacOS

    #endif 
}

void AppObject::ShowUCodeVMWindow()
{
    if (OutputWindow.Type == BackEndType::C89)
    {
        static UCodeLang::Optional<int> Value;
        if (ImGui::Button("Run Main"))
        {


            Path dllfile = "out.lib";
            auto v = Outfilepath();
            auto cfilepath = v.native() + Path(".c").native();
            {
                std::ofstream f(cfilepath);
                f << UCodeLang::Compiler::GetTextFromFile(v);
                f.close();
            }
            UCodeLangAssert(CompileC89ToLib(cfilepath, dllfile));
            
            using namespace  UCodeLang;
            auto staticinitname = C11Backend::UpdateToCindentifier(StaticVariablesInitializeFunc);
            auto threadinitname = C11Backend::UpdateToCindentifier(ThreadVariablesInitializeFunc);

            auto staticdeinitname = C11Backend::UpdateToCindentifier(StaticVariablesUnLoadFunc);
            auto threaddeinitname = C11Backend::UpdateToCindentifier(ThreadVariablesUnLoadFunc);

            auto functocallStr = "main";
            #if UCodeLang_Platform_Windows
            auto lib = LoadLibrary(dllfile.c_str());
            UCodeLangAssert(lib);

            UCodeLangDefer(FreeLibrary(lib));
            auto staticinittocall = GetProcAddress(lib, staticinitname.c_str());
            auto threadinittocall = GetProcAddress(lib, threadinitname.c_str());
            auto staticdeinittocall = GetProcAddress(lib, staticdeinitname.c_str());
            auto threaddeinittocall = GetProcAddress(lib, threaddeinitname.c_str());

            auto functocall = GetProcAddress(lib, functocallStr);
            #elif UCodeLang_Platform_Posix
            auto lib = dlopen(dllfile.c_str(), RTLD_NOW);
            UCodeLangAssert(lib);

            UCodeLangDefer(dlclose(lib));

            auto staticinittocall = dlsym(lib, staticinitname.c_str());
            auto threadinittocall = dlsym(lib, threadinitname.c_str());
            auto staticdeinittocall = dlsym(lib, staticdeinitname.c_str());
            auto threaddeinittocall = dlsym(lib, threaddeinitname.c_str());

            auto functocall = dlsym(lib, functocallStr);
            #endif


            using FuncV = void(*)();
            if (functocallStr != "main") {
                ((FuncV)staticinittocall)();

                ((FuncV)threadinittocall)();
            }

            using Func = int (*)();
            int retvalue = ((Func)functocall)();

            if (functocallStr != "main") {
                ((FuncV)threaddeinittocall)();

                ((FuncV)staticdeinittocall)();
            }

            Value = retvalue;

        }

        if (Value.has_value())
        {
            ImguiHelper::Int32Field("Retrned", Value.value());

        }
    }
    else if (OutputWindow.Type == BackEndType::WebAssembly)
    {
        static UCodeLang::Optional<int> Value;
        if (ImGui::Button("Run Main"))
        {
            auto functocall = "main";
            String JsString = "const wasm = new Uint8Array([";

            std::stringstream ss;
            ss << "const wasm = new Uint8Array([";
            auto v = UCodeLang::Compiler::GetBytesFromFile(Outfilepath());
            for (const auto& b : v) {
                ss << "0x" << std::hex << static_cast<int>(b) << ", ";
            }
            ss << "]);\n";
            ss << "const m = new WebAssembly.Module(wasm);\n";
            ss << "const instance = new WebAssembly.Instance(m, {});\n";
            ss << "console.log(instance.exports.";
            ss << functocall;
            ss << "());";

            Path node_file = Path("test.js").native();
            Path out_file = Path("test.js.out").native();


            std::ofstream nf(node_file);
            nf << ss.str();
            nf << std::flush;


            {
                std::system(("node " + node_file.generic_string() + " > " + out_file.generic_string()).c_str());
            }

            std::stringstream ss_out;
            ss_out << std::ifstream(out_file).rdbuf();
            auto outstr = ss_out.str();

            Value = std::stoi(outstr);
        }

        if (Value.has_value())
        {
            ImguiHelper::Int32Field("Retrned", Value.value());

        }
    }
    else
    {
        static const Vector<ImguiHelper::EnumValue<UCodeVMType>> List =
        {
            { "Interpreter",UCodeVMType::Interpreter },
            { "Jit_Interpreter",UCodeVMType::Jit_Interpreter },
            { "Native_Interpreter",UCodeVMType::Native_Interpreter },
        };
        static const Vector<ImguiHelper::EnumValue<NativeSet>> NativeSetList =
        {
    #if UCodeLang_CPUIs_x86
            { "Native(x86)",NativeSet::Native },
    #else
            { "Native(x86_64)",NativeSet::Native },
    #endif
            { "x86",NativeSet::x86 },
            { "x86_64",NativeSet::x86_64 },
    };


        if (ImguiHelper::EnumField("Type", windowdata.VMType, List))
        {
            switch (windowdata.VMType)
            {
            case UCodeVMType::Interpreter:
            {
                _AnyInterpreter.SetAsInterpreter();
                _AnyInterpreter.Init(&_RunTimeState);
            }
            break;
            case UCodeVMType::Jit_Interpreter:
            {
                _AnyInterpreter.SetAsJitInterpreter();

                _AnyInterpreter.GetAs_JitInterpreter().AlwaysJit = true;
                _AnyInterpreter.Init(&_RunTimeState);
            }
            break;
            case UCodeVMType::Native_Interpreter:
            {
                _AnyInterpreter.SetAsNativeInterpreter();

                _AnyInterpreter.Init(&_RunTimeState);
            }
            break;
            default:
                break;
            }
            ImguiHelper::_Ptr = _AnyInterpreter.GetPtr();
        }


        if (windowdata.VMType == UCodeVMType::Native_Interpreter)
        {
            ImGui::SameLine();
            ImguiHelper::EnumField("CpuType", windowdata.NativeCpuType, NativeSetList);
        }
       
        ImGui::Separator();

        {
            ImGui::Columns(2, "DebugerOrCode");
            {
                {
                    bool CanBeRan = true;
                    if (windowdata.VMType == UCodeVMType::Native_Interpreter)
                    {
                        if (windowdata.NativeCpuType != NativeSet::Native) {
                            CanBeRan = false;
                        }
                    }

                    if (CanBeRan)
                    {
                        ImGui::TextUnformatted("Debuger");
                    }
                    else
                    {
                        ImGui::TextUnformatted("Debuger(Cant run this Code)");
                    }



                    ImGui::BeginDisabled(!CanBeRan);
                    ShowDebugerMenu(windowdata);
                    ImGui::EndDisabled();

                }
                ImGui::NextColumn();
                {
                    if (Debuger.IsinFunc())
                    {
                        String V;
                        V = _RunTimeState.GetName(Debuger.GetStartofFunc(Debuger.GetCurrentInstruction()));
                        ImGui::TextUnformatted(V.c_str());
                        
                        if (ImGui::BeginTable("split2", 2, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
                        {
                           


                            ShowCurrentFuncInsList();
                        }
                    }

                    ImGui::TextUnformatted("Code");


                    if (windowdata.VMType == UCodeVMType::Native_Interpreter)
                    {


                    }
                    else if (windowdata.VMType == UCodeVMType::Jit_Interpreter)
                    {
                        auto& jit = _AnyInterpreter.GetAs_JitInterpreter();
                        String txt = jit.GetJitState();

                        ImGui::BeginDisabled();

                        ImGui::PushID(&txt);

                        ImGui::InputTextMultiline("", &txt, ImGui::GetContentRegionAvail());

                        ImGui::PopID();

                        ImGui::EndDisabled();
                    }
                    else if (windowdata.VMType == UCodeVMType::Interpreter)
                    {
                        if (ImGui::BeginTable("split2", 2, ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
                        {
                            for (auto& Item : windowdata.InsInfo)
                            {
                                bool IsOnIns = false;

                                if (Debuger.IsinFunc())
                                {
                                    IsOnIns = Item.InsAddress == Debuger.GetCurrentInstruction();
                                }

                               
                                ImGui::TableNextColumn();

                               
                                if (IsOnIns)
                                {
                                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                                }

                                ImGui::Text(std::to_string(Item.InsAddress).c_str());

                              

                                ImGui::TableNextColumn();

                                ImGui::Text(Item.StringValue.c_str());  
                                
                                if (IsOnIns)
                                {
                                    ImGui::PopStyleColor();
                                }

                                ImGui::TableNextRow();
                            }

                            ImGui::EndTable();
                        }
                    }
                }
            }
            ImGui::Columns();
        }
    }
}

void AppObject::ShowCurrentFuncInsList()
{
    bool Start = false;
    for (auto& Item : windowdata.InsInfo)
    {
        bool IsOnIns = Item.InsAddress == Debuger.GetCurrentInstruction();

        if (Start == false)
        {
            Start = Item.InsAddress == Debuger.GetStartofFunc(Debuger.GetCurrentInstruction());
        }


        if (Start)
        {
            ImGui::TableNextColumn();


            if (IsOnIns)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            }

            ImGui::Text(std::to_string(Item.InsAddress).c_str());



            ImGui::TableNextColumn();

            ImGui::Text(Item.StringValue.c_str());

            if (IsOnIns)
            {
                ImGui::PopStyleColor();
            }

            ImGui::TableNextRow();
            if (Start)
            {
                auto& ins = _RunTimeState.GetInst(Item.InsAddress);
                if (ins.OpCode == UCodeLang::InstructionSet::Return)
                {
                    break;
                }
            }
        }
    }

    ImGui::EndTable();
}

void AppObject::UpdateInsData(UCodeVMWindow& windowdata)
{
    using namespace UCodeLang::UAssembly;
    windowdata.InsInfo.clear();
    auto& RunTime = _RunTimeState;

    windowdata.InsInfo.reserve(RunTime.Get_Libs().GetInstructions().size());


    auto& InsMapData = UCodeLang::UAssembly::Get_InsToInsMapValue();
    UnorderedMap<UCodeLang::UAddress, String> AddressToName;


    auto& Inslist = RunTime.Get_Libs().GetInstructions();
    const BytesView staticbytesview = BytesView::Make((Byte*)RunTime.Get_StaticMemPtr(), RunTime.Get_Libs().GetStaticBytes().size());
    for (size_t i = 0; i < Inslist.size(); i++)
    {
        auto& Item = Inslist[i];
        UCodeVMWindow::InsData V;
        V._Ins = Item;
        V.InsAddress = i;
        String Vstr;

        i += UAssembly::ParseInstruction(i, Span<UCodeLang::Instruction>::Make(Inslist.data(), Inslist.size()), Vstr,staticbytesview, AddressToName);

        V.StringValue = std::move(Vstr.c_str());

        windowdata.InsInfo.push_back(std::move(V));
    }
}

bool DrawAnyInt64(const char* Name,UCodeLang::AnyInt64& V)
{
    bool updated = false;
    if (ImGui::TreeNode(Name))
    {
        if (ImguiHelper::Int8Field("int8", V.AsInt8))
        {
            updated = true;
        }
        if (ImguiHelper::Int16Field("int16", V.AsInt16))
        {
            updated = true;
        }
        if (ImguiHelper::Int32Field("int32", V.AsInt32))
        {
            updated = true;
        }
        if (ImguiHelper::Int64Field("int64", V.AsInt64))
        {
            updated = true;
        }
        ImguiHelper::float32Field("float32", V.Asfloat32);
        ImguiHelper::float64Field("float64", V.Asfloat64);



        ImGui::TreePop();
    }
    return  updated;
}

void AppObject::ShowDebugerMenu(UCodeVMWindow& windowdata)
{

   

    ImGui::Separator();

    bool IsinFileMode = false;
    ImVec2 Buttonsize = { 80,20 };

    static UCodeLang::DebugData DebugInfo;

    bool InFunction = Debuger.IsinFunc();

    ImGui::Button("Reset", Buttonsize);

    ImguiHelper::BoolEnumField("Call Static/Thread Init On Reload", windowdata.CallStaticVarOnReload);

    ImguiHelper::BoolEnumField("Call Frame", windowdata.CallFrame);


    ImGui::BeginDisabled(!InFunction);

    if (ImGui::Button("Step in", Buttonsize))
    {
        Debuger.StepIn();
    } ImGui::SameLine();

    if (ImGui::Button("Step over", Buttonsize))
    {
        Debuger.StepOver();
    }ImGui::SameLine();

    if (ImGui::Button("Step out", Buttonsize))
    {
        Debuger.StepOut();
    }

    if (ImGui::Button("Vm Step in", Buttonsize))
    {
        Debuger.VM_StepIn();
    } ImGui::SameLine();

    if (ImGui::Button("Vm Step over", Buttonsize))
    {
        Debuger.VM_StepOver();
    }ImGui::SameLine();

    if (ImGui::Button("Vm Step out", Buttonsize))
    {
        Debuger.VM_StepOut();
    }

    ImGui::EndDisabled();

    if (windowdata.VMType == UCodeVMType::Jit_Interpreter)
    {
        if (ImGui::Button("Jit-Precompile All", Buttonsize))
        {
            _AnyInterpreter.GetAs_JitInterpreter().TryBuildAllFuncs();
        }
    }
    
    if (InFunction)
    {
        Debuger.UpdateDebugData(DebugInfo);
        ImGui::Text("Varables");
        auto& thisFrame = DebugInfo._StackFrames.front();


        for (auto& Item : thisFrame._Varables) 
        {
            ImguiHelper::UCodeObjectField(
                Item.VarableName.c_str(),
                Item.GetObjectPtr(),
                Item.Type, 
                _RunTimeState.Get_Assembly());
        }
    }

    if (InFunction) 
    {
        String V = "StackPtrOffset:";
        V += std::to_string(Debuger.GetStackOffset());
        ImGui::Text(V.c_str());

        ImGui::Text("StackFrames");

    }
    if (InFunction)
    {
        ImGui::Text("Registers");

        for (UCodeLang::RegisterID_t i = (UCodeLang::RegisterID_t)UCodeLang::RegisterID::StartRegister;
            i < (UCodeLang::RegisterID_t)UCodeLang::RegisterID::EndRegister + 1; i++)
        {
            auto reg = (UCodeLang::RegisterID)i;
            auto tep = UCodeLang::UAssembly::UAssembly::GetRegisterToString(reg);
            DrawAnyInt64(tep.c_str(), Debuger.GetReg(reg).Value);
        }
    }
    

    if (!InFunction) 
    {
        auto& Assembly = _RunTimeState.Get_Assembly();
        bool Updated = false;
        

        auto GlobalObject = Assembly.Get_GlobalObject_Class();

        ImGui::Text("Call Function:"); ImGui::SameLine();
        
        ImGui::PushID(&callFuncContext.current_method);

        String MethodString = "null";

        if (callFuncContext.current_method)
        {
            MethodString = callFuncContext.current_method->FullName;
        }

        if (ImGui::BeginCombo("", MethodString.c_str(), ImGuiComboFlags_NoArrowButton))
        {
            for (auto& Item : Assembly.Classes)
            {
                if (Item->Get_Type() == UCodeLang::ClassType::Class)
                {
                    auto& ClassItem = Item->Get_ClassData();

                    bool IsGlobalObject = &ClassItem == GlobalObject;
                    for (auto& Method : ClassItem.Methods)
                    {
                        String FuncName= Method.FullName;


                        bool is_selected = (callFuncContext.current_method == &Method);
                        if (ImGui::Selectable(FuncName.c_str(), is_selected))
                        {
                            callFuncContext.current_method = &Method;
                            Updated = true;
                        }
                        if (is_selected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }

                        
                    }
                }
            }
            
            ImGui::EndCombo();
        }
        ImGui::PopID();

        bool Is32bits = sizeof(void*) == sizeof(Int32);
        if (callFuncContext.current_method)
        {
            ImGui::Text("Args");

            if (Updated)
            {
                callFuncContext.Args.resize(callFuncContext.current_method->ParsType.size());
                bool CantMakeArgs = false;
                for (size_t i = 0; i < callFuncContext.current_method->ParsType.size(); i++)
                {
                    const auto& Par = callFuncContext.current_method->ParsType[i];
                    auto& Arg = callFuncContext.Args[i];

                    auto Op_Size = Assembly.GetSize(Par, Is32bits);
                    if (Op_Size.has_value())
                    {
                        Arg.Resize(Op_Size.value());
                        auto ItWorked = Assembly.CallDefaultConstructor(Par,Arg.Data(), Is32bits);
                        if (ItWorked)
                        {
                            if (ItWorked.value().has_value())
                            {
                                auto& ToCall = ItWorked.value().value();
                                for (auto& Item : ToCall)
                                {
                                    _AnyInterpreter.ThisCall(Item.MethodToCall, Item.ThisPtr);
                                }
                            }
                        }
                        else
                        {
                            CantMakeArgs = true;
                            break;
                        }
                    }
                    else
                    {
                        CantMakeArgs = true;
                        break;
                    }

                }
                if (CantMakeArgs)
                {
                    callFuncContext.current_method = nullptr;
                }
            }

            if (callFuncContext.current_method) 
            {
                for (size_t i = 0; i < callFuncContext.current_method->ParsType.size(); i++)
                {
                    const auto& Par = callFuncContext.current_method->ParsType[i];
                    auto& Arg = callFuncContext.Args[i];

                    String ParName = "Arg" + std::to_string(i);

                    ImGui::PushID(Arg.Data());
                    ImguiHelper::UCodeObjectField(ParName.c_str(), (void*)Arg.Data(), Par, Assembly);
                    ImGui::PopID();
                }

                if (windowdata.CallStaticVarOnReload == false) {
                    ImguiHelper::BoolEnumField("Call Stack/Thread init", callFuncContext.CallStaticAndThreadInit);
                    ImguiHelper::BoolEnumField("Call Stack/Thread de-init", callFuncContext.CallStaticAndThreadDeInit);

                }
                if (ImGui::Button(((String)"Call:" + MethodString).c_str()))
                {
                    callFuncContext._LastRetType = callFuncContext.current_method->RetType;
                    callFuncContext._LastRet.Resize(Assembly.GetSize(callFuncContext._LastRetType, Is32bits).value_or(0));

                    
                    if (windowdata.CallStaticVarOnReload || callFuncContext.CallStaticAndThreadInit)
                    {
                        _AnyInterpreter.Call(StaticVariablesInitializeFunc);
                        _AnyInterpreter.Call(ThreadVariablesInitializeFunc);
                    }

                    for (size_t i = 0; i < callFuncContext.current_method->ParsType.size(); i++)
                    {
                        auto& Arg = callFuncContext.Args[i];
                        _AnyInterpreter.PushParameter(Arg.Data(), Arg.Size());
                    }
                    _AnyInterpreter.Call(callFuncContext.current_method);

                    if (windowdata.CallStaticVarOnReload || callFuncContext.CallStaticAndThreadDeInit)
                    {
                        _AnyInterpreter.Call(StaticVariablesUnLoadFunc);
                        _AnyInterpreter.Call(ThreadVariablesUnLoadFunc);
                    }

                    if (callFuncContext._LastRet.Size())
                    {
                        _AnyInterpreter.Get_Return(callFuncContext._LastRet.Data(), callFuncContext._LastRet.Size());
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button(((String)"Step Into:" + MethodString).c_str()))
                {
                    Debuger.Attach(&_RunTimeState);
                    callFuncContext._LastRetType = callFuncContext.current_method->RetType;
                    callFuncContext._LastRet.Resize(Assembly.GetSize(callFuncContext._LastRetType, Is32bits).value_or(0));


                   
                    if (windowdata.CallStaticVarOnReload || callFuncContext.CallStaticAndThreadInit)
                    {
                        _AnyInterpreter.Call(StaticVariablesInitializeFunc);
                        _AnyInterpreter.Call(ThreadVariablesInitializeFunc);
                    } 
                    
                    for (size_t i = 0; i < callFuncContext.current_method->ParsType.size(); i++)
                    {
                        auto& Arg = callFuncContext.Args[i];
                        _AnyInterpreter.PushParameter(Arg.Data(), Arg.Size());
                    }
                    Debuger.StepInto(
                        &_AnyInterpreter.GetAs_Interpreter(), callFuncContext.current_method);
                }
               // ImGui::BeginDisabled();
                if (callFuncContext._LastRetType == callFuncContext.current_method->RetType)
                {
                    ImguiHelper::UCodeObjectField("Returned", callFuncContext._LastRet.Data(),callFuncContext._LastRetType, Assembly);
                }
               // ImGui::EndDisabled();
            }
        }

    }


    if (Debuger.HasExitedFunc())
    {
        Debuger.StepOutof();

        if (callFuncContext._LastRet.Size())
        {
            _AnyInterpreter.Get_Return(callFuncContext._LastRet.Data(), callFuncContext._LastRet.Size());
        }
    }

    {
        if (ImGui::Begin("Stack-Memory"))
        {
            
            if (_AnyInterpreter.Get_InterpreterType() == UCodeLang::InterpreterTypes::Interpreter)
            {
                auto& Inter = _AnyInterpreter.GetAs_Interpreter();
                auto stack = Inter.GetStackSpan();


                ImGui::Columns(2, "Debug/Raw Stack Memory");
                {

                }
                ImGui::NextColumn();
                {
                    static MemoryEditor V;
                    V.DrawContents(stack.Data(), stack.Size());
                }
            }
            

        }ImGui::End();

    }

    {
        if (ImGui::Begin("Heap-Memory"))
        {
            auto& Assembly = _RunTimeState.Get_Assembly();

            void* Staticptr = _RunTimeState.Get_StaticMemPtr();
            void* Threadptr = _AnyInterpreter.GetThreadPtr();

            struct MemData
            {
                bool IsReseved = true;
                UCodeLang::PtrType _Ptr;
                UCodeLang::Allocator::MemData _Mem;
            };
            Vector<MemData> Mems;
            Vector<bool> Isshowing;
            auto& Allocator = _RunTimeState.Get_Allocator();
            auto& AlocsInfo = _RunTimeState.Get_Allocator().Get_AlocsBeingUsed();
            auto& ReservedAlocsInfo = _RunTimeState.Get_Allocator().Get_ReservedAlocs();


            for (auto& Item : AlocsInfo)
            {
                Mems.push_back({ false,Item.first,Item.second });
            }
            for (auto& Item : ReservedAlocsInfo)
            {
                Mems.push_back({ true,Item.first,Item.second});
            }

            std::sort(Mems.begin(), Mems.end(),
                [](const MemData& A, const MemData& B) {return A._Ptr < B._Ptr; });


            Isshowing.resize(Mems.size());

            ImGui::Columns(2, "Debug/Raw Heap Memory");
            {


                String InfoStr;
                for (size_t i = 0; i < Mems.size(); i++)
                {
                    auto& Item = Mems[i];

                    InfoStr.clear();
                   // InfoStr = (String)"Ptr:" + std::to_string((uintptr_t)Item._Ptr);
                    InfoStr += "Size:" + std::to_string(Item._Mem.Size);
                    InfoStr += (String)",IsReseved:" + (Item.IsReseved ? "true" : "false");
                   
                    bool statckhwasset = false;
                    
                    if (Staticptr == Item._Ptr)
                    {
                        InfoStr += ",--StaticMem";
                        statckhwasset = true;
                    }
                    else if (Threadptr == Item._Ptr)
                    {
                        InfoStr += ",--ThreadMem";
                        statckhwasset = true;
                    }
                    else if (_AnyInterpreter.Get_InterpreterType() == UCodeLang::InterpreterTypes::Interpreter)
                    {
                        auto& Inter = _AnyInterpreter.GetAs_Interpreter();
                        if (Inter.GetStackSpan().Data() == Item._Ptr) {
                            InfoStr += ",--StackMem";
                            statckhwasset = true;
                        }
                    }

                    if (statckhwasset==false)
                    {
                        InfoStr += ",--HeapMem";
                    }

                    if (ImGui::TreeNode(InfoStr.c_str()))
                    {
                        Isshowing[i] = true;
                        if (Staticptr == Item._Ptr)
                        {
                            auto& DebugInfo = _RunTimeState.Get_Libs().Get_DebugInfo();
                            for (auto& Item : DebugInfo.VarablesInfo)
                            {
                                if (auto Val = Item.second.TypeLoc.Get_If<UCodeLang::VarableInfo::Static>())
                                {
                                    void* Object = (void*)((uintptr_t)Staticptr + (uintptr_t)Val->offset);

                                    ImGui::Text(("offset:" + std::to_string(Val->offset)).c_str());
                                    ImGui::SameLine();
                                    ImguiHelper::UCodeObjectField(Item.first.c_str(), Object, Item.second.ReflectionType, Assembly);
                                }
                            }
                        }
                        else if (Threadptr == Item._Ptr)
                        {
                            auto& DebugInfo = _RunTimeState.Get_Libs().Get_DebugInfo();
                            for (auto& Item : DebugInfo.VarablesInfo)
                            {
                                if (auto Val = Item.second.TypeLoc.Get_If<UCodeLang::VarableInfo::Thread>())
                                {
                                    void* Object = (void*)((uintptr_t)Threadptr + (uintptr_t)Val->offset);

                                    ImGui::Text(("offset:" + std::to_string(Val->offset)).c_str());
                                    ImGui::SameLine();
                                    ImguiHelper::UCodeObjectField(Item.first.c_str(), Object, Item.second.ReflectionType, Assembly);
                                }
                            }
                        }
                        else if (_AnyInterpreter.Get_InterpreterType() == UCodeLang::InterpreterTypes::Interpreter)
                        {
                            auto& Inter = _AnyInterpreter.GetAs_Interpreter();
                            if (Inter.GetStackSpan().Data() == Item._Ptr) {
                                InfoStr += ",--StackMem";
                            }
                        }
                        else
                        {

                        }
                        ImGui::TreePop();
                    }
                       
                }
            }
            ImGui::NextColumn();
            {
                for (size_t i = 0; i < Mems.size(); i++)
                {
                    auto& Item = Mems[i];
                    if (Isshowing[i]) {
                        static MemoryEditor V;
                        V.DrawContents(Item._Ptr, Item._Mem.Size);
                    }
                }
            }
        }
        ImGui::End();
    }
    {
        if (ImGui::Begin("Static-Memory"))
        {
            auto& Assembly = _RunTimeState.Get_Assembly();
            void* Memptr = _RunTimeState.Get_StaticMemPtr();
            ImGui::Columns(2, "Debug/Raw Static Memory");
            {
                auto& DebugInfo = _RunTimeState.Get_Libs().Get_DebugInfo();
                for (auto& Item : DebugInfo.VarablesInfo)
                {
                    if (auto Val = Item.second.TypeLoc.Get_If<UCodeLang::VarableInfo::Static>())
                    {
                        void* Object = (void*)((uintptr_t)Memptr + (uintptr_t)Val->offset);

                        ImGui::Text(("offset:" + std::to_string(Val->offset)).c_str());
                        ImGui::SameLine();
                        ImguiHelper::UCodeObjectField(Item.first.c_str(), Object, Item.second.ReflectionType, Assembly);
                    }
                }

            }
            ImGui::NextColumn();
            {
                static MemoryEditor V;
                V.DrawContents(Memptr, _RunTimeState.Get_Libs().GetStaticBytes().size());
            }
        }
        ImGui::End();
    }
    //if (windowdata.ShowThreadMemory)
    {
        if (ImGui::Begin("Thread-Memory"))
        {
            auto& Assembly = _RunTimeState.Get_Assembly();
            void* Memptr = _AnyInterpreter.GetThreadPtr();
            ImGui::Columns(2, "Debug/Raw Thread Memory");
            {
                auto& DebugInfo = _RunTimeState.Get_Libs().Get_DebugInfo();
                for (auto& Item : DebugInfo.VarablesInfo)
                {
                    if (auto Val = Item.second.TypeLoc.Get_If<UCodeLang::VarableInfo::Thread>())
                    {
                        void* Object = (void*)((uintptr_t)Memptr + (uintptr_t)Val->offset);

                        ImGui::Text(("offset:" + std::to_string(Val->offset)).c_str());
                        ImGui::SameLine();
                        ImguiHelper::UCodeObjectField(Item.first.c_str(),Object, Item.second.ReflectionType, Assembly);
                    }
                }

            }
            ImGui::NextColumn();
            {
                static MemoryEditor V;
                V.DrawContents(Memptr, _RunTimeState.Get_Libs().GetThreadBytes().size());
            }
        }
        ImGui::End();

    }
}

void AppObject::OnRuntimeUpdated()
{
    UpdateInsData(windowdata);

    callFuncContext.current_method = nullptr;
    callFuncContext._LastRetType = UCodeLang::ReflectionTypeInfo();
}

void AppObject::FullReloadRunTime()
{
    _AnyInterpreter.UnLoad();

    _RuntimeLib.UnLoad();
    _RuntimeLib.Init(&_CompiledLib);

    LinkAppAPICallsTo(_RuntimeLib);
    ECSExample::ECSLink(_RuntimeLib);

    UCodeLang::SandBoxedIOLink::Link(_RuntimeLib);

    _RunTimeState.ClearRunTimeState();
    _RunTimeState.AddLib(&_RuntimeLib);
    _RunTimeState.LinkLibs();


    //ESC
    {
        ECSExample::State.ClearRunTimeState();
        ECSExample::State.AddLib(&_RuntimeLib);
        ECSExample::State.LinkLibs();

        ECSExample::_Context.Interpreter.Init(&ECSExample::State);
    }
    _AnyInterpreter.Init(&_RunTimeState);
    OnRuntimeUpdated();
}
void AppObject::HotReloadRunTime()
{
    UCodeLang::DebugContext::InterpreterInfo n;

    switch (_AnyInterpreter.Get_InterpreterType())
    {
    case  UCodeLang::InterpreterTypes::Interpreter:
    {
        n.ThisInterpreter = &_AnyInterpreter.GetAs_Interpreter();
        n.type = UCodeLang::DebugContext::Type::Interpreter;
    }
    break;
    case  UCodeLang::InterpreterTypes::Jit_Interpreter:
    {
        n.ThisInterpreter = &_AnyInterpreter.GetAs_JitInterpreter();
        n.type = UCodeLang::DebugContext::Type::Jit_Interpreter;
    }
    break;
    case  UCodeLang::InterpreterTypes::NativeInterpreter:
    {
        n.ThisInterpreter = &_AnyInterpreter.GetAs_NativeInterpreter();
        n.type = UCodeLang::DebugContext::Type::Native_Interpreter;
    }
    break;
    default:
        UCodeLangUnreachable();
        break;
    }
    UCodeLang::RunTimeLib teplib;
    teplib.Init(&_CompiledLib);
    LinkAppAPICallsTo(teplib);

    UCodeLang::RunTimeLangState::HotReloadLib reloadlib;
    reloadlib.LibToUpdate = &_RuntimeLib;
    reloadlib.NewLib = &teplib;


    UCodeLang::RunTimeLangState::HotReloadData hotreload;
    hotreload.Interpreters.push_back(n);
    hotreload.LibsToUpdate.push_back(reloadlib);

    if (!_RunTimeState.HotReload(hotreload))
    {
        FullReloadRunTime();
    }
    else 
    {
        OnRuntimeUpdated();
    }
}

void AppObject::OnPublishDiagnostics(const UCodeLanguageSever::json& Params)
{
    LS::PublishDiagnosticsParams params;
    UCodeLanguageSever::from_json(Params, params);

    PublishedDiagnostics = std::move(params);
    OnErrorListUpdated();
}

bool AppObject::LSPHasNoErrors()
{
    for (auto& Item : PublishedDiagnostics.diagnostics)
    {
        if (Item.severity.has_value())
        {
            if (Item.severity.value() == UCodeLanguageSever::DiagnosticSeverity::Error)
            {
                return false;
            }
        }
    }
    return  true;
}

void AppObject::SetRequestCallBack(UCodeLanguageSever::integer RequestID, RequestCallBack CallBack)
{
    auto& Item = RequestCallBacks.GetOrAdd(RequestID, {});
    Item.RequestID = RequestID;
    Item.CallBack = CallBack;
}

void AppObject::OnErrorListUpdated()
{
    TextEditor::ErrorMarkers marks;
    for (auto& Item : Errors)
    {
        marks[(int)Item._Error.Line] = Item._Error._Msg;
    }
    for (auto& Item : PublishedDiagnostics.diagnostics)
    {
        marks[(int)Item.range.start.line] = Item.message;
    }
    _Editor.SetErrorMarkers(marks);
}
const Path tepfilesdir = "tepfiles";
const Path tepfilepath = tepfilesdir / "src.uc";
void AppObject::CompileText(const String& String)
{
    if (IsRuningCompiler) { return; }

    _Compiler.Get_Errors().Remove_Errors();
    const Path tepoutpath = Outfilepath();
    const Path tepintpath = "int";

    std::filesystem::create_directory(tepfilesdir);

    UCodeLang::Compiler::CompilerPathData paths;

    paths.FileDir = tepfilesdir;
    paths.OutFile = tepoutpath;
    paths.IntDir = tepintpath;

    std::ofstream file(tepfilepath);
    file << String;
    file.close();

    auto& Settings = _Compiler.Get_Settings();
    Settings._Flags = OutputWindow.Flags;
    if (OutputWindow.InDebug) {
        Settings._Flags = (UCodeLang::OptimizationFlags)((UCodeLang::OptimizationFlags_t)Settings._Flags | (UCodeLang::OptimizationFlags_t)UCodeLang::OptimizationFlags::Debug);
    }

    IsRuningCompiler = true;
    bool AddStandardLibrary = UCodeLang::StringHelper::Contains(String, "ULang");//OutputWindow.ImportStandardLibrary;
    bool Apifile = UCodeLang::StringHelper::Contains(String, "use API");
    std::function<UCodeLang::Compiler::CompilerRet()> Func = [this, paths, AddStandardLibrary, Apifile]()
    {
        UCodeLang::Compiler::ExternalFiles ExternalFiles;



        if (AddStandardLibrary)
        {
            UCodeLang::ModuleFile f;
            Path modpath = Path(UCodeLang_SoultionDir) / "UCodeAPI";
            modpath /= "StandardLibrary";
            modpath /= UCodeLang::ModuleFile::FileNameWithExt;

            f.FromFile(&f, modpath);
            UCodeLang::ModuleIndex index = UCodeLang::ModuleIndex::GetModuleIndex();


            auto v = f.BuildModule(_Compiler, index, true);
            if (v.CompilerRet.IsError())
            {
                IsRuningCompiler = false;
                auto& r = v.CompilerRet;
                return std::move(r);
            }
            else
            {
                ExternalFiles.Files.push_back(v.OutputItemPath);
            }
        }


        UCodeLang::Compiler::CompilerRet r =UCodeLang::NeverNullptr(&_Compiler.Get_Errors());
        if (Apifile)
        {
            r = _Compiler.CompileFiles_UseIntDir(paths, ExternalFiles);
        }
        else
        {
            r = _Compiler.CompilePathToObj(tepfilepath, paths.OutFile, ExternalFiles);
        }
        IsRuningCompiler = false;
        return r;
    };

    _RuningPaths = std::move(paths);
    
    static bool ItWorked = false;
    if (ItWorked == false)
    {
        ItWorked = true;
        Func();
    }
    else
    { 
        #if NoUseThread

        NoThreadRetChecked = false;
        _NoThreadRuningCompiler =Func();
        #else
        _RuningCompiler = SendTaskToWorkerThread<UCodeLang::Compiler::CompilerRet>(Func);
        #endif
    }
    
}

void AppObject::OnDoneCompileing(UCodeLang::Compiler::CompilerRet& Val, const UCodeAnalyzer::Path& tepoutpath)
{
    NoThreadRetChecked = true;
    if (Val.IsValue())
    {
        Errors.clear();

        _CompilerStr = GetTextEditorString();
        switch (OutputWindow.Type)
        {
        case BackEndType::UCodeVM:
        {
            UCodeLang::UClib lib;
            UCodeLang::UClib::FromFile(&lib, tepoutpath);
            _CompiledLib = std::move(lib);

            _LibInfoString = UCodeLang::UAssembly::UAssembly::ToString(&_CompiledLib
            ,tepfilesdir,false);
        }
        break;
        case BackEndType::IR:
        {
            UCodeLang::IRBuilder ir;
            UCodeLang::IRBuilder::FromFile(ir, tepoutpath);
            _LibInfoString = ir.ToString();
        }
        break;

        case BackEndType::C89:
            _LibInfoString = _Compiler.GetTextFromFile(tepoutpath);
            break;
        case BackEndType::LLVM:
            _LibInfoString = _Compiler.GetTextFromFile(tepoutpath);
            break;
        case BackEndType::WebAssembly: 
        {
            UCodeLang::WasmFile file;
            UCodeLang::WasmFile::FromFile(tepoutpath, file);
            _LibInfoString = file.ToWat();
        }
        break;
        case BackEndType::WindowsExecutable:
        {
            _LibInfoString = "";
        }
        break;
        case BackEndType::LinuxExecutable:
        {
           
            _LibInfoString = "";
        }    
        break;
        default:
            UCodeLangUnreachable();
            break;
        }

        OnErrorListUpdated();

       
    }
    else
    {
        Errors.clear();
        for (auto& Item : _Compiler.Get_Errors().Get_Errors())
        {
            Errors.push_back({ Item });
        }
        OnErrorListUpdated();
    }

    bool HasFailed = _Compiler.Get_Errors().Has_Errors() || GetTextEditorString() != _CompilerStr;
    if (HasFailed)
    {
        AutoCompileTimeOut = 5;
    }
    else
    {
        if (OutputWindow.AutoReload)
        {
            if (OutputWindow.AutoHotReload)
            {
                if (_RuntimeLib.Get_Lib())
                {
                    HotReloadRunTime();
                }
                else
                {
                    FullReloadRunTime();
                }
            }
            else
            {
                FullReloadRunTime();
            }
        }
    }
}

size_t AppObject::GetColumn(const String& text,size_t line,size_t Pos)
{
    size_t OnLine = 0;
    size_t OnColumn = 0;
    for (size_t i = 0; i < text.size(); i++)
    {
        if (text[i] == '\n')
        {
            OnLine++;
        }

        if (OnLine == Pos)
        {
            OnColumn++;
        }
        if (i == Pos)
        {
            break;
        }
    }
    return OnColumn;
}

void AppObject::OnAppEnd()
{
    if (_LangSeverThread)
    {
        bool IsShutingDown = false;

        {
            LS::DidCloseTextDocumentParams p;
            p.textDocument.uri = EditorTextURL;
            SendDidCloseTextDocument(p);
        }
        SendShutdoneRequest().SetCallBack([&IsShutingDown,this](auto Unused)
            { 
                IsShutingDown = true;
                SendExitNotification();
            });

        while (IsShutingDown == false)
        {
            ProcessSeverPackets();
        }
        
        _LangSeverThread->join();
    }
}
void AppObject::OnSeverPacket(SPacket&& packet)
{
    auto DataOp = packet.Parse();
    if (DataOp.has_value())
    {
        auto& Data = DataOp.value();
        if (auto Val = Data.Type.Get_If<SPacket::RequestMessage_t>())
        {
            if (AppClientFuncMap::RequestFuncs.count(Val->method))
            {
                auto func = AppClientFuncMap::RequestFuncs.at(Val->method);
                (*this.*func)(Val->id,Val->params);
            }
        }
        else  if (auto Val = Data.Type.Get_If<SPacket::NotificationMessage_t>())
        {
            if (AppClientFuncMap::NotificationFuncs.count(Val->method))
            {
                auto func = AppClientFuncMap::NotificationFuncs.at(Val->method);
                (*this.*func)(Val->params);
            }
        }
        else if (auto Val = Data.Type.Get_If<SPacket::ResponseMessage_t>())
        {
            if (RequestCallBacks.HasValue(Val->id))
            {
                auto& Item = RequestCallBacks.GetValue(Val->id);
                if (Item.CallBack)
                {
                    Item.CallBack(*Val);
                }

                RequestCallBacks.erase(Val->id);
            }

        }
    }
}

UCodeIDEEnd