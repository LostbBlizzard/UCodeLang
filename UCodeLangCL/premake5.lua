project "UCodeLangCL"
   kind "ConsoleApp"
   language "C++"
   targetname ("uclang")
   
   
   dependson {"UCodeLang"}
   targetdir ("../Output/%{prj.name}/" .. OutDirPath)
   objdir ("../Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "src/**.c",
     "src/**.h",
     "src/**.cpp",
     "src/**.hpp", 
   }
   includedirs{
    "src",
    "../UCodeLang",
   }

   
   links {
      "UCodeLang",
   }
   libdirs { 
      "../Output/UCodeLang/" .. OutDirPath,
   }
   filter { "system:Windows","configurations:Published"}
    kind ("WindowedApp")
   filter { "system:Windows","configurations:Release"  }
    kind ("WindowedApp")

