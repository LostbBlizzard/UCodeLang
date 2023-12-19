; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "UCodeLang"
#define MyAppVersion "0.1"
#define MyAppPublisher "LostbBlizzard"
#define MyAppURL "https://github.com/LostbBlizzard/UCodeLang"
#define MyAppExeName "uclang.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{1CA82B81-B465-41F7-9FFE-7DF174047519}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={%USERPROFILE}\.ucodelang
DisableDirPage=yes
DefaultGroupName=UCodeLang
DisableProgramGroupPage=yes
LicenseFile=LICENSE.txt
; Remove the following line to run in administrative install mode (install for all users.)
PrivilegesRequired=lowest
OutputBaseFilename=UCodeLangSetup
Compression=lzma
SolidCompression=yes
WizardStyle=modern
SetupIconFile=Logo.ico

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "Output\UCodelangCL\Win64\Published\{#MyAppExeName}"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "Output\UCodeLanguageSever\Win64\Published\uclanglsp.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "LICENSE.txt"; DestDir: "{app}\bin";

Source: "UCodeAPI\StandardLibrary\*.ucm"; DestDir: "{app}\module\StandardLibrary"; Flags: ignoreversion recursesubdirs
Source: "UCodeAPI\StandardLibrary\*.uc"; DestDir: "{app}\module\StandardLibrary"; Flags: ignoreversion recursesubdirs
Source: "UCodeAPI\StandardLibrary\LICENSE.txt"; DestDir: "{app}\module\StandardLibrary"; Flags: ignoreversion

Source: "UCodeAPI\NStandardLibrary\*.uc"; DestDir: "{app}\module\NStandardLibrary"; Flags: ignoreversion recursesubdirs
Source: "UCodeAPI\NStandardLibrary\*.ucm"; DestDir: "{app}\module\NStandardLibrary"; Flags: ignoreversion recursesubdirs
Source: "UCodeAPI\NStandardLibrary\LICENSE.txt"; DestDir: "{app}\module\StandardLibrary"; Flags: ignoreversion

Source: "UCodeDocumentation\src\*.md"; DestDir: "{app}\docs"; Flags: ignoreversion recursesubdirs

[UninstallDelete]
Type: files; Name: "{app}\ModuleIndex.ucmi"

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Tasks]
Name: envPath; Description: "Add to PATH variable" 

[Code]
const EnvironmentKey = 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment';

procedure EnvAddPath(Path: string);
var
    Paths: string;
begin
    { Retrieve current path (use empty string if entry not exists) }
    if not RegQueryStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths)
    then Paths := '';

    { Skip if string already found in path }
    if Pos(';' + Uppercase(Path) + ';', ';' + Uppercase(Paths) + ';') > 0 then exit;

    { App string to the end of the path variable }
    Paths := Paths + ';'+ Path +';'

    { Overwrite (or create if missing) path environment variable }
    if RegWriteStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths)
    then Log(Format('The [%s] added to PATH: [%s]', [Path, Paths]))
    else Log(Format('Error while adding the [%s] to PATH: [%s]', [Path, Paths]));
end;

procedure EnvRemovePath(Path: string);
var
    Paths: string;
    P: Integer;
begin
    { Skip if registry entry not exists }
    if not RegQueryStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths) then
        exit;

    { Skip if string not found in path }
    P := Pos(';' + Uppercase(Path) + ';', ';' + Uppercase(Paths) + ';');
    if P = 0 then exit;

    { Update path variable }
    Delete(Paths, P - 1, Length(Path) + 1);

    { Overwrite path environment variable }
    if RegWriteStringValue(HKEY_LOCAL_MACHINE, EnvironmentKey, 'Path', Paths)
    then Log(Format('The [%s] removed from PATH: [%s]', [Path, Paths]))
    else Log(Format('Error while removing the [%s] from PATH: [%s]', [Path, Paths]));
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
    if (CurStep = ssPostInstall) and IsTaskSelected('envPath')
    then EnvAddPath(ExpandConstant('{app}') +'\bin');
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
    if CurUninstallStep = usPostUninstall
    then EnvRemovePath(ExpandConstant('{app}') +'\bin');
end;

[Run]

Filename: "{app}\bin\{#MyAppExeName}"; Parameters: "index {app}\module\StandardLibrary"
Filename: "{app}\bin\{#MyAppExeName}"; Parameters: "index {app}\module\NStandardLibrary"

Filename: "{app}\bin\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

