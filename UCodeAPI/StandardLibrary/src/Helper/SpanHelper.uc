


|ReverseS<T>[ T[:] buf ]:
 uintptr start = 0;
 uintptr end = buf.Size() - 1;
 
 while start < end:
    Swap(buf[start],buf[end]);
    end--;
    start++;


