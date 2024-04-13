
$Endian enum export:
 Little,
 Big,
 Default = Endian::Little -> byte,
 
 


$ByteConverter:
 public:
 
  export |CPUEndian[] -> Endian:
   int v = 1;
   byte[&] ptr = unsafe bitcast<byte[&]>(v);
   if unsafe ptr[0] == 1:
    ret Little;
   else:
    ret Big;

  export |ToBytes[uint8 Val,Endian endian]:
   var V = ToBytes_ThisCPU<uint8>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  export |ToBytes[uint16 Val,Endian endian]:
   var V = ToBytes_ThisCPU<uint16>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  export |ToBytes[uint32 Val,Endian endian]:
   var V = ToBytes_ThisCPU<uint32>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  export |ToBytes[uint64 Val,Endian endian]:
   var V = ToBytes_ThisCPU<uint64>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  export |ToBytes[int8 Val,Endian endian]:
   var V = ToBytes_ThisCPU<int8>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  export |ToBytes[int16 Val,Endian endian]:
   var V = ToBytes_ThisCPU<int16>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  export |ToBytes[int32 Val,Endian endian]:
   var V = ToBytes_ThisCPU<int32>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  export |ToBytes[int64 Val,Endian endian]:
   var V = ToBytes_ThisCPU<int64>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;
 
  export |ToBytes[char Val,Endian endian]:
   var V = ToBytes_ThisCPU<char>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  export |ToBytes[utf8 Val,Endian endian]:
   var V = ToBytes_ThisCPU<utf8>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  export |ToBytes[utf16 Val,Endian endian]:
   var V = ToBytes_ThisCPU<utf16>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  export |ToBytes[utf32 Val,Endian endian]:
   var V = ToBytes_ThisCPU<utf32>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;


 private:
  export |ToBytes_ThisCPU<T>[T Val] -> byte[sizeof(T)]:
   ret unsafe bitcast<byte[sizeof(T)]&>(Val);

