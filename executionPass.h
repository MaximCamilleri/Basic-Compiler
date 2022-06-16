#include "semanticAnalysisPass.h"

class execution{
private:
    scope * globalScope;
    int hasReturn;
    string varType;
    int inIfStatement = 0;
    vector<scope *> * scopes;
    vector<ASTstatement *> * statements;
    vector<func *> * functions;

public:
    execution(vector<ASTstatement *> * statements);
    void beginExecution();

    void parseStatement(ASTstatement * stmt, ASTtype * t);

    // Instructions
    void variableDecl(ASTvariableDecl * varDecl);
    void assignment(ASTassignment * ass);
    void ifStatement(ASTifStatement * ifS, ASTtype * t);
    void forStatement(ASTforStatement * forS);
    void whileStatement(ASTwhileStatement * whileS);
    void rtrnStatement(ASTrtrnStatement * rtrn, ASTtype * t);
    void printStatement(ASTprintStatement * print);
    void functionDecl(ASTfunctionDecl * dec);
    void block(ASTblock * blk, ASTtype * t);
    void forBlock(ASTblock * blk, ASTvariableDecl * v);
    scope * funcBlock(ASTblock * blk, ASTformalParams * param, ASTtype * t);

    string expression(ASTexpression * exp);
    string expression2(ASTexpression * exp);
    void formalParams(ASTformalParams * fparams);
    void formalParam(ASTformalParam * fparam);
    string additive(ASTadditiveOp * add);
    string multiplicative(ASTmultiplicativeOp * mul);
    string relational(ASTrelationalOp * rel);
    string literal(ASTliteral * lit);
    void functionCall(ASTfunctionCall * func);
    string subExpression(ASTsubExpression * sub);
    string unary(ASTunary * u);
    string actualParams(ASTactualParams * a);

    string testType(string LHS, string RHS, string varT);
};

execution::execution(vector<ASTstatement *> * statements){
    this->statements = statements;
    this->globalScope = new scope();
    this->scopes = new vector<scope *>();
    this->scopes->push_back(globalScope);
    this->functions = new vector<func *>();
    this->varType = "";
}

void execution::beginExecution(){
    cout << "Starting Execution" << endl;
    for( vector<ASTstatement *>::iterator itr = this->statements->begin(), itr_end = this->statements->end(); itr != itr_end; ++itr ){
        parseStatement(*itr, nullptr);
        this->varType = "";
    }
    cout << "Finishing Execution" << endl;
}

void execution::parseStatement(ASTstatement * stmt, ASTtype * t){
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
        ifStatement(p4, t);
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
        block(p8, t);
    }

    ASTrtrnStatement* p9 = dynamic_cast<ASTrtrnStatement*>(stmt);
    if(p9 != NULL){
        rtrnStatement(p9, t);
    }
}

void execution::variableDecl(ASTvariableDecl * varDecl){
    var * v = nullptr;
    for(auto itr = this->scopes->begin(), itr_end = this->scopes->end(); itr != itr_end; ++itr){
        if(v != nullptr){
            break;
        }

        v = (*itr)->checkVarName(varDecl->ident->val);
    }
    if(v == nullptr){
        string val;
        this->varType = varDecl->type->val;
        val = expression(varDecl->exp);
        this->scopes->back()->addVar(varDecl->type->val, val, varDecl->ident->val);
    }else{
        throw std::runtime_error("Variable already exists");
    }
    
}

void execution::assignment(ASTassignment * ass){
    var * v = nullptr;
    for(auto itr = this->scopes->begin(), itr_end = this->scopes->end(); itr != itr_end; ++itr){
        if(v != nullptr){
            break;
        }

        v = (*itr)->checkVarName(ass->LHS);
    }

    if(v != nullptr){
        this->varType = v->getVarType();
        string val = expression(ass->RHS);
        v = nullptr;
        for(auto itr = this->scopes->begin(), itr_end = this->scopes->end(); itr != itr_end; ++itr){
            v = (*itr)->checkVarName(ass->LHS);

            if(v != nullptr){
                (*(itr))->updateVar(ass->LHS, val);
                break;
            }
        }
    }else{
        throw std::runtime_error("Variable name does not exist");
    }
    
}

void execution::block(ASTblock * blk, ASTtype * t){
    this->scopes->push_back(new scope());
    for(vector<ASTstatement *>::iterator itr = blk->stmt->begin(), itr_end = blk->stmt->end(); itr != itr_end; ++itr){
        parseStatement(*itr, t);
    }
    this->scopes->pop_back();
}

scope * execution::funcBlock(ASTblock * blk, ASTformalParams * param, ASTtype * t){
    this->scopes->push_back(new scope());
    if(param != nullptr){
        formalParams(param);
    }
    
    for( vector<ASTstatement *>::iterator itr = blk->stmt->begin(), itr_end = blk->stmt->end(); itr != itr_end; ++itr){
        parseStatement(*itr, t);
    }

    if(this->hasReturn < this->inIfStatement + 1){
        throw std::runtime_error("Function has no return");
    }
    this->hasReturn = 0;

    auto temp = this->scopes->end()[-1];
    this->scopes->pop_back();
    return temp;
}

