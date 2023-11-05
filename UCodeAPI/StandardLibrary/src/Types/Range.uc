$Range_t<T>:
 private:
  T _start;
  T _end;
 public:
  |new[this&]:
   _start = [];
   _end = [];
  |new[this&,T start,T end]:
   _start = start;
   _end = end;

  |Start[imut this&] => _start;
  |End[imut this&] => _end;

$RangeInclusive_t<T>:
 private: 
  T _start;
  T _end;
 public:
  |new[this&,T Start,T End]:
   _start = Start;
   _end = End;

  |Start[imut this&] => _start;
  |End[imut this&] => _end;


$RangeFull_t<T>;

$RangeTo_t<T>;

$RangeFrom_t<T>;


|Range<T>[T start,T end] -> Range_t<T>:
 ret [start,end];

|RangeInclusive<T>[T start,T end]  -> RangeInclusive_t<T>:
 ret [start,end];
