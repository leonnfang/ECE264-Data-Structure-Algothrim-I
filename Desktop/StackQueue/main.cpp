/*
 * Jungang Fang 2019Spring
 * DSA project#1
 * this project implements stack and queue by linklist.
 * implemented stack and queue will offer two functions for the users: pop and push.
 * what is more, this program can also deal with corner case.
 * the user will give a input file for the program, and the program will give an output file which
 * describes processed commands and obtained results.
 * the user can type three commands for the program: 'create', 'pop' , 'push'
 */
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <typeinfo>
#include <stdio.h>
std::ifstream inputFile;
std::ofstream outputFile;



void openInputandOutputStream(std::ifstream &input, std::ofstream &output);
void parseLineDetermineType(const std::string &commandLine);
// overall running
int main() {
    openInputandOutputStream(inputFile, outputFile);
    std::string commandLine;
    while(getline(inputFile, commandLine)) {
        parseLineDetermineType(commandLine);
    }
    inputFile.close();
    outputFile.close();
    return 0;
}


/*
 * the following contents implement simplelist
 * this will be a base class for stack and queue
 */
template<typename T>
class Simplelist{
// nested node class
private:
    class Node{
    public:
        T data;
        Node *next;
        Node(T value, Node* pointer){
            data = value;
            next = pointer;
        }
    };

    Node *head;
    Node *tail;
    std::string list_name;
protected:
    void insertAtHead(T value);
    void insertAtTail(T value);
    T removeFromHead();
public:
    bool isEmpty() const;
    std::string getlist_name();
    virtual T pop() = 0;
    explicit Simplelist(std::string name);
    virtual void push(T val) = 0;
};

/****************************************************************************
*****************************************************************************/
// simplelist constructor
template <typename T>
Simplelist<T>::Simplelist(std::string name){
    list_name = name;
    head = nullptr;
    tail = nullptr;
}
// check if the list is empty
template <typename T>
bool Simplelist<T>::isEmpty() const{
    return (head == nullptr && tail == nullptr);
}
// get the name of the list
template<typename T>
std::string Simplelist<T>::getlist_name(){
    return list_name;
}
// insert node at the head of the list
template<typename T>
void Simplelist<T>::insertAtHead(T value) {
    auto *newTempNode = new Node(value, head);
    head = newTempNode;
}
//  insert the node at the end of the list
template<typename T>
void Simplelist<T>::insertAtTail(T value) {
    auto *newTempNode = new Node(value, nullptr);
    ((tail == nullptr) ? head : tail->next) = newTempNode;
    tail = newTempNode;
}
//  remove the node at the end the list for both lists, and this function will also pop out the value
template<typename T>
T Simplelist<T>::removeFromHead() {
    Node *cur_pointer = head;
    T value = cur_pointer->data;
    if (cur_pointer->next == nullptr) {
        tail = nullptr;
    }
    head = cur_pointer->next;
    delete cur_pointer;
    return value;
}
/****************************************************************************
*****************************************************************************/

// class for queue
template<typename T>
class Queue : public Simplelist<T> {
public:
    void push(T val) override;
    T pop() override;
    explicit Queue(std::string queueName);
};

// push the value into queue. this function will push the value at the end of list
template<typename T>
void Queue<T>::push(T val) {
    Simplelist<T>::insertAtTail(val);
}

// pop the value from the end of the list
template<typename T>
T Queue<T>::pop() {
    return Simplelist<T>::removeFromHead();
}

//Queue constructor.
template<typename T>
Queue<T>::Queue(std::string queueName):Simplelist<T>(queueName) {
}

/*********************************************************************************
**********************************************************************************/
// class for stack
template<typename T>
class Stack : public Simplelist<T> {
public:
    void push(T val) override;
    T pop() override;
    explicit Stack(std::string queueName);
};
// push the value into stack. this function will push the value at the start of list
template<typename T>
void Stack<T>::push(T value) {
    Simplelist<T>::insertAtHead(value);
}
// pop the value at the head of the list
template<typename T>
T Stack<T>::pop() {
    return Simplelist<T>::removeFromHead();
}
//Stack constructor.
template<typename T>
Stack<T>::Stack(std::string stackName):Simplelist<T>(stackName) {
}
/****************************************************************************
*****************************************************************************/







