#include "AppObject.hpp"
#include "imgui/imgui.h"
#include "ImGuiHelpers/ImguiHelper.hpp"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"


#include "UCodeLang/Compliation/Back/UCodeBackEnd/UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Back/C89/C89Backend.hpp"
#include "UCodeLang/Compliation/Back/IR/IRBackEnd.hpp"
#include "UCodeLang/Compliation/Back/LLVM/LLVMBackEnd.hpp"
#include "UCodeLang/Compliation/Back/WebAssembly/WebAssembly.hpp"
#include "UCodeLang/Compliation/Back/Windows/WindowsBackEnd.hpp"
#include "UCodeLang/Compliation/Back/Linux/LinuxBackEnd.hpp"

#include "UCodeLang/Compliation/Back/Windows/PE_File.hpp"
//#include "UCodeLang/Compliation/Back/Linux/ELF_File.hpp"
//#include <elfio/elfio_dump.hpp>

#include "UCodeLang/Compliation/Back/x86_64/X86_64UNativeBackEnd.hpp"
#include "UCodeLang/Compliation/Back/x86_64/X86_64JitCompiler.hpp"

#include <fstream>
#include <filesystem>
#include "ImGuiHelpers/imgui_memory_editor/imgui_memory_editor.h"
#include <tests/Test.hpp>
#include <future>
#include <sstream>
#if UCodeLang_Platform_Windows
#include <Windows.h>
#elif UCodeLang_Platform_Posix

#else

#endif 

