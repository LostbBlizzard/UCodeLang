@ulangsevertester::test["BasicFind 1"];
@ulangsevertester::file["func.uc"];

|func[] -> int: 
         int G = 0;
         int V = 5;
         int Item = V + G;
@ulangsevertester::goto[^] is valid if [out.line == 6];
         ret Item;

@ulangsevertester::file["main.uc"];

NameSpace:
             |main[] => func(); 
@ulangsevertester::goto[^] is valid if [out.line == 5,out.filepath == "func.uc"];