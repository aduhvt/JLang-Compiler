#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lexer/lexer.h"
using namespace std;

int main(){
    ifstream in;
    in.open("source_code.txt");

    vector<string> lines;

    string line;
    while(in.eof() == 0){
        getline(in, line);
        lines.push_back(line);
    }
    
    int x = tokens(lines);
    cout << x;
    return 0;
}