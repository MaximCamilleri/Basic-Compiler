#include "lib_std_facilities.h"
#include <stack>
#include "program.h"

enum tokens {
    start,
    errorState,
    variable, 
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
    charVal,
    equals,
    // reserved words
    _fn, _float, _int, _bool, _return, _let, _print, _char, _true, _false, _if, _for, _while, _else, _and, _not, _or,
    // Operators
    _minus, _plus, _multiply, _divide
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
    // !     =     +     -     *     /     <     >     ;     {      }      (     )     .     <space> "     <newLine> :     ,     '
       {33}, {61}, {43}, {45}, {42}, {47}, {60}, {62}, {59}, {123}, {125}, {40}, {41}, {46}, {32},   {34}, {10},     {58}, {44}, {46}
   };

   int transitionTable[37][23] = {
    // <letter> <digit> !   =   +   -   *   /   <   >   ;   {   }   (   )   .   <space> "  \n   :   ,   '   other
       {1,      24,     8,  4,  11, 12, 13, 14, 6,  2,  10, 22, 23, 20, 21, 30, 27,     28, 31, 32, 33, 34, 30}, // S0  start
       {1,      1,      30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S1  variable
       {30,     30,     30, 3,  30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S2  >
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S3  >=
       {30,     30,     30, 5,  30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S4  =
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S5  ==
       {30,     30,     30, 7,  30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S6  <
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S7  <=
       {30,     30,     30, 9,  30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S8  error
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S9  !=
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S10 ;
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S11 +
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 15, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S12 - 
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S13 *
       {30,     30,     30, 30, 30, 30, 17, 16, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S14 /
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S15 ->
       {16,     16,     16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,     16, 30, 16, 16, 16, 16}, // S16 //
       {17,     17,     17, 17, 17, 17, 18, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,     17, 17, 17, 17, 17, 17}, // S17 error
       {17,     17,     17, 17, 17, 17, 18, 19, 17, 17, 17, 17, 17, 17, 17, 17, 17,     17, 17, 17, 17, 17, 17}, // S18 error
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S19 /* ... */
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S20 (
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S21 )
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S22 {
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S23 }
       {30,     24,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 25, 30,     30, 30, 30, 30, 30, 30}, // S24 int
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S25 error
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S26 float
       {30,     30,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 27,     30, 30, 30, 30, 30, 30}, // S27 space
       {28,     28,     28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,     29, 30, 30, 30, 30, 30}, // S28 error
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S29 string
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S30 Final Error State
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S31 NewLine
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S32 colon
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}, // S33 comma
       {35,     35,     35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,     35, 30, 35, 35, 35, 35}, // S34 error
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 36, 30}, // S35 error
       {30,     26,     30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,     30, 30, 30, 30, 30, 30}  // S36 charVal
   };
   tokens tokenTypeTable[37] = {
    // S0      S1        S2          S3          S4          S5          S6          
       start, variable, conditions, conditions, equals, conditions, conditions, 
    // S7          S8          S9          S10              S11        S12        S13        S14
       conditions, errorState, conditions, endOfExpression, _plus, _minus, _multiply, _divide, 
    // S15         S16                S17         S18         S19               S20                S21
       returnFunc, singleLineComment, errorState, errorState, multiLineComment, openCircleBracket, closeCircleBracket,
    // S22               S23                S24     S25          S26       S27    S28        S29        S30
       openCurlyBracket, closeCurlyBracket, intVal, errorState, floatVal, space, errorState, stringVal, errorState,
    // S31      S32    S33    S34         S35         S36
       newline, colon, comma, errorState, errorState, charVal
   };

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

    //check state
    tokens checkReservedWord();

    //tokenization
    void getNextToken(string pName);

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
    m[_fn] = "<_fn>";
    m[_float] = "<_float>";
    m[_int] = "<_int>";
    m[_bool] = "<_bool>";
    m[_return] = "<_return>";
    m[_let] = "<_let>";
    m[_print] = "<_print>";
    m[_char] = "<_char>";
    m[_true] = "<_true>";
    m[_false] = "<_false>";
    m[_if] = "<_if>";
    m[_for] = "<_for>";
    m[_while] = "<_while>";
    m[_else] = "<_else>";
    m[_and] = "<_and>";
    m[_not] = "<_not>";
    m[_or] = "<_or>";
    m[_plus] = "<_plus>";
    m[_minus] = "<_minus>";
    m[_multiply] = "<_multiple>";
    m[_divide] = "<_divide>";
    m[charVal] = "<charVal>";
    m[equals] = "<equals>";
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

tokens scanner::checkReservedWord(){
    if(this->lexeme == "fn"){
        return _fn;
    }else if(this->lexeme == "float"){
        return _float;
    }else if(this->lexeme == "int"){
        return _int;
    }else if(this->lexeme == "bool"){
        return _bool;
    }else if(this->lexeme == "return"){
        return _return;
    }else if(this->lexeme == "let"){
        return _let;
    }else if(this->lexeme == "print"){
        return _print;
    }else if(this->lexeme == "char"){
        return _char;
    }else if(this->lexeme == "true"){
        return _true;
    }else if(this->lexeme == "false"){
        return _false;
    }else if(this->lexeme == "if"){
        return _if;
    }else if(this->lexeme == "for"){
        return _for;
    }else if(this->lexeme == "while"){
        return _while;
    }else if(this->lexeme == "else"){
        return _else;
    }
    return variable;


}

void scanner::getNextToken(string pName){
    program prog(pName);
}

bool rollbackLoop(scanner *sc, program *prog, string &token);
bool scannerLoop(scanner *sc, program *prog, string &token);

bool scannerLoop(scanner *sc, program *prog, string &token){
   sc->initScanner();
   while(sc->getState() != 30){ // 30 will indicate an error state
      char nextChar = prog->getNextChar();
      sc->appendLexeme(nextChar);
      if(sc->getTokenType(sc->getState()) != errorState && sc->getTokenType(sc->getState()) != start) { // we found a final state
         sc->cleanStack(); //delete everything from the stack, indicating that this is the most resent final state found
      }

      sc->pushStack(sc->getState());

      int cat = sc->getClassifier(nextChar); //get the category form the classification table
      sc->setState(sc->getFromTransitionTable(sc->getState(), cat));
   }
   return rollbackLoop(sc, prog, token);
}

bool rollbackLoop(scanner *sc, program *prog, string &token){
   while(sc->getState() != -1 && (sc->getTokenType(sc->getState()) == errorState || sc->getTokenType(sc->getState()) == start)){ 
      sc->setState(sc->popStack());
      prog->decrementChar();
      if (!sc->getLexeme().empty()) {
         string temp = sc->getLexeme();
         temp.pop_back();
         sc->setLexeme(temp);
      }
   }

   if(sc->getTokenType(sc->getState()) != errorState && sc->getState() != -1){
      if (char(sc->getLexeme()[0]) == 10){
          string temp = "\\n " + sc->getTokenName(sc->getTokenType(sc->getState()));
          //cout << temp << endl;
          token = temp;
      }else{
         tokens tType = sc->getTokenType(sc->getState());
         if(tType == variable){
            tType = sc->checkReservedWord();
         }
         string temp = sc->getLexeme() + " " + sc->getTokenName(tType);
         //cout << temp << endl;
         token = temp;
      }
      return false;
   }else{
      cout << "Lexical error";
      return true;
   }
}

