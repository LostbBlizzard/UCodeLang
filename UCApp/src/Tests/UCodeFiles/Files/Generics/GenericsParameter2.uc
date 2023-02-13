$Item<T>:
 |Value[] => sizeof(T);


|func[Item<T> Item]:
 ret Item<T>::Value();

|main[]:
 var A = Item<int>();
 ret func(A) - sizeof(int);