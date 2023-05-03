



eval Item = false;


|main[]:
 $if Item:
  int V = 5 * 10 / 5;
 $else:
  int V = 5 - 10 + 5;

 ret V;
