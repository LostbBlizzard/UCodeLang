
|main[]:
 int[/1] V = [4];
 int[&] Ptr = V[0];
 
 Ptr[0]++;
 Ptr[0] += 5;
 
 ret Ptr[0];
 


|main2[]:
 int[&] V = unsafe new int[1];
 defer unsafe drop(V);

 int[&] Ptr = V[0];
 Ptr[0]++;
 Ptr[0] += 5;

 
 ret Ptr[0];