

|main[]:
 uintptr Size = 5;

 var _data = unsafe new uintptr[Size];
 defer unsafe drop(_data);

 for [uintptr i = uintptr(0);i < Size;i++]:unsafe _data[i] = i;
 
 for [uintptr i = uintptr(0);i < Size;i++]:unsafe _data[i] = _data[i];

 uintptr count = 0;
 
 for [uintptr i = uintptr(0);i < Size;i++]: count += unsafe _data[i];
 
 ret count;