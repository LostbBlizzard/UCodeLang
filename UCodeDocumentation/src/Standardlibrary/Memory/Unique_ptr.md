# Unique_ptr\<T>(Standardlibrary/Memory)

A Unique_ptr is class that owns an object and free of that object when the unique_ptr goes out of scope.

```cpp
int^ obj = unq int(10);
Unique_ptr<int> obj = Unique_ptr<int>::Make(10);
```

Both being equivalent.


## Member functions
|  Signature |  Description
 --- | --- |
\|new[this&] -> void; | default contructer.
\|new[this&,moved this other] -> void; | move contructer.
\|new[this&,imut this& other] -> invalid; | removed copy contructer.
\|drop[this&] -> void; | default destructer.
\|Get[imut this&] -> imut T& | gets owned object.
\|Get[this&] -> T&; | gets owned object.
\|Make[this&] -> this; |
\|Make<T...>[this&,imut T pars] -> this; | allocate memory for type.
\|Make<T...>[this&,moved T pars] -> this; | allocate memory for type.