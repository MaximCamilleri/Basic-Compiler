#include <vector>
#include <String>
#include <list>
using namespace std;

class node{
private:
    int id;
    string value;
public:
    node(int id, string value = NULL){
        this->id = id;
        this->value = value;
    }

    int getId(){
        return this->id;
    }

    string getValue(){
        return this->value;
    }
};

class tree{
private:
    vector<node*> nodes;
    node *startNode;
    int idCounter = 0;
    list<int>* adjList;
public:
    tree(string value = NULL);

    //getters
    int getValue(int id);

    //add node and edge
    void addNode(string value = NULL);
    void addEdge(int n1, int n2);
};

tree::tree(string value = NULL){
    node *temp = new node(this->idCounter, value);
    this->nodes.push_back(temp);
    this->idCounter += 1;
    this->startNode = temp;

    this->adjList = new list<int>[this->idCounter];
}

void tree::addNode(string value = NULL){
    node *temp = new node(this->idCounter, value);
    this->nodes.push_back(temp);
    this->idCounter += 1;
    this->adjList->resize(this->idCounter);
}

void tree::addEdge(int n1, int n2){
    this->adjList[n1].push_back(n2);
}