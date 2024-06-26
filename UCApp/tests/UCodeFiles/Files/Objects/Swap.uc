int a = 0;
int b = 0;
int c = 0;
int d = 0;


$Item export:
 |new[this&] -> void: a++;
 |new[this&,imut this& val] -> void: b++;
 |new[this&,moved this val] -> void: c++;
 |drop[this&] -> void: d++;


export |Swap<T>[T& A,T& B] -> void:
 T C = move A;
 A = move B;
 B = move C;


|main[]:
 block: 
   Item A = [];
   Item B = [];
   Swap(A,B);

 ret a == 2 && b == 0 && c == 3 && d == 5;

