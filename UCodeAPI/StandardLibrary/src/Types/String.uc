

$StringSpan_t<T>:
 private:
  T[&] _data;
  uintptr _size;
 public:
  |new[this&]:
   _data = unsafe bitcast<T[&]>(0);
   _size = 0;
 
  unsafe |new[this&,T[&] data,uintptr size]:
   _data = data;
   _size = size;

  //$String = String_t<T>;

  |Size[imut this&] => _size;
  unsafe |Data[imut this&] -> imut T[&]:ret _data;
  unsafe |Data[this&] -> T[&]:ret _data; 

  |==[imut this&,imut this& Other] => true;
  |!=[imut this&,imut this& Other] => false;

$String_t<T>:
 private: 
  Vector<T> _base;//there's some optimizations we could do but I just need something working
 public:
  $MyStringSpan = StringSpan_t<T>;
  $MySpan = Span<T>;


  unsafe |Data[imut this&] -> imut T[&]:ret unsafe _base.Data();
  unsafe |Data[this&] -> T[&]:ret unsafe _base.Data();
  

  |Size[imut this&] => _base.Size();
  |Capacity[imut this&] => _base.Capacity();

  |Resize[this&,uintptr Size] -> void:_base.Resize(Size);
  |Reserve[this&,uintptr Size] -> void:_base.Reserve(Size);
  |Clear[this&] -> void:_base.Clear();

  |Pop[this&] -> T:ret _base.Pop();
  |Remove[this&,uintptr Index] -> T:ret _base.Remove(Index);

  |Push[this&,imut T& Val] -> void:_base.Push(Val);
  //|Push[this&,moved T Val] -> void:_base.Push(Val);

  |Insert[this&,uintptr Index,imut T& Item] -> void:_base.Insert(Index,Item);
  //|Insert[this&,uintptr Index,moved T Item] -> void:_base.Insert(Index,Item);
  
  //Not required Funcions 
  |[][this&,uintptr Index] -> T&:ret _base[Index];
  |[][imut this&,uintptr Index] -> imut T&:ret _base[Index];

  |Append[this&,imut T[:] Val] -> void:_base.Append(Val);
  //|Append[this&,moved T[:] Val] -> void:_base.Append(Val);

  |==[imut this&,imut this& Other] -> bool;

  |!=[imut this&,imut this& Other]:ret !(this == Other);

  |+[imut this&,imut this& Other] -> this;

  |+=[this&,imut this& Other] -> this;


  |==[imut this&, IPar<MyStringSpan> Other] -> bool;

  |!=[imut this&, IPar<MyStringSpan> Other]:ret !(this == Other);

  //|+[imut this&, IPar<MyStringSpan> Other] -> this;

  |+=[this&, IPar<MyStringSpan> Other] -> void;

$String = String_t<char>;
$StringSpan = StringSpan_t<char>;

$String8 = String_t<utf8>;
$StringSpan8 = StringSpan_t<utf8>;

$String16 = String_t<utf16>;
$StringSpan16 = StringSpan_t<utf16>;

$String32 = String_t<utf32>;
$StringSpan32 = StringSpan_t<utf32>;
