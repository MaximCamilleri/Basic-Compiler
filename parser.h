#include "scanner.h"
#include "ASTclasses.h"

class parser{
private:
    scanner *lexer;
    program *p;
    token *nextToken;
public:
    parser(scanner lexer, program p);
    void parse();
    void getNextToken();

    //parsing algorithms
    ASTstatement * parseStatement();
    void parseBlock();
    void parseFunctionDecl();
    void parseFormalParams();
    void parseFormalParam();
    void parseWhileStatement();
    void parseForStatement();
    ASTstatement * parseIfStatement();
    void parseRtrnStatement();
    ASTstatement * parsePrintStatement();
    void parseVariableDecl();
    ASTstatement * parseAssignment();
    ASTstatement * parseExpression();
    void parseSimpleExpr();
    void parseTerm();
    void parseFactor();
    void parseUnary();
    void parseSubExpression();
    void parseFunctionCall();
    void parseActualParams();
    void parseRelationalOp();
    void parseAdditiveOp();
    void parseMultiplicativeOp();
    void parseLiteral();
    void parseBooleanLiteral();
};

parser::parser(scanner lexer, program p){
    this->lexer = &lexer;
    this->p = &p;

}

void parser::getNextToken(){
    bool lexicalError;
    this->nextToken = scannerLoop(this->lexer, this->p, &lexicalError);
    if(lexicalError == true){
        cout << "Program encountered a lexical error";
        exit(0);
    }else{
        cout << "Lexeme: " << this->nextToken->getLexeme() << " Token Type: " << this->nextToken->getTokenType() << endl;
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
        break;

    case _print:
        parsePrintStatement();
        break;

    case _if:
        parseIfStatement();
        break;

    case _for:
        parseForStatement();
        break;

    case _while:
        parseWhileStatement();
        break;

    case _return:
        parseRtrnStatement();
        break;

    case _fn:
        parseFunctionDecl();
        break;

    case openCurlyBracket:
        parseBlock();
        break;

    default:
        break;
    }

    return node;
}

ASTstatement * parser::parsePrintStatement(){
    auto node = new ASTprintStatement();
    getNextToken();
    return node;

}

void parser::parseRtrnStatement(){
    
}

ASTstatement * parser::parseIfStatement(){
    auto node = new ASTifStatement();
    getNextToken();
    return node;
}

void parser::parseForStatement(){
    
}

void parser::parseWhileStatement(){
    
}

void parser::parseBlock(){

}

void parser::parseVariableDecl(){

}

ASTstatement * parser::parseAssignment(){
    string var_name = this->nextToken->getLexeme();
    getNextToken();
    if(this->nextToken->getTokenType() != equals){
        cout << "Expected '='" << endl;
        return nullptr;
    }
    getNextToken();
    auto expr_node = parseExpression();
    auto ass_node = new ASTassignment();

}

void parser::parse(){
    cout << "starting Parser" << endl;
    vector<ASTstatement *> * statments = new vector<ASTstatement *>();
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
                    statments->push_back(stmt);
                    getNextToken();
                }
            }
            break;
        }
    }
    std::cout<< "Finished Parsing" << endl;
}

