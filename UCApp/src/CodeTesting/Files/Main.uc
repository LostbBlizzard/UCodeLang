
$dropable:
 |new[this&] -> void;
 |drop[this&] -> void;

static int s_a = 1;
thread int t_a = 2;
int it_a = 3;
dropable H = [];
static dropable V = [];

static int& Test;

|main[] -> void:
 Test = s_a;
 