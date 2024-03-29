#include <iostream>
#include <vector>

using namespace std;

class ASTsimpleExpr;
class ASTrelationalOp;
class ASTterm;
class ASTadditiveOp;
class ASTmultiplicativeOp;
class ASTfactor;

// Node
class ASTnode{
public:
    ASTnode();
    virtual ~ASTnode();
    void initialise();
};

ASTnode::ASTnode(){
    
}

ASTnode::~ASTnode(){
    
}

// Statement
class ASTstatement : public ASTnode{
public:
    ASTstatement();
    virtual ~ASTstatement();
};

ASTstatement::ASTstatement(){
    
}

ASTstatement::~ASTstatement(){
    
}

// Expression
class ASTexpression : public ASTnode{
public:
    ASTexpression(ASTexpression * data);
    ASTexpression();
    virtual ~ASTexpression();
    ASTexpression * data;

};

ASTexpression::ASTexpression(ASTexpression * data){
    this->data = data;
}

ASTexpression::ASTexpression(){

}

ASTexpression::~ASTexpression(){
    
}

// Identifier
class ASTidentifier : public ASTexpression{
public:
    ASTidentifier(string val);
    virtual ~ASTidentifier();

    string val;
};

ASTidentifier::ASTidentifier(string val){
    this->val = val;
}

ASTidentifier::~ASTidentifier(){
    
}

// Block
class ASTblock : public ASTstatement{
public:
    ASTblock(vector<ASTstatement *> * stmt);
    virtual ~ASTblock();

    vector<ASTstatement *> * stmt;
};

ASTblock::ASTblock(vector<ASTstatement *> * stmt){
    this->stmt = stmt;
}

ASTblock::~ASTblock(){
    
}

// Type Declaration:
class ASTtype : public ASTstatement{
public:
    ASTtype(string val);
    virtual ~ASTtype();

    string val;
};

ASTtype::ASTtype(string val){
    this->val = val;
}

ASTtype::~ASTtype(){
    
}

// Variable Declaration
class ASTvariableDecl : public ASTstatement{
public:
    ASTvariableDecl(ASTidentifier *ident, ASTtype *type, ASTexpression *exp);
    virtual ~ASTvariableDecl();

    ASTidentifier * ident;
    ASTtype *type;
    ASTexpression *exp;
};

ASTvariableDecl::ASTvariableDecl(ASTidentifier *ident, ASTtype *type, ASTexpression *exp){
    this->ident = ident;
    this->type = type;
    this->exp = exp;
}

ASTvariableDecl::~ASTvariableDecl(){
    
}

// Assignment
class ASTassignment : public ASTstatement{
public:
    ASTassignment(const string name, ASTexpression *RHS); 
    virtual ~ASTassignment();
    
    ASTexpression *RHS;
    string LHS;
    
};

ASTassignment::ASTassignment(const string name, ASTexpression *pRHS){ 
    this->LHS = name;
    this->RHS = pRHS;
}

ASTassignment::~ASTassignment(){
    
}

// Formal Parameter
class ASTformalParam{
public:
    ASTformalParam(ASTidentifier * ident, ASTtype * type);
    virtual ~ASTformalParam();

    ASTidentifier * ident;
    ASTtype * type;
};

ASTformalParam::ASTformalParam(ASTidentifier * ident, ASTtype * type){
    this->ident = ident;
    this->type = type;
}

ASTformalParam::~ASTformalParam(){
    
}

// Formal Parameters
class ASTformalParams{
public:
    ASTformalParams(ASTformalParam * p, vector<ASTformalParam *> * formalParam);
    virtual ~ASTformalParams();

    ASTformalParam * p;
    vector<ASTformalParam *> * formalParam;
};

ASTformalParams::ASTformalParams(ASTformalParam * p, vector<ASTformalParam *> * formalParam){
    this->p = p;
    this->formalParam = formalParam;
}

ASTformalParams::~ASTformalParams(){
    
}

// Function Declaration
class ASTfunctionDecl : public ASTstatement{
public:
    ASTfunctionDecl(ASTidentifier * ident, ASTformalParams * param, ASTtype * type, ASTblock * block);
    ASTfunctionDecl(ASTidentifier * ident, ASTtype * type, ASTblock * block);
    virtual ~ASTfunctionDecl();

    ASTidentifier * ident = nullptr;
    ASTformalParams * param = nullptr;
    ASTtype * type = nullptr;
    ASTblock * block = nullptr;
};

ASTfunctionDecl::ASTfunctionDecl(ASTidentifier * ident, ASTformalParams * param, ASTtype * type, ASTblock * block){
    this->ident = ident;
    this->param = param;
    this->type = type;
    this->block = block;
}

