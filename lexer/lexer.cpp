#include "lexer.h"
#include <iostream>
#include <vector>
#include <cctype>
using namespace std;

void lexer::tokenize(vector<string>& lines){
    for(int i = 0; i < lines.size(); i++){
        string line = lines[i];
        for(int j = 0; j < line.size(); j++){
            if(isspace(line[j])) continue;
            else if(line[j] == '(') tokens.push_back({TokenType::LEFT_PAREN, "(", i});
            else if(line[j] == '{') tokens.push_back({TokenType::LEFT_BRACE, "{", i});
            else if(line[j] == ')') tokens.push_back({TokenType::RIGHT_PAREN, ")", i});
            else if(line[j] == '}') tokens.push_back({TokenType::RIGHT_BRACE, "}", i});
            else if(line[j] == ';') tokens.push_back({TokenType::SEMICOLON, ";", i});
            else if(line[j] == '+') tokens.push_back({TokenType::PLUS, "+", i});
            else if(line[j] == '-') tokens.push_back({TokenType::MINUS, "-", i});
            else if(line[j] == '*') tokens.push_back({TokenType::STAR, "*", i});
            else if(line[j] == '/') tokens.push_back({TokenType::SLASH, "/", i});
            else if(line[j] == '>') tokens.push_back({TokenType::GREATER, ">", i});
            else if(line[j] == '<') tokens.push_back({TokenType::LESS, "<", i});
            else if(j+1 < line.size() && line[j] == '!' && line[j+1] == '='){
                tokens.push_back({TokenType::BANG_EQUAL, "!=", i});
                j++;
            }
            else if(j+1 < line.size() && line[j] == '=' && line[j+1] == '='){
                tokens.push_back({TokenType::EQUAL_EQUAL, "==", i});
                j++;
            }
            else if(line[j] == '=') tokens.push_back({TokenType::EQUAL, "=", i});
            else if(isalpha(line[j])){
                string word = "";
                while(isalnum(line[j]) || line[j] == '_'){
                    word += line[j];
                    j++;
                }
                j--;
                TokenType t;
                if(word == "int") t = TokenType::KW_INT;
                else if(word == "bool") t = TokenType::KW_BOOL;
                else if(word == "print") t = TokenType::KW_PRINT;
                else if(word == "if") t = TokenType::KW_IF;
                else if(word == "else") t = TokenType::KW_ELSE;
                else if(word == "loop") t = TokenType::KW_LOOP;
                else if(word == "break") t = TokenType::KW_BREAK;
                else t = TokenType::IDENTIFIER;
                tokens.push_back({t, word, i});
            }
            else if(isdigit(line[j])){
                string word = "";
                while(isdigit(line[j])){
                    word += line[j];
                    j++;
                }
                j--;
                tokens.push_back({TokenType::NUMBER, word, i});
            }
            else {
                cout << "ERROR : Syntax error at line " << i;
                break;
            }
        }
    }
    tokens.push_back({TokenType::END_OF_FILE, "", (int)lines.size()});
}

const vector<Token>& lexer::getTokens() const{
    return tokens;
}