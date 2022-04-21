#include "lib_std_facilities.h"

class program{
private:
    vector<char> prog;
    int nextChar;

public:
    program(string fileName);
    void readProgram(string fileName);
    void decrementChar();

    char getNextChar();
    int getProgSize();
    int nextCharInt();
    vector<char> getProg();
};

program::program(string fileName){
    this->nextChar = 0;
    readProgram(fileName);
}

void program::readProgram(string fileName){
    string filename("program.txt");

    FILE* input_file = fopen(filename.c_str(), "r");
    if (input_file == nullptr) {
       cout << "File not found";
    }

    char c;
    while ((c = fgetc(input_file)) != EOF) {
       this->prog.push_back(c);
    }
    //this->prog.push_back('\n');

    fclose(input_file);
}

char program::getNextChar(){
    char tempChar = this->prog[nextChar];
    this->nextChar += 1;
    return tempChar;
}

void program::decrementChar(){
    this->nextChar -= 1;
}

int program::getProgSize(){
    return this->prog.size();
}

int program::nextCharInt(){
    return this->nextChar;
}

vector<char> program::getProg(){
    return this->prog;
}



    
