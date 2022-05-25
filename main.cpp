#include "header.h"

int main(){
   program prog("testProgram.txt");
   scanner sc;
   
   bool lexicalError = false;
   string token;
   while((prog.nextCharInt() < prog.getProgSize() - 1) && lexicalError == false){
      lexicalError = scannerLoop(&sc, &prog, token);
      cout << token << endl;
   }

   return EXIT_SUCCESS;
}


