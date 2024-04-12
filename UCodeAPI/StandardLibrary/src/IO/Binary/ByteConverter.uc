
$Endian enum:
 Little,
 Big,
 Default = Endian::Little -> byte,
 
 


$BitConverter:
 public:
 
  |CPUEndian[] -> Endian:
   int v = 1;
   byte[&] ptr = unsafe bitcast<byte[&]>(v);
   if unsafe ptr[0] == 1:
    ret Little;
   else:
    ret Big;

  |ToBytes[uint8 Val,Endian endian]:
   var V = ToBytes_ThisCPU<uint8>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  |ToBytes[uint16 Val,Endian endian]:
   var V = ToBytes_ThisCPU<uint16>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  |ToBytes[uint32 Val,Endian endian]:
   var V = ToBytes_ThisCPU<uint32>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  |ToBytes[uint64 Val,Endian endian]:
   var V = ToBytes_ThisCPU<uint64>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  |ToBytes[int8 Val,Endian endian]:
   var V = ToBytes_ThisCPU<int8>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  |ToBytes[int16 Val,Endian endian]:
   var V = ToBytes_ThisCPU<int16>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  |ToBytes[int32 Val,Endian endian]:
   var V = ToBytes_ThisCPU<int32>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  |ToBytes[int64 Val,Endian endian]:
   var V = ToBytes_ThisCPU<int64>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

 private:
  |ToBytes_ThisCPU<T>[T Val] -> byte[sizeof(T)]:
   ret unsafe bitcast<byte[sizeof(T)]&>(Val);

