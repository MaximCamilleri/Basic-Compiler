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
    ASTstatement * parseBlock();
    ASTstatement * parseFunctionDecl();
    ASTstatement * parseFormalParams();
    ASTstatement * parseFormalParam();
    ASTstatement * parseWhileStatement();
    ASTstatement * parseForStatement();
    ASTstatement * parseIfStatement();
    ASTstatement * parseRtrnStatement();
    ASTstatement * parsePrintStatement();
    ASTstatement * parseVariableDecl();
    ASTstatement * parseAssignment();
    ASTexpression * parseExpression();
    ASTexpression * parseSimpleExpr();
    ASTexpression * parseTerm();
    ASTexpression * parseFactor();
    ASTexpression * parseUnary();
    ASTexpression * parseSubExpression();
    ASTstatement * parseFunctionCall();
    ASTstatement * parseActualParams();
    ASTstatement * parseRelationalOp();
    ASTstatement * parseAdditiveOp();
    ASTstatement * parseMultiplicativeOp();
    ASTexpression * parseLiteral();
    ASTstatement * parseBooleanLiteral();
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
        //node = parseVariableDecl();
        break;

    case variable:
        parseAssignment();
        getNextToken();
        if(this->nextToken->getTokenType() != endOfExpression){
            cout << "Expected Semicolon" << endl;
            exit(0);
        }
        getNextToken();

        break;

    case _print:
        parsePrintStatement();
        break;

    case _if:
        parseIfStatement();
        break;

    case _for:
        //parseForStatement();
        break;

    case _while:
        //parseWhileStatement();
        break;

    case _return:
        //parseRtrnStatement();
        break;

    case _fn:
        //parseFunctionDecl();
        break;

    case openCurlyBracket:
        //parseBlock();
        break;

    default:
        break;
    }

    return node;
}

ASTstatement * parser::parsePrintStatement(){
    getNextToken();
    auto RHS = new ASTexpression();
    auto node = new ASTprintStatement(RHS);
    
    return node;

}

// ASTstatement * parser::parseRtrnStatement(){
    
// }

ASTstatement * parser::parseIfStatement(){
    auto node = new ASTifStatement();
    getNextToken();
    return node;
}

// ASTstatement * parser::parseForStatement(){
    
// }

// ASTstatement * parser::parseWhileStatement(){
    
// }

// ASTstatement * parser::parseBlock(){

// }

// ASTstatement * parser::parseVariableDecl(){

// }

ASTstatement * parser::parseAssignment(){
    string var_name = this->nextToken->getLexeme();
    getNextToken();
    if(this->nextToken->getTokenType() != equals){
        cout << "Expected '='" << endl;
        exit(0);
        return nullptr;
    }
    getNextToken();
    auto expr_node = parseExpression();
    auto ass_node = new ASTassignment(var_name, expr_node);

    return ass_node;

}

ASTexpression * parser::parseExpression(){
    auto LHS = parseSimpleExpr();
    if(!LHS){
        return nullptr;
    }
    return LHS;
    
}

ASTexpression * parser::parseSimpleExpr(){
    auto LHS = parseTerm();
    if(!LHS){
        return nullptr;
    }
    return LHS;
}

ASTexpression * parser::parseTerm(){
    auto LHS = parseFactor();
    if(!LHS){
        return nullptr;
    }
    return LHS;
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

