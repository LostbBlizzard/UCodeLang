
/*
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
 

$Span<T>:
 T[&] _data;
 uintptr _size;
 |new[this&]:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;

 |new[this&,T[&] data,uintptr size]:
  _data = data;
  _size = size;

$Vector<T>:
 T[&] _data;
 uintptr _size;
 uintptr _capacity;

 |new[this&]:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;
  _capacity = 0;
 
 //|new<T...>[this&,T Elems] -> void;

 |[][this&,Range_t<uintptr> V] -> int[:]:
  ret [_data[V._start],V._end - V._start];

 |[][this&,Range_t<int> V] -> int[:]:
  ret [_data[V._start -> uintptr],(V._end - V._start) -> uintptr];

|main[]:
 //int[] list = [0,1,2,3,4,5,6,7,8,9,10];
 int[] list = [];

 int[:] span = list[5..10];

 ret span._size;
 //int[] newlist = span.filter([x] => x % 2 != 0).collect();
 //ret newlist;

*/

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

 