void execution::forBlock(ASTblock * blk, ASTvariableDecl * v){
    this->scopes->push_back(new scope());
    if(v != nullptr){
        variableDecl(v);
    }

    for( vector<ASTstatement *>::iterator itr = blk->stmt->begin(), itr_end = blk->stmt->end(); itr != itr_end; ++itr){
        parseStatement(*itr, nullptr);
    }
    this->scopes->pop_back();
}


void execution::printStatement(ASTprintStatement * print){
    this->varType = "";
    cout << expression(print->RHS) << endl;
}

void execution::functionDecl(ASTfunctionDecl * dec){
    this->hasReturn = 0;
    string f = "";
    for(auto itr = this->functions->begin(), itr_end = this->functions->end(); itr != itr_end; ++itr){
        if((*itr)->getName() == dec->ident->val){
            f = dec->ident->val;
        }
    }

    if(f != ""){
        throw std::runtime_error("Function already exists");
    }

    auto block = funcBlock(dec->block, dec->param, dec->type);
    this->functions->push_back(new func(dec->ident->val, block, dec->type->val));
}

void execution::formalParams(ASTformalParams * fparams){
    formalParam(fparams->p);
    for(auto itr = fparams->formalParam->begin(), itr_end = fparams->formalParam->end(); itr != itr_end; ++itr){
        formalParam(*itr);
    }
}

void execution::formalParam(ASTformalParam * fparam){
    this->scopes->back()->addVar(fparam->type->val, fparam->ident->val);
}

void execution::rtrnStatement(ASTrtrnStatement * rtrn, ASTtype * t){
    if(this->inIfStatement < 0){
        this->hasReturn += std::numeric_limits<int>::max();;
    }else{
        this->hasReturn += 1;
    }
    this->varType = t->val;
    expression(rtrn->exp);
}

void execution::ifStatement(ASTifStatement * ifS, ASTtype * t){
    this->inIfStatement += 1;
    block(ifS->blk, t);
    if(ifS->_else != nullptr){
        block(ifS->_else, t);
    }
    this->inIfStatement -= 1;
}

void execution::forStatement(ASTforStatement * forS){
    forBlock(forS->b, forS->vDecl);
}

void execution::whileStatement(ASTwhileStatement * whileS){
    block(whileS->b, nullptr);
}

string execution::expression(ASTexpression * exp){
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
            if(this->varType == ""){
                this->varType = v->getVarType();
                return v->getValue();
            }else{
                if(v->getVarType() == this->varType){
                    return v->getValue();
                }else{
                    throw std::runtime_error("Incorrect Type");
                }
            }
            
            
        }else{
            throw std::runtime_error("Cannot find variable");
        }
    }

    ASTfunctionCall* func = dynamic_cast<ASTfunctionCall*>(exp->data);
    if(func != NULL){
        functionCall(func);
    }

    ASTsubExpression* sub = dynamic_cast<ASTsubExpression*>(exp->data);
    if(sub != NULL){
        subExpression(sub);
    }

    ASTunary* u = dynamic_cast<ASTunary*>(exp->data);
    if(u != NULL){
        unary(u);
    }

    ASTliteral* lit = dynamic_cast<ASTliteral*>(exp->data);
    if(lit != NULL){
        return literal(lit);
    }

    ASTmultiplicativeOp* mul = dynamic_cast<ASTmultiplicativeOp*>(exp->data);
    if(mul != NULL){
        return multiplicative(mul);
    }

    ASTadditiveOp* add = dynamic_cast<ASTadditiveOp*>(exp->data);
    if(add != NULL){
        return additive(add);
    }

    ASTrelationalOp* rel = dynamic_cast<ASTrelationalOp*>(exp->data);
    if(rel != NULL){
        return relational(rel);
    }
    return "fail";
}

string execution::expression2(ASTexpression * exp){
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
            if(this->varType == ""){
                this->varType = v->getVarType();
            }

            return v->getValue();
            
        }else{
            throw std::runtime_error("Cannot find variable");
        }
    }

    ASTfunctionCall* func = dynamic_cast<ASTfunctionCall*>(exp);
    if(func != NULL){
        functionCall(func);
    }

    ASTsubExpression* sub = dynamic_cast<ASTsubExpression*>(exp);
    if(sub != NULL){
        return subExpression(sub);
    }

    ASTunary* u = dynamic_cast<ASTunary*>(exp);
    if(u != NULL){
        unary(u);
    }

    ASTliteral* lit = dynamic_cast<ASTliteral*>(exp);
    if(lit != NULL){
        return literal(lit);
    }

    ASTmultiplicativeOp* mul = dynamic_cast<ASTmultiplicativeOp*>(exp);
    if(mul != NULL){
        return multiplicative(mul);
    }

    ASTadditiveOp* add = dynamic_cast<ASTadditiveOp*>(exp);
    if(add != NULL){
        return additive(add);
    }

    ASTrelationalOp* rel = dynamic_cast<ASTrelationalOp*>(exp);
    if(rel != NULL){
        return relational(rel);
    }
    return "fail";
}

