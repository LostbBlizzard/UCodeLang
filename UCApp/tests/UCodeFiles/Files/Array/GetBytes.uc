|IsLittleEndian[]:
 int val = 1;
 ret unsafe ToBytes(val)[0] == 1;
 
|IsBigEndian[] => !IsLittleEndian();

|ToBytes<T>[T v] -> byte[/sizeof(T)]:
  $V = byte[/sizeof(T)];
  ret unsafe bitcast<V&>(bitcast<uintptr>(v));

|main[]:
 int val = 4;

 byte checkval = match IsLittleEndian():
                true => 4;
                false => 0;
 ;
 ret unsafe ToBytes(val)[0] == checkval;
