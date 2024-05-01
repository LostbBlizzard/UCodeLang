$StringSpan_t<T> export:
 private:
  T[&] _data;
  uintptr _size;
 public: 
  
  export |new[this&]:
   _data = unsafe bitcast<T[&]>(0);
   _size = 0;
 
  export unsafe |new[this&,T[&] data,uintptr size]:
   _data = data;
   _size = size;
   a++;
  |+[imut this&,imut this& other] -> this:
   b++;
   //ret this();


$StringSpan = StringSpan_t<char>;

int a = 0;
int b = 0;
$for int:
 |ToString[this&] -> StringSpan:a++;

|main[]:  
 var v1 = "{4} and {5}";// +3
 var v2 =  4.ToString() + " and " + 5.ToString();// +3
 
 //ret b;
 ret a == 6 && b == 4;

