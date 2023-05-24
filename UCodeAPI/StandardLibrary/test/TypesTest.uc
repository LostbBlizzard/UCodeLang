
use ULang;


[Test] 
|Vector_Test[] -> void:
 T[] List;
 List.Add(10);
 List.Add(15);
 List.Add(26);

 Debug::CheckBool(List[0] == 10);
 Debug::CheckBool(List[1] == 15);
 Debug::CheckBool(List[2] == 26);


[Test] 
|Vector2_Test[] -> void:
 T[] List;
 List.Add(10);
 List.Add(15);
 
 List.Clear();
 
 List.Add(26);


 Debug::CheckBool(List.Size() == 1);
 Debug::CheckBool(List[0] == 26);

 List.RemoveAtIndex(0);

 Debug::CheckBool(List.Size() == 0);


[Test(Test::Success::MustPanic)] 
|Vector3_Test[] -> void:
 int[] Buffer;
 Buffer.Resize(5);

 Buffer[5] = 10;//out of bounds.

[Test] 
|UniquePtr_Test[] -> void:
 var ValuePtr = unq int(5);

 Debug::CheckBool(ValuePtr.Value() == 5);
 

[Test] 
|UniquePtr2_Test[] -> void:
 var ValuePtr = unq int[5];
 
 
 ValuePtr[0] = 1;
 ValuePtr[1] = 3;
 ValuePtr[2] = 6;
 ValuePtr[3] = 8;
 ValuePtr[4] = 9;


 Debug::CheckBool(ValuePtr[0] == 1);
 Debug::CheckBool(ValuePtr[1] == 3);
 Debug::CheckBool(ValuePtr[2] == 6);
 Debug::CheckBool(ValuePtr[3] == 8);
 Debug::CheckBool(ValuePtr[4] == 9);


 
[Test(Test::Success::MustPanic)] 
|UniquePtr3_Test[] -> void:
 var ValuePtr = unq int[5];
 
 ValuePtr[5] = 9;//out of bounds.

[Test] 
|SharedPtr_Test[] -> void:
 var ValuePtr = shr int(5);

 Debug::CheckBool(ValuePtr.Value() == 5);
 

[Test] 
|SharedPtr2_Test[] -> void:
 var ValuePtr = shr int[5];
 
 
 ValuePtr[0] = 1;
 ValuePtr[1] = 3;
 ValuePtr[2] = 6;
 ValuePtr[3] = 8;
 ValuePtr[4] = 9;


 
 Debug::CheckBool(ValuePtr[0] == 1);
 Debug::CheckBool(ValuePtr[1] == 3);
 Debug::CheckBool(ValuePtr[2] == 6);
 Debug::CheckBool(ValuePtr[3] == 8);
 Debug::CheckBool(ValuePtr[4] == 9);



[Test(Test::Success::MustPanic)] 
|SharedPtr3_Test[] -> void:
 var ValuePtr = shr int[5];
 
 ValuePtr[5] = 9;//out of bounds.


[Test(Test::Success::MustPanic)] 
|Array_Test[] -> void:
 int[5] Buffer;

 Buffer[5] = 10;//out of bounds.

 

