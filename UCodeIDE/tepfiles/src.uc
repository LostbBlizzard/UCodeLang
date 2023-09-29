

use ULang;

eval bool IsC89 = compiler::IsBackendC89();
eval bool IsUCodeVm = compiler::IsBackendUCodeVM();
eval bool IsWasm = compiler::IsBackendWasm();
eval bool IsDebug = compiler::IsDebug();

|C89[] => IsC89;
|UCodeVm[] => IsUCodeVm;
|Wasm[] => IsWasm;
|Debug[] => IsDebug;

|main[]: 
 //Fmt::Print("Hello World");

