# RangeInclusive_t\<T>(Standardlibrary/Miscellaneous)

The Type holds a end of any type that and its inclusive.

The RangeInclusive_t type Can be used like so.
```cpp
var Range = RangeInclusive_t<int>(0,10);
RangeInclusive_t<int> Range2 = RangeInclusive(0,10); 
RangeInclusive_t<int> Range3 = 1..=10; //this is short hand for calling the RangeInclusive funcion
```
All being equivalent.


## Member functions
|  Signature |  Description
 --- | --- |
 \|new[this&] -> void; | Makes a New Range.
 \|new[this&,T start] -> void; | Makes a New Range with a start.

## Global funcions
|  Signature |  Description
 --- | --- |
|RangeInclusive<T>[T start] -> RangeInclusive_t<T>; | Makes a Range with a start.
## Notes
