%ULang:
 $Hash_t = !uintptr;

 trait AutoHash:
  //will make one.
  |Hash[this&] => 0;

 |Hash[umut T& Item] -> Hash_t:
  $if vaid(Item.Hash()):
   ret Item.Hash();
  $else if (vaid(ULang::AddedHash::T::Hash(Item))):
   ret ULang::AddedHash::T::Hash(Item);
  $else:
   invaid("Cant find a Hash for the type '" + type(T).FullName() + "'");
