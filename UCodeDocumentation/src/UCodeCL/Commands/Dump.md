# Dump Command 

Example
```
uclang dump <path>
```

Its Disassembles a .ulib or a .wasm file or a project.

if path not provided it will use the working directory as its input and use a UCodeModule Project in it to Compile and dump its output.


note this is manly used for debuging.

The exit code is 0 it did so successfully else its 1.