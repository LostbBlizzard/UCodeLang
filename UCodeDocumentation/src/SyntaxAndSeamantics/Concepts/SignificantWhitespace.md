# Significant Whitespace(Concepts)

UCode uses whitespace to indicate the structure and indentation of the code. Whitespace is significant in UCode because it affects how the compiler parses and executes the code. For example:


<code>


    //This is a comment
    |add(int x,int y): //A newline ends this statement
    return x + y; //A whitespace indicates this block belongs to the function

    int result = add(2,3); //A newline ends this expression
    Fmt::Print(result); //Another newline ends this expression

    //This is a blank line

</code>