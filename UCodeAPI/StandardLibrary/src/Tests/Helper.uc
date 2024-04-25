Tests:
 
 [Test]
 |ToUpper_1[]:
  ret ToUpper(char('a')) == 'A';

 [Test]
 |ToUpper_2[]:
  ret ToLower(char('A')) == 'a';
 
 [Test]
 |ToUpper_3[]:
  ret IsUpper(char('A')) == true && IsLower(char('A')) == false;

 [Test]
 |ToUpper_4[]:
  ret IsLower(char('z')) == true && IsUpper(char('Z')) == true;

 [Test]
 |ToUpper_5[]:
  String val = "hello";
  val.ToUpper();

  ret val == "HELLO";
 
 [Test]
 |ToUpper_6[]:
  String val = "HELLO";
  val.ToLower();

  ret val == "hello";

 [Test]
 |IsDigit_1[]:
  ret IsDigit('0') == true && IsDigit('H') == false && IsDigit('6') == true;


