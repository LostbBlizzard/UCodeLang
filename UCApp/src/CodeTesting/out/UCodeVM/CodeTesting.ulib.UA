[ClassData]-- 
$[_global]:
.size:0

 |main[] -> sint32;//main


$Test:
.size:4
 sint32 a;//Offset 0



---

$Vec2f_t<T>:
 T X;
 T Y;
 
 |new[this&]:
  X = 0;
  Y = 0;

 |new[this&,T x,T y]:
  X = x;
  Y = y;

 |==[imut this&,imut this& Other]:
  ret this.X == Other.X && this.Y == Other.Y;

 |!=[imut this&,imut this& Other]:
  ret !(this == Other);

 |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;

 |-=[this&,imut this& Other]:
  this.X -= Other.X;
  this.Y -= Other.Y;

 |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy -= Other;
  ret copy;


---
---

$Vec2i_t<T>:
 T X;
 T Y;
 |new[this&]:
  X = 0;
  Y = 0;
 |new[this&,T x,T y]:
  X = x;
  Y = y;

 |==[imut this&,imut this& Other]:
  ret this.X == Other.X && this.Y == Other.Y;

 |!=[imut this&,imut this& Other]:
  ret !(this == Other);
 
 |+=[this&,imut this& Other]:
  this.X += Other.X;
  this.Y += Other.Y;

 |-=[this&,imut this& Other]:
  this.X -= Other.X;
  this.Y -= Other.Y;

 |+[imut this&,imut this& Other] -> this:
  var copy = this;
  copy += Other;
  ret copy;

 |-[imut this&,imut this& Other] -> this:
  var copy = this;
  copy -= Other;
  ret copy;


---
$Vec2 = Vec2f_t<float32>;

$Vec2i = Vec2f_t<sint32>;

$Vec2f_t<float32>:
.size:8
 float32 X;//Offset 0
 float32 Y;//Offset 4

 |(&_new&)[this&] -> void;//Vec2f_t<float32>:(&_new&)^Vec2f_t<float32>&
 |(&_new&)[this&, float32, float32] -> void;//Vec2f_t<float32>:(&_new&)^Vec2f_t<float32>&,f32,f32
 |(&equal&)[umut this&, imut Vec2f_t<float32>&] -> bool;//Vec2f_t<float32>:(&equal&)^Vec2f_t<float32>&imut,Vec2f_t<float32>&imut
 |(&notequal&)[umut this&, imut Vec2f_t<float32>&] -> bool;//Vec2f_t<float32>:(&notequal&)^Vec2f_t<float32>&imut,Vec2f_t<float32>&imut
 |(&Compound_Plus&)[this&, imut Vec2f_t<float32>&] -> void;//Vec2f_t<float32>:(&Compound_Plus&)^Vec2f_t<float32>&,Vec2f_t<float32>&imut
 |(&Compound_Sub&)[this&, imut Vec2f_t<float32>&] -> void;//Vec2f_t<float32>:(&Compound_Sub&)^Vec2f_t<float32>&,Vec2f_t<float32>&imut
 |(&Plus&)[umut this&, imut Vec2f_t<float32>&] -> Vec2f_t<float32>;//Vec2f_t<float32>:(&Plus&)^Vec2f_t<float32>&imut,Vec2f_t<float32>&imut
 |(&Minus&)[umut this&, imut Vec2f_t<float32>&] -> Vec2f_t<float32>;//Vec2f_t<float32>:(&Minus&)^Vec2f_t<float32>&imut,Vec2f_t<float32>&imut


$Vec2f_t<sint32>:
.size:8
 sint32 X;//Offset 0
 sint32 Y;//Offset 4

 |(&_new&)[this&] -> void;//Vec2f_t<sint32>:(&_new&)^Vec2f_t<sint32>&
 |(&_new&)[this&, sint32, sint32] -> void;//Vec2f_t<sint32>:(&_new&)^Vec2f_t<sint32>&,i32,i32
 |(&equal&)[umut this&, imut Vec2f_t<sint32>&] -> bool;//Vec2f_t<sint32>:(&equal&)^Vec2f_t<sint32>&imut,Vec2f_t<sint32>&imut
 |(&notequal&)[umut this&, imut Vec2f_t<sint32>&] -> bool;//Vec2f_t<sint32>:(&notequal&)^Vec2f_t<sint32>&imut,Vec2f_t<sint32>&imut
 |(&Compound_Plus&)[this&, imut Vec2f_t<sint32>&] -> void;//Vec2f_t<sint32>:(&Compound_Plus&)^Vec2f_t<sint32>&,Vec2f_t<sint32>&imut
 |(&Compound_Sub&)[this&, imut Vec2f_t<sint32>&] -> void;//Vec2f_t<sint32>:(&Compound_Sub&)^Vec2f_t<sint32>&,Vec2f_t<sint32>&imut
 |(&Plus&)[umut this&, imut Vec2f_t<sint32>&] -> Vec2f_t<sint32>;//Vec2f_t<sint32>:(&Plus&)^Vec2f_t<sint32>&imut,Vec2f_t<sint32>&imut
 |(&Minus&)[umut this&, imut Vec2f_t<sint32>&] -> Vec2f_t<sint32>;//Vec2f_t<sint32>:(&Minus&)^Vec2f_t<sint32>&imut,Vec2f_t<sint32>&imut



