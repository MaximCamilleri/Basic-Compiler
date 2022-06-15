#include "semanticAnalysis.h"
#include <iostream>

using namespace std;

bool isFloat(string myString);

class semanticAnalysis{
private:
    scope * globalScope;
    vector<scope *> * scopes;
    vector<ASTstatement *> * statements;
    vector<func *> * functions;

public:
    semanticAnalysis(vector<ASTstatement *> * statements);
    void beginSemanticAnalysis();

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
    void forBlock(ASTblock * blk, ASTvariableDecl * v); 
    scope * funcBlock(ASTblock * blk, ASTformalParams * param); 

    string expression(ASTexpression * exp, string varT);
    string expression2(ASTexpression * exp, string varT);
    void formalParams(ASTformalParams * fparams);
    void formalParam(ASTformalParam * fparam);
    string additive(ASTadditiveOp * add, string varT);
    string multiplicative(ASTmultiplicativeOp * mul, string varT);
    string relational(ASTrelationalOp * rel, string varT);
    string literal(ASTliteral * lit);
    void functionCall(ASTfunctionCall * func);
    void subExpression(ASTsubExpression * sub);
    void unary(ASTunary * u);
    void actualParams(ASTactualParams * a);

    string testType(string LHS, string RHS, string varT);
};

semanticAnalysis::semanticAnalysis(vector<ASTstatement *> * statements){
    this->statements = statements;
    this->globalScope = new scope();
    this->scopes = new vector<scope *>();
    this->scopes->push_back(globalScope);
    this->functions = new vector<func *>();
}

void semanticAnalysis::beginSemanticAnalysis(){
    for( vector<ASTstatement *>::iterator itr = this->statements->begin(), itr_end = this->statements->end(); itr != itr_end; ++itr ){
        parseStatement(*itr);
    }
}

