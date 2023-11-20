# invalid(keyword)

the  invalid keyword is the compile time variant of the [Panic keyword](Panic.md)

the invalid keyword will cause an Error at compile time when the compiler evaluates the statement.


for example:
```
|main[] -> void:
  invalid("Error cant find the main implementation.");

```

the compiler will output the Error:
 
on line (Invaild keyword line number) invalid: "Error cant find the main implementation."

also works with [vaild keyword](valid.md) as it makes an error.

invalid can take in the parameters 


- [Span\<char> or StringView](../../Standardlibrary/Types/Classes/StringView.md) //constant expression a span of chars.

- [String](../../Standardlibrary/Types/Classes/String.md) //constant expression string

 - 0 parameters will out put the Error instead "Reached Invaild Statemet"


