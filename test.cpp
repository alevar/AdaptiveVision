#include <iostream>
#include <cmath>
#include <string.h>
#include <unistd.h> 
#include <stdio.h>
#include <vector>
#include <sstream>

using namespace std;
// g++ -std=c++11 -o  test test.cpp `pkg-config opencv --cflags --libs`

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}


int main(){

	vector<string> x = split("one:two::three", ':');

	cout<<x[0]<<endl;

	return 0;
}