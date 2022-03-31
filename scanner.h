#include "lib_std_facilities.h"
#include <stack>

enum stateType {
    general, 
    accepting,
    error
};

class scanner{
private:
   string classificationTable[2][11] = {
       {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "EOF"},
       {"digit", "digit", "digit", "digit", "digit", "digit", "digit", "digit", "digit", "other"}
   };

//    char transitionTable[][] = {
//        {}
//    };
//    char tokenTypeTable[][] = {
//        {}
//    };
   int state;
   string lexeme;
   stack<int> stck;

public:

    scanner();
    void initScanner();
    void cleanStack();

    //gettes and setters

    int getState();
    string getLexeme();
    int popStack();

    void setState(int state);
    void setLexeme(string lexeme);
    void appendLexeme(char l);
    void pushStack(int state);
};

scanner::scanner(){
    initScanner();
}

void scanner::initScanner(){
    cleanStack();
    this->stck.push('bad');
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

