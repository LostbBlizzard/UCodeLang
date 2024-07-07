# Result\<T>(Standardlibrary/ErrorHandleing)

Result is a type that is part of the standard library. that can hold either a value of type T or an error(Null). 

The Result type Can be used like so.
```cpp
$MyErrorType;
int!MyErrorType MayHaveError = Null;
Result<int,Result> MayHaveError = Null;
```
Both being equivalent.


The Result is an [enum](../../SyntaxAndSeamantics/Types/Enum.md) and here its implementation.

```cpp
$Result<T,E>:
 Val(T val),
 Err(E err),
```

The Result type allows you to returning Error in a safe way and predictable.


# Question Mark Operator, 
The question mark operator, ? allows for easier use of the result type.


It lets.
```cpp
|test[] -> Empty!OpenFileError:
 var fileop = File::Open("somefile.txt");
 if Val(fileop,out openedfile):
   openedfile.Write("Hello");
   ret Val(Empty());
 else if Err(fileop,out err):
   ret err; 

```
To become.

```cpp
|test[] -> Empty!OpenFileError:
  var tep = (?File::Open("somefile.txt")).Write("Hello");
  ret Emtpy()
```

When starting an expression with ? UCodeLang will check if its an Error and will do an early return if it is.

makeing your life easier.
