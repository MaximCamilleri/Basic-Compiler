#include "lib_std_facilities.h"

using namespace std;

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
    string filename(fileName);

    FILE* input_file = fopen(filename.c_str(), "r");
    if (input_file == nullptr) {
       std::cout << "File not found";
    }

    char c;
    while ((c = fgetc(input_file)) != EOF) {
       this->prog.push_back(c);
    }
    this->prog.push_back('\n');

    fclose(input_file);
}

char program::getNextChar(){
    try{
        char tempChar = this->prog[nextChar];
        this->nextChar += 1;
        return tempChar;
    } catch(const std::out_of_range& oor){
        return EOF;
    }
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



    
