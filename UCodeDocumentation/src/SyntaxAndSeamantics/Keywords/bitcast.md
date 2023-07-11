# bitcast\<T>(KeyWord)


Converts a ptr into an other pointer or an uintptr.

<code>

    |main[]:
     int& NullPtr = bitcast<int&>(0);
     uintptr PtrAsNumber = bitcast<uintptr>(NullPtr);

</code>