#include "header.h"

int main(){

   program prog("program.h");
   scanner sc;
   
   bool lexicalError = false;

   while((prog.nextCharInt() < prog.getProgSize() - 1) && lexicalError == false){
      lexicalError = scannerLoop(&sc, &prog);
   }

   return EXIT_SUCCESS;
}

bool scannerLoop(scanner *sc, program *prog){
   sc->initScanner();
   while(sc->getState() != 30){ // 30 will indicate an error state
      char nextChar = prog->getNextChar();
      sc->appendLexeme(nextChar);
      if(sc->getTokenType(sc->getState()) != errorState && sc->getTokenType(sc->getState()) != start) { // we found a final state
         sc->cleanStack(); //delete everything from the stack, indicating that this is the most resent final state found
      }

      sc->pushStack(sc->getState());

      int cat = sc->getClassifier(nextChar); //get the category form the classification table
      sc->setState(sc->getFromTransitionTable(sc->getState(), cat));
   }
   return rollbackLoop(sc, prog);
}

bool rollbackLoop(scanner *sc, program *prog){
   while(sc->getState() != -1 && (sc->getTokenType(sc->getState()) == errorState || sc->getTokenType(sc->getState()) == start)){ 
      sc->setState(sc->popStack());
      prog->decrementChar();
      if (!sc->getLexeme().empty()) {
         string temp = sc->getLexeme();
         temp.pop_back();
         sc->setLexeme(temp);
      }
   }

   if(sc->getTokenType(sc->getState()) != errorState && sc->getState() != -1){
      if (char(sc->getLexeme()[0]) == 10){
         cout << "\\n " << sc->getTokenName(sc->getTokenType(sc->getState())) << endl;
      }else{
         tokens tType = sc->getTokenType(sc->getState());
         if(tType == variable){
            tType = sc->checkReservedWord();
         }
         cout << sc->getLexeme() << " " << sc->getTokenName(tType) << endl;
      }
      return false;
   }else{
      cout << "Lexical error";
      return true;
   }
}
