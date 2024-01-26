$Unique_ptr<T>:
 private:
  T& _ptr;
 public:
  |new[this&] -> void:
   unsafe _ptr =: unsafe bitcast<T&>(0);
  
  |new[this&,moved this& Value] -> void:
   unsafe _ptr =: Value._ptr;
   unsafe Value._ptr =: unsafe bitcast<T&>(0);
  
  |drop[this&]:
   uintptr ptr =unsafe bitcast<uintptr>(_ptr);
   if ptr != uintptr(0):
    unsafe drop(_ptr);

  |Get[this&] -> T&:ret _ptr;
  |Make[] -> this:
   this r = [];
   unsafe r._ptr =: unsafe new T();
   ret r;

|main[]:
 int^ V = unq int();
 int& rptr = V.Get();
 rptr++;

 int^ V2 = move V;
 int r = V2.Get();

 ret r;