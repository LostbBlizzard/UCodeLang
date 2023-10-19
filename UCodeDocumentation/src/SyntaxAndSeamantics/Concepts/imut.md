# umut and immutability (keyword)
The umut keyword in UCode is used to indicate that a variable or a type is read-only and cannot be modified. 

<code>

    |main[]:
     umut int Num = 10;
     Num = 5;//the variable is read-only and cannot be modified. 

</code>

The umut keyword can be used with Addresss to indicate that the value pointed to by the Address is read-only. 


<code>

    |main[]:
     int Num = 10
     umut int& NumPtr = Num;
     NumPtr = 5;//the variable is read-only and cannot be modified. 

</code>

The umut keyword cannot be cast away, as casting away the umut qualification allows a program to modify the object referred to by the Addresss, which may result in undefined behavior.

<code>

    |main[]:
     umut int Num = 10
     int& NumPtr = Num;//error trying casting away the umut qualification.

</code>

 
umut also applies to structures and member calls. When a structure is declared as umut, its members cannot be modified. 


<code>

    $Money: 
     int Count = 10;
     
     |UpdateMoney[umut this&,int NewCount]:
      Count = NewCount;//error 'this' is umut it cannot be modified. 

</code>

because umut is allways before a variable declaration you can make variable declaration like this.

<code>
    
    umut Hello = 10;

</code>

the comiler will interpret this as

<code>
    
    umut var Hello = 10;

</code>

and finally

<code>
    
    umut int Hello = 10;

</code>