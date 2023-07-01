
is there a special syntax for these testing files.
here's new the special syntax.

the special syntax will only work after "@ulangtester::" and the line will be excluded from the file when test.

there may only be one action per line and must be the first thing on that line.

- out 
contains the last action result.has the fields.
- out.line.//uint
- out.file.//uint
- out.pos.//uint
- out.value.//any type

- @ulangtester::test[FuncName];//Ex @ulangtester::test["FindVar 1"];
 sets the test name to the string of FuncName

- @ulangtester::endtest[];
 ends the test.

- @ulangtester::file[fileName];//@ulangtester::file["main.uc"];
 will split the file into that path and continue parseing as if in file.



- @ulangsevertester::goto[^] 
 will call goto-definition to the character above the '^' and will update the 
 - out.line.
 - out.filepath.
 - out.pos.