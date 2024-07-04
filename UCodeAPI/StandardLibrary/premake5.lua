project "StandardLibrary"
kind "StaticLib"
language "C++"


dependson { "UCodeLangCL", "NStandardLibrary" }
targetdir("../Output/%{prj.name}/" .. OutDirPath)
objdir("../Output/int/%{prj.name}/" .. OutDirPath)

files {
  "ignoreC.c",
  "src/**.uc",
  "ULangModule.ucm",
}


filter { "system:Windows or system:linux or system:macosx", "platforms:not Web" }
prebuildmessage 'compiling ucodelang files'
prebuildcommands
{
  UCPathExe .. " index ",
  UCPathExe .. " build "
}
