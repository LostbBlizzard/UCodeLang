# RangeFrom_t\<T>(Standardlibrary/Miscellaneous)

The Type holds a range end of any type. 

The Range_t type Can be used like so.
```cpp
var Range = RangeFrom_t<int>(10);
RangeFrom_t<int> Range2 = RangeFrom(10); 
RangeFrom_t<int> Range3 = 10..; //this is short hand for calling the RangeFrom funcion
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
|RangeFrom<T>[T start] -> RangeFrom_t<T>; | Makes a Range with a start.
## Notes

