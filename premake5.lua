
workspace "UCodeLang"
   configurations { "Debug", "Release","Published" }
   platforms { "Win32", "Win64","linux32","linux64", "MacOS","Android","IOS","Web" }
   defines {"UCodeLangDebug","ZYCORE_STATIC_BUILD","ZYDIS_STATIC_BUILD","UCodeLangExperimental"}
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

   UCPathExeDir = "%{wks.location}/Output/UCodeLangCL/" .. OutDirPath .. "/"
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

   filter { "action:xcode*" }
        xcodebuildsettings = { ["ALWAYS_SEARCH_USER_PATHS"] = "YES"}

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
    if cppstl ~= nil then
    cppstl("c++")
    end

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

   

include "UCodeLang"   
include "UCodeLangCL"
include "UCodeIDE"
include "UCApp"
include "UCodeLanguageSever"
include "UCodeLanguageSeverlib"
   
include "UCodeAPI/NStandardLibrary"
include "UCodeAPI/CompilerAPI"
include "UCodeAPI/StandardLibrary"
include "UCodeAPI/BuildSystem"
 
   
function executeorexit(str)
 exit = os.execute(str)

 if exit == nil then
  os.exit(1)
 end 

 if not exit == true then
  os.exit(1)
 end 

end 

newaction {
    trigger = "installdependencies",
    description = "installsdependencies",
    execute = function ()
        print("----installing Dependencies for " .. os.target())
        
        if os.istarget("linux") then

          executeorexit("sudo apt-get update")

          print("----downloading libx11 Packages")
          executeorexit("sudo apt-get install libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev")

          print("----downloading opengl Packages")
          executeorexit("sudo apt install mesa-common-dev")
          
          print("----installing tools completed");
        end

        if os.istarget("windows") then

        end
        
        if os.istarget("macosx") then
          
           print("----downloading glfw Packages")

           executeorexit("arch -arm64 brew install glfw")
          
           print("----installing tools completed");
        end
    end
}


newaction {
    trigger = "installinno",
    description = "downloads inno setup and puts in output",
    execute = function ()
        print("----installing inno setup tools for " .. os.target())
        
        executeorexit("git clone https://github.com/LostbBlizzard/installed-inno Dependencies/bin/inno")
        
    end
}

newaction {
    trigger = "installzydis",
    description = "downloads zydis and zycore and puts in Dependencies",
    execute = function ()
        print("----installing zydis and zycore for " .. os.target())
        
        executeorexit("git clone https://github.com/LostbBlizzard/zydis UCodeLang/Dependencies/zydis")
        executeorexit("git clone https://github.com/LostbBlizzard/zycore-c UCodeLang/Dependencies/zycore")
        
    end
}
---build
newaction {
    trigger = "build",
    description = "builds the everything",
    execute = function ()
        
        if os.istarget("linux") then
         executeorexit("make -j$(nproc)")
        end

        if os.istarget("windows") then
         executeorexit("msbuild UCodeLang.sln /t:Build /p:Configuration=Debug /p:Platform=Win64 -maxcpucount")
        end
        
        if os.istarget("macosx") then
         executeorexit("make UCApp -j$(getconf _NPROCESSORS_ONLN)")
         executeorexit("make StandardLibrary -j$(getconf _NPROCESSORS_ONLN)")
        end
    end
}
newaction {
    trigger = "build_32bit",
    description = "builds everything",
    execute = function ()
        
        if os.istarget("linux") then
         executeorexit("make UCApp config=debug_linux32 -j$(nproc)")
        end

        if os.istarget("windows") then
         executeorexit("msbuild UCodeLang.sln /t:Build /p:Configuration=Debug /p:Platform=Win32 -maxcpucount")
        end
        
        if os.istarget("macosx") then
         -- We should do full build but macos keeps asking for glfw and i dont have a mac to fix it.
         executeorexit("make UCApp -j$(getconf _NPROCESSORS_ONLN)")
         executeorexit("make StandardLibrary -j$(getconf _NPROCESSORS_ONLN)")
        end
    end
}
newaction {
    trigger = "build_published",
    description = "builds everything",
    execute = function ()
        
        if os.istarget("linux") then
         executeorexit("make config=published_linux64 -j$(nproc)")
        end

        if os.istarget("windows") then
         executeorexit("msbuild UCodeLang.sln /t:Build /p:Configuration=Published  /p:Platform=Win64 -maxcpucount")
        end
        
        if os.istarget("macosx") then
         -- We should do full build but macos keeps asking for glfw and i dont have a mac to fix it.
         executeorexit("make UCodeLang config=published_macos -j$(getconf _NPROCESSORS_ONLN)")
         executeorexit("make StandardLibrary config=published_macos -j$(getconf _NPROCESSORS_ONLN)")
        end
    end
}
newaction {
    trigger = "build_published_32bit",
    description = "builds the everything",
    execute = function ()
        
        if os.istarget("linux") then
         executeorexit("make config=published_linux32 -j$(nproc)")
        end

        if os.istarget("windows") then
         executeorexit("msbuild UCodeLang.sln /t:Build /p:Configuration=Published /p:Platform=Win32 -maxcpucount")
        end
        
        if os.istarget("macosx") then
         executeorexit("make config=published_macos -j$(getconf _NPROCESSORS_ONLN)")
        end
    end
}

