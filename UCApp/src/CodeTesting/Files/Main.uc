
$Test:
 |new[this&] -> void;
 |+[this&,this& Other] -> this;

$dropable:
 Test A = [];
 int x =0;
 |new[this&] -> void;
 |drop[this&] -> void;


Test __B;
//int V = 5 * 7;

|main[] -> void:
 Test V = [] + [];
