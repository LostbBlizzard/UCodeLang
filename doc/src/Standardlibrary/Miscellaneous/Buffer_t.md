# Buffer_t\<T>(Standardlibrary/Miscellaneous)

The Buffer_t trait is a helper trait that gives types that have continuous elements helper funcions such as Sort,Find and Filter and others.

## Member functions
|  Signature |  Description
 --- | --- |
|Empty[imut this&] -> bool; | Returns true if the size is 0.
|First[imut this&] -> imut T&?; | Returns First element if it exist.
|First[imut this&] -> imut T&?; | Returns First element if it exist.
|Last[this&] -> T&?; | Returns Last element if it exist.
|Last[this&] -> T&?; | Returns Last element if it exist.
|for[this&] -> Iterator; | Allows looping over all elements.
|for[imut this&] -> IIterator; | Allows looping over all elements.
|Find<[functor [imut T&] -> bool]Func>[this&,Func func] ->  T&? | Loops over the items and returns the item if it matchs the funcs
|Find<[functor [imut T&] -> bool]Func>[imut this&,Func func] ->  imut T&? | Loops over the items and returns the item if it matchs the funcs
|FindIndex<[functor [imut T&] -> bool]Func>[imut this&,Func func] ->  imut T&? | Loops over the items and returns the index if it matchs the funcs
|FindIndex<[functor [imut T&] -> bool]Func>[this&,Func func] -> T&? | Loops over the items and returns the index if it matchs the funcs
|Reverse[this&] -> void; | Reverses all the elements.
|Shuffle<[Random_t]Rand>[this&,Rand& Rng] -> void; | Shuffles all the elements using the any Random Type
|AsSpan[this&] -> T[:]; | Gets the span of elements.
|AsSpan[imut this&] -> imut T[:]; | Gets the span of elements.
|[][this&,[Range_t](./Range_t.md)<uintptr> Range] -> T[:]; | Gets a Span to start and end of the range
|[][imut this&,[Range_t](./Range_t.md)<uintptr> Range] -> imut T[:]; | Gets a Span to start and end of the range
|[][this&,[RangeFull_t](./RangeFull_t.md) Range] -> T[:]; | Gets the span of elements.
|[][imut this&,[RangeFull_t](./RangeFull_t.md) Range] -> imut T[:]; | Gets the Span of elements.
|[][this&,[RangeTo_t](./RangeTo_t.md)<uintptr> Range] -> T[:]; | Gets a Span thats starts at the range.
|[][imut this&,[RangeTo_t](./RangeTo_t.md)<uintptr> Range] -> imut T[:]; | Gets a Span thats starts at the range
|[][this&,[RangeFrom_t](./RangeFrom_t.md)<uintptr> Range] -> T[:]; | Gets a Span thats ends at the range
|[][imut this&,[RangeFrom_t](./RangeFrom_t.md)<uintptr> Range] -> imut T[:]; | Gets a Span thats ends at the range
