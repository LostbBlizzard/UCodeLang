# defer(keyword)

The defer keyword is used close or to free resources or deal with stacks.
defer will execute the code after the scope has ended and even when the program [panics](panic.md).


it can be used like so.
```
defer func();
```
Or even have a list of statements
```
defer:
  func();
  func1();
  func3();
```

Heres an example of when defer will execute the code.
```
use ULang;

int A = 0;
|func[]: 
 A = 10;

|main[]:
 
 Fmt::Print("The Value of 'A' befor defer: {A}");
 block:
   defer func();
   Fmt::Print("The Value of 'A' in the defer scope: {A}");
 
 Fmt::Print("The Value of 'A' Out of defer scope: {A}");
```

Note varables captured on the defer keyword line.

so this will allways print 20 and not 30.

```
|main[]:
 block:
 int a = 20;
 defer Fmt::Print(a);//the varable 'a' is captured here.
 a = 30;
```