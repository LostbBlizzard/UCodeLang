project "UCodeLang"
   kind "StaticLib"
   language "C++"

   targetdir ("../Output/%{prj.name}/" .. OutDirPath)
   objdir ("../Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "**.c",
     "**.h",
     "**.cpp",
     "**.hpp", 
   }
   includedirs{
    "../UCodeLang",
    "Dependencies/zydis/include",
    "Dependencies/zycore/include",
    "Dependencies/zydis/src",
   }
   removefiles{
     "Dependencies/zydis/**.c",
     "Dependencies/zycore/**.c",

     "Dependencies/zydis/**.cpp",
     "Dependencies/zycore/**.cpp",

     "Dependencies/zydis/**.h",
     "Dependencies/zycore/**.h",
   }
   files { 
    "Dependencies/zydis/src/**.c",
    "Dependencies/zycore/src/**.c",

    "Dependencies/zydis/src/**.inc",
    "Dependencies/zycore/src/**.inc",

    "Dependencies/zydis/include/**.h",
    "Dependencies/zycore/include/**.h",
   }

project "UCodeLangNoCompiler"
   kind "StaticLib"
   language "C++"
   defines {"UCodeLangNoCompiler"}
   
   
   targetdir ("../Output/%{prj.name}/" .. OutDirPath)
   objdir ("../Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "**.c",
     "**.h",
     "**.cpp",
     "**.hpp", 
   }
   includedirs{
    "../UCodeLang",
    "Dependencies/zydis/include",
    "Dependencies/zycore/include",
    "Dependencies/zydis/src",
   }
   removefiles{
     "Dependencies/zydis/**.c",
     "Dependencies/zycore/**.c",

     "Dependencies/zydis/**.cpp",
     "Dependencies/zycore/**.cpp",

     "Dependencies/zydis/**.h",
     "Dependencies/zycore/**.h",
   }
   files { 
    "Dependencies/zydis/src/**.c",
    "Dependencies/zycore/src/**.c",

    "Dependencies/zydis/src/**.inc",
    "Dependencies/zycore/src/**.inc",

    "Dependencies/zydis/include/**.h",
    "Dependencies/zycore/include/**.h",
   }


