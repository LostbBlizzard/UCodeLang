# MemoryManagement

>[!IMPORTANT]
>This Is section Is incomplete.

UCode like C++ requires the programmer to manually allocate and deallocate memory useing the [new](./Keywords/new.md) and [drop](./Keywords/drop.md) keywords.

like so:

```

int& MyMemory = unsafe new int();// allocate 4 bytes

unsafe drop(MyMemory);// deallocate 4 bytes

```

you can also allocate an array.

```

int[&] MyMemory = unsafe new int[5];// allocate 20 bytes

unsafe drop(MyMemory);// deallocate 20 bytes

```

because not deallocate memory can cause your Progarm to run out memory.
You can use Smart Pointers in the Standardlibrary.Unique_ptr and Shared_ptr are both Smart Pointers and can deallocate memory when its not used.

