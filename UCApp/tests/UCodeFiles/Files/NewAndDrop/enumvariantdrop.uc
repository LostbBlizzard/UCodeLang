

int R = 2;

$Drop:
 |drop[this&]:
  R = 1;

$Item enum:
 Red[Drop],
 Blue[int],

|main[]:
 
 int B = 0;
 block:
  var Value =Item::Red([]);
  B = R;
 
 if B == 1:
  R = 0;
  block:
   var OtherValue =Item::Blue([]);
  
  ret R + B;

 ret 0;