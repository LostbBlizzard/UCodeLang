local innosetup = postmake.loadplugin("internal/innosetup")
local shellscript = postmake.loadplugin("internal/shellscript")
local githubaction = postmake.loadplugin("internal/githubaction")

local InnoAppID = "{{1CA82B81-B465-41F7-9FFE-7DF174047519}"

-- App Settings
postmake.appname = "uclang"
postmake.appversion = "0.0.1"
postmake.apppublisher = "LostbBlizzard"
postmake.appinstalldir = "~/.ucodelang"
postmake.output = "./Output/install"

local f = io.open("version.txt", "rb")
if f then
    local content = f:read("*all")
    f:close()
    postmake.appversion = content
end

local function executeorexit(str)
    local exit = os.execute(str)

    if exit == nil then
        os.exit(1)
    end

    if not exit == true then
        os.exit(1)
    end
end

local debugmode = os.getenv("POSTMAKEDEBUGMODE") ~= nil
local mod
if debugmode then
    mod = "Debug"
else
    mod = "Published"
end

-- Short Hands
local all = postmake.allconfig

--- Configs
local win64 = postmake.newconfig("windows", "x64")
local win32 = postmake.newconfig("windows", "x32")
local winarm64 = postmake.newconfig("windows", "arm64")

local gnu64 = postmake.newconfig("linux", "x64")
local gnu32 = postmake.newconfig("linux", "x32")
local gnuarm64 = postmake.newconfig("linux", "arm64")

local mac = postmake.newconfig("macos", "universal")

local unix = postmake.foros("unix")
--- flags
local addpathflag = all.newflag("Add Path", true)
--- Add Your files

local mainprogrampath = postmake.installdir() .. "bin/" .. postmake.appname
local winmainprogrampath = mainprogrampath .. ".exe"

local unixprogrampath = postmake.installdir() .. "tools/" .. postmake.appname
local winsmainprogram = unixprogrampath .. ".exe"

local unixprogrampathlsp = postmake.installdir() .. "bin/" .. "uclanglsp"
local winsmainprogramlsp = unixprogrampathlsp .. ".exe"

---@param config Config
---@param input string
---@param output string
local function addxfile(config, input, output)
    if postmake.os.exist(input) then
        config.addxfile(input, output)
    end
end

addxfile(win64, "Output/UCodeLangCL/Win64/" .. mod .. "/uclang", winsmainprogram)
addxfile(win64, "Output/UCodeLanguageSever/Win64/" .. mod .. "/uclanglsp", winsmainprogramlsp)

addxfile(win32, "Output/UCodeLangCL/Win32/" .. mod .. "/uclang", winsmainprogram)
addxfile(win32, "Output/UCodeLanguageSever/Win32/" .. mod .. "/uclanglsp", winsmainprogramlsp)

addxfile(winarm64, "Output/UCodeLangCL/WinArm/" .. mod .. "/uclang", winsmainprogram)
addxfile(winarm64, "Output/UCodeLanguageSever/WinArm/" .. mod .. "/uclanglsp", winsmainprogramlsp)

addxfile(gnu64, "Output/UCodeLangCL/linux64/" .. mod .. "/uclang", unixprogrampath)
addxfile(gnu64, "Output/UCodeLanguageSever/linux64/" .. mod .. "/uclanglsp", unixprogrampathlsp)

addxfile(gnu32, "Output/UCodeLangCL/linux32/" .. mod .. "/uclang", unixprogrampath)
addxfile(gnu32, "Output/UCodeLanguageSever/linux32/" .. mod .. "/uclanglsp", unixprogrampathlsp)

addxfile(gnuarm64, "Output/UCodeLangCL/linuxArm64/" .. mod .. "/uclang", unixprogrampath)
addxfile(gnuarm64, "Output/UCodeLanguageSever/linuxArm64/" .. mod .. "/uclanglsp", unixprogrampathlsp)

addxfile(mac, "Output/UCodeLangCL/MacOS/" .. mod .. "/uclang", unixprogrampath)
addxfile(mac, "Output/UCodeLanguageSever/MacOS/" .. mod .. "/uclang", unixprogrampathlsp)

