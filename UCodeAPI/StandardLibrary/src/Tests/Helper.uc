Tests:
 
 [Test]
 |ToUpper_1[]:
  ret ToUpper('a') == 'A';

 [Test]
 |ToUpper_2[]:
  ret ToLower('A') == 'a';
 
 [Test]
 |ToUpper_3[]:
  ret IsUpper('A') == true && IsLower('A') == false;

 [Test]
 |ToUpper_4[]:
  ret IsLower('z') == true && IsUpper('Z') == true;
