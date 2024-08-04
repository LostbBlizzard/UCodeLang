# RangeTo_t\<T>(Standardlibrary/Miscellaneous)

The Type holds a end of any type. 

The Range_t type Can be used like so.
```cpp
var Range = RangeTo_t<int>(10);
RangeTo_t<int> Range2 = RangeTo(10); 
RangeTo_t<int> Range3 = ..10; //this is short hand for calling the RangeTo funcion
```
All being equivalent.


## Member functions
|  Signature |  Description
 --- | --- |
 \|new[this&] -> void; | Makes a New RangeTo_t.
 \|new[this&,T end] -> void; | Makes a New RangeTo_t with start an end.

## Global funcions
|  Signature |  Description
 --- | --- |
|RangeTo<T>[T end] -> Range_t<T>; | Makes a RangeTo_t with an end.
## Notes
