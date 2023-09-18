$Item<T>:
 |Value[] => sizeof(T);


|func<T>[Item<T> item]:
 ret Item<T>::Value();

|main[]:
 var A = Item<int>();
 ret func(A) - sizeof(int);