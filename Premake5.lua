workspace "UCodeLang"
   configurations { "Debug", "Release","Published" }
   platforms { "Win32", "Win64","linux32","linux64", "MacOS" }
   defines {"UCodeLangDebug","ZYCORE_STATIC_BUILD"}
   

   startproject "UCodeIDE"
   
   OutDirPath ="%{cfg.platform}/%{cfg.buildcfg}"

   UCPathExeDir = "%{wks.location}Output/UCodelangCL/" .. OutDirPath .. "/"
   UCPathExeName ="ucodelang.exe"
   UCPathExe = UCPathExeDir ..  UCPathExeName

   filter { "platforms:Win32" }
    system "Windows"
    architecture "x86"
   filter { "platforms:Win64" }
     system "Windows"
     architecture "x86_64"

   filter { "platforms:linux32" }
    system "linux"
    architecture "x86"
   filter { "platforms:linux64" }
     system "linux"
     architecture "x86_64"

   filter { "MacOS" }
    system "macosx"
   
   filter { "system:Windows" }
     cppdialect "c++17"
     
   filter { "system:linux" }
     cppdialect "c++17"
   
   filter { "system:macosx" }
     cppdialect "c++17"

   filter { "configurations:Debug" }
      defines { "DEBUG" }
      optimize "Debug"
      symbols "On"

   filter { "configurations:Release" }
      defines { "RELASE" }
      optimize "On"
      symbols "On"

   filter { "configurations:Published" }
      defines { "PUBLISHED" , "RELASE"}
      optimize "Speed"
      symbols "off"


   
project "UCApp"
   location "UCApp"
   kind "ConsoleApp"
   language "C++"

   
   dependson {"UCodeLang"}
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 

     "%{prj.name}/tests/**.c",
     "%{prj.name}/tests/**.h",
     "%{prj.name}/tests/**.cpp",
     "%{prj.name}/tests/**.hpp", 
   }
   includedirs{
    "%{prj.name}/src",
    "UCodeLang",
    
    "UCodeLang/Dependencies/Zydis/include",
    "UCodeLang/Dependencies/zycore/include",
    "UCodeLang/Dependencies/Zydis/src"
   }

   links {
      "UCodeLang.lib",
   }
   libdirs { 
      "Output/UCodeLang/" .. OutDirPath,
   }

project "UCodelangCL"
   location "UCodelangCL"
   kind "ConsoleApp"
   language "C++"
   targetname ("ucodelang")
   
   
   dependson {"UCodeLang"}
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 
   }
   includedirs{
    "%{prj.name}/src",
    "UCodeLang",
   }

   links {
      "UCodeLang.lib",
   }
   libdirs { 
      "Output/UCodeLang/" .. OutDirPath,
   }
project "UCodeLang"
   location "UCodeLang"
   kind "StaticLib"
   language "C++"

   
   
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "%{prj.name}/**.c",
     "%{prj.name}/**.h",
     "%{prj.name}/**.cpp",
     "%{prj.name}/**.hpp", 
   }
   includedirs{
    "UCodeLang",
    "%{prj.name}/Dependencies/Zydis/include",
    "%{prj.name}/Dependencies/zycore/include",
    "%{prj.name}/Dependencies/Zydis/src"
   }
   removefiles{
     "%{prj.name}/Dependencies/Zydis/**.c",
     "%{prj.name}/Dependencies/zycore/**.c",

      "%{prj.name}/Dependencies/Zydis/**.cpp",
     "%{prj.name}/Dependencies/zycore/**.cpp",

     "%{prj.name}/Dependencies/Zydis/**.h",
     "%{prj.name}/Dependencies/zycore/**.h",
   }
   files { 
    "%{prj.name}/Dependencies/Zydis/src/**.c",
    "%{prj.name}/Dependencies/zycore/src/**.c",

    "%{prj.name}/Dependencies/Zydis/src/**.inc",
    "%{prj.name}/Dependencies/zycore/src/**.inc",

    "%{prj.name}/Dependencies/Zydis/include/**.h",
    "%{prj.name}/Dependencies/zycore/include/**.h",
   }


