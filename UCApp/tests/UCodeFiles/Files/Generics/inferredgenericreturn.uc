
$Range_t<T>:
  T _start;
  T _end;
  |new[this&]:
   _start = [];
   _end = [];
  |new[this&,T start,T end]:
   _start = start;
   _end = end;

|Range<T>[T start,T end] -> Range_t<T>:
 ret [start,end];

|main[]:
 Range_t<uintptr> V = 0..5;
 Range_t<uint8> V2 = 0..5;

 ret V._end + (V2._end) -> uintptr;