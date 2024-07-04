project "UCodeLanguageSever"
   kind "ConsoleApp"
   language "C++"
   targetname ("uclanglsp")
   
   
   dependson {"UCodeLang","UCodeLanguageSeverlib"}
   targetdir ("../Output/%{prj.name}/" .. OutDirPath)
   objdir ("../Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "src/**.c",
     "src/**.h",
     "src/**.cpp",
     "src/**.hpp",
   }
   includedirs{
    "../UCodeLang",
    "../UCodeLanguageSeverlib/src",
   
    "../UCodeLanguageSeverlib/json/include",
   }

   links {
      "UCodeLanguageSeverlib", 
      "UCodeLang",
   }

   libdirs { 
      "../Output/UCodeLang/" .. OutDirPath,
      "../Output/UCodeLanguageSeverlib/" .. OutDirPath,
   }

