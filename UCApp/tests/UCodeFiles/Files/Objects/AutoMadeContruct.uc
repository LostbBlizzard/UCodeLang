
int calleddrop = 0;
int calledcopy = 0;
int calledmove = 0;
int callednew = 0;

$Hello:
 |new[this&] -> void:
  callednew++; 
 |new[this&,imut this& other] -> void: 
  calledcopy++;
 |new[this&,moved this other] -> void: 
  calledmove++; 
 |drop[this&] -> void:
  calleddrop++;


$BaseType:
 Hello base;

$BaseType2:
 BaseType value;


|main[]:

 block:
  Hello v = [];//new +1
  Hello v2 = v;//copy +1
  Hello v3 =move v;//move +1
  //drop +3


 ret calleddrop == 3 && calledcopy == 1 && calledmove == 1 && callednew == 1;