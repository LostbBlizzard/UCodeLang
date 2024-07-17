

|RangeFull[] => true;
|RangeTo[int val] => true;
|RangeToInclusive[int val] => true;
|RangeFrom[int val] => true;

|func1[] => ..;
|func2[] => ..1;
|func3[] => ..=10;
|func4[] => 9..;

|main[]:ret func1() && func2() && func3() && func4();

