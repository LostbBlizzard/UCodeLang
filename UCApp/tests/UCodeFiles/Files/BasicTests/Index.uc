
|main[]:
 var H = unsafe new int[1];
 unsafe H[0] = 1;
 int Tep =unsafe H[0];
 
 unsafe drop(H);
 
 ret Tep;