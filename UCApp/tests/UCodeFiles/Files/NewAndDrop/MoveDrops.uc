


int count = 0;

$Vector<T>:
 |new[this&] -> void;

 |new[this&,imut this& other] -> void; 
 |new[this&,moved this& other] -> void;

 |drop[this&] -> void:
  count++;


|swap<T>[T& a,T& b]:
 T c = move a;
 a = move b;//a gets droped
 b = move c;//b gets droped
 //c gets drop


|main[]:
 block:
    int[] V = [];
    int[] B2 = [];

    swap(V,B2);

    //V2 gets droped
    //B2 gets droped
 ret count;
