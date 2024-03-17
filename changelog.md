-- Improvements
 - [fixed Static Array Indexing bug]()

-- Additions

- [Added StripFunc to UAssembly]()
  - Allows for keeping funcions and there dependencies and removeing the rest
  
- [Added Strip to UAssembly]()
  - allows for removeing DebugInfo,Funcions To Addresss and Assembly types

- [added short hand for make map type]() 
  - you can now do int[String] to make the type Map<String,int>.
  - if you want to use the array short hand using a varable for the size just use parentheses as the syntax is ambigous
  - and Map has higher president because its used more.
    - int[String] is Map<String,int>.
    - int[0] is Array<int,(0).
    - int[somename] is Map<somename,int>.
    - int[(somename)] is Array<int,(somename)>
 
-- Changes

-- Removed Features