UCodeIDEStart


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


        _LangSeverThread = std::make_unique<std::thread>([this]()
            {
                SandBoxLanguageSever SandBox;
                this->SeverPtr = &SandBox;
                while (SandBox._Sever.Step());
                this->SeverPtr = nullptr;
            });


        TextEditor::LanguageDefinition Def;
        Def.mName = "UCodeLang";
        Def.mTokenize = [](const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end, TextEditor::PaletteIndex& paletteIndex)
        {


            return false;
        };

        _Editor.SetShowWhitespaces(false);
        //_Editor.SetLanguageDefinition(Def);
        /*
        r += ToString(I->ObjectType);
		r += " " + State.GetName(I);
		r += " = ";
		r += "await::new(" + ToString(State, *I, I->Target()) + ")";
        */
      
        UCodeIDEStyle(nullptr);
        /*
        |func[] => 0;
|main[] => 1;
|coolerror[] -> int!bool;
|coolmultypes[] -> int||bool;
|coolmultypes3[] -> int||bool||char;
|CallocTest[] =>unsafe new int[10];

|RangeTest[] => 1..10;
|RangeTest2[] => 1..=10;
$Range_t<T>:
 T start;
 T end;
 |new[this&,T Start,T End]:
  start = Start;
  end = End;
$RangeInclusive_t<T>:
 T start;
 T end;
 |new[this&,T Start,T End]:
  start = Start;
  end = End;

|Range<T>[T start,T end] => Range_t<T>(start,end);
|RangeInclusive<T>[T start,T end] =>RangeInclusive_t<T>(start,end);

$Result<T,E> enum:
 Value[T val],
        */
        _Editor.SetText(
            R"(
|main[] => 0;

/*
|main[] -> async<bool>:
 async<int> a = await func();
 yield a;
*/ 

//yield await [] => 0;
//ret 0;

//$Future<T>;
/*
IntVector VectorTest = [];
String StringTest = [];

//A simplified standard Library below.

$Vec2:
 int X = 0;
 int Y = 0;

$Vec3:
 int X = 0;
 int Y = 0;
 int Z = 0;

//Syntactic alias: T? = Optional<T>
$Optional<T> enum:
 Value[T val],
 None,

//Syntactic alias: T!E = Result<T,E>
$Result<T,E> enum:
 Value[T val],
 Error[E err],

$OpInt = int?;//make type.
$Opbool = bool?;//make type.
$Opchar = char?;//make type.

$IntVector = int[];



|NullPtr<T>[] => bitcast<T>(0);
|NullPtrArr<T>[] => bitcast<T[&]>(0);
|AreSamePtrArr<T>[umut T[&] A,umut T[&] B] => bitcast<uintptr>(A) == bitcast<uintptr>(B);
|IsNullPtrArr<T>[umut T[&] Ptr] => bitcast<uintptr>(Ptr) == uintptr(0);

//Syntactic alias: T[] = Vector<T>
$Vector<T>:
 private:
  T[&] _Data = NullPtrArr<T>();
  uintptr _Capacity = 0;
  uintptr _Size = 0;
  |Data[umut this&] -> T[&]:ret _Data;//So  ClassAssembly::IsString_t will work.
 public: 
  |Size[umut this&] => _Size;
  |Capacity[umut this&] => _Capacity;
 
  |Resize[this&,uintptr size] -> void:
   Reserve(size);
   _Size = size;
  |Reserve[this&,uintptr size] -> void:

    if IsNullPtrArr(_Data):
      drop(_Data);

    _Data =: new T[size]; 
    _Capacity = size;

  |Clear[this&]:_Size = 0;

  |Push[this&,moved T Item] => Insert(Item,_Size);
  |Push[this&,umut T& Item] => Insert(Item,_Size);
  |Pop[this&] => Remove(_Size - uintptr(1));

  |Remove[this&,uintptr Index] -> T;
 
  |Insert[this&,moved T Item,uintptr Index] -> void:
   //_Size++;
   //if _Capacity < _Size:
   //Resize(_Size);

   //_Data[Index] = Item;
    
  |Insert[this&,umut T& Item,uintptr Index] -> void:
   //_Size++;
   //if _Capacity < _Size:
   //Resize(_Size);

   //_Data[Index] = Item;

$String = String_t<char>;

$String_t<T>:
 private:
  Vector<T> Base = [];
  |Data[umut this&] => Base.Data();
 public: 
  
  |Size[umut this&] => Base.Size();
  |Capacity[umut this&] => Base.Capacity();

  |Resize[this&,uintptr size] => Base.Resize(size);
  |Clear[this&] => Base.Clear();
  |Push[this&, T Item] => Base.Push(Item);
  |Pop[this&] => Base.Pop();
  |Remove[this&,uintptr Index] => Base.Remove(Index);
  |Insert[this&,uintptr Index, T Item] => Base.Insert(Item,Index);

//inlined enum variant: X || Y || Z
//$InlinedEnum = int || bool || char;

*/
            )");




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
void AppObject::OnDraw()
{
    thread_local bool SetTextFocus = false;
    {
        auto now = SteadyClock::now();

        auto TimePassed =  now - LastFrame;

        DetaTime = std::chrono::duration_cast<std::chrono::duration<float>>(TimePassed).count();


        LastFrame = now;
    }
    
    ProcessSeverPackets();

    {
        if (IsRuningCompiler == false && _RuningCompiler.valid())
        {
            auto compilerret =_RuningCompiler.get();
            OnDoneCompileing(compilerret, _RuningPaths.OutFile);
        }
    }

    bool Doc = true;
    BeginDockSpace(&Doc);

    //ImGui::ShowDemoWindow();


    if (ImGui::Begin("ShowStyleEditor"))
    {
        ImGui::ShowStyleEditor();

    }  ImGui::End();


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


                _Editor.SetCursorPosition(TextEditor::Coordinates(item._Error.Line, GetColumn(text, item._Error.Line, item._Error.Pos)));
                // handle selection
            }
        }
      
        //ImGui::EndListBox();


        ImGui::PopItemWidth();
    } ImGui::End();

