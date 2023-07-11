# eval(KeyWord)

The eval KeyWord works the same as constexpr KeyWord in c++.It tells the compiler evaluate the value of the function or variable at compile time.with no cost at runtime as there no IR Made for it.unlike static variables.

<code>

    eval int SomeValue = 5;
    eval Item = SomeValue * 10;
    |main[this&]:
     int Value = 50;


</code>