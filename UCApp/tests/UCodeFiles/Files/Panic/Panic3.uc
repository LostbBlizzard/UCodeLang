
$StringSpan_t<T> export:
  T[&] _data;
  uintptr _size;
  
  export |new[this&]:
   _data = unsafe bitcast<T[&]>(0);
   _size = 0;
 
  export unsafe |new[this&,T[&] data,uintptr size]:
   _data = data;
   _size = size;

  export |Size[imut this&] => _size;
  export unsafe |Data[imut this&] -> imut T[&]:ret _data;
  export unsafe |Data[this&] -> T[&]:ret _data; 
  
$StringSpan = StringSpan_t<char>;

bool a = false;
bool b = false;
bool c = false;
|main[]:
 defer a = true;
 panic("TestPanic");
 defer b = true;
 c = true;

|UnitCheck[] => a == true && b == false && c == false;