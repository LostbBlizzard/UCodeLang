
Tests:
 [Test]
 |ByteSerializer_1[]:
  int a = 1;
  var bytes = BitConverter::ToBytes(a,Endian::Default);
  
  uintptr c = 0;
 
  for [item : bytes]:
   c += item -> uintptr;


  
  ret bytes.Size() == 4 && c == 1;
