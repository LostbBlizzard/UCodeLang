
int a = 0;

|func[]:
 defer a++;

|main[]:
 func();
 ret a;