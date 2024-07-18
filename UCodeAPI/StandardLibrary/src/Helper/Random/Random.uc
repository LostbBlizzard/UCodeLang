
$Random_t trait export:
 |NextState[this&] -> uint32; 
 |GetState[imut this&] -> uint32;

 |NextInt<T>[this&,T min, T max]:
   T r = GetState() % max + min;
   NextState();


 
$Random export = RandomAlgorithm::RandomXorshift32;
  