project "UCodeLanguageSeverlib"
   location "UCodeLanguageSeverlib"
   kind "StaticLib"
   language "C++"

   dependson {"UCodeLang"}
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 

     "%{prj.name}/json/include/**.hpp",
   }
   includedirs{
    "UCodeLang",
    "%{prj.name}/json/include",
   }

project "UCodeLanguageSever"
   location "UCodeLanguageSever"
   kind "ConsoleApp"
   language "C++"

   
   
   dependson {"UCodeLang","UCodeLanguageSeverlib"}
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "%{prj.name}/src/**.c",
     "%{prj.name}/src/**.h",
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp",
   }
   includedirs{
    "UCodeLang",
   
    "UCodeLanguageSeverlib/json/include",
    "UCodeLanguageSeverlib/src",
   }

   links {
      "UCodeLang.lib",
      "UCodeLanguageSeverlib.lib",
   }
   libdirs { 
      "Output/UCodeLang/" .. OutDirPath,
      "Output/UCodeLanguageSeverlib/" .. OutDirPath,
   }

project "UCodeDocumentation"
   location "UCodeDocumentation"
   kind "StaticLib"
   language "C++"
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)

project "UCodeIDE"
   location "UCodeIDE" 
   language "C++"
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)

   dependson {"UCodeLang","UCodeLanguageSever"}

   defines {"GLEW_STATIC"}
   
   
   
   files { 
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 

     "%{prj.name}/Dependencies/GLEW/**.h",
     "%{prj.name}/Dependencies/GLFW/include/**.c",
     "%{prj.name}/Dependencies/GLFW/include/**.h", 
     "%{prj.name}/Dependencies/GLFW/src/**.c",
     "%{prj.name}/Dependencies/GLFW/src/**.h", 

     "%{prj.name}/Dependencies/Imgui/*.cpp",
     "%{prj.name}/Dependencies/Imgui/*.h", 
     
     "%{prj.name}/Dependencies/Imgui/backends/imgui_impl_opengl3.h",
     "%{prj.name}/Dependencies/Imgui/backends/imgui_impl_opengl3_loader.h",
     "%{prj.name}/Dependencies/Imgui/backends/imgui_impl_opengl3.cpp",

     "%{prj.name}/Dependencies/Imgui/backends/imgui_impl_glfw.cpp",
     "%{prj.name}/Dependencies/Imgui/backends/imgui_impl_glfw.h",

     "%{prj.name}/Dependencies/Imgui/misc/cpp/*.cpp",
     "%{prj.name}/Dependencies/Imgui/misc/cpp/*.h", 
   }

   includedirs{
    "%{prj.name}/src",
    "UCodeLang",
    "UCodeLang/UCodeLang",
    "UCodeLanguageSeverlib/json/include",
    "UCodeLanguageSeverlib/src",

    "%{prj.name}/Dependencies",
    "%{prj.name}/Dependencies/Imgui",
    "%{prj.name}/Dependencies/GLFW/include",
    "%{prj.name}/Dependencies/GLFW/deps",
    "%{prj.name}/Dependencies/GLEW",

    "UCApp",
   }

   links {
     "UCodeLang.lib",
     "UCodeLanguageSeverlib.lib",
   }
   libdirs { 
      "Output/UCodeLang/" .. OutDirPath,
      "Output/UCodeLanguageSeverlib/" .. OutDirPath,
      "%{prj.name}/Dependencies/GLEW/Lib",
   }

   filter { "system:Windows" }
    kind "ConsoleApp"   
    defines {"_GLFW_WIN32"}

   filter { "system:linux" }
    kind "ConsoleApp"   
    defines {"_GLFW_WIN32"}

   filter { "system:MacOS" }
    kind "ConsoleApp"   
    defines {"_GLFW_WIN32"}

   filter { "system:Windows","configurations:Published" }
    kind ("WindowedApp")
   filter { "system:Windows","configurations:Published" }
    kind ("WindowedApp")

   

   filter { "architecture:x86"}
      links {"glew32s.lib","Opengl32.lib"}
   filter { "architecture:x86_64"}
      links {"glew64s.lib","Opengl32.lib"}

   
