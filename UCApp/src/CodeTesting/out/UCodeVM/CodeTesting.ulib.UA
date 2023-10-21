[ClassData]-- 
$[_global]:
.size:0

 |func[imut sint32&] -> void;//func^i32&imut
 |main[] -> void;//main



[Instructions:UCodeVm]-- 
---func^i32&imut: 
   0 :Debug_FuncStart 
   1 :Debug_FuncEnd 
   2 :Ret 
---main: 

   //StackFrameSize:4

   3 :Store64 [A], 4
   7 :IncrementStackPointer [A]

   //File:/Main.uc
   //Line:5
   //Source Line:[Cant Find Line]

   8 :Debug_FuncStart 
   9 :Store32 [A], 10
   11 :StoreRegToReg32 [A],[B]
   12 :StoreRegOnStackSub32 [B],4
   13 :GetPointerOfStackSub [B],4
   14 :Debug_LineEnter 
   15 :StoreRegToReg64 [B],[D]
   16 :Call {func^i32&imut}
   20 :Store64 [A], 4
   24 :DecrementStackPointer [A]
   25 :Debug_FuncEnd 
   26 :Ret 
