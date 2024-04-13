
Tests:
 [Test]
 |ByteSerializer_1[]:
  int a = 1;
  var bytes = ByteConverter::ToBytes(a,Endian::Default);
   
  uintptr c = 0;
  
  for [item : bytes]:
   c += item -> uintptr;
  

  
  ret bytes.Size() == 4 && c == 1;

 [Test]
 |ByteSerializer_2[]:
  byte[4] v = [1,0,0,0];
  
  int a = 0;
  var span = v.AsSpan();
  var bytes = ByteConverter::FromBytes(span,a,Endian::Little);
  
  ret a == 1;

