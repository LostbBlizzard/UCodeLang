%ULang:
 
 $String_t<T>[Buffer<T>]:
  private:
   Vector<T> Data;
  public:
   |new[this&,umut this& Buffer] -> void;
   |new[this&,umut Vector<T>& Buffer] -> void;
   
   
   |AsView[umut this&] => Data.AsView();
   |AsView[this&] => Data.AsView();

   |Add[this&,umut T& Item] => Data.Add(Item);
   |Add[this&,moved T Item] => Data.Add(Item);
  
   |Add[this&,umut T[:] Items] => Data.Add(Item);
   |Add[this&,moved T[:] Items] => Data.Add(Item);


   |Capacity[umut this&] =>  Data.Capacity();

   |Resize[this&,size_t Offset] -> void;
   |Clear[this&] -> void;

   |+[umut this&,umut T[:] Other] -> this:
     Vector<T> R;
     R.Resize(Data.Size() + Other.Size());
     
     R.Add(AsView());
     R.Add(Other);
     ret R;

   |+[umut this&,umut this& Other] => this + Other.AsView();

   |+[umut this&,moved T[:] Other] -> this:
     Vector<T> R;
     R.Resize(Data.Size() + Other.Size());
     
     R.Add(AsView());
     R.Add(Other);
     ret R;

   |+[umut this&,moved this Other] => this + Other.AsView();

   |+=[this&,umut T[:] Other] -> void:
     Data.Add(AsView());

   |+=[this&,umut this& Other]: this += Other.AsView();

   |+=[this&,moved T[:] Other] -> void:
     Data.Add(AsView());

   |+=[this&,moved this Other]: this += Other.AsView();

 $StringView_t<T> = T[:];

 $String = String_t<char>; 
 $StringView = StringView_t<char>;

 $ASCIIString = String_t<ascllchar>; 
 $ASCIIStringView = String_t<ascllchar>; 

 $UTF8String = String_t<char8>; 
 $UTF8StringView = StringView_t<char8>;

 $UTF16String = String_t<char16>; 
 $UTF16StringView = StringView_t<char16>;

 $UTF32String = String_t<char32>; 
 $UTF32StringView = StringView_t<char32>;