string execution::additive(ASTadditiveOp * add){
    auto LHS = expression2(add->LHS);
    auto RHS = expression2(add->RHS);

    if(LHS == "" || RHS == ""){
        throw std::runtime_error("Variable has no value");
    }

    string test = testType(LHS, RHS, this->varType);
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

string execution::testType(string LHS, string RHS, string varT){
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

string execution::multiplicative(ASTmultiplicativeOp * mul){
    string LHS = expression2(mul->LHS);
    string RHS = expression2(mul->RHS);

    if(LHS == "" && RHS == ""){
        return "";
    }



    string test = testType(LHS, RHS, this->varType);
    if(test == "fail"){
        throw std::runtime_error("Incorrect Type");

    }else if(test == "int"){
        if(mul->val == "*"){
            return to_string(stoi(LHS) * stoi(RHS));
        }else if(mul->val == "/"){
            return to_string(stoi(LHS) / stoi(RHS));
        }else{
            throw std::runtime_error("Can not use 'or' operator with ints");
        }

    }else if(test == "float"){
        if(mul->val == "*"){
            return to_string(stof(LHS) * stof(RHS));
        }else if(mul->val == "/"){
            return to_string(stof(LHS) / stof(RHS));
        }else{
            throw std::runtime_error("Can not use 'or' operator with ints");
        }

    }else if(test == "char"){
        throw std::runtime_error("Cannot perform multiplicative operation of objects of type car");

    }else if(test == "bool"){
        int LHSval;
        int RHSval;
        if(RHS == "true") RHSval = 1;
        if(RHS == "false") RHSval = 0;
        if(LHS == "true") RHSval = 1;
        if(LHS == "false") RHSval = 0;

        if(mul->val == "*" || mul->val == "or"){
            if(LHSval + RHSval == 1){
                return "true";
            }else{
                return "false";
            }
    
        }else{
            throw std::runtime_error("Can not use '/' operator with boolean values");
        }
    }
    
    return "fail";
}

string execution::relational(ASTrelationalOp * rel){
    string LHS = expression2(rel->LHS);
    string RHS = expression2(rel->RHS);

    if(rel->val == "<="){
        if(LHS <= RHS){
            return "true";
        }else{
            return "false";
        }

    }else if(rel->val == ">="){
        if(LHS >= RHS){
            return "true";
        }else{
            return "false";
        }

    }else if(rel->val == "=="){
        if(LHS == RHS){
            return "true";
        }else{
            return "false";
        }
        
    }else if(rel->val == "!="){
        if(LHS != RHS){
            return "true";
        }else{
            return "false";
        }
        
    }else if(rel->val == "<"){
        if(LHS < RHS){
            return "true";
        }else{
            return "false";
        }
        
    }else if(rel->val == ">"){
        if(LHS > RHS){
            return "true";
        }else{
            return "false";
        }
    }

    return LHS;
}

string execution::literal(ASTliteral * lit){
    ASTbooleanLiteral* b = dynamic_cast<ASTbooleanLiteral*>(lit);
    
    if(b != NULL){
        if(this->varType == ""){
            this->varType ="bool";
        }
        if(this->varType != "bool"){
            throw std::runtime_error("incorrect type");
        }
        return b->val;
    }

    ASTintLiteral* i = dynamic_cast<ASTintLiteral*>(lit);
    if(i != NULL){
        if(this->varType == ""){
            this->varType ="int";
        }
        if(this->varType != "int"){
            throw std::runtime_error("incorrect type");
        }
        return i->val;
    }

    ASTfloatLiteral* f = dynamic_cast<ASTfloatLiteral*>(lit);
    if(f != NULL){
        if(this->varType == ""){
            this->varType ="float";
        }
        if(this->varType != "float"){
            throw std::runtime_error("incorrect type");
        }
        return f->val;
    }

    ASTcharLiteral* c = dynamic_cast<ASTcharLiteral*>(lit);
    if(c != NULL){
        if(this->varType == ""){
            this->varType ="char";
        }
        if(this->varType != "char"){
            throw std::runtime_error("incorrect type");
        }
        return c->val;
    }
    return "fail";
}

void execution::functionCall(ASTfunctionCall * funcCall){
    func *f = nullptr;
    
    for(auto itr = this->functions->begin(), itr_end = this->functions->end(); itr != itr_end; ++itr){
        if((*itr)->getName() == funcCall->ident->val){
            f = *itr;
        }
    }
    
    if(f == nullptr){
        throw std::runtime_error("Function does not exist");
    }
    if(this->varType != ""){
        if(this->varType != f->getReturn()){
            throw std::runtime_error("Function return type does not match variable assignment");
        }
    }
}

string execution::subExpression(ASTsubExpression * sub){
    return expression(sub->exp);
}

string execution::unary(ASTunary * u){
    return expression(u->data);
}

string execution::actualParams(ASTactualParams * a){
    return expression(a->data);
}



