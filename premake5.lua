workspace "UCodeLang"
   configurations { "Debug", "Release","Published" }
   platforms { "Win32", "Win64","linux32","linux64", "MacOS","Android","IOS","Web" }
   defines {"UCodeLangDebug","ZYCORE_STATIC_BUILD","UCodeLangExperimental"}
   startproject "UCodeIDE"
   cppdialect "c++17"

   if os.host() == "windows" then
      if os.is64bit() then
         defaultplatform "Win64"
      else
         defaultplatform "Win32"
      end
   end

   if os.host() == "linux" then
      if os.is64bit() then
         defaultplatform "linux64"
      else
         defaultplatform "linux32"
      end
   end

   if os.host() == "macosx" then
      if os.is64bit() then
         defaultplatform "MacOS"
      else
         defaultplatform "MacOS"
      end
   end
   
   OutDirPath ="%{cfg.platform}/%{cfg.buildcfg}"

   UCPathExeDir = "%{wks.location}/Output/UCodelangCL/" .. OutDirPath .. "/"
   UCPathExeName ="uclang"
   UCPathExe = UCPathExeDir ..  UCPathExeName


   if _ACTION == "vs2019" or _ACTION == "vs2022" then
    flags { "MultiProcessorCompile" }
   end 
   

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

   filter { "platforms:MacOS" }
    system "macosx"
    architecture "universal"
    toolset "clang"
   
   filter { "platforms:Android" }
    system "android"
    architecture "ARM"
    androidapilevel (22)
    exceptionhandling ("On")
    rtti ("On")

   filter { "platforms:IOS" }
    system "ios"
    architecture "universal"
    toolset "clang"

   filter { "platforms:Web" }
    system "linux"
    architecture "x86"
    defines { "ZYAN_POSIX" }
    targetextension (".html")
   
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


     "%{prj.name}/**.uc", 
     "%{prj.name}/**.ucm", 
   }
   removefiles{
     "%{prj.name}/tests/PerformanceTests/**", 
     "%{prj.name}/tests/UCodeFiles/Output/**", 
     "%{prj.name}/src/CodeTesting/out/**", 
     "%{prj.name}/src/CodeTesting/int/**", 
   }
   includedirs{
    "%{prj.name}/src",
    "UCodeLang",
    
    "UCodeLang/Dependencies/zydis/include",
    "UCodeLang/Dependencies/zycore/include",
    "UCodeLang/Dependencies/zydis/src"
   }
   
   
   
   links {
      "UCodeLang",
   }
   libdirs { 
      "Output/UCodeLang/" .. OutDirPath,
   }
project "UCodelangCL"
   location "UCodelangCL"
   kind "ConsoleApp"
   language "C++"
   targetname ("uclang")
   
   
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
      "UCodeLang",
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
    "%{prj.name}/Dependencies/zydis/include",
    "%{prj.name}/Dependencies/zycore/include",
    "%{prj.name}/Dependencies/zydis/src",
   }
   removefiles{
     "%{prj.name}/Dependencies/zydis/**.c",
     "%{prj.name}/Dependencies/zycore/**.c",

     "%{prj.name}/Dependencies/zydis/**.cpp",
     "%{prj.name}/Dependencies/zycore/**.cpp",

     "%{prj.name}/Dependencies/zydis/**.h",
     "%{prj.name}/Dependencies/zycore/**.h",
   }
   files { 
    "%{prj.name}/Dependencies/zydis/src/**.c",
    "%{prj.name}/Dependencies/zycore/src/**.c",

    "%{prj.name}/Dependencies/zydis/src/**.inc",
    "%{prj.name}/Dependencies/zycore/src/**.inc",

    "%{prj.name}/Dependencies/zydis/include/**.h",
    "%{prj.name}/Dependencies/zycore/include/**.h",
   }

