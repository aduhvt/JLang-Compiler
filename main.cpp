#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lexer/lexer.h"
#include "parser/parser.h"
using namespace std;

vector<string> readfile(){
    ifstream in;
    in.open("test/source_code.txt");
    // string source_code;
    // cout << "Enter Source Code Path :" << endl;
    // getline(cin, source_code);
    // in.open(source_code);

    vector<string> lines;
    string line;
    while(getline(in, line)) lines.push_back(line);
    in.close();

    if(lines.size() == 0) throw runtime_error("ERROR : Invalid Path or Empty File");
    return lines;
}

void lexerTest(vector<Token> tokens){
    for(int i = 0; i < tokens.size(); i++) cout << tokens[i].lexeme << " " << tokens[i].line << " " << static_cast<int>(tokens[i].type) << endl;
}

int main(){
    try{
        vector<string> lines = readfile();

        lexer l;
        l.tokenize(lines);
        vector<Token> tokens = l.getTokens();   

        parser p;
        p.ast(tokens);

        cout << "success";

    } catch (const exception& e) {
        cerr << e.what();
        return EXIT_FAILURE;
    }

    return 0;
}