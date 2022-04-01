#include "header.h"

int main(){
    string filename("program.txt");

    FILE* input_file = fopen(filename.c_str(), "r");
    if (input_file == nullptr) {
       return EXIT_FAILURE;
    }
    
    char c;
    scanner sc;
    while ((c = fgetc(input_file)) != EOF) {
       scannerLoop(sc, c);
    }

    cout << endl;
    fclose(input_file);

    

    return EXIT_SUCCESS;
}

void scannerLoop(scanner sc, char nextChar){
   while(sc.getState() != -1){ // -1 will indicate an error state
      sc.appendLexeme(nextChar);

      if(sc.getTokenType(sc.getState()) != "reject") {
         sc.cleanStack(); 
      }

      sc.pushStack(sc.getState());

      string tmp_string(1, nextChar);
      int cat = sc.getClassifier(tmp_string); //get the catagory form the clasification table
      sc.setState(sc.getFromTransitionTable(sc.getState(), cat));
      //rollbackLoop(sc);
      cout << sc.getLexeme();
   }
}

void rollbackLoop(scanner sc){
   while(sc.getState() != -2 ){ // -2 will indidicate an accepting state 
      sc.setState(sc.popStack());
   }
}