project "UCodeLangNoCompiler"
   location "UCodeLang"
   kind "StaticLib"
   language "C++"
   defines {"UCodeLangNoCompiler"}
   
   
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   
   files { 
     "UCodeLang/**.c",
     "UCodeLang/**.h",
     "UCodeLang/**.cpp",
     "UCodeLang/**.hpp", 
   }
   includedirs{
    "UCodeLang",
    "UCodeLang/Dependencies/zydis/include",
    "UCodeLang/Dependencies/zycore/include",
    "UCodeLang/Dependencies/zydis/src",
   }
   removefiles{
     "UCodeLang/Dependencies/zydis/**.c",
     "UCodeLang/Dependencies/zycore/**.c",

     "UCodeLang/Dependencies/zydis/**.cpp",
     "UCodeLang/Dependencies/zycore/**.cpp",

     "UCodeLang/Dependencies/zydis/**.h",
     "UCodeLang/Dependencies/zycore/**.h",
   }
   files { 
    "UCodeLang/Dependencies/zydis/src/**.c",
    "UCodeLang/Dependencies/zycore/src/**.c",

    "UCodeLang/Dependencies/zydis/src/**.inc",
    "UCodeLang/Dependencies/zycore/src/**.inc",

    "UCodeLang/Dependencies/zydis/include/**.h",
    "UCodeLang/Dependencies/zycore/include/**.h",
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
   targetname ("uclanglsp")
   
   
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
      "UCodeLanguageSeverlib", 
      "UCodeLang",
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
   
   files { "%{prj.name}/ignoreC.c"}
   files { 
     "%{prj.name}/src/**.md",
   }

project "UCodeIDE"
   location "UCodeIDE" 
   kind "ConsoleApp" 
   language "C++"

   dependson {"UCodelangCL","UCodeLanguageSeverlib"}
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)

   defines {"GLEW_STATIC","GLEW_NO_GLU"}
     
   
   
   files { 
     "%{prj.name}/src/**.cpp",
     "%{prj.name}/src/**.hpp", 

     --
     "%{prj.name}/Dependencies/GLEW/**.h",
      

     "%{prj.name}/Dependencies/imgui/*.cpp",
     "%{prj.name}/Dependencies/imgui/*.h", 
     
     "%{prj.name}/Dependencies/imgui/backends/imgui_impl_opengl3.h",
     "%{prj.name}/Dependencies/imgui/backends/imgui_impl_opengl3_loader.h",
     "%{prj.name}/Dependencies/imgui/backends/imgui_impl_opengl3.cpp",

     "%{prj.name}/Dependencies/imgui/backends/imgui_impl_glfw.cpp",
     "%{prj.name}/Dependencies/imgui/backends/imgui_impl_glfw.h",

     "%{prj.name}/Dependencies/imgui/misc/cpp/*.cpp",
     "%{prj.name}/Dependencies/imgui/misc/cpp/*.h", 
   }
   filter { "platforms:not Web"}
      files { 
      "%{prj.name}/Dependencies/GLFW/include/**.c",
      "%{prj.name}/Dependencies/GLFW/include/**.h", 
      "%{prj.name}/Dependencies/GLFW/src/**.c",
      "%{prj.name}/Dependencies/GLFW/src/**.h", 
      }
   filter {}
   includedirs{
    "%{prj.name}/src",
    "%{prj.name}/Dependencies",
    "%{prj.name}/Dependencies/imgui",
    "%{prj.name}/Dependencies/GLFW/include",
    "%{prj.name}/Dependencies/GLFW/deps",
    "%{prj.name}/Dependencies/GLEW",

    "UCodeLang",
    "UCodeLang/UCodeLang",
    "UCodeLanguageSeverlib/json/include",
    "UCodeLanguageSeverlib/src",

    "UCApp",
   }


   libdirs { 
      "Output/UCodeLang/" .. OutDirPath,
      "Output/UCodeLanguageSeverlib/" .. OutDirPath,
   }

   links {
      "UCodeLanguageSeverlib",
      "UCodeLang",
   }


   filter { "system:Windows or system:linux or system:macosx", "platforms:not Web" }
    prebuildcommands
    {
     UCPathExe.." cpptoulangvm %{prj.location}/src/AppObject/AppAPI.hpp %{prj.location}src/AppObject/AppAPILink.cpp %{prj.location}tepfiles/AppAPI.uc",
     UCPathExe.." cpptoulangvm %{prj.location}/src/AppObject/ECSExample.hpp %{prj.location}src/AppObject/ECSExample.cpp %{prj.location}tepfiles/ECSAPI.uc",
    }
    prebuildmessage "runing cpptoulangvm"

   filter { "system:Windows" }
    kind "ConsoleApp"   
    defines {"_GLFW_WIN32"}
    libdirs { "%{prj.name}/Dependencies/GLEW/Lib"}

   filter {"system:Windows","architecture:x86"}
      links {"glew32s.lib","Opengl32.lib"}
   
   filter { "system:Windows","architecture:x86_64"}
      links {"glew64s.lib","Opengl32.lib"}

      
   filter { "platforms:Web" }
      kind "ConsoleApp"   
      links {"glfw"}
      linkoptions { "-sUSE_GLFW=3","--preload-file wasmassets"}
      postbuildmessage ("copying output to UCodeWebsite")
      postbuildcommands { 
         "rm -r %{wks.location}/UCodeWebsite/static/UCodeIDE",
         "cp -f -r %{cfg.buildtarget.directory}/ %{wks.location}/UCodeWebsite/static/UCodeIDE",
         "cp %{prj.location}/index.html %{wks.location}/UCodeWebsite/static/UCodeIDE/UCodeIDE.html"
      }

   filter { "system:linux","platforms:not Web" }
    kind "ConsoleApp"   
    defines {"_GLFW_X11"}
    links {"GL"}

   filter { "system:macosx" }
    kind "ConsoleApp"   
    defines {"_GLFW_COCOA"}
    links { "glfw",
    "OpenGL.framework",
    "Cocoa.framework",
    "IOKit.framework",
    "CoreVideo.framework",
    "Carbon.framework",
    }
    files
    {
      "%{prj.name}/Dependencies/GLFW/src/**.m"
    }

   filter { "system:Windows","configurations:Published" }
    kind ("WindowedApp")

   
   
