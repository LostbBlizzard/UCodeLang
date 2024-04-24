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
  byte v = 10;
  String str = v.ToString();
  
  Fmt::Print(str);
  ret true;

