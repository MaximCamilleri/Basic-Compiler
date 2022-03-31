#include "scanner.h"

int main(){
    string filename("program.txt");
    vector<char> bytes;

    FILE* input_file = fopen(filename.c_str(), "r");
    if (input_file == nullptr) {
       return EXIT_FAILURE;
    }
    
    int c;
    while ((c = fgetc(input_file)) != EOF) {
       putchar(c);
       cout << "-";
    }

    cout << endl;
    fclose(input_file);

    

    return EXIT_SUCCESS;
}

void scannerLoop(scanner sc, char nextChar){
   while(sc.getState() != -1){ // -1 will indicate an error state
      sc.appendLexeme(nextChar);
      //if(state in Sa) stack.clear(); (what is sa?)
      //sc.pushStack(state) (where am i getting state from)
      //cat = CharCat(char); get the catagory form the clasification table
      //state = TX[state, cat]; get the next state from the transion table 
      rollbackLoop(sc);
   }
}

void rollbackLoop(scanner sc){
   while(sc.getState() != -2 ){ // -2 will indidicate an accepting state 
      sc.setState(sc.popStack());
   }
}
