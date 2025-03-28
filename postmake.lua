local innosetup = postmake.loadplugin("internal/innosetup")
local shellscript = postmake.loadplugin("internal/shellscript")

local InnoAppID = "{{1CA82B81-B465-41F7-9FFE-7DF174047519}"

-- App Settings
postmake.appname = "UCodeLang"


local function readAll(file)
	local f = assert(io.open(file, "rb"))
	local content = f:read("*all")
	f:close()
	return content
end

postmake.appversion = "0.0.1"
postmake.appversion = readAll("version.txt")

postmake.output = "./Output/install"

postmake.appinstalldir = "~/.ucodelang"
postmake.applicensefile = "LICENSE.txt"
postmake.apppublisher = "LostbBlizzard"
postmake.appwebsite = "https://github.com/LostbBlizzard/UCodeLang"


if postmake.target() == "symlink" then
	local configtype = ""
	if postmake.os.uname.os() == 'windows' then
		configtype = "Win64"
	elseif postmake.os.uname.os() == 'linux' then
		configtype = "linux64"
	elseif postmake.os.uname.os() == 'macos' then
		configtype = "MacOS"
	end

	local function symlink(input, output)
		print("symlink " .. output .. " >> " .. input)

		local issymlink = false
		if not issymlink then
			if postmake.os.exist(output) then
				if postmake.os.IsFile(output) then
					postmake.os.rm(output)
				else
					postmake.os.rmall(output)
				end
			end

			postmake.os.ln(input, output)
		end
	end

	local thisinstalldir = postmake.path.absolute(postmake.appinstalldir) .. postmake.installdir()

	local mainfile = thisinstalldir .. "tools/" .. "uclang"
	local lspfile = thisinstalldir .. "bin/" .. "uclanglsp"
	if postmake.os.uname.os() == 'windows' then
		mainfile = mainfile .. ".exe"
		lspfile = lspfile .. ".exe"
	end
	postmake.os.mkdirall(thisinstalldir .. "/bin")
	postmake.os.mkdirall(thisinstalldir .. "/tools")

	if postmake.os.exist(thisinstalldir .. "/module") then
		postmake.os.rmall(thisinstalldir .. "/module")
	end
	postmake.os.mkdirall(thisinstalldir .. "/module")

	symlink(postmake.path.absolute("./Output/UCodeLangCL/" .. configtype .. "/Debug/uclang"), mainfile)


	symlink(
		postmake.path.absolute("./Output/UCodeLanguageSever/" .. configtype .. "/Debug/uclanglsp"),
		lspfile)
	symlink(postmake.path.absolute("./Output/UCodeDocumentation"), thisinstalldir .. "doc")
	symlink(mainfile, thisinstalldir .. "bin/uclang")

	symlink(postmake.path.absolute("./NStandardLibrary"), thisinstalldir .. "module/NStandardLibrary")
	symlink(postmake.path.absolute("./StandardLibrary"), thisinstalldir .. "module/StandardLibrary")
	symlink(postmake.path.absolute("./BuildSystem"), thisinstalldir .. "module/BuildSystem")
	symlink(postmake.path.absolute("./CompilerAPI"), thisinstalldir .. "module/CompilerAPI")

	symlink(postmake.path.absolute("./LICENSE.txt"), thisinstalldir .. "LICENSE.txt")

	return
end

-- Short Hands
local all = postmake.allconfig
local unix = postmake.foros("unix")


local mainstartup = postmake.installdir() .. "./bin/" .. "uclang"

local unixmainprogram = postmake.installdir() .. "./tools/" .. "uclang"
local winsmainprogram = unixmainprogram .. ".exe"

local unixlspmainprogram = postmake.installdir() .. "./bin/uclanglsp"
local winslspmainprogram = unixlspmainprogram .. ".exe"

--- Configs
local win = postmake.newconfig("windows", "x64")
local win32 = postmake.newconfig("windows", "x32")
local winarm = postmake.newconfig("windows", "arm64")

local gnu = postmake.newconfig("linux", "x64")
local gnu32 = postmake.newconfig("linux", "x32")
local gnuarm64 = postmake.newconfig("linux", "arm64")

local mac = postmake.newconfig("macos", "universal")

--- Add Your files

local installwebsite =
"https://github.com/LostbBlizzard/UCodeLang/releases/download/Release-"
installwebsite = installwebsite .. postmake.appversion

--- flags
local addpathflag = all.newflag("Add Path", true)

all.If(addpathflag).addpath(postmake.installdir() .. "/bin")

local mode = "Debug"
if postmake.os.exist("./Output/UCodeLang/linux64/Published") then
	mode = "Published"
end

