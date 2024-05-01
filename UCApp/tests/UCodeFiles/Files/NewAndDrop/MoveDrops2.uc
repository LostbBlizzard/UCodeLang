
int count = 0;

$Vector<T>:
 |new[this&] -> void;

 |new[this&,imut this& other] -> void; 
 |new[this&,moved this& other] -> void;

 |drop[this&] -> void:
  count++;


|main[]:
 block:
    int[] A = [];
    int[] B = [];

    int[] C = move A;

    A = move B;
    B = move C;


 ret count;
