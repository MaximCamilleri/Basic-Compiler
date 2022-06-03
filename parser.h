#include "scanner.h"
#include "ASTclasses.h"

class parser{
private:
    scanner lexer;
    program *p;
    token *nextToken;
public:
    parser(scanner lexer, string name);
    void parse();
    void getNextToken();

    //parsing algorithms
    ASTstatement * parseStatement();
    ASTblock * parseBlock();
    ASTstatement * parseFunctionDecl();
    ASTformalParams * parseFormalParams();
    ASTformalParam * parseFormalParam();
    ASTstatement * parseWhileStatement();
    ASTstatement * parseForStatement();
    ASTstatement * parseIfStatement();
    ASTstatement * parseRtrnStatement();
    ASTstatement * parsePrintStatement();
    ASTvariableDecl * parseVariableDecl();
    ASTassignment * parseAssignment();
    ASTexpression * parseExpression();
    ASTexpression * parseSimpleExpr();
    ASTexpression * parseTerm();
    ASTexpression * parseFactor();
    ASTexpression * parseUnary();
    ASTexpression * parseSubExpression();
    ASTstatement * parseFunctionCall();
    ASTstatement * parseActualParams();
    ASTexpression * parseRelationalOp();
    ASTexpression * parseAdditiveOp();
    ASTexpression * parseMultiplicativeOp();
    ASTexpression * parseLiteral();
    ASTstatement * parseBooleanLiteral();
    ASTidentifier * parseIdent();
    ASTtype * parseType();
};

parser::parser(scanner lexer, string name){
    this->lexer = lexer;
    this->p = new program(name);

}

void parser::getNextToken(){
    bool lexicalError;
    this->nextToken = scannerLoop(&(this->lexer), this->p, &lexicalError);
    if(lexicalError == true){
        cout << "Program encountered a lexical error";
        exit(0);
    }else{
        if(this->nextToken->getTokenType() == space){
            getNextToken();
        }else{
            cout << "Lexeme: " << this->nextToken->getLexeme() << " Token Type: " << this->nextToken->getTokenType() << endl;
        }
        
    }

}

ASTstatement * parser::parseStatement(){
    ASTstatement * node = nullptr;

    switch (this->nextToken->getTokenType()){
    case _let:
        node = parseVariableDecl();
        if(this->nextToken->getTokenType() != endOfExpression){
            cout << "Expected Semicolon" << endl;
            exit(0);
        }
        break;

    case variable:
        node = parseAssignment();
        if(this->nextToken->getTokenType() != endOfExpression){
            cout << "Expected Semicolon" << endl;
            exit(0);
        }

        break;

    case _print:
        node = parsePrintStatement();
        getNextToken();
        if(this->nextToken->getTokenType() != endOfExpression){
            cout << "Expected Semicolon" << endl;
            exit(0);
        }
        break;

    case _if:
        getNextToken();
        if(this->nextToken->getTokenType() != openCircleBracket){
            cout << "Expected '('";
            exit(0);
        }
        node = parseIfStatement();
        break;

    case _for:
        getNextToken();
        if(this->nextToken->getTokenType() != openCircleBracket){
            cout << "Expected '('";
            exit(0);
        }
        node = parseForStatement();
        break;

    case _while:
        getNextToken();
        if(this->nextToken->getTokenType() != openCircleBracket){
            cout << "Expected '('";
            exit(0);
        }
        node = parseWhileStatement();
        break;

    case _return:
        node = parseRtrnStatement();
        getNextToken();
        if(this->nextToken->getTokenType() != endOfExpression){
            cout << "Expected Semicolon" << endl;
            exit(0);
        }
        break;

    case _fn:
        node = parseFunctionDecl();
        break;

    case openCurlyBracket:
        node = parseBlock();
        break;

    default:
        break;
    }

    return node;
}

ASTstatement * parser::parsePrintStatement(){
    getNextToken();
    auto RHS = parseExpression();
    if(RHS == nullptr){
        cout << "Expected Expression";
        exit(0);
    }
    auto node = new ASTprintStatement(RHS);
    
    return node;

}

ASTstatement * parser::parseRtrnStatement(){
    getNextToken();
    auto exp = parseExpression();
    if(exp = nullptr){
        cout << "Expected Expression";
        exit(0);
    }
    auto node =new ASTrtrnStatement(exp);
    return node;
}

ASTstatement * parser::parseIfStatement(){
    getNextToken();
    auto exp = parseExpression();
    if(this->nextToken->getTokenType() != closeCircleBracket){
        cout << "Expected ')'";
        exit(0);
    } 

    getNextToken();
    auto block = parseBlock();
    
    getNextToken();
    if(this->nextToken->getTokenType() == _else){
        ASTblock * elseBlock;
        while(this->nextToken->getTokenType() == _else){
            getNextToken();
            elseBlock = parseBlock();
            getNextToken();
        }
        return new ASTifStatement(exp, block, elseBlock);
    }else{
        return new ASTifStatement(exp, block);
    }
}

