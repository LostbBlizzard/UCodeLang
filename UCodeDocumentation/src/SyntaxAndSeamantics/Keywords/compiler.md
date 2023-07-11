# compiler(KeyWord)

is a type that refers to systematic analysis part of the compiler. systematic analysis is are were symbols and types are checked.

it is used the make new symbols,type,funcions.
and can used to read compiler data and Settings.

Funcions:
- GetOutputTarget();//x86,x86_64,IR,UCodeVM
- GetOutOS();//windows,iOS,Android,Linux

- NewFuncionSymbol(char[\\] FuncionName);
- NewTypeSymbol(char[\\] FuncionName)

- HasFlag(char[\\] FlagName) -> bool;

- GetFlagValue(char[\\] FlagName,out int OutValue) -> bool;

- GetFlagValue(char[\\] FlagName,out char[\\] OutValue) -> bool;

- GetFlagValue(char[\\] FlagName,out bool OutValue) -> bool;

- GetFlagValue(char[\] FlagName,out char OutValue) -> bool;

- GetFlagValue(char[\] FlagName,out float OutValue) -> bool;