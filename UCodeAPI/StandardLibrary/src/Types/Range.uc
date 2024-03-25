$Range_t<T> export:
 private:
  T _start;
  T _end;
 public:
  export |new[this&]:
   _start = [];
   _end = [];
  export |new[this&,T start,T end]:
   _start = start;
   _end = end;

  export |Start[imut this&] => _start;
  export |End[imut this&] => _end;

$RangeInclusive_t<T> export:
 private: 
  T _start;
  T _end;
 public:
  export |new[this&,T Start,T End]:
   _start = Start;
   _end = End;

  export |Start[imut this&] => _start;
  export |End[imut this&] => _end;


$RangeFull_t<T> export;

$RangeTo_t<T> export;

$RangeFrom_t<T> export;


export |Range<T>[T start,T end] -> Range_t<T>:
 ret [start,end];

export |RangeInclusive<T>[T start,T end]  -> RangeInclusive_t<T>:
 ret [start,end];
