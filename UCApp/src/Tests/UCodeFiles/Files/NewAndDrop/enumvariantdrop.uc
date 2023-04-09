

int R = 2;

$Drop:
 |drop[this&]:
  R = 1;

enum Item:
 Red[Drop],
 Blue[int],

|main[]:
 
 int B = 0;
 |
  var Value =Item::Red([]);
  B = R;
 |
 if B == 1:
  R = 0;
  |
   var OtherValue =Item::Blue([]);
  |
  ret R + B;

 ret 0;