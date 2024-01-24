
|main[]:
 int[/1] V = [4];
 int[&] Ptr =unsafe V[0];
 
 unsafe Ptr[0]++;
 unsafe Ptr[0] += 5;
 
 ret unsafe Ptr[0];
 


|main2[]:
 int[&] V = unsafe new int[1];
 defer unsafe drop(V);

 int[&] Ptr =unsafe V[0];
 unsafe Ptr[0]++;
 unsafe Ptr[0] += 5;

 
 ret unsafe Ptr[0];