

//Based on https://en.wikipedia.org/wiki/Xorshift
RandomAlgorithm:
  $RandomXorshift32[Random_t] export:

    |new[this&]: state = 1;
    |new[this&,uint32 state]: this.state = state;
    
    |SetSeed[this&,uint32 val]: state = val;
    |GetSeed[imut this&] => state;

    |NextState[this&] -> uint32:
     var x = state;
     x = x ^ (x << 13);
     x = x ^ (x >> 17);
     x = x ^ (x << 5);

     state = x;
     ret x;
    private:
     uint32 state;

  //$RandomXorshift64[Random_t] export;

  //$RandomXorshift128[Random_t] export;
