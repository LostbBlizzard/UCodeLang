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


$RangeFull_t export;

$RangeTo_t<T> export:
 private: 
  T _to;
 public:
  |new[this&,T to]:
   _to = to;
  |To[this&] => _to;

$RangeFrom_t<T> export:
 private: 
  T _from;
 public:
  |new[this&,T from]:
   _from = from;

  |From[this&] => _from;

$RangeToInclusive_t<T> export:
 private: 
  T _to;
 public:
  |new[this&,T to]:
   _to = to;
  |To[this&] => _to;

// Map from 1..10
export |Range<T>[T start,T end] -> Range_t<T>:
 ret [start,end];

// Map from 1..=10
export |RangeInclusive<T>[T start,T end]  -> RangeInclusive_t<T>:
 ret [start,end];

// Map from ..
export |RangeFull[] -> RangeFull_t:
 ret [];

// Map from ..10
export |RangeTo<T>[T to] -> RangeTo_t<T>:
 ret [to];

// Map from 10..
export |RangeFrom<T>[T from] -> RangeFrom_t<T>:
 ret [from];

// Map from ..=10
export |RangeToInclusive<T>[T to]  -> RangeToInclusive_t<T>:
 ret [to];

