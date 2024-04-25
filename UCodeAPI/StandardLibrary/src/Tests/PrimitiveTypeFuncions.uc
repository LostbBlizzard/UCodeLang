Tests:
 [Test]
 |Bool_Check[] -> bool:
  String truestr = true.ToString();
  String falsestr = false.ToString();

  bool t = bool::Parse(truestr).Unwrap();
  bool f = bool::Parse(falsestr).Unwrap();

  ret t == true && f == false;

 [Test]
 |Byte_Check[] -> bool:

  String str = byte(10).ToString(); 
  String str2 = byte(50).ToString();
  String str3 = byte(72).ToString();
 
  ret str == "10" && str2 == "50" && str3 == "72";

 [Test]
 |Byte_Check2[] -> bool:

  byte val = byte::Parse("10").Unwrap(); 
  byte val2 = byte::Parse("72").Unwrap(); 
  byte val3 = byte::Parse("50").Unwrap();  

  ret val == 10 && val2 == 72 && val3 == 50;
 
 [Test]
 |Byte_Check3[] -> bool://number too big 
  ret byte::Parse("5000").IsOpt() == false; 


