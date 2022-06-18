#include "parser.h"

int main(){
   scanner sc;

   parser Pr(sc, "Programs/program.txt");
   // parser Pr(sc, "Programs/task4Program.txt");
   Pr.parse();

   return EXIT_SUCCESS;
}