project "UCodeWebsite"
   location "UCodeWebsite" 
   language "C++"
   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)
   kind "StaticLib" 

   files { "%{prj.name}/ignoreC.c"}

   includedirs{
   }

group "UCodeAPIs"
 project "StandardLibrary"
  location "UCodeAPI/StandardLibrary"
  kind "StaticLib"
  language "C++"

  
  dependson {"UCodelangCL","NStandardLibrary"}
  targetdir ("Output/%{prj.name}/" .. OutDirPath)
  objdir ("Output/int/%{prj.name}/" .. OutDirPath)

  files { 
  "UCodeAPI/%{prj.name}/ignoreC.c",
  "UCodeAPI/%{prj.name}/src/**.uc",
  "UCodeAPI/%{prj.name}/ULangModule.ucm",
  }


  filter { "system:Windows or system:linux or system:macosx", "platforms:not Web" }
   prebuildmessage 'compiling ucodelang files'
   prebuildcommands  
   {
    UCPathExe.." index %{prj.location}",
    -- UCPathExe.." build %{prj.location}"
   }
  
 project "NStandardLibrary"
  location "UCodeAPI/NStandardLibrary"
  kind "StaticLib"
  language "C++"

  targetdir ("Output/%{prj.name}/" .. OutDirPath)
  objdir ("Output/int/%{prj.name}/" .. OutDirPath)
  
  dependson {"UCodelangCL"}

  files { 
  "UCodeAPI/%{prj.name}/ignoreC.c",
  "UCodeAPI/%{prj.name}/src/**.uc",
  "UCodeAPI/%{prj.name}/ULangModule.ucm",
  }


  filter {"system:Windows or system:linux or system:macosx", "platforms:not Web" }
   prebuildmessage 'compiling ucodelang files'
   prebuildcommands 
   {
    UCPathExe.." index %{prj.location}",
    --UCPathExe.." build %{prj.location}"
   }
 project "Win32"
  location "UCodeAPI/Win32"
  kind "StaticLib"
  language "C++"

  targetdir ("Output/%{prj.name}/" .. OutDirPath)
  objdir ("Output/int/%{prj.name}/" .. OutDirPath)

  
  dependson {"UCodelangCL","NWin32"}
  files { 
  "UCodeAPI/%{prj.name}/ignoreC.c",
  "UCodeAPI/%{prj.name}/src/**.uc",
  "UCodeAPI/%{prj.name}/ULangModule.ucm",
  }

  
  filter { "system:Windows or system:linux or system:macosx", "platforms:not Web" }
   prebuildmessage 'compiling ucodelang files'
   prebuildcommands 
   {
     UCPathExe.." index %{prj.location}",
     --UCPathExe.." build %{prj.location}"
   }
 project "NWin32"
   location "UCodeAPI/NWin32"
   kind "StaticLib"
   language "C++"

   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)

   
   dependson {"UCodelangCL"}
   files { 
   "UCodeAPI/%{prj.name}/ignoreC.c",
   "UCodeAPI/%{prj.name}/src/**.uc",
   "UCodeAPI/%{prj.name}/ULangModule.ucm",
   }

   filter {"system:Windows or system:linux or system:macosx", "platforms:not Web" }
    prebuildmessage 'compiling ucodelang files'
    prebuildcommands 
    {
     UCPathExe.." index %{prj.location}",
     --UCPathExe.." build %{prj.location}"
    }
 project "Example"
   location "UCodeAPI/Example"
   kind "ConsoleApp"
   language "C++"

   targetdir ("Output/%{prj.name}/" .. OutDirPath)
   objdir ("Output/int/%{prj.name}/" .. OutDirPath)

   
   dependson {"StandardLibrary","UCodelangCL"}
   files { 
   "UCodeAPI/%{prj.name}/out/CLang89/Example.c",
   "UCodeAPI/%{prj.name}/src/**.uc",
   "UCodeAPI/%{prj.name}/ULangModule.ucm",
   }

   prebuildmessage 'compiling ucodelang files'

   if not os.host() == "macosx" then--MacOs CL build fail because of this. 
   prebuildcommands 
   {
    --UCPathExe.." build %{prj.location} -c11",
   }
   end
