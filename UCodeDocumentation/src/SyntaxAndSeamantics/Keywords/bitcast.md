# bitcast\<T>(keyword)


Converts a ptr into an other pointer or an uintptr.

<code>

    |main[]:
     int& NullPtr = bitcast<int&>(0);
     uintptr PtrAsNumber = unsafe bitcast<uintptr>(NullPtr);

</code>

>[!IMPORTANT]
>This Is section Is incomplete
>
>This can cause undefined Behavior.