
$Item:
 public:
  int X = 0;
  |PubFunc[this&] -> void;
 private:
  int Y = 0;
  |PriFunc[this&] -> void;

|main[]:
 Item V = [];
 V.X = 0;
 V.Y = 0;

 V.PubFunc();
 V.PriFunc();