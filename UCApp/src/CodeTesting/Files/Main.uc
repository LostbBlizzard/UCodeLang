
|main[ int[&] V ] -> int&:
 int[&] C = V[1];
 int& B = V[1];
 B = 10;

 int& o = B;

 ret B;