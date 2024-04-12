|Swap<T>[T& A,T& B] -> void:
 T C = move A;
 A = move B;
 B = move C;

|main[]:
 int a = 5;
 int b = 15;

 Swap(a,b);

 ret a == 15 && b == 5;
