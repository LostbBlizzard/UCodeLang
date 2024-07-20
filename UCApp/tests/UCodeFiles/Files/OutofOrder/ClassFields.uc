$Type:
 public:
  |Func[this&] => SomeVal * OtherVal;
 private:
  int SomeVal = 10;
  int OtherVal = 5;

|main[]:
 Type val = [];

 ret val.Func() == 50;
