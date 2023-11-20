# Lambda

a lambda expression is an anonymous function that can contain either an expression or a sequence of statements.

a lambda looks like this
```
|[x,y,z] => [expression]
```

or like this 

```
|[x,y,z] : [statement/statements]
```

an example of shown below

```
var MyLambdaObject = |[x,y] => x * y;//expression
    
var MyLambdaObject2 = |[x,y]:
    var Tep = x * y;
    ret Tep;
    ;//Starements

```

lambdas also have a short-form syntax. shown below
```
var MyLambdaObject = [x,y] => x * y;//expression
 ```

# Lambda Capturing
Lambda will automatically  Capture varables by value from the outer scope.
```

int a = 10;
int b = 10;
var LambdaObject = |[] => a * b;
//both a and b are captured by value.

a = 0;

var Value = LambdaObject();
//the Value will be 100 and not 0 because its captured by value on were it was declared.

```

Sometimes it is needed to capture by reference to do this you can explicitly say it captured by reference.

```

int a = 10;
int b = 10;


//b is captured by value
//a is captured reference.
var LambdaObject = |[](&a) => a * b;
    

a = 0;

var Value = LambdaObject();
//the Value will be 0 because its captured by reference.

```

also note you can put expressions in the capture list.

```

int a = 10;
int b = 10;

var LambdaObject = |[](&a) => a * b;

var LambdaObject2 = |[](int& a = a) => a * b;
                                
//both mean the same thing.
    
``` 

```

int a = 10;//outer scope 'a'
int b = 10;

var LambdaObject2 = |[](int& a = a) => a * b;
                                 ^ this 'a' is for capture

var LambdaObject2 = |[](int& a = a) => a * b;
                                     ^ is referring to the outer scope.
                                     
``` 

Note it is not possible to explicitly assigned the captures varable in the short-form syntax.

>[!IMPORTANT]
>
>Explicitly assigned Lambda Capturing has no implementation on the compiler yet.
>
>Lambda inside Lambdas is not tested yet.