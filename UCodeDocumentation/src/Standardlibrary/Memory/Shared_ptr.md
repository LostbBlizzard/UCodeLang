# Shared_ptr(Standardlibrary/Memory)

A Shared_ptr is class that can  share ownership of an object.useing reference counting to free the resource.
```cpp
int$ obj = shr int(10);
Shared_ptr<int> obj = Shared_ptr<int>::Make(10);
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