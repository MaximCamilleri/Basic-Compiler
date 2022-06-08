#include "header.h"
#include "parser.h"

int main(){
   program prog("testProgram.txt");
   scanner sc;
   
   // bool lexicalError;
   // token *token;
   // while((prog.nextCharInt() < prog.getProgSize()) && lexicalError == false){
   //    token = scannerLoop(&sc, &prog, &lexicalError);
   //    cout << "Lexeme: " << token->getLexeme() << " Token Type: " << token->getTokenType() << endl;
   // }

   parser Pr(sc, "program.txt");
   Pr.parse();

   return EXIT_SUCCESS;
}

 
