$Funcion<R,Pars...>:
 private:
  $FPointer = |[Pars] -> R;
  $DynObject trait:
   dynamic |Invoke[this&,Pars pars] -> R;
  
  $BaseDyn = dynamic<DynObject>;
  $Base enum:
    None,
    Pointer[FPointer],
    Dynamic[BaseDyn], 
   
  Base _base = None;
 public:
  |new<[functor [Pars] -> R]F>[this&,F func]:

    $if type(F).GetFuncionPointerInfo(out typeinfo FuncTypeValue):
      _base = Pointer(func);
    $else:
     $MyDynObject[DynObject]:
      F _func;
      |new[this&,F f]: _func = f;
      |Invoke[this&,Pars pars] -> R:
        ret _func(pars);
      |drop[this&] -> void;
     
     var& object = unsafe new MyDynObject(func);
     _base = Dynamic(object);

   
  |()[this&,Pars pars] -> R:
   match _base:
     Pointer(out pointer):
        ret pointer(pars);
     Dynamic(out pointer):
        ret pointer.Invoke(pars);
   