[Instructions:UCodeVm]-- 
---Test:(&_new&)^Test&: 
   0 :Debug_FuncStart 
   1 :Debug_FuncEnd 
   2 :Ret 
---main: 
   3 :Debug_FuncStart 
   4 :Store32 [A], 0
   6 :StoreRegToReg32 [A],[F]
   7 :Debug_LineEnter 
   8 :Debug_FuncEnd 
   9 :Ret 
---Vec2f_t<float32>:(&_new&)^Vec2f_t<float32>&: 
   10 :Debug_FuncStart 
   11 :Storef32 [A], 0.000000
   13 :Debug_LineEnter 
   14 :StoreRegToPtr32 [A],[D]
   15 :Debug_LineEnter 
   16 :Load32 [D],[A],4
   17 :Debug_FuncEnd 
   18 :Ret 
---Vec2f_t<float32>:(&_new&)^Vec2f_t<float32>&,f32,f32: 
   19 :Debug_FuncStart 
   20 :Debug_LineEnter 
   21 :StoreRegToPtr32 [E],[D]

   //File:/Main.uc
   //Line:9
   //Source Line:[Cant Find Line]

   22 :Debug_LineEnter 
   23 :Load32 [D],[F],4
   24 :Debug_FuncEnd 
   25 :Ret 
---Vec2f_t<float32>:(&equal&)^Vec2f_t<float32>&imut,Vec2f_t<float32>&imut: 
   26 :Store64 [A], 1
   30 :IncrementStackPointer [A]

   //Line:8
   //Source Line:[Cant Find Line]

   31 :Debug_FuncStart 
   32 :StoreFromPtrToReg32 [D],[A]
   33 :Debug_LineEnter 
   34 :StoreFromPtrToReg32 [E],[B]
   35 :equalto8 [B],[A],[F]

   //File:/OtherFile.uc
   //Line:11
   //Source Line:[Cant Find Line]

   36 :Read32 [E],[C],4
   37 :Read32 [D],[A],4

   //Line:12
   //Source Line:[Cant Find Line]

   38 :StoreRegOnStackSub8 [F],1
   39 :equalto8 [A],[C],[F]

   //StackFrameSize:1

   40 :GetFromStackSub8 [A],1
   41 :Ins 38:5,0,5
   42 :Store64 [A], 1
   46 :DecrementStackPointer [A]
   47 :Debug_FuncEnd 
   48 :Ret 
---Vec2f_t<float32>:(&notequal&)^Vec2f_t<float32>&imut,Vec2f_t<float32>&imut: 
   49 :Store64 [A], 24
   53 :IncrementStackPointer [A]
   54 :Debug_FuncStart 
   55 :StoreFromPtrToReg64 [D],[A]
   56 :StoreRegOnStackSub64 [A],24
   57 :StoreFromPtrToReg64 [D],[B]
   58 :StoreRegOnStackSub64 [B],16
   59 :StoreFromPtrToReg64 [D],[C]
   60 :StoreRegOnStackSub64 [C],8

   //StackFrameSize:24

   61 :GetFromStackSub64 [F],8
   62 :StoreRegOnStackSub64 [F],16
   63 :Debug_LineEnter 

   //File:/OtherFile.uc
   //Line:18
   //Source Line:[Cant Find Line]

   64 :GetFromStackSub64 [A],24
   65 :StoreRegOnStackSub64 [A],24
   66 :GetFromStackSub64 [A],24
   67 :Push64 [A]
   68 :StoreRegToReg64 [E],[D]
   69 :Call {Vec2f_t<float32>:(&equal&)^Vec2f_t<float32>&imut,Vec2f_t<float32>&imut}
   73 :LogicalNot8 [F],[A]
   74 :StoreRegToReg8 [A],[F]
   75 :Store64 [B], 24
   79 :DecrementStackPointer [B]
   80 :Debug_FuncEnd 
   81 :Ret 
