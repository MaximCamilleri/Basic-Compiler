# Basic-Compiler

This project implements a basic complier for the Tinylang language. Tinylang is a basic programming language which supports variables, functions, for and while loops, if statements, and print commands. 

The project is split into 5 main parts. These are lexical analysis, parsing and building of an AST tree, XML representation of the AST tree, Semantic analysis, and execution. The language provides comprehensive error messages for an error encountered in each section. 

### Sample Tinylang Program:
```
fn Sq(x:float) -> float {
    return x*x;
}

fn XGraterY(x:float, y:float) -> bool {
    let ans:bool = true;
    if (y > x) { ans = false; }
    return ans;
}

//Same functionality as function above but using less code
fn XGreaterY_2(x:float, y:float) -> bool {
    return x > y;
}

fn AverageOfThree(x:float, y:float, z:float) -> float {
    let total:float = x + y + z;
    return total / 3.0;
}

/*
* Same functionality as the function above but using less code.
* Note the use of the brackets in the expression following 
* the return statement.
*/
fn AverageOfThree_2(x:float, y:float, z:float) -> float {
    return (x + y + z) /3.0;
}


// Execution (program entry point) starts at the first statement
// that is not a function declaration.

let x:float = 2.4;
let y:float = Sq(2.5);
let z:float = Sq(x);
print y;                              //6.25
print x * z;                          //13.824
print XGraterY(x, 2.3);               //true
print XGreaterY_2(Sq(1.5), y);        //false
print AverageOfThree(x, y, 1.2);      //3.28
```
