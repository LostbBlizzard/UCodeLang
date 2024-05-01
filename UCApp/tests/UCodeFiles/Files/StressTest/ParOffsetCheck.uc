$Vec3i:
 uintptr X;
 uintptr Y;
 uintptr Z;
 |new[this&,uintptr x,uintptr y,uintptr z]:
  X = x;
  Y = y;
  Z = z;
 |==[imut this&,imut this& Other]:
  ret this.X == Other.X && this.Y == Other.Y && this.Z == Other.Z;

|func1[Vec3i val,Vec3i val2,Vec3i val3] => val;
|func2[Vec3i val,Vec3i val2,Vec3i val3] => val2;
|func3[Vec3i val,Vec3i val2,Vec3i val3] => val3;

Vec3i a = [1,2,3];
Vec3i b = [2,3,4];
Vec3i c = [3,4,5];
|check1[] => func1(a,b,c) == a;
|check2[] => func2(a,b,c) == b;
|check3[] => func3(a,b,c) == c;

|main[] => check1() && check2() && check3(); 