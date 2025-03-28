# Range_t\<T>(Standardlibrary/Miscellaneous)

The Type holds a start and end of any type. 

The Range_t type Can be used like so.
```cpp
var Range = Range_t<int>(0,10);
Range_t<int> Range2 = Range(0,10); 
Range_t<int> Range3 = 0..10; //this is short hand for calling the Range funcion
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
|Range<T>[T start,T end] -> Range_t<T>; | Makes a Range with the start and end.
## Notes
