#include "lib_std_facilities.h"

enum tokenType{
    invalid,
    intLiteral
};

class llnode{
private:
    tokenType type;
    string lexeme;
    llnode *nextNode = NULL;

public:
    llnode(string lexeme, tokenType type);

    //setters and getters
    void setType(tokenType type);
    void setLexeme(string lexeme);
    void setNextNode(llnode *nextNode);

    tokenType getType();
    string getLexeme();
    llnode *getNextNode();

    //other functions
    void addNode();
    void deleteNode();
};

llnode::llnode(string lexeme, tokenType type){
    setType(type);
    setLexeme(lexeme);
}

//setters 

void llnode::setType(tokenType type){
    this->type = type;
}
void llnode::setLexeme(string lexeme){
    this->lexeme = lexeme;
}
void llnode::setNextNode(llnode *nextNode){
    this->nextNode = nextNode;
}

//getters

tokenType llnode::getType(){
    return this->type;
}
string llnode::getLexeme(){
    return this->lexeme;
}
llnode *llnode::getNextNode(){
    return this->nextNode;
}

//Linked list

class linkedList{
private:
    llnode *head;
    llnode *tail;

public:
    linkedList(tokenType type, string lexeme);

    //getters and setters 
    llnode *getHead();
    llnode *getTail();

    void setHead(llnode *head);
    void setTail(llnode *tail);
};

linkedList::linkedList(tokenType type, string lexeme){
    this->head = new llnode(lexeme, type);
    this->tail = head;
}