---test
newaction {
    trigger = "test",
    description = "runs tests",
    execute = function ()
        
        if os.istarget("linux") then
          executeorexit("cd ./UCApp;../Output/UCApp/linux64/Debug/UCApp --RunTests")
        end

        if os.istarget("windows") then
          executeorexit("cd ./UCApp && ..\\Output\\UCApp\\Win64\\Debug\\UCApp.exe --RunTests")
        end
        
        if os.istarget("macosx") then
          executeorexit("cd ./UCApp;../Output/UCApp/MacOS/Debug/UCApp --RunTests")
        end
    end
}
newaction {
    trigger = "build_android",
    description = "builds Android",
    execute = function ()
        executeorexit("export PREMAKE_ANDROIDNDK_CONFIGURATIONS=\"Debug\";export NDK_PROJECT_PATH=.;export APP_BUILD_SCRIPT=UCodeLang/UCodeLang.prj.Android.mk;ndk-build UCodeLang config=Android -j$(getconf _NPROCESSORS_ONLN) NDK_APPLICATION_MK=UCodeLang.Application.mk")
    end
}
newaction {
    trigger = "test_32bit",
    description = "runs test for 32bit mode",
    execute = function ()
        
        if os.istarget("linux") then
         executeorexit("cd ./UCApp;../Output/UCApp/linux32/Debug/UCApp --RunTests")
        end

        if os.istarget("windows") then
         executeorexit("cd ./UCApp && ..\\Output\\UCApp\\Win32\\Debug\\UCApp.exe --RunTests")
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
         executeorexit("make clean")
        end

        if os.istarget("windows") then
         executeorexit("msbuild UCodeLang.sln /t:Clean")
        end
        
        if os.istarget("macosx") then
         executeorexit("make clean")
        end
    end
}

--Docs
newaction {
    trigger = "build_docs",
    description = "build Docs",
    execute = function ()

        if os.istarget("linux") or os.istarget("macosx") then
         executeorexit("cd ./doc;mdbook build --dest-dir ../Output/UCodeDocumentation;cp -r ../Output/UCodeDocumentation ../website/static/doc")
        end

        if os.istarget("windows") then
         executeorexit("cd doc && mdbook build --dest-dir ..\\Output\\UCodeDocumentation && xcopy /e /i /q ..\\Output\\UCodeDocumentation ..\\website\\static\\doc")
        end
        
    end
}
-- Website
newaction {
    trigger = "build_website",
    description = "build website",
    execute = function ()
        
    end
}
--web
newaction {
    trigger = "web_build",
    description = "Builds for the Web",
    execute = function ()
        
        if os.istarget("macosx") then
         executeorexit("emmake make UCodeLang config=debug_web -j$(getconf _NPROCESSORS_ONLN)")
        else 
         executeorexit("emmake make UCodeLang config=debug_web -j$(nproc)")
        end

    end
}
newaction {
    trigger = "web_build_published",
    description = "Builds for the Web",
    execute = function ()
        
        if os.istarget("macosx") then
         executeorexit("emmake make UCodeLang config=published_web -j$(getconf _NPROCESSORS_ONLN)")
        else 
         executeorexit("emmake make UCodeLang config=published_web -j$(nproc)")
        end

    end
}

newaction {
    trigger = "buildinstaller",
    description = "build the installer",
    execute = function ()
        
        if os.istarget("linux") then
        end

        if os.istarget("windows") then
          executeorexit("Dependencies\\bin\\inno\\ISCC.exe install.iss")
        end
        
        if os.istarget("macosx") then
        end
    end
}

function readAll(file)
    local f = assert(io.open(file, "rb"))
    local content = f:read("*all")
    f:close()
    return content