/****************************************************************************
*****************************************************************************/
// give out two prompts for users to input and output files
void openInputandOutputStream(std::ifstream &input, std::ofstream &output);

void openInputandOutputStream(std::ifstream &input, std::ofstream &output) {
    std::string inputFileName;
    std::cout << "Enter name of input file: ";
    std::cin >> inputFileName;
    input.open(inputFileName.c_str());

    std::string outputFileName;
    std::cout << "Enter name of output file: ";
    std::cin >> outputFileName;
    output.open(outputFileName.c_str());
}
/****************************************************************************
*****************************************************************************/

//obtain commands from the input file
template<typename T>
void processLine(std::list<Simplelist<T> *> &listSLType, std::string commands[], T value) {
    // if the first command is create, the program will check if this name exits or not.
    //  if the name does not exit, the program will creat the coresponding objects
    if (commands[0] == "create") {
        if (SimpleListSearch(listSLType, commands[1]) != nullptr) {
            outputFile << "ERROR: This name already exists!" <<std::endl;
        } else {
            if (commands[2] == "stack") {
                auto *pointerSL = new Stack<T>(commands[1]);
                listSLType.push_front(pointerSL);
            } else if (commands[2] == "queue") {
                auto *pointerSL = new Queue<T>(commands[1]);
                listSLType.push_front(pointerSL);
            }
        }
    }
    // if the first word is push, the program will check if it exists or not
    // and the program will push specific value into the object
    else if (commands[0] == "push") {
        Simplelist <T> *pointer = SimpleListSearch(listSLType, commands[1]);
        if (pointer == nullptr) {
            outputFile << "ERROR: This name does not exist!" <<std::endl;
        } else {
            pointer->push(value);
        }
    }
    // if the first command is pop, the program will check the name first
    // if the the name exists, the program will check if the object is empty
    // if the object is not empty, the program will pop out the value related with the object
    else if (commands[0] == "pop") {
        Simplelist <T> *pointer = SimpleListSearch(listSLType, commands[1]);
        if (pointer == nullptr) {
            outputFile << "ERROR: This name does not exist!" <<std::endl;
        } else if (pointer->isEmpty()) {
            outputFile << "ERROR: This list is empty!" <<std::endl;
        } else {
            outputFile << "Value popped: " << pointer->pop()<<std::endl;
        }
    }
}
// this function will be called to search with a specific list or stack, and it will check if they already exist or not
template<typename T>
Simplelist<T> *SimpleListSearch(std::list<Simplelist<T> *> &searchList, std::string listName) {
    for (typename std::list<Simplelist<T> *>::const_iterator itor = searchList.begin(); itor != searchList.end(); ++itor) {
        if ((*itor)->getlist_name() == listName) {
            return *itor;
        }
    }
    return nullptr;
}
/****************************************************************************
*****************************************************************************/

std::list<Simplelist<int> *> listSLi; //list of stacks and queues of integers
std::list<Simplelist<double> *> listSLd; //list of stacks and queues of doubles
std::list<Simplelist<std::string> *> listSLs; //list of stacks and queues of strings

// parses the file and prepare it for processes
void parseLineDetermineType(const std::string &commandLine){
    outputFile << "PROCESSING COMMAND: " << commandLine << std::endl;
    std::string swap;
    std::stringstream ss1(commandLine);
    auto * commands = new std::string[3];
    int i=0;
    while(ss1 >> swap)
        commands[i++] = swap;
    if(commands[1].substr(0,1) == "i"){
        int val = 0;
        if(!commands[2].empty()){
            val = atoi(commands[2].c_str());
        }
        processLine(listSLi, commands, val);
    }
    else if(commands[1].substr(0,1) == "d"){
        double val = 0.0;
        if(!commands[2].empty()){
            val = atof(commands[2].c_str());
        }
        processLine(listSLd, commands, val);
    }
    else if(commands[1].substr(0,1) == "s"){
        processLine(listSLs, commands, commands[2]);
    }
}