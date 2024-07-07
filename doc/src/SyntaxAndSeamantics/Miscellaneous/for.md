# for(KeyWord)

The for KeyWord can be used to make a loop or to loop for a list for items.


# traditional for loop

a traditional for loop looks like this and works the same way as in C.

```
for [var Item = 0;Item < 10;Item++]:
  Fmt::Print(Item);
```

this will Log 1,2,3,4,5,6,7,8,9 and 10.

# modern for loops

a modern for loop can be used to loop over a list for Items and can be overloaded.As showed below.

```
$LoopAbleObject:
  $LoopObject:   
   LoopAbleObject& _List;
   uintptr I = 0;
     
   //new loop
   |new[this&,LoopAbleObject& List]:
    _List &= List;

    //Check Value
    |?[this&] => I < _List.Size(); 


    //Get Value and MoveNext   
    |->[this&]:
      var& Item = _List._Buffer[I];
      I++;
      ret Item;

    //loop ended
    |drop[this&] -> void;
    //

  int[\3] _Buffer;
  |Size[this&] => 3;
     
  //overload
  |for[this&] => LoopObject(this); 

     

```


# modern for int loops 

modern for int loops can be showed below.


```

for [var I : 0..10]:
  DoSomeThing();
  
```

This will Call DoSomeThing 10 times.

here's its equivalent code.
```

for [var Item = 0;Item < 10;Item++]:
  DoSomeThing();

```

it also was its reverse variant.

```

for [var I : (0..10).Reverse()]:
  DoSomeThing();
  
```

here's its equivalent code.

```

for [var Item = 10;Item != (0-1);Item--]:
  DoSomeThing();

```