project "UCodeWebsite"
   location "UCodeWebsite" 
   language "C++"
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   kind "ConsoleApp" 

   files { 
   }

   includedirs{
   }

group "UCodeAPIs"
 project "StandardLibrary"
  location "UCodeAPI/StandardLibrary"
  kind "StaticLib"
  language "C"

  
  dependson {"UCodelangCL"}
  targetdir ("Output/%{prj.name}/" .. OutDirPath)
  objdir ("Output/int/%{prj.name}/" .. OutDirPath)

  files { 
  "%{prj.name}/src/**.uc",
  "%{prj.name}/%{prj.name}.ucm",

  "%{prj.name}/src/**.c",
  "%{prj.name}/src/**.h",
  "%{prj.name}/src/**.cpp",
  "%{prj.name}/src/**.hpp", 
  "%{prj.name}/src/**.uc",
  "%{prj.name}/test/**.uc",
  
  }


 

  postbuildcommands 
  {
   -- UCPathExe.." build %{prj.location}ULangModule.ucm"
  }
  project "NStandardLibrary"
  location "UCodeAPI/NStandardLibrary"
  kind "StaticLib"
  language "C"

  targetdir ("Output/%{prj.name}/" .. OutDirPath)
  objdir ("Output/int/%{prj.name}/" .. OutDirPath)
  
  dependson {"UCodelangCL"}

  files { 
  "%{prj.name}/src/**.uc",
  "%{prj.name}/%{prj.name}.ucm",

  "%{prj.name}/src/**.c",
  "%{prj.name}/src/**.h",
  "%{prj.name}/src/**.cpp",
  "%{prj.name}/src/**.hpp",

  "%{prj.name}/src/**.uc",
  "%{prj.name}/test/**.uc",
  "%{prj.name}**.ucm",
  }


 

  postbuildcommands 
  {
  -- UCPathExe.." \"build %{prj.location}ULangModule.ucm\" "
  }
 project "Win32"
  location "UCodeAPI/Win32"
  kind "StaticLib"
  language "C"

  targetdir ("Output/%{prj.name}/" .. OutDirPath)
  objdir ("Output/int/%{prj.name}/" .. OutDirPath)

  
  dependson {"UCodelangCL"}
  files { 
  "%{prj.name}/src/**.uc",
  "%{prj.name}/%{prj.name}.ucm",

  "%{prj.name}/src/**.c",
  "%{prj.name}/src/**.h",
  "%{prj.name}/src/**.cpp",
  "%{prj.name}/src/**.hpp",
  
  "%{prj.name}/src/**.uc",
  "%{prj.name}/test/**.uc",
  
  }

  postbuildcommands 
  {
   -- UCPathExe.. " build %{prj.location}ULangModule.ucm" 
  }
 project "NWin32"
   location "UCodeAPI/NWin32"
   kind "StaticLib"
   language "C"

   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)

   
   dependson {"UCodelangCL"}
   files { 
   "%{prj.name}/src/**.uc",
   "%{prj.name}/%{prj.name}.ucm",

   "%{prj.name}/src/**.c",
   "%{prj.name}/src/**.h",
   "%{prj.name}/src/**.cpp",
   "%{prj.name}/src/**.hpp", 
   
   "%{prj.name}/src/**.uc",
   "%{prj.name}/test/**.uc",
   }

   postbuildcommands 
   {
   -- UCPathExe.. " build %{prj.location}ULangModule.ucm "
   }