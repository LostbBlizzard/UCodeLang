$Item:
 char& V;
 |new[this&]:
  V =: unsafe new char('A');

|main[]:
 Item item = [];

 item.V = '0';
 char f = item.V;

 unsafe drop(item.V);


 ret f;