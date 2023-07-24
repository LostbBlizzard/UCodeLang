[ClassData]-- 
$[_global]:
.size:0

 |Testing:main[] -> sint32;//Testing:main



[Instructions:UCodeVm]-- 
---Testing:main: 

   //File:/Main.uc
   //Line:30
   //Source Line:|main[] => 0;
   //IR-Data:{
      int32 B = 0;
      ret B;
      ;
   //}

   0 :Store32 [A],0
   1 :StoreRegToReg32 [A],[F]
   2 :Ret 