void semanticAnalysis::parseStatement(ASTstatement * stmt){
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

void semanticAnalysis::variableDecl(ASTvariableDecl * varDecl){
    string val;
    val = expression(varDecl->exp, varDecl->type->val);
    this->scopes->back()->addVar(varDecl->type->val, varDecl->ident->val);
}

void semanticAnalysis::assignment(ASTassignment * ass){
    var * v = nullptr;
    for(auto itr = this->scopes->begin(), itr_end = this->scopes->end(); itr != itr_end; ++itr){
        if(v != nullptr){
            break;
        }

        v = (*itr)->checkVarName(ass->LHS);
    }

    if(v != nullptr){
        string val = expression(ass->RHS, ass->LHS);
        v = nullptr;
        for(auto itr = this->scopes->begin(), itr_end = this->scopes->end(); itr != itr_end; ++itr){
            if(v != nullptr){
                (*(itr-1))->updateVar(ass->LHS, val);
                break;
            }

            v = (*itr)->checkVarName(ass->LHS);
        }
    }else{
        throw std::runtime_error("Variable name does not exist");
    }
    
}

void semanticAnalysis::block(ASTblock * blk){
    this->scopes->push_back(new scope());
    for(vector<ASTstatement *>::iterator itr = blk->stmt->begin(), itr_end = blk->stmt->end(); itr != itr_end; ++itr){
        parseStatement(*itr);
    }
    this->scopes->pop_back();
}

scope * semanticAnalysis::funcBlock(ASTblock * blk, ASTformalParams * param){
    this->scopes->push_back(new scope());
    formalParams(param);

    for( vector<ASTstatement *>::iterator itr = blk->stmt->begin(), itr_end = blk->stmt->end(); itr != itr_end; ++itr){
        parseStatement(*itr);
    }

    auto temp = this->scopes->end()[-1];
    this->scopes->pop_back();
    return temp;
}

void semanticAnalysis::forBlock(ASTblock * blk, ASTvariableDecl * v){
    this->scopes->push_back(new scope());
    if(v != nullptr){
        variableDecl(v);
    }

    for( vector<ASTstatement *>::iterator itr = blk->stmt->begin(), itr_end = blk->stmt->end(); itr != itr_end; ++itr){
        parseStatement(*itr);
    }
    this->scopes->pop_back();
}


void semanticAnalysis::printStatement(ASTprintStatement * print){

}

void semanticAnalysis::functionDecl(ASTfunctionDecl * dec){
    string f = "";
    for(auto itr = this->functions->begin(), itr_end = this->functions->end(); itr != itr_end; ++itr){
        if((*itr)->getName() == dec->ident->val){
            f = dec->ident->val;
        }
    }

    if(f != ""){
        throw std::runtime_error("Function already exists");
    }

    auto block = funcBlock(dec->block, dec->param);
    this->functions->push_back(new func(dec->ident->val, block, dec->type->val));
}

void semanticAnalysis::formalParams(ASTformalParams * fparams){
    formalParam(fparams->p);
    for(auto itr = fparams->formalParam->begin(), itr_end = fparams->formalParam->end(); itr != itr_end; ++itr){
        formalParam(*itr);
    }
}

void semanticAnalysis::formalParam(ASTformalParam * fparam){
    this->scopes->back()->addVar(fparam->type->val, fparam->ident->val);
}

void semanticAnalysis::rtrnStatement(ASTrtrnStatement * rtrn){

}

void semanticAnalysis::ifStatement(ASTifStatement * ifS){
    block(ifS->blk);
}

void semanticAnalysis::forStatement(ASTforStatement * forS){
    forBlock(forS->b, forS->vDecl);
}

void semanticAnalysis::whileStatement(ASTwhileStatement * whileS){
    block(whileS->b);
}

string semanticAnalysis::expression(ASTexpression * exp, string varT){
    ASTidentifier* ident = dynamic_cast<ASTidentifier*>(exp->data);
    if(ident != NULL){
        var * v = nullptr;
        for(auto itr = this->scopes->begin(), itr_end = this->scopes->end(); itr != itr_end; ++itr){

            if(v != nullptr){
                break;
            }
            v = (*itr)->checkVarName(ident->val);
        }

        if(v != nullptr){
            return v->getValue();
        }else{
            throw std::runtime_error("Cannot find variable");
        }
    }

    // ASTfunctionCall* func = dynamic_cast<ASTfunctionCall*>(exp->data);
    // if(func != NULL){
    //     functionCall(func);
    // }

    // ASTsubExpression* sub = dynamic_cast<ASTsubExpression*>(exp->data);
    // if(sub != NULL){
    //     subExpression(sub);
    // }

    // ASTunary* u = dynamic_cast<ASTunary*>(exp->data);
    // if(u != NULL){
    //     unary(u);
    // }

    ASTliteral* lit = dynamic_cast<ASTliteral*>(exp->data);
    if(lit != NULL){
        return literal(lit);
    }

    ASTmultiplicativeOp* mul = dynamic_cast<ASTmultiplicativeOp*>(exp->data);
    if(mul != NULL){
        return multiplicative(mul, varT);
    }

    ASTadditiveOp* add = dynamic_cast<ASTadditiveOp*>(exp->data);
    if(add != NULL){
        return additive(add, varT);
    }

    ASTrelationalOp* rel = dynamic_cast<ASTrelationalOp*>(exp->data);
    if(rel != NULL){
        return relational(rel, varT);
    }
    return "fail";
}

string semanticAnalysis::expression2(ASTexpression * exp, string varT){
    ASTidentifier* ident = dynamic_cast<ASTidentifier*>(exp);
    if(ident != NULL){
        var * v = nullptr;
        for(auto itr = this->scopes->begin(), itr_end = this->scopes->end(); itr != itr_end; ++itr){

            if(v != nullptr){
                break;
            }
            v = (*itr)->checkVarName(ident->val);
        }

        if(v != nullptr){
            return v->getValue();
        }else{
            throw std::runtime_error("Cannot find variable");
        }
    }

    // ASTfunctionCall* func = dynamic_cast<ASTfunctionCall*>(exp);
    // if(func != NULL){
    //     functionCall(func);
    // }

    // ASTsubExpression* sub = dynamic_cast<ASTsubExpression*>(exp);
    // if(sub != NULL){
    //     subExpression(sub);
    // }

    // ASTunary* u = dynamic_cast<ASTunary*>(exp);
    // if(u != NULL){
    //     unary(u);
    // }

    ASTliteral* lit = dynamic_cast<ASTliteral*>(exp);
    if(lit != NULL){
        return literal(lit);
    }

    ASTmultiplicativeOp* mul = dynamic_cast<ASTmultiplicativeOp*>(exp);
    if(mul != NULL){
        return multiplicative(mul, varT);
    }

    ASTadditiveOp* add = dynamic_cast<ASTadditiveOp*>(exp);
    if(add != NULL){
        return additive(add, varT);
    }

    ASTrelationalOp* rel = dynamic_cast<ASTrelationalOp*>(exp);
    if(rel != NULL){
        return relational(rel, varT);
    }
    return "fail";
}

string semanticAnalysis::additive(ASTadditiveOp * add, string varT){
    string LHS = expression2(add->LHS, varT);
    string RHS = expression2(add->RHS, varT);

    if(LHS == "" && RHS == ""){
        return "";
    }

    string test = testType(LHS, RHS, varT);
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
    
    return "fail";
}

string semanticAnalysis::testType(string LHS, string RHS, string varT){
    if(varT == "int"){
        for (char const &c : LHS) {
            if (std::isdigit(c) == 0) return "fail";
        }
        for (char const &c : RHS) {
            if (std::isdigit(c) == 0) return "fail";
        }
        return "int";

    }else if(varT == "float"){
        if(!isFloat(LHS)) return "fail";
        if(!isFloat(RHS)) return "fail";

        return "float";

    }else if(varT == "char"){
        if(LHS.length() != 1) return "fail";
        if(RHS.length() != 1) return "fail";

        return "char";

    }else if(varT == "bool"){
        if(LHS != "true" || LHS != "false") return "fail";
        if(RHS != "true" || RHS != "false") return "fail";

        return "bool";
    }
    
    return "fail";
}

string semanticAnalysis::multiplicative(ASTmultiplicativeOp * mul, string varT){
    string LHS = expression2(mul->LHS, varT);
    string RHS = expression2(mul->RHS, varT);

    return LHS;
}

string semanticAnalysis::relational(ASTrelationalOp * rel, string varT){
    string LHS = expression2(rel->LHS, varT);
    string RHS = expression2(rel->RHS, varT);

    return LHS;
}

string semanticAnalysis::literal(ASTliteral * lit){
    ASTbooleanLiteral* b = dynamic_cast<ASTbooleanLiteral*>(lit);
    if(b != NULL){
        return b->val;
    }

    ASTintLiteral* i = dynamic_cast<ASTintLiteral*>(lit);
    if(i != NULL){
        return i->val;
    }

    ASTfloatLiteral* f = dynamic_cast<ASTfloatLiteral*>(lit);
    if(f != NULL){
        return f->val;
    }

    ASTcharLiteral* c = dynamic_cast<ASTcharLiteral*>(lit);
    if(c != NULL){
        return c->val;
    }
    return "fail";
}

void semanticAnalysis::functionCall(ASTfunctionCall * func){

}

void semanticAnalysis::subExpression(ASTsubExpression * sub){

}

void semanticAnalysis::unary(ASTunary * u){

}

void semanticAnalysis::actualParams(ASTactualParams * a){

}


bool isFloat(string myString) {
    std::istringstream iss(myString);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail(); 
}