---@type {config: Config, path:string,lsp:string}[]
local programs = {
	{
		config = win32,
		path = "Output/UCodeLangCL/Win32/" .. mode .. "/uclang",
		lsp = "Output/UCodeLanguageSever/Win32/" .. mode .. "/uclanglsp"
	},
	{
		config = win,
		path = "Output/UCodeLangCL/Win64/" .. mode .. "/uclang",
		lsp = "Output/UCodeLanguageSever/Win64/" .. mode .. "/uclanglsp"
	},

	{
		config = gnu,
		path = "Output/UCodeLangCL/linux64/" .. mode .. "/uclang",
		lsp = "Output/UCodeLanguageSever/linux64/" .. mode .. "/uclanglsp"
	},
	{
		config = gnu32,
		path = "Output/UCodeLangCL/linux32/" .. mode .. "/uclang",
		lsp = "Output/UCodeLanguageSever/linux32/" .. mode .. "/uclanglsp"
	},

	{
		os = mac,
		arch = "arm64",
		path = "Output/UCodeLangCL/MacOS/" .. mode .. "/uclang",
		lsp = "Output/UCodeLanguageSever/MacOS/" .. mode .. "/uclanglsp"
	},

}
for _, value in ipairs(programs) do
	if postmake.os.exist(value.path) then
		if value.config.os() == 'windows' then
			value.config.addxfile(value.path, winsmainprogram)
		else
			value.config.addxfile(value.path, unixmainprogram)
		end
	end
	if postmake.os.exist(value.lsp) then
		if value.config.os() == 'windows' then
			value.config.addxfile(value.lsp, winslspmainprogram)
		else
			value.config.addxfile(value.lsp, unixlspmainprogram)
		end
	end
end

local ModuleIndex = postmake.installdir() .. "/ModuleIndex.ucmi"
--- all version
all.addfile("UCodeAPI/StandardLibrary/**.ucm", postmake.installdir() .. "module/StandardLibrary")
all.addfile("UCodeAPI/StandardLibrary/**.uc", postmake.installdir() .. "module/StandardLibrary")
all.addfile("UCodeAPI/StandardLibrary/LICENSE.txt", postmake.installdir() .. "module/StandardLibrary/LICENSE.txt")

all.addfile("UCodeAPI/NStandardLibrary/**.ucm", postmake.installdir() .. "module/NStandardLibrary")
all.addfile("UCodeAPI/NStandardLibrary/**.uc", postmake.installdir() .. "module/NStandardLibrary")
all.addfile("UCodeAPI/NStandardLibrary/LICENSE.txt", postmake.installdir() .. "module/NStandardLibrary/LICENSE.txt")

all.addfile("UCodeAPI/BuildSystem/**.ucm", postmake.installdir() .. "module/BuildSystem")
all.addfile("UCodeAPI/BuildSystem/**.uc", postmake.installdir() .. "module/BuildSystem")
all.addfile("UCodeAPI/BuildSystem/LICENSE.txt", postmake.installdir() .. "module/BuildSystem/LICENSE.txt")

all.addfile("UCodeAPI/CompilerAPI/**.ucm", postmake.installdir() .. "module/CompilerAPI")
all.addfile("UCodeAPI/CompilerAPI/**.uc", postmake.installdir() .. "module/CompilerAPI")
all.addfile("UCodeAPI/CompilerAPI/LICENSE.txt", postmake.installdir() .. "module/CompilerAPI/LICENSE.txt")

all.addfile("LICENSE.txt", postmake.installdir() .. "License.txt")

all.addfile("Output/UCodeDocumentation/**", postmake.installdir() .. "doc")

all.addinstallcmd(unixmainprogram, { "index", postmake.installdir() .. "module/StandardLibrary" })
all.addinstallcmd(unixmainprogram, { "index", postmake.installdir() .. "module/NStandardLibrary" })
all.addinstallcmd(unixmainprogram, { "index", postmake.installdir() .. "module/BuildSystem" })
all.addinstallcmd(unixmainprogram, { "index", postmake.installdir() .. "module/CompilerAPI" })

unix.adduninstallcmd("rm ", { ModuleIndex })


postmake.make(shellscript, { gnu, gnu32, gnuarm64, mac },
	---@type ShellScriptConfig
	{
		weburl = installwebsite,
		uploaddir = "./Output/upload",
		testmode = postmake.target() == "test",
		checksum = 'sha256',
		style = 'modern',
		uninstallfile = mainstartup,
		proxy = {
			uninstallcmd = "uninstall",
			program = unixmainprogram,
		},
	}
);
postmake.make(innosetup, { win, win32, winarm },
	---@type InnoSetConfig
	{
		AppId = InnoAppID,
		LaunchProgram = winsmainprogram,
		UninstallDelete = {
			ModuleIndex,
		},

	});
