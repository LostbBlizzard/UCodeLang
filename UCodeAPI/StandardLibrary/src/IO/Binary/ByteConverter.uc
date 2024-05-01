
$Endian enum export:
 Little,
 Big,
 Default = Endian::Little -> byte,
 
 


$ByteConverter export:
 public:
 
  export |CPUEndian[] -> Endian:
   int v = 1;
   byte[&] ptr = unsafe bitcast<byte[&]>(bitcast<uintptr>(v));
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

  export |ToBytes[bool Val,Endian endian]:
   var V = ToBytes_ThisCPU<bool>(Val);

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

  export |ToBytes[float32 Val,Endian endian]:
   var V = ToBytes_ThisCPU<float32>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;

  export |ToBytes[float64 Val,Endian endian]:
   var V = ToBytes_ThisCPU<float64>(Val);

   if CPUEndian() != endian:
    V.Reverse();
   
   ret V;
 

  export |FromBytes[byte[:] bytes,uint8& Out,Endian endian] => FromBytes_t<uint8>(bytes,Out,endian);  
  export |FromBytes[byte[:] bytes,uint16& Out,Endian endian] => FromBytes_t<uint16>(bytes,Out,endian);  
  export |FromBytes[byte[:] bytes,uint32& Out,Endian endian] => FromBytes_t<uint32>(bytes,Out,endian);  
  export |FromBytes[byte[:] bytes,uint64& Out,Endian endian] => FromBytes_t<uint64>(bytes,Out,endian);  
    
  export |FromBytes[byte[:] bytes,int8& Out,Endian endian] => FromBytes_t<int8>(bytes,Out,endian);  
  export |FromBytes[byte[:] bytes,int16& Out,Endian endian] => FromBytes_t<int16>(bytes,Out,endian);  
  export |FromBytes[byte[:] bytes,int32& Out,Endian endian] => FromBytes_t<int32>(bytes,Out,endian);  
  export |FromBytes[byte[:] bytes,int64& Out,Endian endian] => FromBytes_t<int64>(bytes,Out,endian);  
   
  export |FromBytes[byte[:] bytes,bool& Out,Endian endian] => FromBytes_t<bool>(bytes,Out,endian);  
  export |FromBytes[byte[:] bytes,char& Out,Endian endian] => FromBytes_t<char>(bytes,Out,endian);  
  export |FromBytes[byte[:] bytes,utf8& Out,Endian endian] => FromBytes_t<utf8>(bytes,Out,endian);  
  export |FromBytes[byte[:] bytes,utf16& Out,Endian endian] => FromBytes_t<utf16>(bytes,Out,endian);  
  export |FromBytes[byte[:] bytes,utf32& Out,Endian endian] => FromBytes_t<utf32>(bytes,Out,endian);  
    
  export |FromBytes[byte[:] bytes,float32& Out,Endian endian] => FromBytes_t<float32>(bytes,Out,endian);  
  export |FromBytes[byte[:] bytes,float64& Out,Endian endian] => FromBytes_t<float64>(bytes,Out,endian);
 private:
  |FromBytes_t<T>[byte[:] bytes,T& Out,Endian endian]:
   $if compiler::IsDebug():
     if sizeof(T) > bytes.Size():panic("Byte Span is too small");

   byte[sizeof(T)]& V = unsafe bitcast<byte[sizeof(T)]&>(bitcast<uintptr>(Out));
   unsafe LowLevel::Memcopy(V.Data()[0],bytes.Data()[0],sizeof(T));
   
   if CPUEndian() != endian:
    V.Reverse();

  |ToBytes_ThisCPU<T>[T Val] -> byte[sizeof(T)]:
   ret unsafe bitcast<byte[sizeof(T)]&>(bitcast<uintptr>(Val));

