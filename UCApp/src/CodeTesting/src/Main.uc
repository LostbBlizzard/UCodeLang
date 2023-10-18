


//|main[] => Hello<int>();

$Test:
 int a;
 |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy.a = 10;
  ret copy;
 |==[imut this&,imut this& Other] => true;
 |!=[imut this&,imut this& Other] => true;
  
 /*
 |Test[imut this&]:
  var copy = this;
  ret copy == this;
 
 |Test2[imut this&]:
  var copy = this;
  ret copy != this;
 */
