#include "XML.h"

using namespace std;
class scope;


// var
class var{
private:
    string type;
    string value;
    string name;
public:
    var(string type, string val, string name);
    var(string type, string name);
    virtual ~var();

    string getVarType();
    string getValue();
    string getName();

    void setValue(string val);
};

var::var(string type, string val, string name){
    this->type = type;
    this->value = val;
    this->name = name;
}

var::var(string type, string name){
    this->type = type;
    this->name = name;
    this->value = "";
}

var::~var(){

}

string var::getVarType(){
    return this->type;
}

string var::getValue(){
    return this->value;
}

string var::getName(){
    return this->name;
}

void var::setValue(string val){
    this->value = val;
}


// func
class func{
private:
    string name;
    scope * s;
    string returnType;

public:
    func(string name, scope * s, string returnType);

    string getName();
    string getReturn();
};

func::func(string name, scope * s, string returnType){
    this->name = name;
    this->s = s;
    this->returnType = returnType;
}

string func::getName(){
    return this->name;
}

string func::getReturn(){
    return this->returnType;
}


// scope
class scope{
private:
    vector<var *> * vars;
    vector<scope *> * scopes;
    scope * parentScope;
    string returnType;
public:
    scope();
    scope(scope * s);

    void addVar(string type, string val, string name);
    void addVar(string type, string name);
    void addScope(scope * s);

    var * checkVarName(string name);
    void updateVar(string varName, string val);

};

scope::scope(){
    this->vars = new vector<var *>();
    this->parentScope = nullptr;
    this->scopes = new vector<scope *>();
}

scope::scope(scope * s){
    this->vars = new vector<var *>();
    this->parentScope = s;
    this->scopes = new vector<scope *>();
}

void scope::addVar(string type, string val, string name){
    this->vars->push_back(new var(type, val, name));
}

void scope::addVar(string type, string name){
    this->vars->push_back(new var(type, name));
}


void scope::addScope(scope * s){
    this->scopes->push_back(s);
}


void scope::updateVar(string varName, string val){
    for(vector<var *>::iterator itr = this->vars->begin(), itr_end = this->vars->end(); itr != itr_end; ++itr){
        if((*itr)->getName() == varName){
            (*itr)->setValue(val);
        }
    }
}

var * scope::checkVarName(string name){
    for(vector<var *>::iterator itr = this->vars->begin(), itr_end = this->vars->end(); itr != itr_end; ++itr){
        if((*itr)->getName() == name){
            return *itr;
        }
    }
    return nullptr;
    
}
