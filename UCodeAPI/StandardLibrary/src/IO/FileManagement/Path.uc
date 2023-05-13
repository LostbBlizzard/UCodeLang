%ULang:
 $PathChar = ULang::Native::PathChar;
 eval PreferredPathSeparator = ULang::Native::PreferredPathSeparator;
 
 $Path[Buffer<T>]:
  private:
   String_t<PathChar> _Data;
  public:
   |AsView[umut this&] => _Data.AsView();
   |AsView[this&] => _Data.AsView();

   |Clear[this&] => _Data.Clear();
   |Capacity[umut this&] => _Data.Capacity();

   |new[this&] -> void;
   |new[this&,umut PathChar[:] Value]:
    _Data.Add(Value);

   |+[umut this&,umut PathChar[:] Other] -> this:ret [_Data + Other._Data];
   |+[umut this&,umut this& Other] -> this:ret [_Data + Other._Data];
   
   |+=[this&,umut PathChar[:] Other]: _Data += Other._Data;
   |+=[this&,umut this& Other]: _Data += Other._Data;

   |/=[this&,umut this& Other] -> void:
     this += PreferredPathSeparator;
     this += Other;
     ret R;
   
   |/[umut this&,umut this& Other] -> this:ret [_Data + PreferredPathSeparator + Other._Data];

 $PathView = StringView_t<PathChar>;