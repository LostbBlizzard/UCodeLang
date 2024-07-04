project "UCodeIDE"
   kind "ConsoleApp" 
   language "C++"

   dependson {"UCodeLangCL","UCodeLanguageSeverlib"}
   targetdir ("../Output/%{prj.name}/" .. OutDirPath)
   objdir ("../Output/int/%{prj.name}/" .. OutDirPath)

   defines {"GLEW_STATIC","GLEW_NO_GLU"}
     
   
   
   files { 
     "src/**.cpp",
     "src/**.hpp", 

     --
     "Dependencies/GLEW/**.h",
      

     "Dependencies/imgui/*.cpp",
     "Dependencies/imgui/*.h", 
     
     "Dependencies/imgui/backends/imgui_impl_opengl3.h",
     "Dependencies/imgui/backends/imgui_impl_opengl3_loader.h",
     "Dependencies/imgui/backends/imgui_impl_opengl3.cpp",

     "Dependencies/imgui/backends/imgui_impl_glfw.cpp",
     "Dependencies/imgui/backends/imgui_impl_glfw.h",

     "Dependencies/imgui/misc/cpp/*.cpp",
     "Dependencies/imgui/misc/cpp/*.h", 
   }
   filter { "platforms:not Web"}
      files { 
      "Dependencies/GLFW/include/**.c",
      "Dependencies/GLFW/include/**.h", 
      "Dependencies/GLFW/src/**.c",
      "Dependencies/GLFW/src/**.h", 
      }
   filter {}
   includedirs{
    "src",
    "Dependencies",
    "Dependencies/imgui",
    "Dependencies/GLFW/include",
    "Dependencies/GLFW/deps",
    "Dependencies/GLEW",

    "../UCodeLang",
    "../UCodeLang/UCodeLang",
    "../UCodeLanguageSeverlib/json/include",
    "../UCodeLanguageSeverlib/src",

    "../UCApp",
   }


   libdirs { 
      "../Output/UCodeLang/" .. OutDirPath,
      "../Output/UCodeLanguageSeverlib/" .. OutDirPath,
   }

   links {
      "UCodeLanguageSeverlib",
      "UCodeLang",
   }


   filter { "system:Windows or system:linux or system:macosx", "platforms:not Web" }
    prebuildcommands
    {
     UCPathExe.." cpptoulangvm ../%{prj.location}/src/AppObject/AppAPI.hpp ../%{prj.location}src/AppObject/AppAPILink.cpp ../%{prj.location}tepfiles/AppAPI.uc",
     UCPathExe.." cpptoulangvm ../%{prj.location}/src/AppObject/ECSExample.hpp ../%{prj.location}src/AppObject/ECSExample.cpp ../%{prj.location}tepfiles/ECSAPI.uc",
    }
    prebuildmessage "runing cpptoulangvm"

   filter { "system:Windows" }
    kind "ConsoleApp"   
    defines {"_GLFW_WIN32"}
    libdirs { "%{prj.name}/Dependencies/GLEW/Lib"}

   filter {"system:Windows","architecture:x86"}
      links {"glew32s.lib","Opengl32.lib"}
   
   filter { "system:Windows","architecture:x86_64"}
      links {"glew64s.lib","Opengl32.lib"}      

   filter { "system:linux","platforms:not Web" }
    kind "ConsoleApp"   
    defines {"_GLFW_X11"}
    links {"GL"}

   filter { "system:macosx" }
    kind "ConsoleApp"   
    defines {"_GLFW_COCOA"}
    links { "glfw",
    "OpenGL.framework",
    "Cocoa.framework",
    "IOKit.framework",
    "CoreVideo.framework",
    "Carbon.framework",
    }
    files
    {
      "Dependencies/GLFW/src/**.m"
    }

   filter { "system:Windows","configurations:Published" }
    kind ("WindowedApp")


