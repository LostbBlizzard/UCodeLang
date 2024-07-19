
$Random_t trait export:
 |NextState[this&] -> uint32; 
 |GetState[imut this&] -> uint32;

 |NextInt<T>[this&,T min, T max] -> T:
   T r = (GetState() -> T) % max + min;
   NextState();
   ret r;


 
$Random export = RandomAlgorithm::RandomXorshift32;
  
