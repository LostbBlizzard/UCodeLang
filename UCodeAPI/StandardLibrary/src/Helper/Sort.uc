

|Swap<T>[T& A,T& B] -> void:
 T C = move A;
 A = move B;
 B = move C;