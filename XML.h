#include "ASTclasses.h"

class xml{
private:
    vector<ASTstatement *> * statements;
    int tabCounter = 0;
public:
    xml(vector<ASTstatement *> * statements);
    void tab();
    void beginXML();
    void parseStatement(ASTstatement * stmt);

    // Instructions
    void variableDecl(ASTvariableDecl * varDecl); // x
    void assignment(ASTassignment * ass);  // x
    void ifStatement(ASTifStatement * ifS); //x
    void forStatement(ASTforStatement * forS); // x
    void whileStatement(ASTwhileStatement * whileS);
    void rtrnStatement(ASTrtrnStatement * rtrn); // x
    void printStatement(ASTprintStatement * print); // x
    void functionDecl(ASTfunctionDecl * dec); // x
    void block(ASTblock * blk); // x

    void expression(ASTexpression * exp);
    void formalParams(ASTformalParams * fparams);
    void formalParam(ASTformalParam * fparam);
};

xml::xml(vector<ASTstatement *> * statements){
    this->statements = statements;
}

void xml::beginXML(){
    for( vector<ASTstatement *>::iterator itr = this->statements->begin(), itr_end = this->statements->end(); itr != itr_end; ++itr ){
        parseStatement(*itr);
    }
}

void xml::parseStatement(ASTstatement * stmt){
    ASTvariableDecl* p1 = dynamic_cast<ASTvariableDecl*>(stmt);
    if(p1 != NULL){
        variableDecl(p1);
    }

    ASTassignment* p2 = dynamic_cast<ASTassignment*>(stmt);
    if(p2 != NULL){
        assignment(p2);
    }

    ASTprintStatement* p3 = dynamic_cast<ASTprintStatement*>(stmt);
    if(p3 != NULL){
        printStatement(p3);
    }

    ASTifStatement* p4 = dynamic_cast<ASTifStatement*>(stmt);
    if(p4 != NULL){
        ifStatement(p4);
    }

    ASTwhileStatement* p5 = dynamic_cast<ASTwhileStatement*>(stmt);
    if(p5 != NULL){
        whileStatement(p5);
    }

    ASTforStatement* p6 = dynamic_cast<ASTforStatement*>(stmt);
    if(p6 != NULL){
        forStatement(p6);
    }

    ASTfunctionDecl* p7 = dynamic_cast<ASTfunctionDecl*>(stmt);
    if(p7 != NULL){
        functionDecl(p7);
    }

    ASTblock* p8 = dynamic_cast<ASTblock*>(stmt);
    if(p8 != NULL){
        block(p8);
    }

    ASTrtrnStatement* p9 = dynamic_cast<ASTrtrnStatement*>(stmt);
    if(p9 != NULL){
        rtrnStatement(p9);
    }
}

void xml::tab(){
    for(int i = 0; i < this->tabCounter; i++){
        cout << "    ";
    }
}

void xml::variableDecl(ASTvariableDecl * varDecl){
    tab();
    cout << "<Decl>" << endl;
    this->tabCounter += 1;
    tab();
    cout << "<Var Type=" + varDecl->type->val + ">" + varDecl->ident->val + "</Var>" << endl;
    tab();
    expression(varDecl->exp);
    this->tabCounter -= 1;
    tab();
    cout << "<\\Decl>" << endl;
    
    
}

void xml::assignment(ASTassignment * ass){
    tab();
    cout << "Assignment" << endl;
    this->tabCounter += 1;
    tab();
    cout << "<Var>" + ass->LHS + "</Var>" << endl;
    expression(ass->RHS);
    this->tabCounter -= 1;
    tab();
    cout << "<\\Assignment>" << endl;
    

}

void xml::block(ASTblock * blk){
    tab();
    cout << "<block>" << endl;
    this->tabCounter += 1;
    parseStatement(blk->stmt);
    this->tabCounter -= 1;
    tab();
    cout << "<\\block>" << endl;
}

void xml::expression(ASTexpression * exp){
    cout << "expression" << endl;
}

void xml::printStatement(ASTprintStatement * print){
    tab();
    cout << "<Print>" << endl;
    this->tabCounter += 1;
    tab();
    expression(print->RHS);
    this->tabCounter -= 1;
    tab();
    cout << "<\\Print>" << endl;
}

void xml::functionDecl(ASTfunctionDecl * dec){
    tab();
    cout << "<Function Declaration>" << endl;
    this->tabCounter += 1;
    tab();
    cout << "<Identifier>" + dec->ident->val + "<\\Identifier>" << endl;
    formalParams(dec->param);
    tab();
    cout << "<Type>" + dec->type->val + "<\\Type>" << endl;
    block(dec->block);
    this->tabCounter -= 1;
    tab();
    cout << "<\\Function Declaration>" << endl;
}

void xml::formalParams(ASTformalParams * fparams){
    tab();
    cout << "<Formal Parameters>" << endl;
    this->tabCounter += 1;
    formalParam(fparams->p);
    for(auto itr = fparams->formalParam->begin(), itr_end = fparams->formalParam->end(); itr != itr_end; ++itr){
        formalParam(*itr);
    }
    this->tabCounter -= 1;
    tab();
    cout << "<\\Formal Parameters>" << endl;
}

void xml::formalParam(ASTformalParam * fparam){
    tab();
    cout << "<Formal Param>" << endl;
    this->tabCounter += 1;
    tab();
    cout << "<Identifier>" + fparam->ident->val + "<\\Identifier>" << endl;
    tab();
    cout << "<Type>" + fparam->type->val + "<\\Type>" << endl;
    this->tabCounter -= 1;
    tab();
    cout << "<\\Formal Param>" << endl;
}

void xml::rtrnStatement(ASTrtrnStatement * rtrn){
    tab();
    cout << "<return>" << endl;
    this->tabCounter += 1;
    tab();
    expression(rtrn->exp);
    this->tabCounter -= 1;
    tab();
    cout << "<\\return>" << endl;
}

void xml::ifStatement(ASTifStatement * ifS){
    tab();
    cout << "<if>" << endl;
    this->tabCounter += 1;
    tab();
    expression(ifS->exp);
    tab();
    block(ifS->blk);
    if(ifS->_else != nullptr){
        tab();
        cout << "<else>" << endl;
        this->tabCounter += 1;
        tab();
        block(ifS->_else);
        this->tabCounter -= 1;
        tab();
        cout << "<\\else>" << endl;
    }
    this->tabCounter -= 1;
    tab();
    cout << "<\\if>" << endl;
}

void xml::forStatement(ASTforStatement * forS){
    tab();
    cout << "for" << endl;
    this->tabCounter += 1;
    variableDecl(forS->vDecl);
    expression(forS->exp);
    assignment(forS->ass);
    block(forS->b);
    this->tabCounter -= 1;
    tab();
    cout << "<\\for>" << endl;
}

void xml::whileStatement(ASTwhileStatement * whileS){
    tab();
    cout << "while" << endl;
    this->tabCounter += 1;
    expression(whileS->exp);
    block(whileS->b);
    this->tabCounter -= 1;
    tab();
    cout << "<\\while>" << endl;
}