#if UCodeLangDebug   

    static constexpr size_t TestCount = ULangTest::Tests.size();
    struct TestInfo
    {
        bool TestAsRan = false;
        enum class TestState
        {
            Null,
            Passed,
            Fail,
            Exception
        };
        

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
            bool LogErrors(std::ostream& out,UCodeLang::Compiler& _Compiler)
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
            bool RunTestForFlag(const ULangTest::TestInfo& Test,UCodeLang::OptimizationFlags flag)
            {
               
                Logs.clear();

                using namespace UCodeLang;
                using namespace ULangTest;

                Compiler::CompilerPathData paths;
                Compiler Com;
                Com.Get_Settings()._Flags = flag;
                Com.Get_Settings().PtrSize = IntSizes::Native;

                Compiler::CompilerRet Com_r;
                std::string InputFilesPath = UCodeLang_UCAppDir_Test_UCodeFiles + Test.InputFilesOrDir;
                std::string OutFileDir = UCodeLang_UCAppDir_Test_OutputFiles + Test.TestName;
                std::filesystem::path p = OutFileDir;
                OutFileDir = p.parent_path().generic_string() + "/" + +Test.TestName + "/";

                std::filesystem::create_directories(OutFileDir);
                std::string OutFilePath = OutFileDir + Test.TestName + ULangTest::ModeType(flag) + ".ulibtest";

               

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
                    Logs += "fail from Compile [exception] '" + (String)ex.what() + "' : " + "'"  + Test.TestName + "'" + '\n';
                    //ErrStream << "fail from Compile [exception] '" << ex.what() << "' : " << "'" << Test.TestName << "'" << std::endl;
                    return false;
                }


                if (Test.Condition == SuccessCondition::Compilation
                    || Test.Condition == SuccessCondition::CompilationFail)
                {
                    if (
                        (Com_r._State == Compiler::CompilerState::Success && Test.Condition == SuccessCondition::Compilation)
                        ||
                        (Com_r._State == Compiler::CompilerState::Fail && Test.Condition == SuccessCondition::CompilationFail)
                        )
                    {
                        Logs += "Success from test '" +  (String)Test.TestName + ModeType(flag) + "'" + '\n';
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

                if (Com_r._State != Compiler::CompilerState::Success)
                {
                    Logs += (String)"fail from test [Cant Compile File/Files] '" + String(Test.TestName) + ModeType(flag) + "'" + '\n';

                    std::stringstream errs;
                    LogErrors(errs, Com);
                    Logs += errs.str();
                    State = TestState::Fail;
                    return false;
                }

                
                UClib lib;
                if (!UClib::FromFile(&lib, OutFilePath))
                {
                    State = TestState::Fail;

                    Logs += (String)"fail from test [Cant Open ULib File] '" + Test.TestName + ModeType(flag) + "'" +  '\n';
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
                       r = RunTime.Call(Test.FuncToCall);

                    }
                    catch (const std::exception& ex)
                    {
                        State = TestState::Exception;

                        Logs += (String)"fail from test [exception] '" + ex.what() + "' : " + "'" + Test.TestName + "'" + ModeType(flag) + '\n';
                        return false;
                    }

                    if (Test.Condition == SuccessCondition::RunTimeValue)
                    {
                        std::unique_ptr<Byte[]> RetState = std::make_unique<Byte[]>(Test.RunTimeSuccessSize);
                        RunTime.Get_Return(RetState.get(), Test.RunTimeSuccessSize);

                        String Type = "Interpreter";

                        bool IsSame = true;
                        for (size_t i = 0; i < Test.RunTimeSuccessSize; i++)
                        {
                            if (RetState[i] != Test.RunTimeSuccess[i])
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
                            Logs += OutputBytesToString(RetState.get(), Test.RunTimeSuccessSize);

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
                return "";
                break;
            }
        }
        bool DebugMode = true;
        UCodeLang::OptimizationFlags Flags;
    };
    static TestInfo TestWindowData;

    if (ImGui::Begin("Tests"))
    {
        //Set These to make fixing tests more mangeabe 
        size_t MinTestIndex = 0;
        size_t MaxTestCount = 10;//ULangTest::Tests.size()


        UCodeLang::OptimizationFlags flags = TestWindowData.Flags;

        if (TestWindowData.DebugMode)
        {
            *(UCodeLang::OptimizationFlags_t*)&flags |= (UCodeLang::OptimizationFlags_t)UCodeLang::OptimizationFlags::Debug;
        }
        

        thread_local UCodeLang::BinaryVectorMap<String, String> Openedfiles;
        thread_local UCodeLang::BinaryVectorMap<String,UCodeLang::Optional<std::shared_ptr<UCodeLang::UClib>>> Outputfiles;
        thread_local UCodeLang::BinaryVectorMap<String, String> OutputIRStr;
        thread_local UCodeLang::BinaryVectorMap<String, String> OutputLibStr;
        if (ImGui::Button("Run Tests"))
        {
            Openedfiles.clear();
            Outputfiles.clear();
            OutputIRStr.clear();
            OutputLibStr.clear();

            TestWindowData.TestAsRan = true;
            const auto& Tests = ULangTest::Tests;
            for (size_t i = 0; i < MaxTestCount; i++)
            {
                auto& ItemTest = ULangTest::Tests[i];
                auto& ItemTestOut = TestWindowData.Testinfo[i];
                auto& Thread = TestWindowData.Threads[i];
               

                Thread = std::make_unique< std::future<bool>>(std::async(std::launch::async, [i,flags]
                    {
                        auto& ItemTest = ULangTest::Tests[i];
                        auto& ItemTestOut = TestWindowData.Testinfo[i];

                        ItemTestOut.State == TestInfo::TestState::Exception;
                        ItemTestOut.RunTestForFlag(ItemTest, flags);
                        return false;
                    }));
            }
        }
        bool FlagsWasUpdated = false;
        if (ImguiHelper::BoolEnumField("DebugFlag", TestWindowData.DebugMode))
        {
            
            FlagsWasUpdated = true;
        }
       
        static const Vector<ImguiHelper::EnumValue<UCodeLang::OptimizationFlags>> OptimizationList =
        {
            { "NoOptimization",UCodeLang::OptimizationFlags::NoOptimization},
            { "ForSize",UCodeLang::OptimizationFlags::ForSize},
            { "ForSpeed",UCodeLang::OptimizationFlags::ForSpeed},
            { "SizeAndSpeed",UCodeLang::OptimizationFlags::ForMaxSpeed},
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
        for (size_t i = MinTestIndex; i < MaxTestCount; i++)
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
        info += std::to_string(MaxTestCount- MinTestIndex);
        info += " :" + std::to_string((int)(((float)TestPassedCount / (float)(MaxTestCount- MinTestIndex)) * 100));
        info += " percent";
        ImGui::Text(info.c_str());
        }
        {
            String info;
            info += "TestRuning:";
            info += std::to_string(TestRuningCount);
            info += "/";
            info += std::to_string(MaxTestCount- MinTestIndex);
            info += " :" + std::to_string((int)(((float)TestRuningCount / (float)(MaxTestCount- MinTestIndex)*100)));
            info += " percent";
            ImGui::Text(info.c_str());
        }
        static const Vector<ImguiHelper::EnumValue<TestInfo::TestState>> List =
        {
            { "Null ",TestInfo::TestState::Null},
            { "Passed",TestInfo::TestState::Passed},
            { "Fail",TestInfo::TestState::Fail},
            { "Exception",TestInfo::TestState::Exception},
        };

        ImguiHelper::InputText("Sort:", TestWindowData.SortByName);
        ImGui::SameLine();
        ImguiHelper::EnumField("Type:", TestWindowData.SortBy, List);

        ImGui::Separator();

        for (size_t i = MinTestIndex; i < MaxTestCount; i++)
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
                String TestV = "Test:";
                TestV += ItemTest.TestName;
                TestV += ",State:" + TestInfo::GetToString(ItemTestOut.State);
                if (Thread.get())
                {
                    if (!Thread->valid())
                    {
                        TestV += ",Working...";
                    }
                }

                if (ImGui::TreeNode(TestV.c_str()))
                {
                    bool IsWorking = false;

                    if (Thread.get())
                    {
                        if (!Thread->valid())
                        {
                            IsWorking = true;
                        }
                    }

                    ImGui::BeginDisabled();
                    
                    String tepstr1 = ItemTest.TestName;
                    ImguiHelper::InputText("TestName", tepstr1);
                    
                    String tepstr2 = ItemTest.InputFilesOrDir;
                    ImguiHelper::InputText("TestPath", tepstr2);

                    //ImGui::SameLine();
                    ImGui::EndDisabled();


                    if (ImGui::Button("Show in files"))
                    {
                        ShowInFiles(UCodeLang_UCAppDir_Test_UCodeFiles + ItemTest.InputFilesOrDir);
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
                            Openedfiles.AddValue(ItemTest.TestName,filetxt);
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
                            Outputfiles.AddValue(ItemTest.TestName,std::make_shared<UCodeLang::UClib>(std::move(lib)));
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
                                OutputIRStr.AddValue(ItemTest.TestName,str);
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
                                OutputLibStr.AddValue(ItemTest.TestName,filetxt);
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

                    }

                    ImGui::TreePop();
                }
            }

        }

    }ImGui::End();
