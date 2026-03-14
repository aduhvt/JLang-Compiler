#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <vector>
#include <string>
#include "../parser/parser.h"
#include <unordered_map>
#include <utility>

struct BreakException { };

class Interpreter{
    private : 
        std::vector<std::unordered_map<std::string, int>> scopes;

    public :
        Interpreter(){
            scopes.push_back({});
        }

        void enterBlock();
        void exitBlock();
        int find(std::string name);
        
        void run(Stmt* program);
        void execute(Stmt* st);
        int evaluate(Expr* st);
};

#endif 
