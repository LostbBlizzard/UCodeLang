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
   
   files { 
     "%{prj.name}/**.c",
     "%{prj.name}/**.h",
     "%{prj.name}/**.cpp",
     "%{prj.name}/**.hpp", 
   }
   includedirs{
    "UCodeLang",
   }