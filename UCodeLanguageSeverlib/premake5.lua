project "UCodeLanguageSeverlib"
   kind "StaticLib"
   language "C++"

   dependson {"UCodeLang"}
   targetdir ("../Output/%{prj.name}/" .. OutDirPath)
   objdir ("../Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "src/**.c",
     "src/**.h",
     "src/**.cpp",
     "src/**.hpp", 

     "json/include/**.hpp",
   }
   includedirs{
    "../UCodeLang",
    "json/include",
   }
