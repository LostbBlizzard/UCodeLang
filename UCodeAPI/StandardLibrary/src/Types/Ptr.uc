

export |iptr<T>[imut T& V] => Ptr<T>::Make(V);

export |ptr<T>[T& V] => Ptr<T>::Make(V);

$Ptr<T> export:
 private: 
  T& _base;
 public:
  export |new[this&,T& base]:
   unsafe _base =: base;
  
  export |Get[this&] -> T&:ret _base;
  export |Get[imut this&] -> imut T&:ret _base;
  
  export |Write[this&,imut T& newvalue] -> void:Get() = newvalue;
  export |Write[this&,moved T newvalue] -> void:Get() = newvalue;

  export |ReadPtr[imut this&] -> imut T&:ret Get();

  export |~>[this&] -> T&:ret Get();

  export |~>[imut this&] -> imut T&:ret Get();

  export |ReassignTo[this&,T& newbase]:unsafe _base =: newbase;



  export |Make[imut T& V] -> imut this:ret [unsafe bitcast<T&>(bitcast<uintptr>(V))];
  export |Make[T& V] -> this:ret [V];



//$Ptr<T = void> = Ptr<Empty>;
$VoidPtr export = Ptr<Empty>;
