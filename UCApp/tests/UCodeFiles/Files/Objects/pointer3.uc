$Item:
 char& V;
 |new[this&]:
  unsafe V =: unsafe new char('A');

|main[]:
 Item item = [];

 item.V = '0';
 char f = item.V;

 unsafe drop(item.V);


 ret f;