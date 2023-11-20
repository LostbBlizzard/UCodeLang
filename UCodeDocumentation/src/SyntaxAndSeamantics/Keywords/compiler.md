# compiler(Keyword)

Is a type that refers to systematic analysis part of the compiler. systematic analysis is are were symbols and types are checked.

it is used the make new symbols,type,funcions.
and can used to read compiler data and Settings.

```
bool onwindows = compiler::IsBackendWindows();
```


# Methods
| Funcion | Parameters | Return | Description |
|--- |--- |--- | --- |
| IsBackendC89 | None | eval bool | Is true if are targeting the C backend.
| IsBackendWasm | None | eval bool | Is true if we are targeting Wasm for the web.
|IsBackendUCodeVM | None | eval bool | Is true if we are targeting the UCodeVM.
| IsBackendWindows | None | eval bool | Is true if we are targeting windows.
| IsBackendLinux | None | eval bool | Is true if we are targeting Linux.
| IsBackendMacOS | None | eval bool | Is true if we are targeting MaxOS.
| IsCpuX86 | None | eval bool | Is true if we are targeting X86 Chips.
| IsCpuArm | None | eval bool | Is true if we are targeting Arm Chips.
| IsDebug | None | eval bool | Is true if the Debug flag was set.
| HasFlag | [char[\\] Name] | eval bool | Is true a flag exists.
| GetFlagValue | [char[\\] FlagName,out char[\\] OutValue] | eval bool | returns the flag if it exists and sets OutValue the to the Associated value as a String.
| GetFlagValue | [char[\\] FlagName,out int OutValue] | eval bool | returns the flag if it exists and sets OutValue the to the Associated value as a int.
| GetFlagValue | [char[\\] FlagName,out bool OutValue] | eval bool | returns the flag if it exists and sets OutValue the to the Associated value as a bool.
| GetFlagValue | [char[\\] FlagName,out float OutValue] | eval bool | returns the flag if it exists and sets OutValue the to the Associated value as a float.


> [!IMPORTANT]
> 
> GetFlagValue and HasFlag is not implemented in the compiler right now.
