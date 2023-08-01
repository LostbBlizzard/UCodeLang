#include "AppObject.hpp"
#include "imgui/imgui.h"
#include "LanguageSever.hpp"
#include "ImGuiHelpers/ImguiHelper.hpp"
#include "Imgui/misc/cpp/imgui_stdlib.h"
#include "UCodeLang/Compliation/UAssembly/UAssembly.hpp"


#include "UCodeLang/Compliation/Back/UCodeBackEnd/UCodeBackEnd.hpp"
#include "UCodeLang/Compliation/Back/C89/C89Backend.hpp"
#include "UCodeLang/Compliation/Back/IR/IRBackEnd.hpp"
#include <fstream>
#include <filesystem>
#include "ImGuiHelpers/imgui_memory_editor/imgui_memory_editor.h"
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

		UCodeIDEStyle(nullptr);

        _Editor.SetText("\n\n|main[] => 0;\n\n");
        CompileText(GetTextEditorString());


        _AnyInterpreter.SetAsInterpreter();
        _AnyInterpreter.Init(&_RunTimeState);
        ImguiHelper::_Ptr = _AnyInterpreter.GetPtr();
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
void EndDockSpace()
{
    ImGui::End();//EndDockSpace
}

void AppObject::OnDraw()
{
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
       
        _Editor.Render("File.uc");
       
    } ImGui::End();

    if (ImGui::Begin("Error List"))
    {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);
        //ImGui::ListBoxHeader("Errors");
        for (auto& item : Errors)
        {
            
            if (ImGui::Selectable(item._Error._Msg.c_str(), item.IsSelected))
            {
                auto text = GetTextEditorString();
                
               
                _Editor.SetCursorPosition(TextEditor::Coordinates(item._Error.Line, GetColumn(text, item._Error.Line, item._Error.Pos)));
                // handle selection
            }
        }
        //ImGui::EndListBox();
   
       
        ImGui::PopItemWidth();
    } ImGui::End(); 
    
    
  

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
        };

        if (ImguiHelper::EnumField("Type", OutputWindow.Type, List))
        {
            _CompilerStr = "";
            _RunTimeStr = "";
            _LibInfoString = "";
            UCodeLang::BackEndObject_Ptr _BackEnd;
            
            switch (OutputWindow.Type)
            {
            case BackEndType::UCodeVM:
                _BackEnd = UCodeLang::UCodeBackEndObject::MakeObject;
                break;
            case BackEndType::C89:
                _BackEnd = UCodeLang::C89Backend::MakeObject;
                break;
            case BackEndType::IR:
                _BackEnd = UCodeLang::IRBackEnd::MakeObject;
                break;
            default:
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


        if (OutputWindow.AutoCompile == false)
        {
            ImGui::BeginDisabled(!IsCodeUpdated);
            if (ImGui::Button("Compile"))
            {
                CompileText(GetTextEditorString());
            }
            ImGui::EndDisabled();
        }
        if (OutputWindow.AutoReload == false)
        {
            ImGui::BeginDisabled(!IsCompiledDiff);
            if (ImGui::Button("Reload RunTime"))
            {
                _RunTimeStr = GetTextEditorString();


                _AnyInterpreter.UnLoad();

                _RuntimeLib.UnLoad();
                _RuntimeLib.Init(&_CompiledLib);

                _RunTimeState.ClearRunTimeState();
                _RunTimeState.AddLib(&_RuntimeLib);
                _RunTimeState.LinkLibs();

                _AnyInterpreter.Init(&_RunTimeState);
                OnRuntimeUpdated();
            }
            if (OutputWindow.AutoHotReload == false) 
            {
               
                if (ImGui::Button("Hot Reload"))
                {
                    _RunTimeStr = GetTextEditorString();

                }
            }
            ImGui::EndDisabled();
        }


        String CompilerState = "idle";
        ImGui::Text(((String)"CompilerState:" + CompilerState).c_str());

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

        ImGui::BeginDisabled();

        ImGui::PushID(&_LibInfoString);

        ImGui::InputTextMultiline("", &_LibInfoString,ImGui::GetContentRegionAvail());

        ImGui::PopID();

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
                ImGui::TextUnformatted("Debuger");


                ShowDebugerMenu(windowdata);


            }
            ImGui::NextColumn();
            {
                ImGui::TextUnformatted("Code");

                if (ImGui::BeginTable("split2", 2,ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
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
    for (size_t i = 0; i < Inslist.size(); i++)
    {
        auto& Item = Inslist[i];
        UCodeVMWindow::InsData V;
        V._Ins = Item;
        V.InsAddress = i;
        String Vstr;

        if (InsMapData.count(Item.OpCode))
        {
            auto& MapData = InsMapData[Item.OpCode];
            Vstr += (String)MapData->InsName;
            Vstr += " ";

            auto staticbytesview = BytesView::Make((const Byte*)RunTime.Get_StaticMemPtr(), RunTime.Get_Libs().GetStaticBytes().size());
            if (MapData->Op_0 != OpCodeType::NoOpCode)
            {
                UAssembly::OpValueToString(MapData->Op_0, Item.Value0, AddressToName, staticbytesview, Vstr);
            }
            if (MapData->Op_1 != OpCodeType::NoOpCode)
            {
                Vstr += ",";
                UAssembly::OpValueToString(MapData->Op_1, Item.Value1, AddressToName, staticbytesview, Vstr);
            }

        }
        else
        {
            Vstr += "Ins " + std::to_string((uintptr_t)Item.OpCode) + ":" + std::to_string((uintptr_t)Item.Value0.AsPtr) + ","
                + std::to_string((uintptr_t)Item.Value1.AsPtr);
        }

        V.StringValue = std::move(Vstr.c_str());

        windowdata.InsInfo.push_back(std::move(V));
    }
}

void AppObject::ShowDebugerMenu(UCodeVMWindow& windowdata)
{

   // ImguiHelper::BoolEnumField("Show Registers", windowdata.ShowRegisters);

    //ImGui::SameLine();
   // ImguiHelper::BoolEnumField("Show Stack", windowdata.ShowStack);
    //ImGui::SameLine();
   // ImguiHelper::BoolEnumField("Show Static-Memory", windowdata.ShowStaticMemory);
    //ImGui::SameLine();
   // ImguiHelper::BoolEnumField("Show Thread-Memory", windowdata.ShowThreadMemory);
    //ImGui::SameLine();
   // ImguiHelper::BoolEnumField("Show Heap-Memory", windowdata.ShowHeapMemory);

    ImGui::Separator();

    bool IsinFileMode = false;
    ImVec2 Buttonsize = { 80,20 };

    bool InFuncion = false;

    ImGui::Button("Reset", Buttonsize);


    ImGui::BeginDisabled(!InFuncion);

    ImGui::Button("Step in", Buttonsize); ImGui::SameLine();
    ImGui::Button("Step over", Buttonsize); ImGui::SameLine();
    ImGui::Button("Step out", Buttonsize);

    ImGui::EndDisabled();

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

                if (ImGui::Button(((String)"Call:" + MethodString).c_str()))
                {
                    callFuncContext._LastRetType = callFuncContext.current_method->RetType;
                    callFuncContext._LastRet.Resize(Assembly.GetSize(callFuncContext._LastRetType, Is32bits).value_or(0));

                    for (size_t i = 0; i < callFuncContext.current_method->ParsType.size(); i++)
                    {
                        auto& Arg = callFuncContext.Args[i];
                        _AnyInterpreter.PushParameter(Arg.Data(), Arg.Size());
                    }
                    _AnyInterpreter.Call(callFuncContext.current_method);

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

void AppObject::CompileText(const String& String)
{
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
    auto Val = _Compiler.CompileFiles(paths);

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
        default:
            _LibInfoString = "";
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
}

size_t AppObject::GetColumn(const String& text,size_t line,size_t Pos)
{
    size_t OnLine = 0;
    size_t OnColumn = 0;
    for (size_t i = 0; i < text.size(); i++)
    {
        if (text[i] == '/n')
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
        SeverPtr->_Sever.StopRuning();
        _LangSeverThread->join();
    }
}
UCodeIDEEnd