Tests:
 
 [Test]
 |Optional_1[]:
  int? v = Opt(10);

  ret v.IsOpt();

 [Test]
 |Optional_2[]:
  int? v = Opt(10);

  ret v.Unwrap() == 10;

 [Test]
 |Optional_3[]:
  int? v = Opt(10);

  ret v.OptOr(5) == 10;

 [Test]
 |Optional_4[]:
  int? v = None;

  ret v.OptOr(5) == 5;
