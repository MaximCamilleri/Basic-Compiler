#include "lib_std_facilities.h"
#include <stack>

class scanner{
private:
   string classificationTable[2][11] = {
       {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "EOF"},
       {"0", "0", "0", "0", "0", "0", "0", "0", "0", "1"}
   };

   String transitionTable[3][3] = {
       {"", "0", "1"}, //0 digit, 1 other
       {"0", "0", "1"},
       {"1", "1", "1"}
   };
   string tokenTypeTable[2][2] = {
       {"0", "1"},
       {"integerLiteral","reject"}
   };
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

    //table functions
    int getFromTransitionTable(int state, int classifier);
    string getTokenType(int state);
    int getClassifier(string a);
};

scanner::scanner(){
    initScanner();
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

string scanner::getTokenType(int state){
    return tokenTypeTable[state][1];
}

int scanner::getClassifier(string a){
    for(int i = 0; i <  sizeof(classificationTable[0])/sizeof(string); i++){
        if(classificationTable[0][i] == a){
            return stoi(classificationTable[1][i]);
        }
    }
    return -1;
}

