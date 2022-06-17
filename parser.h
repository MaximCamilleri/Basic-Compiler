#include "scanner.h"
#include "executionPass.h"

using namespace std;

class parser{
private:
    scanner lexer;
    program *p;
    token *nextToken;
    token *nextNextToken;
    int blockDept = 0;
public:
    parser(scanner lexer, string name);
    void parse();
    void getNextToken();
    void getNextNextToken();

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
    ASTexpression * parseFunctionCall();
    ASTactualParams * parseActualParams();
    ASTrelationalOp * parseRelationalOp(ASTexpression * LHS);
    ASTadditiveOp * parseAdditiveOp(ASTexpression * LHS);
    ASTmultiplicativeOp * parseMultiplicativeOp(ASTexpression * LHS);
    ASTexpression * parseLiteral();
    ASTstatement * parseBooleanLiteral();
    ASTidentifier * parseIdent();
    ASTtype * parseType();
};

parser::parser(scanner lexer, string name){
    this->lexer = lexer;
    this->p = new program(name);
    getNextNextToken();
}

void parser::getNextToken(){
    this->nextToken = this->nextNextToken;
    getNextNextToken();
    //cout << "Lexeme: " << this->nextToken->getLexeme() << " Token Type: " << this->nextToken->getTokenType() << endl;
}

