

$StringSpan_t<T>[Buffer_t<T>] export:
 private:
  T[&] _data;
  uintptr _size;
 public: 
  $MyString = String_t<T>;
  $MySpan = Span<T>;
  
  export |new[this&]:
   _data = unsafe bitcast<T[&]>(0);
   _size = 0;
 
  export unsafe |new[this&,T[&] data,uintptr size]:
   _data = data;
   _size = size;

  export |Size[imut this&] => _size;
  export unsafe |Data[imut this&] -> imut T[&]:ret _data;
  export unsafe |Data[this&] -> T[&]:ret _data; 

  export |==[imut this&,imut this& Other] -> bool:
   if this.Size() != Other.Size():ret false;

   for [uintptr i = 0;i < this.Size();i++]:

    if this[i] != Other[i]:ret false;

   ret true;

  export |!=[imut this&,imut this& Other] => !(this == Other);

  export |+[imut this&,imut MyString& Other] => this + Other.Str();
  export |+[imut this&,imut this& Other]:
   MyString r = this;
   r += Other;
   ret r;
   
  
  export |ToStr[MySpan& span] -> this: ret unsafe [span.Data(),span.Size()];
  export |ToStr[imut MySpan& span] -> this:ret unsafe [span.Data(),span.Size()];

  export |[][this&,uintptr Index] -> T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");

    ret unsafe _data[Index];
  export |[][imut this&,uintptr Index] -> imut T&:
    $if compiler::IsDebug():
      if Index >= _size:panic("Index is out of bounds");

    ret unsafe _data[Index];

 
$String_t<T>[Buffer_t<T>] export:
 private: 
  Vector<T> _base;//there's some optimizations we could do but I just need something working
 public:
  $MyStringSpan = StringSpan_t<T>;
  $MySpan = Span<T>;

  export |new[this&] -> void;

  
  export |new[this&,IPar<MyStringSpan> span] -> void:
   Resize(span.Size());
   for [uintptr i = 0;i < span.Size();i++]:this[i] = span[i];

  export unsafe |Data[imut this&] -> imut T[&]:ret unsafe _base.Data();
  export unsafe |Data[this&] -> T[&]:ret unsafe _base.Data();
  

  export |Size[imut this&] => _base.Size();
  export |Capacity[imut this&] => _base.Capacity();

  export |Resize[this&,uintptr Size] -> void:_base.Resize(Size);
  export |Reserve[this&,uintptr Size] -> void:_base.Reserve(Size);
  export |Clear[this&] -> void:_base.Clear();

  export |Pop[this&] -> T:ret _base.Pop();
  export |Remove[this&,uintptr Index] -> T:ret _base.Remove(Index);

  export |Push[this&,imut T& Val] -> void:_base.Push(Val);
  export |Push[this&,moved T Val] -> void:_base.Push(Val);

  export |Insert[this&,uintptr Index,imut T& Item] -> void:_base.Insert(Index,Item);
  export |Insert[this&,uintptr Index,moved T Item] -> void:_base.Insert(Index,Item);
  
  //Not required Functions 
  export |[][this&,uintptr Index] -> T&:ret _base[Index];
  export |[][imut this&,uintptr Index] -> imut T&:ret _base[Index];

  export |Append[this&,imut T[:] Val] -> void:_base.Append(Val);
  export |Append[this&,moved Span<T> Val] -> void:_base.Append(Val);

  export |==[imut this&,imut this& Other] -> bool:
   ret this.Str() == Other.Str();

  export |!=[imut this&,imut this& Other]:ret !(this == Other);

  export |+[imut this&,imut this& Other] -> this:
   ret this + Other.Str(); 

  export |+=[this&,imut this& Other] -> void:
   this += Other.Str();


  export |==[imut this&, IPar<MyStringSpan> Other] -> bool:
   ret this.Str() == Other;

  export |!=[imut this&, IPar<MyStringSpan> Other]:ret !(this == Other);

  export |+[imut this&, IPar<MyStringSpan> Other] -> this:
   this copy = this;
   copy += Other;
   ret copy;

  export |+=[this&, IPar<MyStringSpan> Other] -> void:
   _base.Append(Other.AsSpan());

  export |Str[this&] -> MyStringSpan:ret unsafe [Data(),Size()];
  export |Str[imut this&] -> imut MyStringSpan:ret unsafe [Data(),Size()];
 
$String export = String_t<char>;
$StringSpan export = StringSpan_t<char>;

$String8 export = String_t<utf8>;
$String8Span export = StringSpan_t<utf8>;

$String16 export = String_t<utf16>;
$String16Span export = StringSpan_t<utf16>;

$String32 export = String_t<utf32>;
$String32Span export= StringSpan_t<utf32>;
