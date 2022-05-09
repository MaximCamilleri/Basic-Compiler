#include "lib_std_facilities.h"
#include <stack>

enum tokens {
    start,
    errorState,
    variable, 
    operators,
    singleLineComment,
    multiLineComment,
    intVal,
    floatVal,
    space,
    stringVal,
    conditions,
    endOfExpression,
    returnFunc,
    openCircleBracket,
    closeCircleBracket,
    openCurlyBracket,
    closeCurlyBracket,
    newline,
    colon,
    comma,
    // actual words
    _fn, _float, _int, _bool, _return, _let, _print, _char, _true, _false, _if, _for, _while, _else
};

class scanner{
private:
   vector<vector<int>> classificationTable = {
    // <letter>  
       {65, 67, 68, 69, 70, 71, 72, 
       73, 74, 75, 76, 78, 79, 80,
       81, 82, 83, 84, 85, 86, 87,
       88, 89, 90, 97, 98, 99, 100,
       101, 102, 103, 104, 105, 106, 
       107, 108, 109, 110, 111, 112, 
       113, 114, 115, 116, 117, 118, 
       119, 120, 121, 122, 95},         
    // <digit>
       {48, 49, 50, 51, 
       52, 53, 54, 56, 57},         
    // !     =     +     -     *     /     <     >     ;     {      }      (     )     .     <space> "     <newLine> :     ,
       {33}, {61}, {43}, {45}, {42}, {47}, {60}, {62}, {59}, {123}, {125}, {40}, {41}, {46}, {32},   {34}, {10},     {58}, {44}
   };

   int transitionTable[34][22] = {
    // <letter> <digit> !   =   +   -   *   /   <   >   ;   {   }   (   )   .   <space> "  \n   :   ,  other
       {1,      24,     8,  4,  11, 12, 13, 14, 6,  2,  10, 22, 23, 20, 21, 30, 27,     28, 31, 32, 33, 30}, // S0  start
       {1,      1,      30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S1  variable
       {30,     30,     30, 3,  30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S2  >
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S3  >=
       {30,     30,     30, 5,  30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S4  =
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S5  ==
       {30,     30,     30, 7,  30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S6  <
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S7  <=
       {30,     30,     30, 9,  30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S8  error
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S9  !=
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S10 ;
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S11 +
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 15, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S12 - 
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S13 *
       {30,     30,     30, 30, 30, 30, 17, 16, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S14 /
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S15 ->
       {16,     16,     16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,     16, 30, 16, 16, 16}, // S16 //
       {17,     17,     17, 17, 17, 17, 18, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,     17, 17, 17, 17, 17}, // S17 error
       {17,     17,     17, 17, 17, 17, 18, 19, 17, 17, 17, 17, 17, 17, 17, 17, 17,     17, 17, 17, 17, 17}, // S18 error
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S19 /* ... */
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S20 (
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S21 )
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S22 {
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S23 }
       {30,     24,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 25, 30,     30, 30, 30, 30, 30}, // S24 int
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S25 error
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S26 float
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 27,     30, 30, 30, 30, 30}, // S27 space
       {28,     28,     28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,     29, 30, 30, 30, 30}, // S28 error
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S29 string
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30},  // S30 Final Error State
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S31 NewLine
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}, // S32 colon
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30}  // S33 comma
   };
   tokens tokenTypeTable[34] = {
    // S0      S1        S2          S3          S4          S5          S6          
       start, variable, conditions, conditions, conditions, conditions, conditions, 
    // S7          S8          S9          S10              S11        S12        S13        S14
       conditions, errorState, conditions, endOfExpression, operators, operators, operators, operators, 
    // S15         S16                S17         S18         S19               S20                S21
       returnFunc, singleLineComment, errorState, errorState, multiLineComment, openCircleBracket, closeCircleBracket,
    // S22               S23                S24     S25          S26       S27    S28        S29        S30
       openCurlyBracket, closeCurlyBracket, intVal, errorState, floatVal, space, errorState, stringVal, errorState,
    // S31      S32    S33
       newline, colon, comma
   };

   String reservedWords[14] =  {"fn", "float", "int", "bool", "return", "let", "print", "char", "true", "false", "if", "for", "while", "else"};

   std::map<tokens,std::string> m;
   int state;
   string lexeme;
   stack<int> stck;

public:

    scanner();
    void initScanner();
    void cleanStack();
    void initTokenMap();

    //getters and setters

    int getState();
    string getLexeme();
    int popStack();
    string getTokenName(tokens t);

    void setState(int state);
    void setLexeme(string lexeme);
    void appendLexeme(char l);
    void pushStack(int state);

    //table functions
    int getFromTransitionTable(int state, int classifier);
    tokens getTokenType(int state);
    int getClassifier(char a);

    //debugging
    void printStack();
};

scanner::scanner(){
    initScanner();
    initTokenMap();
}

void scanner::initScanner(){
    cleanStack();
    this->stck.push(-1);
    this->state = 0;
    this->lexeme = "";
}

void scanner::cleanStack(){
    while (!stck.empty()){
        stck.pop();
    }
}

int scanner::getState(){
    return this->state;
}
string scanner::getLexeme(){
    return this->lexeme;
}
int scanner::popStack(){
    int temp = this->stck.top();
    stck.pop();
    return temp;
}
void scanner::setState(int state){
    this->state = state;
}
void scanner::setLexeme(string lexeme){
    this->lexeme = lexeme;
}
void scanner::appendLexeme(char l){
    setLexeme(getLexeme() + l);
}
void scanner::pushStack(int state){
    this->stck.push(state);
}

tokens scanner::getTokenType(int state){
    return tokenTypeTable[state];
}

int scanner::getClassifier(char a){
    for(int i = 0; i <  classificationTable.size(); i++){
        for(int j = 0; j < classificationTable[i].size(); j++){
            if(classificationTable[i][j] == a){
                return i;
            }
        }
    }
    return 21;
}

int scanner::getFromTransitionTable(int state, int classifier){
    return transitionTable[state][classifier];
}

void scanner::initTokenMap(){
    m[start] = "<start>";
    m[variable] = "<variable>";
    m[conditions] = "<conditions>";
    m[errorState] = "<errorState>";
    m[endOfExpression] = "<endOfExpression>";
    m[operators] = "<operators>";
    m[returnFunc] = "<returnFunc>";
    m[singleLineComment] = "<singleLineComment>";
    m[multiLineComment] = "<multiLineComment>";
    m[openCircleBracket] = "<openCircleBracket>";
    m[closeCircleBracket] = "<closeCircleBracket>";
    m[openCurlyBracket] = "<openCurlyBracket>";
    m[closeCurlyBracket] = "<closeCurlyBracket>";
    m[intVal] = "<intVal>";
    m[floatVal] = "<floatVal>";
    m[space] = "<space>";
    m[stringVal] = "<stringVal>";
    m[newline] = "<newLine>";
    m[colon] = "<colon>";
    m[comma] = "<comma>";
} 

string scanner::getTokenName(tokens t){
    return this->m[t];
}

void scanner::printStack(){
    stack<int> stcktemp = this->stck;

    while(!stcktemp.empty()){
        int temp = stcktemp.top();
        stcktemp.pop();
        cout << temp << " ";
    }
    cout << endl;
}