newaction {
    trigger = "install",
    description = "installs compiler tool/librarys",
    execute = function ()
        print("----installing tools for " .. os.target())
        
        if os.istarget("linux") then

          os.execute("sudo apt-get update")

          print("----downloading libx11 Packages")
          os.execute("sudo apt-get install libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev")

          print("----downloading opengl Packages")
          os.execute("sudo apt install mesa-common-dev")
          
          print("----installing tools completed");
        end

        if os.istarget("windows") then

        end
        
        if os.istarget("macosx") then
          
           print("----downloading glfw Packages")

           os.execute("brew install glfw")
          
           print("----installing tools completed");
        end
    end
}
newaction {
    trigger = "installwasm",
    description = "installs compiler tool/librarys for wasm",
    execute = function ()
        print("installing wasm tools for " .. os.target())
        
        if os.istarget("linux") then
         os.execute("cd ~;git clone https://github.com/emscripten-core/emsdk.git;cd emsdk;git pull;./emsdk install latest;./emsdk activate latest;source ./emsdk_env.sh;echo 'source "/home/runner/emsdk/emsdk_env.sh"' >> $HOME/.bash_profile;")
        end

        if os.istarget("windows") then

        end
        
        if os.istarget("macosx") then

        end
    end
}

---build
newaction {
    trigger = "build_UCApp",
    description = "builds the UCApp",
    execute = function ()
        
        if os.istarget("linux") then
         os.execute("make UCApp -j4")
        end

        if os.istarget("windows") then
         os.execute("msbuild ../UCodeLang.sln /t:Build /p:Configuration=Debug /p:Platform=Win64 -maxcpucount")
        end
        
        if os.istarget("macosx") then
         os.execute("make UCApp -j4")
        end
    end
}
newaction {
    trigger = "build_UCodelangCL",
    description = "builds the UCodelangCL",
    execute = function ()
        
        if os.istarget("linux") then
         os.execute("make UCodelangCL -j4")
        end

        if os.istarget("windows") then
         os.execute("msbuild ../UCodeLang.sln /t:Build /p:Configuration=Debug /p:Platform=Win64 -maxcpucount")
        end
        
        if os.istarget("macosx") then
         os.execute("make UCodelangCL -j4")
        end
    end
}
newaction {
    trigger = "build",
    description = "builds the everything",
    execute = function ()
        
        if os.istarget("linux") then
         os.execute("make -j4")
        end

        if os.istarget("windows") then
         os.execute("msbuild ../UCodeLang.sln /t:Build /p:Configuration=Debug /p:Platform=Win64 -maxcpucount")
        end
        
        if os.istarget("macosx") then
         os.execute("make -j4")
        end
    end
}
newaction {
    trigger = "build_32bit",
    description = "builds the everything",
    execute = function ()
        
        if os.istarget("linux") then
         os.execute("make -j4")
        end

        if os.istarget("windows") then
         os.execute("msbuild ../UCodeLang.sln /t:Build /p:Configuration=Debug /p:Platform=Win64 -maxcpucount")
        end
        
        if os.istarget("macosx") then
         os.execute("make -j4")
        end
    end
}
newaction {
    trigger = "build_published",
    description = "builds the everything",
    execute = function ()
        
        if os.istarget("linux") then
         os.execute("make config=published_linux64 -j4")
        end

        if os.istarget("windows") then
         os.execute("msbuild ../UCodeLang.sln /t:Build /p:Configuration=Debug /p:Platform=Win64 -maxcpucount")
        end
        
        if os.istarget("macosx") then
         os.execute("make config=published_macos -j4")
        end
    end
}
newaction {
    trigger = "build_published_32bit",
    description = "builds the everything",
    execute = function ()
        
        if os.istarget("linux") then
         os.execute("make config=published_linux32 -j4")
        end

        if os.istarget("windows") then
         os.execute("msbuild ../UCodeLang.sln /t:Build /p:Configuration=Debug /p:Platform=Win32 -maxcpucount")
        end
        
        if os.istarget("macosx") then
         os.execute("make config=published_macos -j4")
        end
    end
}

