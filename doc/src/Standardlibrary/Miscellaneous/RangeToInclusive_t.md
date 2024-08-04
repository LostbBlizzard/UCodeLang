# RangeToInclusive_t\<T>(Standardlibrary/Miscellaneous)

The Type holds a start and end of any type and its inclusive.

The Range_t type Can be used like so.
```cpp
var Range = RangeToInclusive_t<int>(0,10);
RangeToInclusive_t<int>  Range2 = RangeToInclusive(0,10); 
RangeToInclusive_t<int>  Range3 = ..=10; //this is short hand for calling the RangeToInclusive funcion
```
All being equivalent.


## Member functions
|  Signature |  Description
 --- | --- |
 \|new[this&] -> void; | Makes a New Range.
 \|new[this&,T start,T end] -> void; | Makes a New Range with start and end.

## Global funcions
|  Signature |  Description
 --- | --- |
|RangeToInclusive<T>[T start,T end] -> RangeToInclusive_t<T>; | Makes a Range with the start and end.
## Notes