ASTstatement * parser::parseForStatement(){
    
    auto vDecl = parseVariableDecl();
    if(vDecl = nullptr){
        cout << "Expected Variable Declaration";
        exit(0);
    }

    getNextToken();
    auto exp = parseExpression();
    if(vDecl = nullptr){
        cout << "Expected Expression";
        exit(0);
    }

    getNextToken();
    auto ass = parseAssignment();
    if(vDecl = nullptr){
        cout << "Expected Assignment";
        exit(0);
    }

    getNextToken();
    auto block = parseBlock();
    if(vDecl = nullptr){
        cout << "Expected Block";
        exit(0);
    }

    auto forS = new ASTforStatement(vDecl, exp, ass, block);

    return forS;
}

ASTstatement * parser::parseWhileStatement(){
    getNextToken();
    auto exp = parseExpression();
    if(this->nextToken->getTokenType() != closeCircleBracket){
        cout << "Expected ')'";
        exit(0);
    }
    getNextToken();
    auto b = parseBlock();

    return new ASTwhileStatement(exp, b);
}

ASTblock * parser::parseBlock(){
    if(this->nextToken->getTokenType() != openCurlyBracket){
        cout << "Expected '{'";
        exit(0);
    }
    getNextToken();

    auto stmt = parseStatement();
    ASTblock * block = new ASTblock(stmt);

    getNextToken();
    while(stmt != nullptr){
        auto stmt = parseStatement();
        getNextToken();
    }

    return block;
}

ASTvariableDecl * parser::parseVariableDecl(){
    getNextToken();
    ASTidentifier * ident = parseIdent();
    if(ident == nullptr){
        cout << "Expected Identifier" << endl;
        exit(0);
    }

    getNextToken();
    if(this->nextToken->getTokenType() != colon){
        cout << "Expected ':'" << endl;
        exit(0);
    }

    getNextToken();
    auto type = parseType();
    if(type == nullptr){
        cout << "Expected Type" << endl;
        exit(0);
    }

    getNextToken();
    if(this->nextToken->getTokenType() != equals){
        cout << "Expected '='" << endl;
        exit(0);
    }

    getNextToken();
    auto exp = parseExpression();
    if(type == nullptr){
        cout << "Expected Expression" << endl;
        exit(0);
    }

    auto node = new ASTvariableDecl(ident, type, exp);
    return node;
}

ASTstatement * parser::parseFunctionDecl(){
    getNextToken();
    bool flag = false;
    auto ident = parseIdent();
    if(this->nextToken->getTokenType() != openCircleBracket){
        cout << "Expected '('" << endl;
        exit(0);
    }
    getNextToken();
    auto formalp = parseFormalParams();
    if(formalp != nullptr){
        flag = true;
        getNextToken();
    }
    
    if(this->nextToken->getTokenType() != returnFunc){
        cout << "Expected '->'" << endl;
        exit(0);
    }

    getNextToken();
    auto type = parseType();

    getNextToken();
    auto block = parseBlock();

    if(flag = true){
        return new ASTfunctionDecl(ident, formalp, type, block);
    }

    return new ASTfunctionDecl(ident, type, block);
}

ASTformalParams * parser::parseFormalParams(){
    auto formalp = parseFormalParam();
    getNextToken();
    while(this->nextToken->getTokenType() != comma){
        getNextToken();
        formalp = parseFormalParam();
        getNextToken();
    }

    auto node = new ASTformalParams(formalp);
    return node;
}

ASTformalParam * parser::parseFormalParam(){
    auto ident = parseIdent();
    auto type = parseType();

    return new ASTformalParam(ident, type);
}

ASTidentifier * parser::parseIdent(){
    ASTidentifier * ident = nullptr;
    if(this->nextToken->getTokenType() == variable){
        ident = new ASTidentifier(this->nextToken->getLexeme());
    }
    return ident;
}

ASTtype * parser::parseType(){
    ASTtype * term = nullptr;
    if(this->nextToken->getTokenType() == _int ||
       this->nextToken->getTokenType() == _float ||
       this->nextToken->getTokenType() == _bool ||
       this->nextToken->getTokenType() == _char){

        term = new ASTtype(this->nextToken->getLexeme());
    }
    return term;
}

ASTassignment * parser::parseAssignment(){
    string var_name = this->nextToken->getLexeme();
    getNextToken();
    if(this->nextToken->getTokenType() != equals){
        cout << "Expected '='" << endl;
        exit(0);
        return nullptr;
    }
    getNextToken();
    auto expr_node = parseExpression();
    if(expr_node == nullptr){
        cout << "Expected Expression";
        exit(0);
    }
    auto ass_node = new ASTassignment(var_name, expr_node);

    return ass_node;

}