---Vec2f_t<float32>:(&Compound_Plus&)^Vec2f_t<float32>&,Vec2f_t<float32>&imut: 
   82 :Debug_FuncStart 
   83 :Debug_LineEnter 
   84 :StoreFromPtrToReg32 [D],[A]
   85 :StoreFromPtrToReg32 [E],[B]
   86 :Addf32 [A],[B],[F]
   87 :StoreRegToPtr32 [F],[D]
   88 :Debug_LineEnter 
   89 :Read32 [D],[C],4
   90 :Read32 [E],[A],4
   91 :Addf32 [C],[A],[F]
   92 :Load32 [D],[F],4
   93 :Debug_FuncEnd 
   94 :Ret 
---Vec2f_t<float32>:(&Compound_Sub&)^Vec2f_t<float32>&,Vec2f_t<float32>&imut: 

   //File:/OtherFile.uc
   //Line:21
   //Source Line:[Cant Find Line]

   95 :Debug_FuncStart 
   96 :Debug_LineEnter 
   97 :StoreFromPtrToReg32 [D],[A]
   98 :StoreFromPtrToReg32 [E],[B]
   99 :Subf32 [A],[B],[F]

   //Line:22
   //Source Line:[Cant Find Line]

   100 :StoreRegToPtr32 [F],[D]
   101 :Debug_LineEnter 
   102 :Read32 [D],[C],4
   103 :Read32 [E],[A],4
   104 :Subf32 [C],[A],[F]
   105 :Load32 [D],[F],4
   106 :Debug_FuncEnd 
   107 :Ret 
---Vec2f_t<float32>:(&Plus&)^Vec2f_t<float32>&imut,Vec2f_t<float32>&imut: 

   //File:/OtherFile.uc
   //Line:25
   //Source Line:[Cant Find Line]

   108 :Store64 [A], 32
   112 :IncrementStackPointer [A]

   //Line:26
   //Source Line:[Cant Find Line]

   113 :Debug_FuncStart 
   114 :Debug_LineEnter 
   115 :StoreFromPtrToReg64 [D],[A]
   116 :StoreRegOnStackSub64 [A],32
   117 :StoreFromPtrToReg64 [D],[B]

   //StackFrameSize:32

   118 :StoreRegOnStackSub64 [B],24
   119 :StoreFromPtrToReg64 [D],[C]
   120 :StoreRegOnStackSub64 [C],16

   //File:/OtherFile.uc
   //Line:29
   //Source Line:[Cant Find Line]

   121 :GetFromStackSub64 [F],16
   122 :StoreRegOnStackSub64 [F],24
   123 :GetFromStackSub64 [A],32
   124 :StoreRegOnStackSub64 [A],8
   125 :GetPointerOfStackSub [A],8
   126 :Debug_LineEnter 
   127 :StoreRegToReg64 [A],[D]
   128 :Call {Vec2f_t<float32>:(&Compound_Plus&)^Vec2f_t<float32>&,Vec2f_t<float32>&imut}
   132 :GetFromStackSub64 [A],8

   //Line:30
   //Source Line:[Cant Find Line]

   133 :StoreRegOnStackSub64 [A],8
   134 :GetFromStackSub64 [F],8
   135 :Debug_LineEnter 
   136 :Store64 [B], 32
   140 :DecrementStackPointer [B]

   //Line:31
   //Source Line:[Cant Find Line]

   141 :Debug_FuncEnd 
   142 :Ret 
---Vec2f_t<float32>:(&Minus&)^Vec2f_t<float32>&imut,Vec2f_t<float32>&imut: 
   143 :Store64 [A], 32
   147 :IncrementStackPointer [A]
   148 :Debug_FuncStart 
   149 :Debug_LineEnter 
   150 :StoreFromPtrToReg64 [D],[A]

   //StackFrameSize:32

   151 :StoreRegOnStackSub64 [A],32
   152 :StoreFromPtrToReg64 [D],[B]
   153 :StoreRegOnStackSub64 [B],24

   //File:/OtherFile.uc
   //Line:34
   //Source Line:[Cant Find Line]

   154 :StoreFromPtrToReg64 [D],[C]
   155 :StoreRegOnStackSub64 [C],16
   156 :GetFromStackSub64 [F],16
   157 :StoreRegOnStackSub64 [F],24
   158 :GetFromStackSub64 [A],32
   159 :StoreRegOnStackSub64 [A],8
   160 :GetPointerOfStackSub [A],8
   161 :Debug_LineEnter 
   162 :StoreRegToReg64 [A],[D]
   163 :Call {Vec2f_t<float32>:(&Compound_Sub&)^Vec2f_t<float32>&,Vec2f_t<float32>&imut}
   167 :GetFromStackSub64 [A],8
   168 :StoreRegOnStackSub64 [A],8
   169 :GetFromStackSub64 [F],8
   170 :Debug_LineEnter 
   171 :Store64 [B], 32
   175 :DecrementStackPointer [B]
   176 :Debug_FuncEnd 
   177 :Ret 
