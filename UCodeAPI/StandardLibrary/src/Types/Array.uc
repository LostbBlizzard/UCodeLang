

$Array<T,(Size)> export;


/*
$Array<T,(Size)> export:
 private:
  T[/Size] _data;
 public:
  unsafe |iData[imut this&] -> imut T[&]:ret _data;
  unsafe |Data[this&] -> T[&]:ret _data;

  |Size[imut this&] => Size;

  |[][this&,uintptr Index] -> T&:ret _data[Index];
  |[][imut this&,uintptr Index] -> imut T&:ret _data[Index];

  |[][this&,Range_t<uintptr> Range] -> T[:]:ret AsSpan()[Range]; 
  |[][imut this&,Range_t<uintptr> Range] -> imut T[:]:ret AsSpan()[Range];

  |AsSpan[this&] -> T[:]:ret unsafe [_data,Size];
  |iAsSpan[imut this&] -> imut T[:]:ret unsafe [_data,Size];

*/