ASTexpression * parser::parseExpression(){
    auto LHS = parseSimpleExpr();
    if(!LHS){
        return nullptr;
    }

    auto temp = parseRelationalOp();

    while(temp != nullptr){
        getNextToken();
        auto temp2 = parseSimpleExpr();
        if(temp2 == nullptr){
            cout << "Expected Simple Expression";
            exit(0);
        }
        temp = parseRelationalOp();
    }

    return LHS;
}

ASTexpression * parser::parseRelationalOp(){
    ASTexpression * node = nullptr;
    if(this->nextToken->getTokenType() == conditions){
        node = new ASTrelationalOp(this->nextToken->getLexeme());
    }
    return node;
}

ASTexpression * parser::parseSimpleExpr(){
    auto LHS = parseTerm();
    if(!LHS){
        return nullptr;
    }

    auto temp = parseAdditiveOp();

    while(temp != nullptr){
        getNextToken();
        auto temp2 = parseTerm();
        if(temp2 == nullptr){
            cout << "Expected Term";
            exit(0);
        }
        temp = parseAdditiveOp();
    }

    return LHS;
}

ASTexpression * parser::parseAdditiveOp(){
    ASTexpression * node = nullptr;
    if(this->nextToken->getTokenType() == _plus ||
       this->nextToken->getTokenType() == _minus ||
       this->nextToken->getTokenType() == _or){
           node = new ASTadditiveOp(this->nextToken->getLexeme());
    }

    return node;
}

ASTexpression * parser::parseTerm(){
    vector<ASTexpression *> *term = new vector<ASTexpression *>();
    auto LHS = parseFactor();
    if(!LHS){
        return nullptr;
    }else{
        term->push_back(LHS);
    }

    getNextToken();
    auto temp = parseMultiplicativeOp();

    while(temp != nullptr){
        getNextToken();
        auto temp2 = parseFactor();
        if(temp2 == nullptr){
            cout << "Expected Factor";
            exit(0);
        }else{
            term->push_back(temp);
            term->push_back(temp2);
        }
        getNextToken();
        temp = parseMultiplicativeOp();
    }
    return LHS;
}

ASTexpression * parser::parseMultiplicativeOp(){
    ASTexpression * node = nullptr;
    switch (this->nextToken->getTokenType())
    {
    case _divide:
        node = new ASTmultiplicativeOp(this->nextToken->getLexeme());
        break;
    case _multiply:
        node = new ASTmultiplicativeOp(this->nextToken->getLexeme());
        break;
    case _and:
        node = new ASTmultiplicativeOp(this->nextToken->getLexeme());
        break;

    default:
        break;
    }
    return node;
}

ASTexpression * parser::parseFactor(){
    ASTexpression * node = nullptr;
    string ident;

    switch(this->nextToken->getTokenType()){
    case variable:
        ident = this->nextToken->getLexeme(); 
        node = new ASTidentifier(ident);
        break;
    case openCircleBracket:
        node = parseSubExpression();
        break;
    case _minus:
        node = parseUnary();
        break;
    case _not:
        node = parseUnary();
        break;
    case intVal:
        node = parseLiteral();
        break;
    case floatVal:
        node = parseLiteral();
        break;
    case charVal:
        node = parseLiteral();
        break;
    case _true:
        node = parseLiteral();
        break;
    case _false:
        node = parseLiteral();
        break;
    default:
        cout << "Expected Factor" << endl;
        exit(0);
        break;
    }
    return node;
}

ASTexpression * parser::parseLiteral(){
    ASTexpression * node = nullptr;
    string val = this->nextToken->getLexeme();

    switch(this->nextToken->getTokenType()){
        case intVal:
            node = new ASTintLiteral(val);
            break;
        case floatVal:
            node = new ASTfloatLiteral(val);
            break;
        case charVal:
            node = new ASTcharLiteral(val);
            break;
        case _true:
            node = new ASTbooleanLiteral(val);
            break;
        case _false:
            node = new ASTbooleanLiteral(val);
            break;
        default:
            cout << "Expected Literal" << endl;
            exit(0);
            break;
    }
    return node;
}

ASTexpression * parser::parseUnary(){
    string LHS = this->nextToken->getLexeme();
    getNextToken();
    auto RHS = parseExpression();

    return new ASTunary(LHS, RHS);

}

ASTexpression * parser::parseSubExpression(){
    getNextToken();
    auto exp = parseExpression();
    return new ASTsubExpression(exp);
}


void parser::parse(){
    cout << "starting Parser" << endl;
    vector<ASTstatement *> * statements = new vector<ASTstatement *>();
    getNextToken();

    while(this->nextToken->getTokenType() != endOfFile){
        switch (this->nextToken->getTokenType()){
        case endOfFile:
            break;
        
        default:
            auto stmt = parseStatement();
            if(!stmt){
                getNextToken();
            }else{
                if(this->nextToken->getTokenType() ==  endOfExpression){
                    statements->push_back(stmt);
                    getNextToken();
                }
            }
            break;
        }
    }
    std::cout<< "Finished Parsing" << endl;
}