void parser::getNextNextToken(){
    bool lexicalError;
    this->nextNextToken = scannerLoop(&(this->lexer), this->p, &lexicalError);
    if(lexicalError == true){
        cout << "Program encountered a lexical error";
        exit(0);
    }else{
        if(this->nextNextToken->getTokenType() == space ||
           this->nextNextToken->getTokenType() == multiLineComment ||
           this->nextNextToken->getTokenType() == singleLineComment ||
           this->nextNextToken->getTokenType() == newline){
            getNextNextToken();
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
    if(exp == nullptr){
        cout << "Expected Expression";
        exit(0);
    }
    auto node = new ASTrtrnStatement(exp);
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
    if(this->nextNextToken->getTokenType() == _else){
        getNextToken();
        ASTblock * elseBlock;
        while(this->nextToken->getTokenType() == _else){
            getNextToken();
            elseBlock = parseBlock();
        }
        //getNextToken();
        return new ASTifStatement(exp, block, elseBlock);
    }else{
        return new ASTifStatement(exp, block);
    }
}

ASTstatement * parser::parseForStatement(){
    bool flag = false;
    auto vDecl = parseVariableDecl();
    ASTforStatement * node = nullptr;
    if(vDecl != nullptr){
        flag = true;
    }

    if(this->nextToken->getTokenType() != endOfExpression){
        cout << "Expected ';'";
        exit(0);
    }

    getNextToken();
    auto exp = parseExpression();
    if(exp == nullptr){
        cout << "Expected Expression";
        exit(0);
    }

    if(this->nextToken->getTokenType() != endOfExpression){
        cout << "Expected ';'";
        exit(0);
    }

    getNextToken();
    auto ass = parseAssignment();
    if(ass == nullptr){
        cout << "Expected Assignment";
        exit(0);
    }

    if(this->nextToken->getTokenType() != closeCircleBracket){
        cout << "Expected ')'";
        exit(0);
    }

    getNextToken();
    auto block = parseBlock();
    if(block == nullptr){
        cout << "Expected Block";
        exit(0);
    }

    if(flag){
        node = new ASTforStatement(vDecl, exp, ass, block);
    }else{
        node = new ASTforStatement(exp, ass, block);
    }
    

    return node;
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
    vector<ASTstatement *> * stmts = new vector<ASTstatement *>();
    ASTblock * block;
    if(this->nextToken->getTokenType() != openCurlyBracket){
        cout << "Expected '{'";
        exit(0);
    }
    getNextToken();

    for(;;){
        auto stmt = parseStatement();
        stmts->push_back(stmt);
        getNextToken();
        if(this->nextToken->getTokenType() == closeCurlyBracket){
            break;
        }
    }
    block = new ASTblock(stmts);
    return block;
}

ASTvariableDecl * parser::parseVariableDecl(){
    getNextToken();
    ASTvariableDecl * node = nullptr;
    if(this->nextToken->getTokenType() == variable){
        ASTidentifier * ident = parseIdent();
        if(ident == nullptr){
            cout << "Expected Identifier" << endl;
            std::exit(0);
        }

        getNextToken();
        if(this->nextToken->getTokenType() != colon){
            cout << "Expected ':'" << endl;
            std::exit(0);
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

        node = new ASTvariableDecl(ident, type, exp);
    }
    
    return node;
}

ASTstatement * parser::parseFunctionDecl(){
    getNextToken();
    bool flag = false;
    auto ident = parseIdent();
    if(ident == nullptr){
        cout << "Expected identifier" << endl;
        exit(0);
    }
    
    getNextToken();
    if(this->nextToken->getTokenType() != openCircleBracket){
        cout << "Expected '('" << endl;
        exit(0);
    }

    getNextToken();
    auto formalp = parseFormalParams();
    if(formalp != nullptr){
        flag = true;
    }

    if(this->nextToken->getTokenType() != closeCircleBracket){
        cout << "Expected ')'" << endl;
        exit(0);
    }
    
    getNextToken();
    if(this->nextToken->getTokenType() != returnFunc){
        cout << "Expected '->'" << endl;
        exit(0);
    }

    getNextToken();
    auto type = parseType();
    if(type == nullptr){
        cout << "Expected type" << endl;
        exit(0);
    }

    getNextToken();
    auto block = parseBlock();
    if(block == nullptr){
        cout << "Expected block" << endl;
        exit(0);
    }

    if(flag == true){
        return new ASTfunctionDecl(ident, formalp, type, block);
    }

    return new ASTfunctionDecl(ident, type, block);
}

ASTformalParams * parser::parseFormalParams(){
    auto formalp = parseFormalParam();
    vector<ASTformalParam *> * formalparams = new vector<ASTformalParam *>();
    if(formalp == nullptr){
        return nullptr;
    }else{
        getNextToken();
        ASTformalParam * formalp2;
        while(this->nextToken->getTokenType() == comma){
            getNextToken();
            formalp2 = parseFormalParam();
            if(formalp2 == nullptr){
                return nullptr;
            }else{
                formalparams->push_back(formalp2);
            }
            getNextToken();
        }

        auto node = new ASTformalParams(formalp, formalparams);
        return node;
    }
    
}

ASTformalParam * parser::parseFormalParam(){
    ASTformalParam * node = nullptr;

    if(this->nextToken->getTokenType() == variable){
        auto ident = parseIdent();
        getNextToken();
        if(this->nextToken->getTokenType() != colon){
            cout << "Expected ':'";
            exit(0);
        }
        getNextToken();
        auto type = parseType();
        node = new ASTformalParam(ident, type);
    }
    
    return node;
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
    while(this->nextToken->getTokenType() == conditions){
        LHS = parseRelationalOp(LHS);
    }

    auto exp = new ASTexpression(LHS);

    return exp;
}

ASTrelationalOp * parser::parseRelationalOp(ASTexpression * LHS){
    ASTrelationalOp * node = nullptr;
    auto token = this->nextToken->getLexeme();
    getNextToken();
    auto temp = parseSimpleExpr();
    if(temp != nullptr){
        node = new ASTrelationalOp(token, LHS, temp);
    }else{
        cout << "Expected Simple Expr" << endl;
    }
    return node;
}

ASTexpression * parser::parseSimpleExpr(){
    auto LHS = parseTerm();
    if(!LHS){
        return nullptr;
    }
    while(this->nextToken->getTokenType() == _plus ||
          this->nextToken->getTokenType() == _minus ||
          this->nextToken->getTokenType() == _or){

        LHS = parseAdditiveOp(LHS);
    }

    return LHS;
}

ASTadditiveOp * parser::parseAdditiveOp(ASTexpression * LHS){
    ASTadditiveOp * node = nullptr;
    auto token = this->nextToken->getLexeme();
    getNextToken();
    auto temp = parseTerm();
    if(temp != nullptr){
        node = new ASTadditiveOp(token, LHS, temp);
    }else{
        cout << "Expected Term" << endl;
    }
    return node;
}

ASTexpression * parser::parseTerm(){
    ASTexpression * LHS = parseFactor();
    if(!LHS){
        return nullptr;
    }
    ASTunary * check = dynamic_cast<ASTunary *>(LHS);
    if(check == NULL){
        getNextToken();
    }
    
    while(this->nextToken->getTokenType() == _divide ||
          this->nextToken->getTokenType() == _multiply ||
          this->nextToken->getTokenType() == _and){

        LHS = parseMultiplicativeOp(LHS);
        getNextToken();
    }

    return LHS;
}

ASTmultiplicativeOp * parser::parseMultiplicativeOp(ASTexpression * LHS){
    ASTmultiplicativeOp * node = nullptr;
    auto token = this->nextToken->getLexeme();

    getNextToken();
    auto temp = parseFactor();
    if(temp != nullptr){
        node = new ASTmultiplicativeOp(token, LHS, temp);
    }else{
        cout << "Expected Factor" << endl;
        exit(0);
    }

    return node;
}

ASTexpression * parser::parseFactor(){
    ASTexpression * node = nullptr;
    string ident;

    switch(this->nextToken->getTokenType()){
    case variable:
        if(this->nextNextToken->getTokenType() == openCircleBracket){
            node = parseFunctionCall();
            break;
        }else{
            ident = this->nextToken->getLexeme(); 
            node = new ASTidentifier(ident);
            break;
        }
        
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
        break;
    }
    return node;
}

ASTexpression * parser::parseFunctionCall(){
    auto ident = parseIdent();
    bool flag = false;
    getNextToken();
    if(this->nextToken->getTokenType() != openCircleBracket){
        cout << "Expected '('" << endl;
        exit(0);
    }
    getNextToken();

    auto Aparam = parseActualParams();
    if(Aparam != nullptr){
        flag = true;
    }

    if(this->nextToken->getTokenType() != closeCircleBracket){
        cout << "Expected ')'" << endl;
        exit(0);
    }

    if(flag){
        return new ASTfunctionCall(ident, Aparam);
    }
    return new ASTfunctionCall(ident);
}

ASTactualParams * parser::parseActualParams(){
    auto exp = parseExpression();
    vector<ASTexpression * > * exps = new vector<ASTexpression *>();
    if(exp == nullptr){
        return nullptr;
    }

    while(this->nextToken->getTokenType() == comma){
        getNextToken();
        auto exp2 = parseExpression();
        if(exp2 == nullptr){
            return nullptr;
        }else{
            exps->push_back(exp2);
        }
    }

    return new ASTactualParams(exp, exps);
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
            if(stmt != nullptr){
                statements->push_back(stmt);
                if(this->nextToken->getTokenType() == endOfExpression){
                    getNextToken();
                }
            }else{
                //cout << "rip" << endl;
                getNextToken();
            }
            
            break;
        }
    }
    std::cout<< "Finished Parsing" << endl;

    // XML pass
    xml xmlParse(statements);
    cout << endl;
    xmlParse.beginXML();

    // Semantic Analysis Pass
    semanticAnalysis saParse(statements);
    saParse.beginSemanticAnalysis();

    execution ex(statements);
    ex.beginExecution();
}

