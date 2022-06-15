#include "parser.h"

int main(){
   scanner sc;

   parser Pr(sc, "Programs/task5Program.txt");
   Pr.parse();

   return EXIT_SUCCESS;
}

 
