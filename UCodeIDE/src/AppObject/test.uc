
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
 
 |new<X...>[this&,X Elems] -> void:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;
  _capacity = 0;

  Resize(Elems.Count());
  
  uintptr Index = 0;
  $for [var& Item : Elems]:
   _data[Index] = Item;
   Index++;

 |Resize[this&,uintptr size]:
  if size > _capacity:
   var oldsize = _size;
   var old = _data;

   _capacity = size;
   _size = size;
   _data = unsafe new T[size];
   for [uintptr i = uintptr(0);i < oldsize;i++]:
     _data[i] = old[i];

   uintptr ptr =unsafe bitcast<uintptr>(old);
   if ptr != uintptr(0):
      unsafe drop(old);

 |[][this&,Range_t<uintptr> V] -> int[:]:
  ret [_data[V._start],V._end - V._start];

 |drop[this&]:
  uintptr ptr =unsafe bitcast<uintptr>(_data);
  if ptr != uintptr(0):
   unsafe drop(_data);

 $FindFunc = |[T& Ptr] -> bool;
 |Find[FindFunc Func] -> int&?:
  ret Null;

$Optional<T> enum:
 Value[T Value],
 Null,
 


|main[] -> int:
 int[] list = [0,1,2,3,4,5,6,7,8,9,10];
 
 int[:] span = list[5..10];

 int&? newlist = span.Find([x] => x == 7);

 match newlist:
  Null:ret [];
  Value(out Value):ret Value;

*/

$FuncPtr = |[int A] -> int;
|Func[FuncPtr ptr] -> int:ret 0;

|main[]:
 var V = Func([A] => 0);

