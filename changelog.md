-- Improvements
 - [Fixed Static Array Indexing bug](https://github.com/LostbBlizzard/UCodeLang/commit/961aebc4c2327921f8c5937669641b156ca5c802)
 - [Fixed ReferenceType loesing imutable flag](https://github.com/LostbBlizzard/UCodeLang/commit/63290c537c670b8cf779309ac763cedf1f348c99)
 - Fixed lots of other Bugs
-- Additions

- [Added StripFunc to UAssembly]()
  - Allows for keeping funcions and there dependencies and removeing the rest
  
- [Added Strip to UAssembly]()
  - allows for removeing DebugInfo,Funcions To Addresss and Assembly types

- [Added short hand for make map type]() 
  - you can now do int[String] to make the type Map<String,int>.
  - if you want to use the array short hand using a varable for the size just use parentheses as the syntax is ambigous
  - and Map has higher president because its used more.
    - int[String] is Map<String,int>.
    - int[0] is Array<int,(0).
    - int[somename] is Map<somename,int>.
    - int[(somename)] is Array<int,(somename)>

- [Added ToString,Parse on Primitve Type in Stanrd](https://github.com/LostbBlizzard/UCodeLang/commit/06c1ad478145c18498a491eee5801c2af0a0dca3)

- [Allowed Primitive on ForType](https://github.com/LostbBlizzard/UCodeLang/commit/de00a58fc4a47079c478147697374f16722066fa)

- [Added Enable Attribute](https://github.com/LostbBlizzard/UCodeLang/commit/b8b8d0c6a49607e7a8c41b3c13d0f914aa2fc473)

- [Added StringInterpolation](https://github.com/LostbBlizzard/UCodeLang/commit/a0f409728a95c1692414ce7967872b0e6666cbc1)

- [Added ByteConverter FromBytesFuncion](https://github.com/LostbBlizzard/UCodeLang/commit/8d4e2b4350b734f75280055074335c52206b3627)

- [Added MemCopy and MemMove On LowLevel](https://github.com/LostbBlizzard/UCodeLang/commit/86b866b8f95cbb681895cbf2cb45c79dba39af8f)

-- Changes

-- Removed Features
