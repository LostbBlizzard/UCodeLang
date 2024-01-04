# Build Command 

By default uclang build will compile to UCodeVM being 32 or 64 bit if the  current cpu is 32 or 64.

if path not provided it will use the working directory as its input and use a UCodeModule Project in it to Compile.

The exit is 0 if compilation succeed or 1 for compilation fail.

Example
```
uclang build -c89
```

```
uclang build -uvm:x86_64
```

| Flag | extra | Description |
|--- |--- |--- |
  -01 | none | builds using  using optimizations for Size.
  -02 | none | builds using  using optimizations for Speed.
  -03 | none | builds using  using optimizations for Maximum speed.
  -ndebug | none | disable debug mode and removes debug Symbols
  -32 | none | Sets build Target as 32bit Machine 
  -64 | none | Sets build Target as 64bit Machine
  -c11 | none | build using C BackEnd.
  -wasm | none | build using the wasm BackEnd.
  -ir | none | Outputs the IR.
  -uvm | this,x86_64,x86,x86,arm_64,arm | Build Native Code for the UCodeVm the .ulib can run on any Operating system(Most of the time) for that cpu chip. 
 -native | this,linux,windows,mac | Build Native Code Executable


 