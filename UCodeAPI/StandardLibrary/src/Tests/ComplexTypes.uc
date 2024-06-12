
Tests:

 [Test]
 |ComplexType_1[]:
  String[] val = [String("Hello")];
  val[0] += " World";

  ret val[0].Size() == 11;