ASTfunctionDecl::ASTfunctionDecl(ASTidentifier * ident, ASTtype * type, ASTblock * block){
    this->ident = ident;
    this->type = type;
    this->block = block;
    this->param = nullptr;
}

ASTfunctionDecl::~ASTfunctionDecl(){
    
}

// While Loop
class ASTwhileStatement : public ASTstatement{
public:
    ASTwhileStatement(ASTexpression * exp, ASTblock *b);
    virtual ~ASTwhileStatement();

    ASTexpression * exp;
    ASTblock *b;
};

ASTwhileStatement::ASTwhileStatement(ASTexpression * exp, ASTblock *b){
    this->exp = exp;
    this->b = b;
}

ASTwhileStatement::~ASTwhileStatement(){
    
}

// For Loop
class ASTforStatement : public ASTstatement{
public:
    ASTforStatement(ASTvariableDecl *vDecl, ASTexpression *exp, ASTassignment *ass, ASTblock *b); 
    ASTforStatement(ASTexpression *exp, ASTassignment *ass, ASTblock *b); 
    virtual ~ASTforStatement();

    ASTvariableDecl *vDecl;
    ASTexpression *exp;
    ASTassignment *ass;
    ASTblock *b;
};

ASTforStatement::ASTforStatement(ASTvariableDecl *vDecl, ASTexpression *exp, ASTassignment *ass, ASTblock *b){
    this->vDecl = vDecl;
    this->exp = exp;
    this->ass = ass;
    this->b = b;
}

ASTforStatement::ASTforStatement(ASTexpression *exp, ASTassignment *ass, ASTblock *b){
    this->exp = exp;
    this->ass = ass;
    this->b = b;
    this->vDecl = nullptr;
}

ASTforStatement::~ASTforStatement(){
    
}

// If Statement
class ASTifStatement : public ASTstatement{
public:
    ASTifStatement(ASTexpression * exp, ASTblock * blk, ASTblock * _else);
    ASTifStatement(ASTexpression * exp, ASTblock * blk);
    virtual ~ASTifStatement();

    ASTexpression * exp;
    ASTblock * blk;
    ASTblock * _else;
};

ASTifStatement::ASTifStatement(ASTexpression * exp, ASTblock * blk, ASTblock * _else){
    this->blk = blk;
    this->exp = exp;
    this->_else = _else;
}

ASTifStatement::ASTifStatement(ASTexpression * exp, ASTblock * blk){
    this->blk = blk;
    this->exp = exp;
    this->_else = nullptr;
}

ASTifStatement::~ASTifStatement(){
    
}

// Return Statement
class ASTrtrnStatement : public ASTstatement{
public:
    ASTrtrnStatement(ASTexpression *exp);
    virtual ~ASTrtrnStatement();

    const string r = "return";
    ASTexpression *exp;
};

ASTrtrnStatement::ASTrtrnStatement(ASTexpression *exp){
    this->exp = exp;
}

ASTrtrnStatement::~ASTrtrnStatement(){
    
}

// Print
class ASTprintStatement : public ASTstatement{
public:
    ASTprintStatement(ASTexpression *RHS);
    virtual ~ASTprintStatement();

    const string val = "print";
    ASTexpression *RHS;
};

ASTprintStatement::ASTprintStatement(ASTexpression *pRHS){
    this->RHS = pRHS;
}

ASTprintStatement::~ASTprintStatement(){
    
}

// Simple Expresion
class ASTsimpleExpr : public ASTexpression{
public:
    ASTsimpleExpr();
    virtual ~ASTsimpleExpr();
};

ASTsimpleExpr::ASTsimpleExpr(){

}

ASTsimpleExpr::~ASTsimpleExpr(){
    
}

// Term
class ASTterm : public ASTsimpleExpr{
public:
    ASTterm();
    virtual ~ASTterm();
};

ASTterm::ASTterm(){

}

ASTterm::~ASTterm(){
    
}

// Factor
class ASTfactor : public ASTterm{
public:
    ASTfactor(ASTfactor * fac);
    ASTfactor();
    virtual ~ASTfactor();

    ASTfactor * fac;
};

ASTfactor::ASTfactor(ASTfactor * fac){
    this->fac = fac;
}

ASTfactor::ASTfactor(){

}

ASTfactor::~ASTfactor(){
    
}

// Unary
class ASTunary : public ASTfactor{
public:
    ASTunary(string LHS, ASTexpression *pRHS);
    virtual ~ASTunary();

    string LHS;
    ASTexpression *RHS;
};

ASTunary::ASTunary(string LHS, ASTexpression *pRHS){
    this->LHS = LHS;
    this->RHS = pRHS;
}

ASTunary::~ASTunary(){

}

// Sub Expression
class ASTsubExpression : public ASTexpression{
public:
    ASTsubExpression(ASTexpression *exp);
    virtual ~ASTsubExpression();

