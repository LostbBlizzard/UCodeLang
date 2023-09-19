$Unique_ptr<T>:
 private:
  T& _ptr;
 public:
  |new[this&] -> void:
   _ptr =: unsafe bitcast<T&>(0);
  |new[this&,moved this& Value] -> void:
   _ptr =: Value._ptr;
   Value._ptr =: unsafe bitcast<T&>(0);

  |Get[this&] -> T&:ret _ptr;
  |Make[] -> this:
   this r = [];
   r._ptr =: unsafe new T();
   ret r;

|main[]:
 int^ V = unq int();
 int& rptr = V.Get();
 rptr++;

 int^ V2 = move V;
 int r = V2.Get();

 ret r;