#include <iostream>

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

// Block
class ASTblock : public ASTstatement{
public:
    ASTblock();
    virtual ~ASTblock();
};

ASTblock::ASTblock(){
    
}

ASTblock::~ASTblock(){
    
}

// Function Declaration
class ASTfunctionDecl : public ASTstatement{
public:
    ASTfunctionDecl();
    virtual ~ASTfunctionDecl();
};

ASTfunctionDecl::ASTfunctionDecl(){
    
}

ASTfunctionDecl::~ASTfunctionDecl(){
    
}


// While Loop
class ASTwhileStatement : public ASTstatement{
public:
    ASTwhileStatement();
    virtual ~ASTwhileStatement();
};

ASTwhileStatement::ASTwhileStatement(){
    
}

ASTwhileStatement::~ASTwhileStatement(){
    
}

// For Loop
class ASTforStatement : public ASTstatement{
public:
    ASTforStatement();
    virtual ~ASTforStatement();
};

ASTforStatement::ASTforStatement(){
    
}

ASTforStatement::~ASTforStatement(){
    
}

// If Statement
class ASTifStatement : public ASTstatement{
public:
    ASTifStatement();
    virtual ~ASTifStatement();
};

ASTifStatement::ASTifStatement(){
    
}

ASTifStatement::~ASTifStatement(){
    
}

// Return Statement
class ASTrtrnStatement : public ASTstatement{
public:
    ASTrtrnStatement();
    virtual ~ASTrtrnStatement();
};

ASTrtrnStatement::ASTrtrnStatement(){
    
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

ASTprintStatement::ASTprintStatement(ASTexpression *RHS){
    this->RHS = RHS;
}

ASTprintStatement::~ASTprintStatement(){
    
}

// Formal Parameters
class ASTformalParams{
public:
    ASTformalParams();
    virtual ~ASTformalParams();
};

ASTformalParams::ASTformalParams(){
    
}

ASTformalParams::~ASTformalParams(){
    
}

// Formal Parameter
class ASTformalParam{
public:
    ASTformalParam();
    virtual ~ASTformalParam();
};

ASTformalParam::ASTformalParam(){
    
}

ASTformalParam::~ASTformalParam(){
    
}

// Variable Declaration
class ASTvariableDecl : public ASTstatement{
public:
    ASTvariableDecl();
    virtual ~ASTvariableDecl();
};

ASTvariableDecl::ASTvariableDecl(){
    
}

ASTvariableDecl::~ASTvariableDecl(){
    
}

// Expression
class ASTexpression : public ASTnode{
public:
    ASTexpression();
    virtual ~ASTexpression();
};

ASTexpression::ASTexpression(){
    
}

ASTexpression::~ASTexpression(){
    
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

// Simple Expresion 
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
    ASTfactor();
    virtual ~ASTfactor();
};

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

// Function Call
class ASTfunctionCall{
public:
    ASTfunctionCall();
    virtual ~ASTfunctionCall();
};

ASTfunctionCall::ASTfunctionCall(){
    
}

ASTfunctionCall::~ASTfunctionCall(){
    
}

// Actual Paramater
class ASTactualParams{
public:
    ASTactualParams();
    virtual ~ASTactualParams();
};

ASTactualParams::ASTactualParams(){
    
}

ASTactualParams::~ASTactualParams(){
    
}

// Relational Operator 
class ASTrelationalOp{
public:
    ASTrelationalOp();
    virtual ~ASTrelationalOp();
};

ASTrelationalOp::ASTrelationalOp(){
    
}

ASTrelationalOp::~ASTrelationalOp(){
    
}

// Additive Operator
class ASTadditiveOp{
public:
    ASTadditiveOp();
    virtual ~ASTadditiveOp();
};

ASTadditiveOp::ASTadditiveOp(){
    
}

ASTadditiveOp::~ASTadditiveOp(){
    
}

// Multiplicative Operator
class ASTmultiplicativeOp{
public:
    ASTmultiplicativeOp();
    virtual ~ASTmultiplicativeOp();
};

ASTmultiplicativeOp::ASTmultiplicativeOp(){
    
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