---Vec2f_t<sint32>:(&_new&)^Vec2f_t<sint32>&: 
   178 :Debug_FuncStart 
   179 :Store32 [A], 0
   181 :Debug_LineEnter 
   182 :StoreRegToPtr32 [A],[D]
   183 :Debug_LineEnter 
   184 :Load32 [D],[A],4
   185 :Debug_FuncEnd 
   186 :Ret 
---Vec2f_t<sint32>:(&_new&)^Vec2f_t<sint32>&,i32,i32: 
   187 :Debug_FuncStart 
   188 :Debug_LineEnter 

   //File:/OtherFile.uc
   //Line:7
   //Source Line:[Cant Find Line]

   189 :StoreRegToPtr32 [E],[D]
   190 :Debug_LineEnter 

   //Line:8
   //Source Line:[Cant Find Line]

   191 :Load32 [D],[F],4
   192 :Debug_FuncEnd 
   193 :Ret 
---Vec2f_t<sint32>:(&equal&)^Vec2f_t<sint32>&imut,Vec2f_t<sint32>&imut: 
   194 :Store64 [A], 1

   //Line:12
   //Source Line:[Cant Find Line]

   198 :IncrementStackPointer [A]
   199 :Debug_FuncStart 

   //StackFrameSize:1

   200 :StoreFromPtrToReg32 [D],[A]
   201 :Debug_LineEnter 
   202 :StoreFromPtrToReg32 [E],[B]

   //File:/OtherFile.uc
   //Line:15
   //Source Line:[Cant Find Line]

   203 :equalto8 [B],[A],[F]
   204 :Read32 [E],[C],4
   205 :Read32 [D],[A],4
   206 :StoreRegOnStackSub8 [F],1
   207 :equalto8 [A],[C],[F]
   208 :GetFromStackSub8 [A],1
   209 :Ins 38:5,0,5
   210 :Store64 [A], 1
   214 :DecrementStackPointer [A]
   215 :Debug_FuncEnd 
   216 :Ret 
---Vec2f_t<sint32>:(&notequal&)^Vec2f_t<sint32>&imut,Vec2f_t<sint32>&imut: 
   217 :Store64 [A], 24

   //StackFrameSize:24

   221 :IncrementStackPointer [A]
   222 :Debug_FuncStart 
   223 :StoreFromPtrToReg64 [D],[A]

   //File:/OtherFile.uc
   //Line:18
   //Source Line:[Cant Find Line]

   224 :StoreRegOnStackSub64 [A],24
   225 :StoreFromPtrToReg64 [D],[B]
   226 :StoreRegOnStackSub64 [B],16
   227 :StoreFromPtrToReg64 [D],[C]
   228 :StoreRegOnStackSub64 [C],8
   229 :GetFromStackSub64 [F],8
   230 :StoreRegOnStackSub64 [F],16
   231 :Debug_LineEnter 
   232 :GetFromStackSub64 [A],24
   233 :StoreRegOnStackSub64 [A],24
   234 :GetFromStackSub64 [A],24
   235 :Push64 [A]
   236 :StoreRegToReg64 [E],[D]
   237 :Call {Vec2f_t<sint32>:(&equal&)^Vec2f_t<sint32>&imut,Vec2f_t<sint32>&imut}
   241 :LogicalNot8 [F],[A]
   242 :StoreRegToReg8 [A],[F]
   243 :Store64 [B], 24
   247 :DecrementStackPointer [B]
   248 :Debug_FuncEnd 
   249 :Ret 
---Vec2f_t<sint32>:(&Compound_Plus&)^Vec2f_t<sint32>&,Vec2f_t<sint32>&imut: 
   250 :Debug_FuncStart 
   251 :Debug_LineEnter 
   252 :StoreFromPtrToReg32 [D],[A]
   253 :StoreFromPtrToReg32 [E],[B]
   254 :Add32 [A],[B],[F]

   //File:/OtherFile.uc
   //Line:21
   //Source Line:[Cant Find Line]

   255 :StoreRegToPtr32 [F],[D]
   256 :Debug_LineEnter 
   257 :Read32 [D],[C],4
   258 :Read32 [E],[A],4
   259 :Add32 [C],[A],[F]

   //Line:22
   //Source Line:[Cant Find Line]

   260 :Load32 [D],[F],4
   261 :Debug_FuncEnd 
   262 :Ret 
