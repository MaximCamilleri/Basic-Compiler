//#include "semanticAnalysisPass.h"

class execution{
private:
    vector<ASTstatement *> * statements;
    scope * head;
public:
    execution(vector<ASTstatement *> * statements, scope * head);
    void beginExecution();
    void parseStatement(ASTstatement * stmt);

    // Instructions
    void variableDecl(ASTvariableDecl * varDecl); 
    void assignment(ASTassignment * ass);  
    void ifStatement(ASTifStatement * ifS); 
    void forStatement(ASTforStatement * forS); 
    void whileStatement(ASTwhileStatement * whileS);
    void rtrnStatement(ASTrtrnStatement * rtrn); 
    void printStatement(ASTprintStatement * print); 
    void functionDecl(ASTfunctionDecl * dec); 
    void block(ASTblock * blk); 

    void expression(ASTexpression * exp);
    void expression2(ASTexpression * exp);
    void formalParams(ASTformalParams * fparams);
    void formalParam(ASTformalParam * fparam);
    void additive(ASTadditiveOp * add);
    void multiplicative(ASTmultiplicativeOp * mul);
    void relational(ASTrelationalOp * rel);
    void literal(ASTliteral * lit);
    void functionCall(ASTfunctionCall * func);
    void subExpression(ASTsubExpression * sub);
    void unary(ASTunary * u);
    void actualParams(ASTactualParams * a);
};

execution::execution(vector<ASTstatement *> * statements, scope * head){
    this->statements = statements;
    this->head = head;
}

void execution::beginExecution(){
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


void execution::variableDecl(ASTvariableDecl * varDecl){
    
}

void execution::assignment(ASTassignment * ass){

}

void execution::block(ASTblock * blk){

}

void execution::printStatement(ASTprintStatement * print){

}

void execution::functionDecl(ASTfunctionDecl * dec){

}

void execution::formalParams(ASTformalParams * fparams){

}

void execution::formalParam(ASTformalParam * fparam){

}

void execution::rtrnStatement(ASTrtrnStatement * rtrn){

}

void execution::ifStatement(ASTifStatement * ifS){

}

void execution::forStatement(ASTforStatement * forS){

}

void execution::whileStatement(ASTwhileStatement * whileS){

}

void execution::expression(ASTexpression * exp){

}

void execution::expression2(ASTexpression * exp){
    
}

void execution::additive(ASTadditiveOp * add){
if(test == "fail"){
        throw std::runtime_error("Incorrect Type");

    }else if(test == "int"){
        if(add->val == "+"){
            return to_string(stoi(LHS) + stoi(RHS));
        }else if(add->val == "-"){
            return to_string(stoi(LHS) - stoi(RHS));
        }else{
            throw std::runtime_error("Can not use 'or' operator with ints");
        }

    }else if(test == "float"){
        if(add->val == "+"){
            return to_string(stof(LHS) + stof(RHS));
        }else if(add->val == "-"){
            return to_string(stof(LHS) - stof(RHS));
        }else{
            throw std::runtime_error("Can not use 'or' operator with ints");
        }

    }else if(test == "char"){
        throw std::runtime_error("Cannot perform additive operation of objects of type car");

    }else if(test == "bool"){
        int LHSval;
        int RHSval;
        if(RHS == "true") RHSval = 1;
        if(RHS == "false") RHSval = 0;
        if(LHS == "true") RHSval = 1;
        if(LHS == "false") RHSval = 0;

        if(add->val == "+" || add->val == "or"){
            if(LHSval + RHSval == 1){
                return "true";
            }else{
                return "false";
            }
    
        }else{
            throw std::runtime_error("Can not use '-' operator with boolean values");
        }
    }
}

void execution::multiplicative(ASTmultiplicativeOp * mul){

}

void execution::relational(ASTrelationalOp * rel){

}

void execution::literal(ASTliteral * lit){

}

void execution::functionCall(ASTfunctionCall * func){

}

void execution::subExpression(ASTsubExpression * sub){

}

void execution::unary(ASTunary * u){

}

void execution::actualParams(ASTactualParams * a){

}