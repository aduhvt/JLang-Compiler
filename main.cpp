#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lexer/lexer.h"
using namespace std;

int main(){
    ifstream in;
    in.open("test/source_code.txt");

    vector<string> lines;

    string line;
    while(getline(in, line)) lines.push_back(line);
    
    lexer l;
    l.tokenize(lines);
    vector<Token> tokens = l.getTokens();   

    for(int i = 0; i < tokens.size(); i++){
        cout << tokens[i].lexeme << " " << tokens[i].line << " " << static_cast<int>(tokens[i].type) << endl;
    }

    return 0;
}