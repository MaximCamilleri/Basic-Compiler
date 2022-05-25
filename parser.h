#include "scanner.h"
#include <stack>

enum productions {
    // <Program> -> 
    // <block> ->
    // <statement> ->
    // <functionDecl> ->
    // <formalParams> ->
    // <formalParam> ->
    // <whileStatement> ->
    // <forStatement> ->
    // <ifStatement> ->
    // <rtrnStatement> ->
    // <printStatement> ->
    // <variableDecl> ->
    // <assignment> ->
    // <expression> ->
    // <simpleExpr> ->
    // <term> ->
    // <factor> ->
    // <unary> ->
    // <subExpression> ->
    // <functionCall> ->
    // <actualParams> ->
    // <additiveOp> ->
    // <multiplicativeOp> ->
    // <literal> ->
    // <booleanLiteral> ->
    // <type> ->
};

enum symbols {
    // Non Terminals
    type, booleanLiteral, literals, multiplicativeOp, additiveOp, actualParams, functionCall, subExpression,
    unary, factor, term, simpleExpr, subExpression, assignment, variableDecl, printState, rtrnState, ifState,
    forState, whileState, formalParam, formalParams, functionDecl, statement, black, program, $,

    // Terminals
    start, errorState, variable, singleLineComment, multiLineComment, intVal, floatVal, space, stringVal,  
    conditions, endOfExpression, returnFunc, openCircleBracket, closeCircleBracket, openCurlyBracket, closeCurlyBracket, 
    newline, colon, comma, charVal, equals,
    // reserved words
    _fn, _float, _int, _bool, _return, _let, _print, _char, _true, _false, _if, _for, _while, _else, _and, _not, _or,
    // Operators
    _minus, _plus, _multiply, _divide


};

class parser{
private:
    int pTable[50][50] = {
//      <start> <variable> <conditions> <errorState> <;> <operators> <->> <singleLineComment> <MultiLineComment> <(> <)> <{> <}> <int> <float> <space> <string> <\n> <:> <,> <_fn> <_float> <_int> <_bool> <_return> <_let> <_print> <_char> <_true> <_false> <_if> <_for> <_while> <_else> <_and> <_or> <_not> <+> <-> <*> </> <char> <> <> <> <> <> <> <> <> <> <> <> <>
        {}, // type
        {}, //
        {}, //
        {}, //
        {}, //
        {}
    };

    stack<symbols> myStack;

   
public:
    parser();
};

parser::parser(){
    this->myStack.push($);
    this->myStack.push(program);
}