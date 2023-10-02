
$Item:
 int A = 1;
 |new[this&] -> void;

 |GetValue[this&] => A;


|main[] -> int:
 Item V = [];
 ret V.GetValue(); 
