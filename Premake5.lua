workspace "UCodeLang"
   configurations { "Debug", "Release","Published" }
   platforms { "Win32", "Win64"}
   defines {}
   
   

   startproject "UCApp"
   
   dependson {"UCApp","UCodeLang","UC","UCodeAPI/StandardLibrary","UCodeAPI/NWin32","UCodeAPI/Win32"}

   OutDirPath ="%{cfg.platform}/%{cfg.buildcfg}"

   UCPathExeDir = "%{wks.location}Output/UC/" .. OutDirPath .. "/"
   UCPathExeName ="uc.exe"
   UCPathExe = UCPathExeDir ..  UCPathExeName

   filter { "platforms:Win32" }
    system "Windows"
    architecture "x86"
   filter { "platforms:Win64" }
     system "Windows"
     architecture "x86_64"
   filter { "system:Windows" }
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
      "Output/UCodeCross/" .. OutDirPath,
   }

project "UC"
   location "UC"
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
    "UCodeLang/Dependencies/Zydis/include",
    "UCodeLang/Dependencies/zycore/include",
    "UCodeLang/Dependencies/Zydis/src"
   }
   removefiles{
     "UCodeLang/Dependencies/Zydis/**.c",
     "UCodeLang/Dependencies/zycore/**.c",

      "UCodeLang/Dependencies/Zydis/**.cpp",
     "UCodeLang/Dependencies/zycore/**.cpp",

     "UCodeLang/Dependencies/Zydis/**.h",
     "UCodeLang/Dependencies/zycore/**.h",
   }
   files { 
    "UCodeLang/Dependencies/Zydis/src/**.c",
    "UCodeLang/Dependencies/zycore/src/**.c",

    "UCodeLang/Dependencies/Zydis/src/**.inc",
    "UCodeLang/Dependencies/zycore/src/**.inc",

    "UCodeLang/Dependencies/Zydis/include/**.h",
    "UCodeLang/Dependencies/zycore/include/**.h",
   }


project "UCodeLanguageSever"
   location "UCodeLanguageSever"
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

     "%{prj.name}/json-develop/include/**.hpp",
     "%{prj.name}/json-rpc-cxx-master/include/**.hpp",
   }
   includedirs{
    "UCodeLang",
   
    "%{prj.name}/json-rpc-cxx-master/include",

    "%{prj.name}/json-develop/include",
   }

   links {
      "UCodeLang.lib",
   }
   libdirs { 
      "Output/UCodeLang/" .. OutDirPath,
   }

project "UCodeDocumentation"
   location "UCodeDocumentation"
   kind "StaticLib"
   language "C++"



group "UCodeAPIs"
 project "StandardLibrary"
  location "UCodeAPI/StandardLibrary"
  kind "StaticLib"
  language "C"

  
  dependson {"UC"}
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
   "ucodelang --build %{prj.location}ULangModule.ucm"
  }
  project "NStandardLibrary"
  location "UCodeAPI/NStandardLibrary"
  kind "StaticLib"
  language "C"

  targetdir ("Output/%{prj.name}/" .. OutDirPath)
  objdir ("Output/int/%{prj.name}/" .. OutDirPath)
  
  dependson {"UC"}

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
   "ucodelang --build %{prj.location}ULangModule.ucm"
  }
 project "Win32"
  location "UCodeAPI/Win32"
  kind "StaticLib"
  language "C"

  targetdir ("Output/%{prj.name}/" .. OutDirPath)
  objdir ("Output/int/%{prj.name}/" .. OutDirPath)

  
  dependson {"UC"}
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
   "ucodelang --build %{prj.location}ULangModule.ucm" 
  }
 project "NWin32"
   location "UCodeAPI/NWin32"
   kind "StaticLib"
   language "C"

   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)

   
   dependson {"UC"}
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
   "ucodelang --build %{prj.location}ULangModule.ucm"
   }