#endif // DEBUG

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
                _BackEnd = UCodeLang::C89Backend::MakeObject;
                break;
            case BackEndType::IR:
                _BackEnd = UCodeLang::IRBackEnd::MakeObject;
                break;
            case BackEndType::LLVM:
                _BackEnd = UCodeLang::LLVMBackEnd::MakeObject;
                break;
            case BackEndType::WebAssembly:
                _BackEnd = UCodeLang::WebAssemblyBackEnd::MakeObject;
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

        if (ImguiHelper::EnumField("Optimizations", OutputWindow.Flags, OpflagList))
        {
            CompileText(GetTextEditorString());
        }
        if (ImguiHelper::BoolEnumField("In Debug Mode", OutputWindow.InDebug))
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
    #if UCodeLang_Platform_Windows
    ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
    #elif UCodeLang_Platform_Posix
    auto cmd =(String("open ") + path.generic_string());
    system(cmd.c_str());
    #else //MacOS

    #endif 
}

void AppObject::ShowUCodeVMWindow()
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

                            //ImGui::TableNextColumn(); 
                            //ImGui::SetColumnWidth(0, 20.0f);
                            //ImGui::Dummy({20,20});

                            ImGui::TableNextColumn();

                            //mGui::TableNextColumn(0, 20.0f);

                            ImGui::Text(std::to_string(Item.InsAddress).c_str());

                            ImGui::TableNextColumn();

                            ImGui::Text(Item.StringValue.c_str());

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

