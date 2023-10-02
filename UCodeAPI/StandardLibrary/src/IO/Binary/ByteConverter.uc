
/*
$Endian enum:
 Little,
 Big,
 Default,
 
 


$BitConverter:
 public:
 
  |CPUEndian[] -> Endian:
   int v = 1;
   byte[&] ptr = bitcast<byte[&]>(v);
   if ptr == 1:
    ret Little;
   else:
    ret Big;

  |ToBytes<T>[T Val]:
   ret ToBytes<T>(Val,Endian::Default);

  |ToBytes<T>[T Val,Endian endian]:
   var V = ToBytes_ThisCPU<T>(Val);

   //if CPUEndian() != endian:
   
   ret V;

 private:
  |ToBytes_ThisCPU<T>[T Val] -> byte[sizeof(T)]:
   ret bitcast<byte[sizeof(T)]>(Val);


*/