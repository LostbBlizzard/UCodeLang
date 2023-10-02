
|main[]:
 var H = unsafe new int[1];
 H[0] = 1;
 int Tep = H[0];
 
 unsafe drop(H);
 
 ret Tep;