

|iptr<T>[imut T& V] => Ptr<T>::Make(V);

|ptr<T>[T& V] => Ptr<T>::Make(V);

$Ptr<T>:
 private: 
  T& _base;
 public:
  |new[this&,T& base]:
   unsafe _base =: base;
  
  |Get[this&] -> T&:ret _base;
  |Get[imut this&] -> imut T&:ret _base;
  
  |Write[this&,imut T& newvalue] -> void:Get() = newvalue;
  |Write[this&,moved T newvalue] -> void:Get() = newvalue;

  |ReadPtr[imut this&] -> imut T&:ret Get();

  |~>[this&] -> T&:ret Get();

  |~>[imut this&] -> imut T&:ret Get();

  |ReassignTo[this&,T& newbase]:unsafe _base =: newbase;



  |Make[imut T& V] -> imut this:ret [unsafe bitcast<T&>(bitcast<uintptr>(V))];
  |Make[T& V] -> this:ret [V];



//$Ptr<T = void> = Ptr<Empty>;
$VoidPtr = Ptr<Empty>;