    ASTexpression *exp;
};

ASTsubExpression::ASTsubExpression(ASTexpression *exp){
    this->exp = exp;
}

ASTsubExpression::~ASTsubExpression(){
    
}

// Actual Paramater
class ASTactualParams : public ASTexpression{
public:
    ASTactualParams(ASTexpression * exp, vector<ASTexpression *> * exps);
    virtual ~ASTactualParams();

    ASTexpression * exp;
    vector<ASTexpression *> * exps;
};

ASTactualParams::ASTactualParams(ASTexpression * exp, vector<ASTexpression *> * exps){
    this->exp = exp;
    this->exps = exps;
}

ASTactualParams::~ASTactualParams(){
    
}

// Function Call
class ASTfunctionCall : public ASTexpression{
public:
    ASTfunctionCall(ASTidentifier * ident, ASTactualParams * a);
    ASTfunctionCall(ASTidentifier * ident);
    virtual ~ASTfunctionCall();

    ASTidentifier * ident;
    ASTactualParams * a;
};

ASTfunctionCall::ASTfunctionCall(ASTidentifier * ident, ASTactualParams * a){
    this->ident = ident;
    this->a = a;
}

ASTfunctionCall::ASTfunctionCall(ASTidentifier * ident){
    this->ident = ident;
}

ASTfunctionCall::~ASTfunctionCall(){
    
}

// Relational Operator 
class ASTrelationalOp : public ASTexpression{
public:
    ASTrelationalOp(string val, ASTexpression * LHS, ASTexpression * RHS);
    virtual ~ASTrelationalOp();

    string val;
    ASTexpression * LHS;
    ASTexpression * RHS;
};

ASTrelationalOp::ASTrelationalOp(string val, ASTexpression * LHS, ASTexpression * RHS){
    this->val = val;
    this->LHS = LHS;
    this->RHS = RHS;
}

ASTrelationalOp::~ASTrelationalOp(){
    
}

// Additive Operator
class ASTadditiveOp : public ASTsimpleExpr{
public:
    ASTadditiveOp(string val, ASTexpression * LHS, ASTexpression * RHS);
    virtual ~ASTadditiveOp();

    string val;
    ASTexpression * LHS;
    ASTexpression * RHS;
};

ASTadditiveOp::ASTadditiveOp(string val, ASTexpression * LHS, ASTexpression * RHS){
    this->val = val;
    this->LHS = LHS;
    this->RHS = RHS;
}

ASTadditiveOp::~ASTadditiveOp(){
    
}

// Multiplicative Operator
class ASTmultiplicativeOp : public ASTterm{
public:
    ASTmultiplicativeOp(string val, ASTexpression * LHS, ASTexpression * RHS);
    virtual ~ASTmultiplicativeOp();

    string val;
    ASTexpression * LHS;
    ASTexpression * RHS;
};

ASTmultiplicativeOp::ASTmultiplicativeOp(string val, ASTexpression * LHS, ASTexpression * RHS){
    this->val = val;
    this->LHS = LHS;
    this->RHS = RHS;
}

ASTmultiplicativeOp::~ASTmultiplicativeOp(){
    
}

// Literal
class ASTliteral : public ASTfactor{
public:
    ASTliteral();
    virtual ~ASTliteral();
};

ASTliteral::ASTliteral(){
    
}

ASTliteral::~ASTliteral(){
    
}

// Boolean Literal
class ASTbooleanLiteral : public ASTliteral{
public:
    ASTbooleanLiteral(string val);
    virtual ~ASTbooleanLiteral();

    string val;
};

ASTbooleanLiteral::ASTbooleanLiteral(string val){
    this->val = val;
}

ASTbooleanLiteral::~ASTbooleanLiteral(){
    
}

// Float Literal
class ASTfloatLiteral : public ASTliteral{
public:
    ASTfloatLiteral(string val);
    virtual ~ASTfloatLiteral();

    string val;
};

ASTfloatLiteral::ASTfloatLiteral(string val){
    this->val = val;
}

ASTfloatLiteral::~ASTfloatLiteral(){
    
}

// Int Literal
class ASTintLiteral : public ASTliteral{
public:
    ASTintLiteral(string val);
    virtual ~ASTintLiteral();

    string val;
};

ASTintLiteral::ASTintLiteral(string val){
    this->val = val;
}

ASTintLiteral::~ASTintLiteral(){
    
}

// Char Literal
class ASTcharLiteral : public ASTliteral{
public:
    ASTcharLiteral(string val);
    virtual ~ASTcharLiteral();

    string val;
};

ASTcharLiteral::ASTcharLiteral(string val){
    this->val = val;
}

ASTcharLiteral::~ASTcharLiteral(){
    
}

