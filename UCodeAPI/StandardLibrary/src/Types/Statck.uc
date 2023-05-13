%ULang:
 $Stack<T>:
  private:
   Vector<T> Data;
  public:
   |new[this&] -> void;

   |Size[umut this&] => Data.Size();
   |Capacity[umut this&] => Data.Capacity();

   |Push[this&,umut T& Item] => Data.Add(Item);
   |Push[this&,moved T Item] => Data.Add(Item);

   |Pop[this&] => Data.Pop();
   |Peek[this&] => Data[Data.Size() - 1];