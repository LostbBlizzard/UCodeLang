$Range_t<T>:
  T _start;
  T _end;
  |new[this&]:
   _start = [];
   _end = [];
  |new[this&,T start,T end]:
   _start = start;
   _end = end;

$RangeInclusive_t<T>:
 T _start;
 T _end;
 |new[this&,T Start,T End]:
  _start = Start;
  _end = End;

|Range<T>[T start,T end] -> Range_t<T>:
 ret [start,end];

|RangeInclusive<T>[T start,T end]  -> RangeInclusive_t<T>:
 ret [start,end];