---test
newaction {
    trigger = "test",
    description = "installs compiler tool/librarys for wasm",
    execute = function ()
        
        if os.istarget("linux") then
          
        end

        if os.istarget("windows") then

        end
        
        if os.istarget("macosx") then

        end
    end
}
newaction {
    trigger = "test_32bit",
    description = "runs test for 32bit mode",
    execute = function ()
        
        if os.istarget("linux") then
          
        end

        if os.istarget("windows") then

        end
        
        if os.istarget("macosx") then

        end
    end
}

---clean
newaction {
    trigger = "clean",
    description = "clean project outputs",
    execute = function ()

        if os.istarget("linux") then
         os.execute("make clean")
        end

        if os.istarget("windows") then
         os.execute("msbuild UCodeLang.sln /t:Clean")
        end
        
        if os.istarget("macosx") then
         os.execute("make clean")
        end
    end
}


--web
newaction {
    trigger = "web_build",
    description = "Builds for the Web",
    execute = function ()

        if os.istarget("linux") then
         os.execute("emmake make config=debug_web -j4")
        end

        if os.istarget("windows") then
         os.execute("emmake make config=debug_web -j4")
        end
        
        if os.istarget("macosx") then
         os.execute("emmake make config=debug_web -j4")
        end
    end
}
newaction {
    trigger = "web_build_published",
    description = "Builds for the Web",
    execute = function ()
        print("installing wasm tools for " .. os.target())
        
        if os.istarget("linux") then
         os.execute("emmake make config=published_web -j4")
        end

        if os.istarget("windows") then
         os.execute("emmake make config=published_web -j4")
        end
        
        if os.istarget("macosx") then
         os.execute("emmake make config=published_web -j4")
        end
    end
}
newaction {
    trigger = "web_build_UCodeIDE",
    description = "Builds for the Web",
    execute = function ()

        if os.istarget("linux") then
         os.execute("emmake make UCodeIDE config=debug_web -j4")
        end

        if os.istarget("windows") then
         os.execute("emmake make UCodeIDE config=debug_web -j4")
        end
        
        if os.istarget("macosx") then
         os.execute("emmake make UCodeIDE config=debug_web -j4")
        end
    end
}

--Docs
newaction {
    trigger = "build_Docs",
    description = "build Docs",
    execute = function ()

        if os.istarget("linux") then
         os.execute("cd ./UCodeDocumentation;./tools/mdbook build --dest-dir ../Output/UCodeDocumentation;cp -r ../Output/UCodeDocumentation ../UCodeWebsite/static")
        end

        if os.istarget("windows") then
         os.execute("cd UCodeDocumentation;tools/mdbook.exe build --dest-dir ../Output/UCodeDocumentation;cp -r ../Output/UCodeDocumentation ../UCodeWebsite/static")
        end
        
        if os.istarget("macosx") then
         os.execute("emmake make UCodeIDE config=debug_web -j4")
        end
    end
}