thread int V1 = 0;
thread int V2 = 0;
thread int V3 = 0;

thread int V4 = 0;
thread int V5 = 0;
thread int V6 = 0;

|main[] => coolp(5);

|coolp[int a, int b, int c]:

int m1 = a;
int m2 = b;
int m3 = c;

a++;
b++;
c++;

V1 = m1;
V2 = m2;
V3 = m3;


V4 = a;
V5 = b;
V6 = c;

ret a;
