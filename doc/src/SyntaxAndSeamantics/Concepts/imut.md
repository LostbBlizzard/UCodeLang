# imut and immutability (keyword)
The umut keyword in UCode is used to indicate that a variable or a type is read-only and cannot be modified. 

```
|main[]:
  imut int Num = 10;
  
  Num = 5;//the variable is read-only and cannot be modified. 
```

The umut keyword can be used with Addresss to indicate that the value pointed to by the Address is read-only. 


```
|main[]:
  int Num = 10
     
  imut int& NumPtr = Num;
  
  NumPtr = 5;//the variable is read-only and cannot be modified. 

```

The umut keyword cannot be cast away, as casting away the umut qualification allows a program to modify the object referred to by the Addresss, which may result in undefined behavior.

```
|main[]:
  imut int Num = 10
  
  int& NumPtr = Num;//error trying casting away the umut qualification.

```

 
umut also applies to structures and member calls. When a structure is declared as umut, its members cannot be modified. 


```
$Money: 
  int Count = 10;
     
  |UpdateMoney[imut this&,int NewCount]:
    
    Count = NewCount;//error 'this' is imut it cannot be modified. 

```

because umut is allways before a variable declaration you can make variable declaration like this.

```
imut Hello = 10;
```

the comiler will interpret this as

``` 
imut var Hello = 10;
```
and finally
``` 
imut int Hello = 10;
```