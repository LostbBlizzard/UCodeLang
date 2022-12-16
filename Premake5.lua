workspace "UCodeLang"
   configurations { "Debug", "Release","Published" }
   platforms { "Win32", "Win64"}
   defines {}
   
   startproject "UCApp"
   
   dependson {"UCApp","UCodeLang","UCodeCross"}

   OutDirPath ="%{cfg.platform}/%{cfg.buildcfg}"

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
    "UCodeCross"
   }

   links {
      "UCodeLang.lib",
      "UCodeCross.lib",
   }
   libdirs { 
      "Output/UCodeLang/" .. OutDirPath,
      "Output/UCodeCross/" .. OutDirPath,
   }
project "UCodeLang"
   location "UCodeLang"
   kind "StaticLib"
   language "C++"

   
   
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   includedirs{
      "%{prj.name}/%{prj.name}",
     }
   files { 
     "%{prj.name}/UCodeLang/**.c",
     "%{prj.name}/UCodeLang/**.h",
     "%{prj.name}/UCodeLang/**.cpp",
     "%{prj.name}/UCodeLang/**.hpp", 
   }
project "UCodeCross"
   location "UCodeCross"
   kind "StaticLib"
   language "C++"

   
   
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)

   includedirs{
      "%{prj.name}/%{prj.name}",
      "UCodeLang",
     }
   files { 
     "%{prj.name}/UCodeCross/**.c",
     "%{prj.name}/UCodeCross/**.h",
     "%{prj.name}/UCodeCross/**.cpp",
     "%{prj.name}/UCodeCross/**.hpp", 
   }