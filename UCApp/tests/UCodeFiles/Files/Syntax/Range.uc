
|RangeTest[] => 1..10;
|RangeTest2[] => 1..=10;
$Range_t<T>:
 T start;
 T end;
 |new[this&,T Start,T End]:
  start = Start;
  end = End;
$RangeInclusive_t<T>:
 T start;
 T end;
 |new[this&,T Start,T End]:
  start = Start;
  end = End;

|Range<T>[T start,T end] => Range_t<T>(start,end);
|RangeInclusive<T>[T start,T end] =>RangeInclusive_t<T>(start,end);

|main[] -> int:
 ret RangeTest().start + RangeTest2().start + RangeTest().end + RangeTest2().end;