---Vec2f_t<sint32>:(&Compound_Sub&)^Vec2f_t<sint32>&,Vec2f_t<sint32>&imut: 
   263 :Debug_FuncStart 
   264 :Debug_LineEnter 
   265 :StoreFromPtrToReg32 [D],[A]
   266 :StoreFromPtrToReg32 [E],[B]
   267 :Sub32 [A],[B],[F]

   //File:/OtherFile.uc
   //Line:25
   //Source Line:[Cant Find Line]

   268 :StoreRegToPtr32 [F],[D]
   269 :Debug_LineEnter 
   270 :Read32 [D],[C],4
   271 :Read32 [E],[A],4
   272 :Sub32 [C],[A],[F]

   //Line:26
   //Source Line:[Cant Find Line]

   273 :Load32 [D],[F],4
   274 :Debug_FuncEnd 
   275 :Ret 
---Vec2f_t<sint32>:(&Plus&)^Vec2f_t<sint32>&imut,Vec2f_t<sint32>&imut: 
   276 :Store64 [A], 32
   280 :IncrementStackPointer [A]

   //File:/OtherFile.uc
   //Line:29
   //Source Line:[Cant Find Line]

   281 :Debug_FuncStart 
   282 :Debug_LineEnter 
   283 :StoreFromPtrToReg64 [D],[A]
   284 :StoreRegOnStackSub64 [A],32
   285 :StoreFromPtrToReg64 [D],[B]
   286 :StoreRegOnStackSub64 [B],24
   287 :StoreFromPtrToReg64 [D],[C]
   288 :StoreRegOnStackSub64 [C],16
   289 :GetFromStackSub64 [F],16
   290 :StoreRegOnStackSub64 [F],24
   291 :GetFromStackSub64 [A],32
   292 :StoreRegOnStackSub64 [A],8

   //Line:30
   //Source Line:[Cant Find Line]

   293 :GetPointerOfStackSub [A],8
   294 :Debug_LineEnter 
   295 :StoreRegToReg64 [A],[D]
   296 :Call {Vec2f_t<sint32>:(&Compound_Plus&)^Vec2f_t<sint32>&,Vec2f_t<sint32>&imut}
   300 :GetFromStackSub64 [A],8

   //Line:31
   //Source Line:[Cant Find Line]

   301 :StoreRegOnStackSub64 [A],8
   302 :GetFromStackSub64 [F],8
   303 :Debug_LineEnter 
   304 :Store64 [B], 32
   308 :DecrementStackPointer [B]
   309 :Debug_FuncEnd 
   310 :Ret 
---Vec2f_t<sint32>:(&Minus&)^Vec2f_t<sint32>&imut,Vec2f_t<sint32>&imut: 

   //StackFrameSize:32

   311 :Store64 [A], 32
   315 :IncrementStackPointer [A]
   316 :Debug_FuncStart 
   317 :Debug_LineEnter 
   318 :StoreFromPtrToReg64 [D],[A]
   319 :StoreRegOnStackSub64 [A],32
   320 :StoreFromPtrToReg64 [D],[B]
   321 :StoreRegOnStackSub64 [B],24
   322 :StoreFromPtrToReg64 [D],[C]
   323 :StoreRegOnStackSub64 [C],16
   324 :GetFromStackSub64 [F],16
   325 :StoreRegOnStackSub64 [F],24

   //Line:35
   //Source Line:[Cant Find Line]

   326 :GetFromStackSub64 [A],32
   327 :StoreRegOnStackSub64 [A],8
   328 :GetPointerOfStackSub [A],8
   329 :Debug_LineEnter 
   330 :StoreRegToReg64 [A],[D]
   331 :Call {Vec2f_t<sint32>:(&Compound_Sub&)^Vec2f_t<sint32>&,Vec2f_t<sint32>&imut}
   335 :GetFromStackSub64 [A],8
   336 :StoreRegOnStackSub64 [A],8
   337 :GetFromStackSub64 [F],8
   338 :Debug_LineEnter 
   339 :Store64 [B], 32
   343 :DecrementStackPointer [B]
   344 :Debug_FuncEnd 
   345 :Ret 
