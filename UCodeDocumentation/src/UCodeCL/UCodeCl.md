# UCodeCL command-line reference

The UCodeLang Command-line tool it serves as a Compiler,Runtime,build system and package manager for UCodeLang.

# Syntax

<code>

    uclang [command] <arguments>

</code>

# Commands
| Name | arguments | Return Exit Code | Description |
|--- |--- |--- | --- |
help | none | 0 | Outputs all the all the available commands.
new | none | 0 if succeed else 1.  | makes a new UCodeLang Module Project.
build | Path(optional),flags(optional) | 0 if succeed else 1. | Builds a UCodeLang File,Module,Folder. [See More](#Build-Command)
run | Path(optional),flags(optional) | 0 if succeed else 1. | Builds a UCodeLang File,Module,Folder or .ulib and runs it. [See More](#Run-Command)
get | Path(optional) | 0 if succeed else 1. | Downloads the Modules dependencies.
dump | Path | 0 if succeed else 1. | Builds a UCodeLang File,Module,Folder or .ulib and attempts to convert the output into readable textworks with wasm,window,linux,macos,c89,.ulib,.uir outputs.
clear | Path| clears a Module a intermediate and output files.
cpptoulangvm | PatCppPath,CpplinkPath,ulangout | 0 if succeed else 1. | uses C++ source code to create bindings for UCodeVm
fmt | Path(optional)  | 0 if succeed else 1. |Formats A UCodeLang Module or file.
index | Path(optional)  | 0 if succeed else 1. | Adds Module to to the Module Index so it can be used/referenced by other module in your system.
test | Path(optional)  | 0 if succeed else 1. | Runs Tests on your UCodeLang Module Project.
update | Path(optional)  | 0 if succeed else 1. | updates Module Dependencies uses Tests to avoid breaking changes and revert back if needed this may take Sometime.
updatetools | none  | 0 if succeed else 1. | updates UCodeLangTools.
uninstalltools | none | uninstalls uclang and uclanglsp all downloaded modules will be left alone.
install | Name  | 0 if succeed else 1. | Installs a UCodeLangModule.
runmod | Name  | 0 if succeed else 1. | runs a Installed UCodeLangModule 
uninstallmod  | Name  | 0 if succeed else 1. | uninstalls a Installed UCodeLangModule
exits | none | 0 | exits ucodelang if opened without command line arguments.
# Build Command 

By default uclang build will compile to UCodeVM being 32 or 64 bit if the  current cpu is 32 or 64.

if path not provided it will use the working directory as its input and use a UCodeModule Project in it to Compile.

Example
<code>

    uclang build -c89

</code>

<code>

    uclang build -uvm:x86_64

</code>

| Flag | extra | Description |
|--- |--- |--- |
  -01 | none | builds using  using optimizations for Size.
  -02 | none | builds using  using optimizations for Speed.
  -03 | none | builds using  using optimizations for Maximum speed.
  -ndebug | none | disable debug mode and removes debug Symbols
  -32 | none | Sets build Target as 32bit Machine 
  -64 | none | Sets build Target as 64bit Machine
  -c89 | none | build using C BackEnd.
  -wasm | none | build using the wasm BackEnd.
  -ir | none | Outputs the IR.
  -uvm | this,x86_64,x86,x86,arm_64,arm | Build Native Code for the UCodeVm the .ulib can run on any Operating system(Most of the time) for that cpu chip. 
 -native | this,linux,windows,mac | Build Native Code Executable

# Run Command 

By default uclang will try to call the main funcion.

<code>

    uclang run

</code>

This can be overwritten and basic arguments can be passed.

<code>

    uclang run func(4)

</code>

These flags only apply if path was UCodeLang File,Module,Folder or .ulib.
| Flag | extra | Description |
|--- |--- |--- |
-jit | none | Uses the jit compiler compiler instead of the interpreter.
-03 | none | Activates all optimization.
-native | none | Compiles native code before running.