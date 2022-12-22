
$Vec2:
 byte X = 0;
 byte Y = 0;
 |SomeFunc[this&]:
  X = 1;
  Y = 1;

|Main[]:
 byte A = 5 + 5;
 byte C = A - 5 - 5;
 ret C;