end
function mysplit(inputstr, sep)
        if sep == nil then
                sep = "%s"
        end
        local t={}
        for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
                table.insert(t, str)
        end
        return t
end

function replacetextinfile(file_path, stringtomatch, replacement)
    local file = io.open(file_path, "r")  
    if not file then
        print("Error: File not found or unable to open.")
        return
    end

    local content = file:read("*a")  
    file:close()  
    
    local modified_content = content:gsub(stringtomatch, replacement)

    local new_file = io.open(file_path, "w")
    if not new_file then
        print("Error: Unable to write to file.")
        return
    end
    new_file:write(modified_content)
    new_file:close()
end

function isdigit(char)

 if char == '0' or char == '1' or char == '2'
    or  char == '3' or char == '4' or char == '5'
    or char == '6' or char == '6' or char == '7' or char == '8'
    or char == '8' or char == '9' 
 then
    return true
 else
    return false
 end 

end

function keeponlyfirstnumberpart(str)
   local r = ""

   for i=1, string.len(str) do 
    
    local cha = str:sub(i,i)--lua cant index strings? why

    if not isdigit(cha) then
        break
    end

    r = r .. cha
   end

   return r
end 

function file_exists(name)
   local f = io.open(name, "r")
   return f ~= nil and io.close(f)
end

newaction {
    trigger = "updateverion",
    description = "updates the verion number",
    execute = function ()
        local major = "0"
        local minor = "0"
        local patch = "0"

        local veriontext = readAll("version.txt")
        local verionspit = mysplit(veriontext,".");

        major = verionspit[1]

        major = verionspit[2]

        patch = keeponlyfirstnumberpart(verionspit[3])

        print("updating source files to " .. major .. "." .. minor .. "." .. patch)

        --Version.hpp
        local file = io.open("./UCodeLang/UCodeLang/LangCore/Version.hpp","w")

        file:write("#pragma once\n")
        file:write("#define UCodeLangMajorVersion " .. major .. "\n")
        file:write("#define UCodeLangMinorVersion " .. minor .. "\n")
        file:write("#define UCodeLangPatchVersion " .. patch .. "\n\n")
   
        
        file:write("#define UCodeLangVersionNumber (UCodeLangMajorVersion << 16) + (UCodeLangMinorVersion << 8) + UCodeLangPatchVersion")

        file:close()

        --install.sh
        replacetextinfile("./install.sh","#VersionMajor#",major)
        replacetextinfile("./install.sh","#VersionMinor#",minor)
        replacetextinfile("./install.sh","#VersionPatch#",patch)

        --install.iss
        replacetextinfile("./install.iss","#VersionMajor#",major)
        replacetextinfile("./install.iss","#VersionMinor#",minor)
        replacetextinfile("./install.iss","#VersionPatch#",patch)



    end
}

newaction {
    trigger = "zipforunix",
    description = "makes a zip file for install.sh to use",
    execute = function ()
        local files = "./LICENSE.txt ./UCodeAPI ./Output/UCodeDocumentation " 
        
        if os.istarget("linux") then
            if file_exists("./Output/UCodeLangCL/linux64/Published/uclang") then
               files = files .. "./Output/UCodeLangCL/linux64/Published/uclang "
            end

            if file_exists("./Output/UCodeLanguageSever/linux64/Published/uclanglsp") then
               files = files .. "./Output/UCodeLanguageSever/linux64/Published/uclanglsp "
            end
        
            if file_exists("./Output/UCodeLangCL/linux32/Published/uclang") then
               files = files .. "./Output/UCodeLangCL/linux32/Published/uclang " 
            end

            if file_exists("./Output/UCodeLanguageSever/linux32/Published/uclanglsp") then
               files = files .. "./Output/UCodeLanguageSever/linux32/Published/uclanglsp "
            end
        end

        if os.istarget("macos") then
        
            files = files .. "./Output/UCodeLangCL/MacOS/Published/uclang " .. "./Output/UCodeLanguageSever/MacOS/Published/uclanglsp "
        
        end

        executeorexit("tar -czvf ./Output/ULangPacked.tar.gz " .. files)
        

    end
}
newaction {
    trigger = "fixios",
    description = "",
    execute = function ()
        replacetextinfile("./UCodeLang/UCodeLang.xcodeproj/project.pbxproj","ALWAYS_SEARCH_USER_PATHS = NO","ALWAYS_SEARCH_USER_PATHS = YES")
    end
}
