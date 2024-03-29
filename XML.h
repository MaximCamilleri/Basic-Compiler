#include "ASTclasses.h"
#include <stdexcept>

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

xml::xml(vector<ASTstatement *> * statements){
    this->statements = statements;
}

void xml::beginXML(){
    cout << "XML Pass:" << endl << endl;
    for( vector<ASTstatement *>::iterator itr = this->statements->begin(), itr_end = this->statements->end(); itr != itr_end; ++itr ){
        parseStatement(*itr);
    }
    cout << endl;
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
    expression(varDecl->exp);
    this->tabCounter -= 1;
    tab();
    cout << "<\\Decl>" << endl;
    
    
}

void xml::assignment(ASTassignment * ass){
    tab();
    cout << "<Assignment>" << endl;
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
    for(auto itr = blk->stmt->begin(), itr_end = blk->stmt->end(); itr != itr_end; ++itr){
        parseStatement(*itr);
    }
    this->tabCounter -= 1;
    tab();
    cout << "<\\block>" << endl;
}

void xml::printStatement(ASTprintStatement * print){
    tab();
    cout << "<Print>" << endl;
    this->tabCounter += 1;
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
    if(dec->param != nullptr){
        formalParams(dec->param);
    }
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
    expression(rtrn->exp);
    this->tabCounter -= 1;
    tab();
    cout << "<\\return>" << endl;
}

