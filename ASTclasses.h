#include <iostream>

class ASTnode{
public:
    ASTnode();
    virtual ~ASTnode();
    void initialise();
};


class ASTstatement : public ASTnode{
public:
    ASTstatement();
    virtual ~ASTstatement();
};

class ASTblock : public ASTstatement{
public:
    ASTblock();
    virtual ~ASTblock();
};

class ASTfunctionDecl : public ASTstatement{
public:
    ASTfunctionDecl();
    virtual ~ASTfunctionDecl();
};

class ASTwhileStatement : public ASTstatement{
public:
    ASTwhileStatement();
    virtual ~ASTwhileStatement();
};

class ASTforStatement : public ASTstatement{
public:
    ASTforStatement();
    virtual ~ASTforStatement();
};

class ASTifStatement : public ASTstatement{
public:
    ASTifStatement();
    virtual ~ASTifStatement();
};

class ASTrtrnStatement : public ASTstatement{
public:
    ASTrtrnStatement();
    virtual ~ASTrtrnStatement();
};

class ASTprintStatement : public ASTstatement{
public:
    ASTprintStatement();
    virtual ~ASTprintStatement();
};

class ASTformalParams{
public:
    ASTformalParams();
    virtual ~ASTformalParams();
};

class ASTformalParam{
public:
    ASTformalParam();
    virtual ~ASTformalParam();
};

class ASTvariableDecl{
public:
    ASTvariableDecl();
    virtual ~ASTvariableDecl();
};

class ASTassignment{
public:
    ASTassignment();
    virtual ~ASTassignment();
};

class ASTexpression{
public:
    ASTexpression();
    virtual ~ASTexpression();
};

class ASTsimpleExpr{
public:
    ASTsimpleExpr();
    virtual ~ASTsimpleExpr();
};

class ASTterm{
public:
    ASTterm();
    virtual ~ASTterm();
};

class ASTfactor{
public:
    ASTfactor();
    virtual ~ASTfactor();
};

class ASTunary{
public:
    ASTunary();
    virtual ~ASTunary();
};

class ASTsubExpression{
public:
    ASTsubExpression();
    virtual ~ASTsubExpression();
};

class ASTfunctionCall{
public:
    ASTfunctionCall();
    virtual ~ASTfunctionCall();
};

class ASTactualParams{
public:
    ASTactualParams();
    virtual ~ASTactualParams();
};

class ASTrelationalOp{
public:
    ASTrelationalOp();
    virtual ~ASTrelationalOp();
};

class ASTadditiveOp{
public:
    ASTadditiveOp();
    virtual ~ASTadditiveOp();
};

class ASTmultiplicativeOp{
public:
    ASTmultiplicativeOp();
    virtual ~ASTmultiplicativeOp();
};

class ASTliteral{
public:
    ASTliteral();
    virtual ~ASTliteral();
};

class ASTbooleanLiteral{
public:
    ASTbooleanLiteral();
    virtual ~ASTbooleanLiteral();
};