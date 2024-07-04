project "UCApp"
   kind "ConsoleApp"
   language "C++"

   
   dependson {"UCodeLang"}
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "src/**.c",
     "src/**.h",
     "src/**.cpp",
     "src/**.hpp", 

     "tests/**.c",
     "tests/**.h",
     "tests/**.cpp",
     "tests/**.hpp", 


     "**.uc", 
     "**.ucm", 
   }
   removefiles{
     "tests/PerformanceTests/**", 
     "tests/UCodeFiles/Output/**", 
     "src/CodeTesting/out/**", 
     "src/CodeTesting/int/**", 
   }
   includedirs{
    "src",
    "../UCodeLang",
    
    "../UCodeLang/Dependencies/zydis/include",
    "../UCodeLang/Dependencies/zycore/include",
    "../UCodeLang/Dependencies/zydis/src"
   }
   
    
  
   filter {}
  
   links {
      "UCodeLang",
   }
   libdirs { 
      "Output/UCodeLang/" .. OutDirPath,
   }

