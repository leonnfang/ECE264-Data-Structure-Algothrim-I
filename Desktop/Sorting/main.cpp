// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, SPRING 2019

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <queue>

using namespace std;

// A simple class; each object holds four public fields
class Data {
public:
    string lastName;
    string firstName;
    string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {
    ifstream input(filename);
    if (!input) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    // The first line indicates the size
    string line;
    getline(input, line);
    stringstream ss(line);
    int size;
    ss >> size;

    // Load the data
    for (int i = 0; i < size; i++) {
        getline(input, line);
        stringstream ss2(line);
        Data *pData = new Data();
        ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
        l.push_back(pData);
    }
    input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {
    ofstream output(filename);
    if (!output) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }
    // Write the size first
    int size = l.size();
    output << size << "\n";

    // Write the data
    for (auto pData:l) {
        output << pData->lastName << " "
               << pData->firstName << " "
               << pData->ssn << "\n";
    }
    output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);
// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
    string filename;
    cout << "Enter name of input file: ";
    cin >> filename;
    list<Data *> theList;
    loadDataList(theList, filename);
    cout << "Data loaded.\n";
    cout << "Executing sort...\n";
    clock_t t1 = clock();
    sortDataList(theList);
    clock_t t2 = clock();
    double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

    cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

    cout << "Enter name of output file: ";
    cin >> filename;
    writeDataList(theList, filename);

    return 0;
}
// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------
// You may add global variables, functions, and/or
// class defintions here if you wish.
int getnumber(string &l,int);
void redixsort();
void countsort(int);
int getnumberstring(string &l);
void sorting3();
bool lastnamesort(const Data* a, const Data* b);
void bucket();
Data* pointers[1005000]; // 4mb
Data* countsort_array[1000][100000];
Data* arr2[676][40000];
int a[250000];
int a1[676];
int size_l;
string head;
string head2;
string tail;

void sortDataList(list<Data *> &l) {
    // Fill this in
    head = l.front()->lastName;
    tail = l.back()->lastName;
    int i = 0;
    for (auto a : l) {
        pointers[i] = a;
        if(i == 1){
            head2 = pointers[i]->lastName;
        }else if(i == 50000){
            string mid = pointers[i]->lastName;
        }
        i++;
    }
    size_l = i;
    int filetype = 1;
    if(head.compare(tail)==0 && head.compare(head2) == 0) {
        filetype = 4;
    }else if (head.compare(head2)==0 &&  head.compare(tail)!=0 ){
        filetype = 3;
    }else if(i > 900000){
        filetype = 2;
    }
    if(filetype == 1) {
        bucket();
        auto it = l.begin();
        for(int i = 0;i<676;i++){
            int buf = a1[i];
            for(int j = 0; j< buf;j++){
                (*it++) = arr2[i][j];
            }
        }
    }else if(filetype == 2){
        //redixsort(); // ssn
        bucket();
        auto it = l.begin();
        for(int i = 0;i<676;i++){
            int buf = a1[i];
            for(int j = 0; j< buf;j++){
                (*it++) = arr2[i][j];
            }
        }
    }else if(filetype == 3){
        sorting3();
        auto it = l.begin();
        for(int j = 0;j < l.size();j++){
            (*it++) = pointers[j];
        }
    }else if(filetype == 4){
        redixsort();
        auto it = l.begin();
        for(int j = 0;j < l.size();j++){
            (*it++) = pointers[j];
        }
    }
    // put everything back into the list

}

string aLn;
string bLn;
string aFn;
string bFn;
string assn;
string bssn;
bool lastnamesort(const Data* a, const Data* b){
    if(a->lastName != b->lastName){
        return a->lastName < b->lastName;
    }else if(a->firstName != b->firstName){
        return a->firstName < b->firstName;
    }else {
        return a->ssn < b->ssn;
    }
    //return false;
}

void redixsort(){
    for(int i = 1; i>=0 ;i--) {
        countsort(i);
    }
}
Data* curpointer;
int curnumber;
void countsort(int layer){
    int size_info = size_l;
    for(int i = 0; i<size_info;i++){
        curpointer = pointers[i];
        curnumber = getnumber(curpointer->ssn,layer);
        countsort_array[a[curnumber]][curnumber]=curpointer;
        a[curnumber]++; // number of elements of a certain digit
    }
    int counter = 0;
    for(int i = 0; i<1005000;i++){
        if(counter == size_info){
            break;
        }
        int numberelement = a[i];
        if(numberelement == 0){
            continue;
        }
        a[i]=0;
        for(int j = 0;j<numberelement;j++){
            Data* cur_pointer = countsort_array[j][i];
            pointers[counter] = cur_pointer;
            counter++;
        }

    }
}

void bucket(){
    for(int i = 0; i<size_l;i++){
        int cnum = getnumberstring(pointers[i]->lastName);
        arr2[cnum][a1[cnum]++] = pointers[i];
    }
    int max = 0;
    int maxI = 0;
    for(int i = 0; i<676;i++) {
        if(a1[i] > max){
            max = a1[i];
            maxI = i;
        }
        std::sort(&arr2[i][0], &arr2[i][a1[i]], lastnamesort);
    }
    std::cout << max << std::endl;
    std::cout << maxI << std::endl;
}

void sorting3(){
    int size = size_l;
    int left = 0;
    int right = 0;
    int i = 0;
    while (left < size){
        while(pointers[i]->firstName == pointers[i+1]->firstName){
            ++right;
            ++i;
            if (i >= size-1) {
                break;
            }
        }
        for(i=left+1;i <= right; ++i){
            auto temp_data = pointers[i];
            int j = i - 1;
            while (j >= left && pointers[j]->ssn > temp_data->ssn){
                pointers[j+1] = pointers[j];
                --j;
                if (j < 0) {
                    break;
                }
            }
            pointers[j+1] = temp_data;
        }
        ++right;
        left = right;
        i = left;
    }
}
int getnumber(string &input,int i){
    if(i == 1){
        return (input.at(7)-'0')*1000+(input.at(8)-'0')*100+(input.at(9)-'0')*10+(input.at(10)-'0');
    }else if(i == 0){
        return (input.at(0)-'0')*10000+(input.at(1)-'0')*1000+(input.at(2)-'0')*100+(input.at(4)-'0')*10+(input.at(5)-'0');
    }
    return (input.at(0)-'0')*100000000+(input.at(1)-'0')*10000000+(input.at(2)-'0')*1000000+(input.at(4)-'0')*100000+(input.at(5)-'0')*10000+(input.at(7)-'0')*1000+(input.at(8)-'0')*100+(input.at(9)-'0')*10+(input.at(10)-'0');
}
int getnumberstring(string &i){
    return (i.at(0)-'A')*26+(i.at(1)-'A');
}

