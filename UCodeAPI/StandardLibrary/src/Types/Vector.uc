
$Vector<T>:
 T[&] _data;
 uintptr _size;
 uintptr _capacity;
 //$Span = Span<T>;
 |new[this&]:
  _data = unsafe bitcast<T[&]>(0);
  _size = 0;
  _capacity = 0;

 |Push[imut T& Val] => 0;
 //|Push[moved T Val] => 0;
 
 |Push[imut T[:] Val] => 0;
 //|Push[moved T[:] Val] => 0;