void AppObject::UpdateInsData(UCodeVMWindow& windowdata)
{
    using namespace UCodeLang::UAssembly;
    windowdata.InsInfo.clear();
    auto& RunTime = _RunTimeState;

    windowdata.InsInfo.reserve(RunTime.Get_Libs().GetInstructions().size());


    auto& InsMapData = UCodeLang::UAssembly::Get_InsToInsMapValue();
    BinaryVectorMap<UCodeLang::UAddress, String> AddressToName;


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

void AppObject::ShowDebugerMenu(UCodeVMWindow& windowdata)
{

    ImGui::Separator();

    bool IsinFileMode = false;
    ImVec2 Buttonsize = { 80,20 };

    bool InFuncion = false;

    ImGui::Button("Reset", Buttonsize);

    ImguiHelper::BoolEnumField("Call Static/Thread Init On Reload", windowdata.CallStaticVarOnReload);

    ImGui::BeginDisabled(!InFuncion);

    ImGui::Button("Step in", Buttonsize); ImGui::SameLine();
    ImGui::Button("Step over", Buttonsize); ImGui::SameLine();
    ImGui::Button("Step out", Buttonsize);

    ImGui::EndDisabled();

    if (windowdata.VMType == UCodeVMType::Jit_Interpreter)
    {
        if (ImGui::Button("Jit-Precompile All", Buttonsize))
        {
            _AnyInterpreter.GetAs_JitInterpreter().TryBuildAllFuncs();
        }
    }

    if (!InFuncion) {
        ImGui::Text("Varables");
    }

    if (!InFuncion) {
        ImGui::Text("StackFrames");
    }

    if (!InFuncion) 
    {
        auto& Assembly = _RunTimeState.Get_Assembly();
        bool Updated = false;
      

        auto GlobalObject = Assembly.Get_GlobalObject_Class();

        ImGui::Text("Call Funcion:"); ImGui::SameLine();
        
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
                    ImguiHelper::UCodeObjectField(ParName.c_str(), (void*)Arg.Data(), Par, Assembly);
                }

                if (windowdata.CallStaticVarOnReload == false) {
                    ImguiHelper::BoolEnumField("Call Stack/Thread init", callFuncContext.CallStaticAndThreadInit);
                    ImguiHelper::BoolEnumField("Call Stack/Thread de-init", callFuncContext.CallStaticAndThreadDeInit);

                }
                if (ImGui::Button(((String)"Call:" + MethodString).c_str()))
                {
                    callFuncContext._LastRetType = callFuncContext.current_method->RetType;
                    callFuncContext._LastRet.Resize(Assembly.GetSize(callFuncContext._LastRetType, Is32bits).value_or(0));

                    for (size_t i = 0; i < callFuncContext.current_method->ParsType.size(); i++)
                    {
                        auto& Arg = callFuncContext.Args[i];
                        _AnyInterpreter.PushParameter(Arg.Data(), Arg.Size());
                    }
                    if (windowdata.CallStaticVarOnReload || callFuncContext.CallStaticAndThreadInit)
                    {
                        _AnyInterpreter.Call(StaticVariablesInitializeFunc);
                        _AnyInterpreter.Call(ThreadVariablesInitializeFunc);
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
                ImGui::BeginDisabled();
                if (callFuncContext._LastRetType == callFuncContext.current_method->RetType)
                {
                    ImguiHelper::UCodeObjectField("Returned", callFuncContext._LastRet.Data(),callFuncContext._LastRetType, Assembly);
                }
                ImGui::EndDisabled();
            }
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
                Mems.push_back({ false,Item._Key,Item._Value });
            }
            for (auto& Item : ReservedAlocsInfo)
            {
                Mems.push_back({ true,Item._Key,Item._Value });
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
                    if (Staticptr == Item._Ptr)
                    {
                        InfoStr += ",--StaticMem";
                    }
                    if (Threadptr == Item._Ptr)
                    {
                        InfoStr += ",--ThreadMem";
                    }
                    if (_AnyInterpreter.Get_InterpreterType() == UCodeLang::InterpreterTypes::Interpreter)
                    {
                        auto& Inter = _AnyInterpreter.GetAs_Interpreter();
                        if (Inter.GetStackSpan().Data() == Item._Ptr) {
                            InfoStr += ",--StackMem";
                        }
                    }
                    if (ImGui::TreeNode(InfoStr.c_str()))
                    {
                        Isshowing[i] = true;
                        if (Staticptr == Item._Ptr)
                        {
                            auto& DebugInfo = _RunTimeState.Get_Libs().Get_DebugInfo();
                            for (auto& Item : DebugInfo.VarablesInfo)
                            {
                                if (auto Val = Item._Value.TypeLoc.Get_If<UCodeLang::VarableInfo::Static>())
                                {
                                    void* Object = (void*)((uintptr_t)Staticptr + (uintptr_t)Val->offset);

                                    ImGui::Text(("offset:" + std::to_string(Val->offset)).c_str());
                                    ImGui::SameLine();
                                    ImguiHelper::UCodeObjectField(Item._Key.c_str(), Object, Item._Value.ReflectionType, Assembly);
                                }
                            }
                        }
                        else if (Threadptr == Item._Ptr)
                        {
                            auto& DebugInfo = _RunTimeState.Get_Libs().Get_DebugInfo();
                            for (auto& Item : DebugInfo.VarablesInfo)
                            {
                                if (auto Val = Item._Value.TypeLoc.Get_If<UCodeLang::VarableInfo::Thread>())
                                {
                                    void* Object = (void*)((uintptr_t)Threadptr + (uintptr_t)Val->offset);

                                    ImGui::Text(("offset:" + std::to_string(Val->offset)).c_str());
                                    ImGui::SameLine();
                                    ImguiHelper::UCodeObjectField(Item._Key.c_str(), Object, Item._Value.ReflectionType, Assembly);
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
                int a = 0;
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
                    if (auto Val = Item._Value.TypeLoc.Get_If<UCodeLang::VarableInfo::Static>())
                    {
                        void* Object = (void*)((uintptr_t)Memptr + (uintptr_t)Val->offset);

                        ImGui::Text(("offset:" + std::to_string(Val->offset)).c_str());
                        ImGui::SameLine();
                        ImguiHelper::UCodeObjectField(Item._Key.c_str(), Object, Item._Value.ReflectionType, Assembly);
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
                    if (auto Val = Item._Value.TypeLoc.Get_If<UCodeLang::VarableInfo::Thread>())
                    {
                        void* Object = (void*)((uintptr_t)Memptr + (uintptr_t)Val->offset);

                        ImGui::Text(("offset:" + std::to_string(Val->offset)).c_str());
                        ImGui::SameLine();
                        ImguiHelper::UCodeObjectField(Item._Key.c_str(),Object, Item._Value.ReflectionType, Assembly);
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

    _RunTimeState.ClearRunTimeState();
    _RunTimeState.AddLib(&_RuntimeLib);
    _RunTimeState.LinkLibs();

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
    auto& Item = RequestCallBacks[RequestID];
    Item.RequestID = RequestID;
    Item.CallBack = CallBack;
}

void AppObject::OnErrorListUpdated()
{
    TextEditor::ErrorMarkers marks;
    for (auto& Item : Errors)
    {
        marks[Item._Error.Line] = Item._Error._Msg;
    }
    for (auto& Item : PublishedDiagnostics.diagnostics)
    {
        marks[Item.range.start.line] = Item.message;
    }
    _Editor.SetErrorMarkers(marks);
}

void AppObject::CompileText(const String& String)
{
    if (IsRuningCompiler) { return;}

    _Compiler.Get_Errors().Remove_Errors();
    const Path tepfilesdir = "tepfiles";
    const Path tepfilepath = tepfilesdir / "src.uc";
    const Path tepoutpath = "out.data";
    
    std::filesystem::create_directory(tepfilesdir);

    UCodeLang::Compiler::CompilerPathData paths;
    paths.FileDir = tepfilesdir.generic_string();
    paths.OutFile = tepoutpath.generic_string();

    std::ofstream file(tepfilepath);
    file << String;
    file.close();

    auto& Settings = _Compiler.Get_Settings();
    Settings._Flags = OutputWindow.Flags;
    if (OutputWindow.InDebug) {
        Settings._Flags = (UCodeLang::OptimizationFlags)((UCodeLang::OptimizationFlags_t)Settings._Flags | (UCodeLang::OptimizationFlags_t)UCodeLang::OptimizationFlags::Debug);
    }
    auto r = _Compiler.CompileFiles(paths);
    
    IsRuningCompiler = true;
    std::function<UCodeLang::Compiler::CompilerRet()> Func = [this, paths]()
    {
       auto r = _Compiler.CompileFiles(paths);
       IsRuningCompiler = false;
       return r;
    };
     
    _RuningPaths = std::move(paths);
    _RuningCompiler = SendTaskToWorkerThread<UCodeLang::Compiler::CompilerRet>(Func);

   
}

void AppObject::OnDoneCompileing(UCodeLang::Compiler::CompilerRet& Val, const UCodeAnalyzer::Path& tepoutpath)
{
    if (Val._State == UCodeLang::Compiler::CompilerState::Success)
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

            _LibInfoString = UCodeLang::UAssembly::UAssembly::ToString(&_CompiledLib);
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
            _LibInfoString = _Compiler.GetTextFromFile(tepoutpath);
            break;
        case BackEndType::WindowsExecutable:
        {
            /*
            enma::pe_image image(tepoutpath);

            _LibInfoString += "Portable Executable:\n";


            {
                _LibInfoString += " Machine:";

                switch (image.get_machine())
                {
                case IMAGE_FILE_MACHINE_AMD64:
                    _LibInfoString += "AMD64";
                    break;
                case IMAGE_FILE_MACHINE_UNKNOWN:
                default:
                    _LibInfoString += "UNKNOWN";
                    break;
                }
                _LibInfoString += '\n';
                _LibInfoString += " SectionsNumber:";
                _LibInfoString += std::to_string(image.get_sections_number());
             
                _LibInfoString += '\n';
                _LibInfoString += " Characteristics";
                if (image.get_characteristics() & IMAGE_FILE_EXECUTABLE_IMAGE)
                {
                    _LibInfoString += ":EXECUTABLE";
                }
                _LibInfoString += '\n';
                _LibInfoString += " DllCharacteristics";
                if (image.get_characteristics_dll() & IMAGE_DLLCHARACTERISTICS_NX_COMPAT)
                {
                    _LibInfoString += ":NX_COMPAT";
                }
                if (image.get_characteristics_dll() & IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA)
                {
                    _LibInfoString += ":HIGH_ENTROPY_VA";
                }
                if (image.get_characteristics_dll() & IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE)
                {
                    _LibInfoString += ":DYNAMIC_BASE";
                }
                if (image.get_characteristics_dll() & IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE)
                {
                    _LibInfoString += ":TERMINAL_SERVER_AWARE";
                }
                _LibInfoString += '\n';
                _LibInfoString += " Subsystem:";
                switch (image.get_sub_system())
                {
                case IMAGE_SUBSYSTEM_WINDOWS_CUI:
                    _LibInfoString += "CUI";
                    break;
                case IMAGE_SUBSYSTEM_WINDOWS_GUI:
                    _LibInfoString += "GUI";
                    break;
                default:
                    _LibInfoString += "UNKNOWN";
                    break;
                }
                _LibInfoString += " Sections:\n";
                for (auto& Item : image.get_sections())
                {
                    _LibInfoString += "\n  ";
                    _LibInfoString += Item->get_section_name();
                    _LibInfoString += ":\n";

                    _LibInfoString += "   Characteristics";
                    if (Item->get_characteristics() & IMAGE_SCN_MEM_EXECUTE)
                    {
                        _LibInfoString += ":EXECUTABLE";
                    }
                    if (Item->get_characteristics() & IMAGE_SCN_MEM_WRITE)
                    {
                        _LibInfoString += ":WRITE";
                    }
                    if (Item->get_characteristics() & IMAGE_SCN_MEM_READ)
                    {
                        _LibInfoString += ":READ";
                    }
                    _LibInfoString += '\n';
                    _LibInfoString += "   Data:"; 
                    auto& datas = Item->get_section_data();

                }

            }
            */
            _LibInfoString = "";
        }break;
        case BackEndType::LinuxExecutable:
        {
            /*
            std::stringstream s;
            ELFIO::elfio reader;
            reader.load(tepoutpath.generic_string());
            ELFIO::dump::header(s, reader);
            ELFIO::dump::section_headers(s, reader);
            ELFIO::dump::symbol_tables(std::cout, reader);
            ELFIO::dump::segment_datas(std::cout, reader);

            _LibInfoString = s.str();
            */
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
                auto& Item = RequestCallBacks.at(Val->id);
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