# import(Keyword)

The import keyword allows for faster compilation speed by forceing you to declaring the functions,type and classes up front for the current file.

Note : import must be at the top of the file before classes,namespace,functions or will throw an error and can't be declared inside of them.

for example
<code>

    import {ULang::Vector};

    |main[]:
     int[] List;
</code>

We are importing the type Vector.

But in doing so we unable to see any other types,function this file will only be compiled with that class and nothing else.

we can make the code above to compile by useing an alias because T[] just maps to any Vector in the scope and not specifically [ULang::Vector](../../Standardlibrary/Types/Generic/Vector.md).


-note import alias are only in the current file scope.
it won't be seen outside of the file

-also alias work on  such as funcions,overloaded funcions,generics and Macros.

-will also throw an error on trying to alias a  namespace.

-the more specific the better the optimizations such as importing only functions instead of the helper class.

<code>

    import {Vector = ULang::Vector};

    |main[]:
     int[] list;
</code>



you can also import Type and funcions as a list.

<code>

    import {Vector = ULang::Vector,ULang::Array,Span = ULang::Span};

    |main[]:
     int[] list;
     int[:] intSpan;
</code>

lastly if you have a really long name spaces you can do something like this. (indentation is not need) 
<code>

    import VeryVeryLong::NameSpace::ThatYouHave::PartOne::{   
    TypeOne = LongBoringType1,
    TypeTwo = LongBoringType2<int>,
    TypeThree = LongBoringType3::SubType};

</code>

that is the same as 

<code>

    import {   
    TypeOne = VeryVeryLong::NameSpace::ThatYouHave::PartOne::LongBoringType1,
    TypeTwo = VeryVeryLong::NameSpace::ThatYouHave::PartOne::LongBoringType2<int>,
    TypeThree = VeryVeryLong::NameSpace::ThatYouHave::PartOne::LongBoringType3::SubType
    };

</code>


lasty Imports will throw an error when unused and will throw an error when a file has a cycle dependency.even if only one file of the two or more files has the import.

also will will throw an error if the import is the current file.

but this is opt in.if you have no imports in the file you will cant none of side effects.but have also have none of the benefits.