#ifndef SEMANTIC_SEMANTIC_H
#define SEMANTIC_SEMANTIC_H

#include <vector>
#include <string>
#include <unordered_map> 
#include "../parser/parser.h"

class Semantic{
    private:
        std::vector<std::unordered_map<std::string, TokenType>> scopes;
        int loopDepth = 0;
    public:
        Semantic(){
            scopes.push_back({});
        }

        void enterBlock();
        void exitBlock();
        void enterLoop();
        void exitLoop();
        void newVar(std::string name, TokenType type);
        bool checkType(TokenType expected, TokenType x);
        TokenType getType(std::string name);

        void analyzeStmt(Stmt* st);
        TokenType analyzeExpr(Expr* ex);
};

#endif

// -------------------------------------

// enforce var dec rules
// enforce scopes
// enforce type rules
// enforce controll flow rules

// 1. Symbol Table
// It tracks:
    // Variable names
    // Their types
    // Their scope
// vector<unordered_map<string, TokenType>> scopes;

// 2. Type Checking
// Every expression has a type.
// TokenType evaluate(Expr*)
    // Literal 10 → int
    // Variable x → look up in symbol table
    // x + 3 → int
    // x < 3 → bool
    // bool + int → error

// 3. Control Flow Rules
    // break must be inside loop
    // if condition must be bool
    // loop condition must be bool
// int loopDepth = 0;
    // when entering a loop ++
    // when exiting a loop --
    // if break and loopD == 0 -> error

// What Errors Should We Catch for V1:
// Variable errors
//     Using undeclared variable
//     Redeclaring variable in same scope
// Type errors
//     int + bool
//     int == bool
//     assigning wrong type
// Control errors
//     break outside loop
//     non-bool condition in if/loop