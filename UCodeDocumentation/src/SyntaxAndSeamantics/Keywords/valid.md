# valid(keyword)

the valid keyword is an expression that returns a [constant expression](../Concepts/conditional%20compliation.md) [bool](../Types/bool.md).

the keyword will check if an expression or statements will throw an compile time error.

for example:
<code>

        |main[] -> void:
         bool DoesFuncExist = valid(:Func();); 
         $if DoesFuncExist:
          Console::Log("found func");
</code>


the varable "DoesFuncExist" is false because the compiler will detect that Func cant be found in the scope.


>[!NOTE]
>The valid keyword only checks the statement or expression.the statement or expression will never execute the code at runtime.