void xml::ifStatement(ASTifStatement * ifS){
    tab();
    cout << "<if>" << endl;
    this->tabCounter += 1;
    expression(ifS->exp);
    block(ifS->blk);
    if(ifS->_else != nullptr){
        tab();
        cout << "<else>" << endl;
        this->tabCounter += 1;
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
    cout << "<for>" << endl;
    this->tabCounter += 1;
    if(forS->vDecl != nullptr){
        variableDecl(forS->vDecl);
    }
    expression(forS->exp);
    assignment(forS->ass);
    block(forS->b);
    this->tabCounter -= 1;
    tab();
    cout << "<\\for>" << endl;
}

void xml::whileStatement(ASTwhileStatement * whileS){
    tab();
    cout << "<while>" << endl;
    this->tabCounter += 1;
    expression(whileS->exp);
    block(whileS->b);
    this->tabCounter -= 1;
    tab();
    cout << "<\\while>" << endl;
}

void xml::expression(ASTexpression * exp){
    tab();
    cout << "<Expression>" << endl;
    this->tabCounter += 1;
    bool test = false;
    ASTidentifier* ident = dynamic_cast<ASTidentifier*>(exp->data);
    if(ident != NULL){
        tab();
        cout << "<ident>" + ident->val + "<\\ident>" << endl;
        test = true;
    }

    ASTfunctionCall* func = dynamic_cast<ASTfunctionCall*>(exp->data);
    if(func != NULL){
        functionCall(func);
        test = true;
    }

    ASTsubExpression* sub = dynamic_cast<ASTsubExpression*>(exp->data);
    if(sub != NULL){
        subExpression(sub);
        test = true;
    }

    ASTunary* u = dynamic_cast<ASTunary*>(exp->data);
    if(u != NULL){
        unary(u);
        test = true;
    }

    ASTliteral* lit = dynamic_cast<ASTliteral*>(exp->data);
    if(lit != NULL){
        literal(lit);
        test = true;
    }

    ASTmultiplicativeOp* mul = dynamic_cast<ASTmultiplicativeOp*>(exp->data);
    if(mul != NULL){
        multiplicative(mul);
        test = true;
    }

    ASTadditiveOp* add = dynamic_cast<ASTadditiveOp*>(exp->data);
    if(add != NULL){
        additive(add);
        test = true;
    }

    ASTrelationalOp* rel = dynamic_cast<ASTrelationalOp*>(exp->data);
    if(rel != NULL){
        relational(rel);
        test = true;
    }

    if(!test){
        cout << "Nothing found" << endl;
    }

    this->tabCounter -= 1;
    tab();
    cout << "<\\Expression>" << endl;
}

void xml::expression2(ASTexpression * exp){
    tab();
    cout << "<Expression>" << endl;
    this->tabCounter += 1;
    bool test = false;
    ASTidentifier* ident = dynamic_cast<ASTidentifier*>(exp);
    if(ident != NULL){
        tab();
        cout << "<ident>" + ident->val + "<\\ident>" << endl;
        test = true;
    }

    ASTfunctionCall* func = dynamic_cast<ASTfunctionCall*>(exp);
    if(func != NULL){
        functionCall(func);
        test = true;
    }

    ASTsubExpression* sub = dynamic_cast<ASTsubExpression*>(exp);
    if(sub != NULL){
        subExpression(sub);
        test = true;
    }

    ASTunary* u = dynamic_cast<ASTunary*>(exp);
    if(u != NULL){
        unary(u);
        test = true;
    }

    ASTliteral* lit = dynamic_cast<ASTliteral*>(exp);
    if(lit != NULL){
        literal(lit);
        test = true;
    }

    ASTmultiplicativeOp* mul = dynamic_cast<ASTmultiplicativeOp*>(exp);
    if(mul != NULL){
        multiplicative(mul);
        test = true;
    }

    ASTadditiveOp* add = dynamic_cast<ASTadditiveOp*>(exp);
    if(add != NULL){
        additive(add);
        test = true;
    }

    ASTrelationalOp* rel = dynamic_cast<ASTrelationalOp*>(exp);
    if(rel != NULL){
        relational(rel);
        test = true;
    }

    if(!test){
        cout << "Nothing found" << endl;
    }

    this->tabCounter -= 1;
    tab();
    cout << "<\\Expression>" << endl;
}

void xml::additive(ASTadditiveOp * add){
    tab();
    cout << "<Additive symbol = '" + add->val + "'>" << endl;
    this->tabCounter += 1;
    expression2(add->LHS);
    expression2(add->RHS);
    this->tabCounter -= 1;
    tab();
    cout << "<\\Additive>" << endl;
}

void xml::multiplicative(ASTmultiplicativeOp * mul){
    tab();
    cout << "<Multiplicative symbol = '" + mul->val + "'>" << endl;
    this->tabCounter += 1;
    expression2(mul->LHS);
    expression2(mul->RHS);
    this->tabCounter -= 1;
    tab();
    cout << "<\\Multiplicative>" << endl;
}

void xml::relational(ASTrelationalOp * rel){
    tab();
    cout << "<Relational symbol = '" + rel->val + "'>" << endl;
    this->tabCounter += 1;
    expression2(rel->LHS);
    expression2(rel->RHS);
    this->tabCounter -= 1;
    tab();
    cout << "<\\Relational>" << endl;
}

void xml::literal(ASTliteral * lit){
    ASTbooleanLiteral* b = dynamic_cast<ASTbooleanLiteral*>(lit);
    if(b != NULL){
        tab();
        cout << "<Boolean>" + b->val + "<\\Boolean>" << endl;
    }

    ASTintLiteral* i = dynamic_cast<ASTintLiteral*>(lit);
    if(i != NULL){
        tab();
        cout << "<Integer>" + i->val + "<\\Integer>" << endl;
    }

    ASTfloatLiteral* f = dynamic_cast<ASTfloatLiteral*>(lit);
    if(f != NULL){
        tab();
        cout << "<Float>" + f->val + "<\\Float>" << endl;
    }

    ASTcharLiteral* c = dynamic_cast<ASTcharLiteral*>(lit);
    if(c != NULL){
        tab();
        cout << "<Char>" + c->val + "<\\Char>" << endl;
    }
}

void xml::functionCall(ASTfunctionCall * func){
    tab();
    cout << "<Function Call>" << endl;
    this->tabCounter += 1;
    tab();
    cout << "<Identifier>" + func->ident->val + "<\\Identifier>" << endl;
    actualParams(func->a);
    this->tabCounter -= 1;
    tab();
    cout << "<\\Function Call>" << endl;
}

void xml::subExpression(ASTsubExpression * sub){
    tab();
    cout << "<Sub Expression>" << endl;
    this->tabCounter += 1;
    expression(sub->exp);
    this->tabCounter -= 1;
    tab();
    cout << "<\\Sub Expression>" << endl;
}

void xml::unary(ASTunary * u){
    tab();
    cout << "<Unary>" << endl;
    this->tabCounter += 1;
    tab();
    cout << "<Indicator symbol = '" + u->LHS + "'>" << endl;
    expression(u->RHS);
    this->tabCounter -= 1;
    tab();
    cout << "<\\Unary>" << endl;
}

void xml::actualParams(ASTactualParams * a){
    tab();
    cout << "<Actual Parameters>" << endl;
    this->tabCounter += 1;
    expression(a->exp);
    for(vector<ASTexpression *>::iterator itr = a->exps->begin(), itr_end = a->exps->end(); itr != itr_end; ++itr){
        expression(*itr);
    }
    this->tabCounter -= 1;
    tab();
    cout << "<\\Actual Parameters>" << endl;
}