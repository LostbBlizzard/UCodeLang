# Test Command


Example
```
uclang test <path>
```

if path not provided it will use the working directory as its input.

Runs all the test in the module.

```
use ULang;
[Test]
|MyTest[]: 
 Assert(1 == 1);
```
The exit code is 0 it did so successfully and all the test pass else its 1.