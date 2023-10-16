
/*
$ByteSerializer:
 public:
  |GetBytes<T>[imut T& Object] => GetBytes<T>(Object,Endian::Default);
  |GetBytes<T>[imut T& Object,Endian endian] -> byte[];
  
  |WriteTo<X,T>[X& writer,imut T& Object]:
   ret WriteTo<X,T>(writer,Object,Endian::Default);
   
  |WriteTo<X,T>[X& writer,imut T& Object,Endian endian] -> void;

 //private:


*/
