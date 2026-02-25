#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include <vector>
#include <string>
#include "../lexer/lexer.h"

// ---------------------------------------

struct Stmt{
    virtual ~Stmt() {};
};

struct Expr{
    virtual ~Expr() {};
};

// ---------------------------------------
// Statements

struct IfStmt : Stmt{
    Expr* condition;
    Stmt* thenBranch;
    Stmt* elseBranch; // null ptr if none
    IfStmt(Expr* c, Stmt* t, Stmt* e) : condition(c), thenBranch(t), elseBranch(e) {}
};

struct LoopStmt : Stmt{
    Expr* condition;
    Stmt* body;
    LoopStmt(Expr* e, Stmt* b) : condition(e), body(b) {}
};

struct PrintStmt : Stmt{
    Expr* expression;
    PrintStmt(Expr* e) : expression(e) {}
};

struct BreakStmt : Stmt{ };

struct BlockStmt : Stmt{
    std::vector<Stmt*> statements;
    BlockStmt(std::vector<Stmt*>& x) : statements(x) {}
};

struct VarDecStmt : Stmt{
    TokenType type;
    std::string name;
    Expr* initializer;
    VarDecStmt(TokenType t, std::string n, Expr* i) : name(n), initializer(i), type(t) {}
};

struct ExprStmt : Stmt{
    Expr* expression;
    ExprStmt(Expr* e) : expression(e) {}
};

// ---------------------------------------
// Expression

struct LiteralExpr : Expr{
    std::string value;
    LiteralExpr(std::string v) : value(v) {}
};

struct VariableExpr : Expr{
    std::string value;
    VariableExpr(std::string v) : value(v) {}
};

struct BinaryExpr : Expr{
    Expr* left;
    TokenType ops;
    Expr* right;
    BinaryExpr(Expr* l, TokenType o, Expr* r) : left(l), right(r), ops(o) {}
};

struct UnaryExpr : Expr{
    TokenType ops;
    Expr* value;
    UnaryExpr(TokenType o, Expr* v) : ops(o), value(v) {}
};

// ---------------------------------------

class parser{
    public:
        int cur = 0;
        std::vector<Token> tokens;

        Token& peek();
        Token& previous();
        Token& advance();
        bool isAtEnd();
        Token& consume(TokenType expected, std::string msg);
        bool check(TokenType expected);
        bool match(TokenType expected);
        std::string errorMsg(std::string msg);

        void ast(std::vector<Token>& tokens);
        Stmt* start(std::vector<Token>& tokens);

        Stmt* statement();
        Stmt* blockSt();
        Stmt* printSt();
        Stmt* loopSt();
        Stmt* ifelseSt();
        Stmt* expressionSt();
        Stmt* varDecSt();
        
        Expr* expression();
        Expr* equality();
        Expr* comparison();
        Expr* term();
        Expr* factor();
        Expr* unary();
        Expr* primary();
};

#endif