# runmod Command 

runs a module on the system.
By default uclang will try to call the main funcion.

```
uclang runmod [module]
```

This can be overwritten and basic arguments can be passed.

```
uclang runmod [module] func(4)
```

The exit code is 0 if compilation succeed or 1 for compilation fail or if it panic or returns an error if the return type is an result type.

after runing the console will look like the following
```
returned:
void
```
or 
```
returned:
1
```

| Flag | extra | Description |
|--- |--- | --- |
-jit | none | Uses the jit compiler compiler instead of the interpreter.
-03 | none | Activates all optimization.
-native | none | Compiles native code before running. 
