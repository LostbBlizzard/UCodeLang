$Test<[bool](test)> export:
 private:
  $if test:
   $Base = bool;
  $else:
   $Base = int;

  Base _base;
 public:
  |Sizeof[this&] => sizeof(Base);

$Test1 = Test<(true)>;
$Test2 = Test<(false)>;

|main[]:
 Test1 a = [];
 Test2 b = [];
 ret a.Sizeof() == 1 && b.Sizeof() == 4;
