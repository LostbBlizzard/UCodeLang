# Update Command


Example
```
uclang <path>
```

if path not provided it will use the working directory as its input.

Attempts the update the dependencieses and  reverse the changes if the dependency update breaks the module build and the test fail.

this may take a while as it does a binary search to find the newest compatible dependeny without breaking the module for each dependency.


The exit code is 0 it did so successfully or all module are up too date else its 1.