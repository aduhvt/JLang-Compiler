#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include <vector>
#include <string>
#include <memory>
#include "../lexer/lexer.h"

// ---------------------------------------

struct Stmt{
    virtual ~Stmt() = default;
    virtual void print(int indent = 0) const = 0;
};

struct Expr{
    virtual ~Expr() = default;
    virtual void print(int indent = 0) const = 0;
};

// ---------------------------------------
// Statements

struct IfStmt : Stmt{
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch; // null ptr if none

    IfStmt(std::unique_ptr<Expr> c, std::unique_ptr<Stmt> t, std::unique_ptr<Stmt> e) 
    : condition(std::move(c)), thenBranch(std::move(t)), elseBranch(std::move(e)) {}

    void print(int indent) const override;
};

struct LoopStmt : Stmt{
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    LoopStmt(std::unique_ptr<Expr> e, std::unique_ptr<Stmt> b) 
    : condition(std::move(e)), body(std::move(b)) {}

    void print(int indent) const override;
};

struct PrintStmt : Stmt{
    std::unique_ptr<Expr> expression;
   
    PrintStmt(std::unique_ptr<Expr> e) : expression(std::move(e)) {}
    void print(int indent) const override;
};

struct BreakStmt : Stmt{ 
    void print(int indent) const override;
};

struct BlockStmt : Stmt{
    std::vector<std::unique_ptr<Stmt>> statements;
    
    BlockStmt(std::vector<std::unique_ptr<Stmt>> x) 
    : statements(std::move(x)) {}

    void print(int indent) const override;
};

struct VarDecStmt : Stmt{
    TokenType type;
    std::string name;
    std::unique_ptr<Expr> initializer;
    
    VarDecStmt(TokenType t, std::string n, std::unique_ptr<Expr> i) 
    : name(n), initializer(std::move(i)), type(t) {}

    void print(int indent) const override;
};

struct ExprStmt : Stmt{
    std::unique_ptr<Expr> expression;
    ExprStmt(std::unique_ptr<Expr> e) 
    : expression(std::move(e)) {}

    void print(int indent) const override;
};

// ---------------------------------------
// Expression

struct LiteralExpr : Expr{
    std::string value;
    
    LiteralExpr(std::string v) : value(v) {}
    void print(int indent) const override;
};

struct VariableExpr : Expr{
    std::string value;

    VariableExpr(std::string v) : value(v) {}
    void print(int indent) const override;
};

struct BinaryExpr : Expr{
    std::unique_ptr<Expr> left;
    TokenType ops;
    std::unique_ptr<Expr> right;

    BinaryExpr(std::unique_ptr<Expr> l, TokenType o, std::unique_ptr<Expr> r) 
    : left(std::move(l)), right(std::move(r)), ops(o) {}

    void print(int indent) const override;

};

struct UnaryExpr : Expr{
    TokenType ops;
    std::unique_ptr<Expr> value;

    UnaryExpr(TokenType o, std::unique_ptr<Expr> v) 
    : ops(o), value(std::move(v)) {}

    void print(int indent) const override;
};

// ---------------------------------------

class Parser{
    public:
        int cur = 0;
        std::vector<Token> tokens;

        Parser(std::vector<Token> t) : tokens(t), cur(0) {}

        Token& peek();
        Token& previous();
        Token& advance();
        bool isAtEnd();
        Token& consume(TokenType expected, std::string msg);
        bool check(TokenType expected);
        bool match(TokenType expected);
        std::string errorMsg(std::string msg);

        std::unique_ptr<Stmt> ast();
        std::unique_ptr<Stmt> start();

        std::unique_ptr<Stmt> statement();
        std::unique_ptr<Stmt> blockSt();
        std::unique_ptr<Stmt> printSt();
        std::unique_ptr<Stmt> loopSt();
        std::unique_ptr<Stmt> ifelseSt();
        std::unique_ptr<Stmt> expressionSt();
        std::unique_ptr<Stmt> varDecSt();
        
        std::unique_ptr<Expr> expression();
        std::unique_ptr<Expr> equality();
        std::unique_ptr<Expr> comparison();
        std::unique_ptr<Expr> term();
        std::unique_ptr<Expr> factor();
        std::unique_ptr<Expr> unary();
        std::unique_ptr<Expr> primary();
};

#endif