all.addfile("UCodeAPI/NStandardLibrary/**.ucm", postmake.installdir() .. "module/NStandardLibrary")
all.addfile("UCodeAPI/NStandardLibrary/**.uc", postmake.installdir() .. "module/NStandardLibrary")
all.addfile("UCodeAPI/NStandardLibrary/LICENSE.txt", postmake.installdir() .. "module/NStandardLibrary/LICENSE.txt")
all.addfile("UCodeAPI/NStandardLibrary/.gitignore", postmake.installdir() .. "module/NStandardLibrary/.gitignore")

all.addfile("UCodeAPI/StandardLibrary/**.ucm", postmake.installdir() .. "module/StandardLibrary")
all.addfile("UCodeAPI/StandardLibrary/**.uc", postmake.installdir() .. "module/StandardLibrary")
all.addfile("UCodeAPI/StandardLibrary/LICENSE.txt", postmake.installdir() .. "module/StandardLibrary/LICENSE.txt")
all.addfile("UCodeAPI/StandardLibrary/.gitignore", postmake.installdir() .. "module/StandardLibrary/.gitignore")

all.addfile("UCodeAPI/BuildSystem/**.ucm", postmake.installdir() .. "module/BuildSystem")
all.addfile("UCodeAPI/BuildSystem/**.uc", postmake.installdir() .. "module/BuildSystem")
all.addfile("UCodeAPI/BuildSystem/LICENSE.txt", postmake.installdir() .. "module/BuildSystem/LICENSE.txt")
all.addfile("UCodeAPI/BuildSystem/.gitignore", postmake.installdir() .. "module/BuildSystem/.gitignore")

all.addfile("UCodeAPI/CompilerAPI/**.ucm", postmake.installdir() .. "module/CompilerAPI")
all.addfile("UCodeAPI/CompilerAPI/**.uc", postmake.installdir() .. "module/CompilerAPI")
all.addfile("UCodeAPI/CompilerAPI/LICENSE.txt", postmake.installdir() .. "module/CompilerAPI/LICENSE.txt")
all.addfile("UCodeAPI/CompilerAPI/.gitignore", postmake.installdir() .. "module/CompilerAPI/.gitignore")

executeorexit("cd ./doc && mdbook build --dest-dir ../Output/UCodeDocumentation")

all.addfile("Output/UCodeDocumentation/**", postmake.installdir() .. "doc")

all.If(addpathflag).addpath(postmake.installdir())

all.addinstallcmd(unixprogrampath, { "index", postmake.installdir() .. "module/NStandardLibrary" })
all.addinstallcmd(unixprogrampath, { "index", postmake.installdir() .. "module/StandardLibrary" })
all.addinstallcmd(unixprogrampath, { "index", postmake.installdir() .. "module/BuildSystem" })
all.addinstallcmd(unixprogrampath, { "index", postmake.installdir() .. "module/CompilerAPI" })

unix.adduninstallcmd("rm", { postmake.installdir() .. "ModuleIndex.ucmi" })

local installwebsite = "https://github.com/LostbBlizzard/UCodeLang/releases/tag/Release-" .. postmake.appversion

postmake.make(shellscript, { gnu64, gnu32, gnuarm64, mac },
    ---@type ShellScriptConfig
    {
        weburl = installwebsite,
        uploaddir = "./Output/upload/",
        singlefile = "ShellInstallFiles",
        testmode = debugmode,
        uninstallfile = mainprogrampath,
        proxy = {
            uninstallcmd = "uninstall",
            program = unixprogrampath
        },
        style = 'modern'
    });

postmake.make(innosetup, { win64, win32, winarm64 },
    ---@type InnoSetConfig
    {
        AppId = InnoAppID,
        LaunchProgram = winsmainprogram,
        proxy = {
            path = winsmainprogram,
            uninstallcmd = "uninstall",
            program = winsmainprogram
        },
        UninstallDelete = {
            postmake.installdir() .. "ModuleIndex.ucmi"
        }
    });

postmake.make(githubaction, { win64, win32, winarm64, gnu64, gnu32, gnuarm64, mac },
    ---@type GitHubActionConfig
    {
        weburl = installwebsite,
        uploaddir = "./Output/githubactionupload/",
        singlefile = "GitHubActionInstallFiles",
        version = {}
    });
