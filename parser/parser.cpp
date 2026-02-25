#include "parser.h"
#include <iostream>
using namespace std;

// -------------------------------------------------
// helper functions

Token& parser::peek() { return tokens[cur]; }

Token& parser::previous() {
    return tokens[cur-1];
}

Token& parser::advance() {
    if(!isAtEnd()) cur++;
    return previous();
}

bool parser::isAtEnd() { return peek().type == TokenType::END_OF_FILE; }

bool parser::check(TokenType expected){
    if(expected != peek().type) return false;
    return true;
}

bool parser::match(TokenType expected){
    if(!check(expected)) return false;
    advance();
    return true;
}

Token& parser::consume(TokenType expected, string msg){
    if(!check(expected)) throw runtime_error(errorMsg(msg));
    return advance();
}

string parser::errorMsg(string msg){
    return "Syntax Error : at line " + to_string(peek().line) +  " :: " + msg;
}

// -------------------------------------------------
// Main

void parser::ast(vector<Token>& tokens){
    Stmt* AST = start(tokens);
}

Stmt* parser::start(vector<Token>& t){
    cout << "Start" << endl;
    tokens = t;
    vector<Stmt*> Statements;
    while(!isAtEnd()){
        Stmt* st = statement();
        Statements.push_back(st);
    }
    return new BlockStmt(Statements);
}

// -------------------------------------------------
// Statements

Stmt* parser::blockSt(){
    cout << "Block" << endl;
    consume(TokenType::LEFT_BRACE, "Expected {");
    vector<Stmt*> Statements;
    while(!isAtEnd() && !check(TokenType::RIGHT_BRACE)){
        Stmt* st = statement();
        Statements.push_back(st);
    }
    consume(TokenType::RIGHT_BRACE, "Expected }");
    return new BlockStmt(Statements);
}

Stmt* parser::statement(){
    cout << "Statement" << endl;
    if(check(TokenType::LEFT_BRACE)) return blockSt();
    else if(check(TokenType::KW_PRINT)) return printSt();
    else if(check(TokenType::KW_LOOP)) return loopSt();
    else if(check(TokenType::KW_IF)) return ifelseSt();
    else if(check(TokenType::KW_INT) || check(TokenType::KW_BOOL)) return varDecSt();
    else return expressionSt();
}

Stmt* parser::printSt(){
    cout << "Print" << endl;
    consume(TokenType::KW_PRINT, "Expected 'print' here");
    consume(TokenType::LEFT_PAREN, "Expected ( here");
    Expr* exp = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ) here");
    consume(TokenType::SEMICOLON, "Expected ; here");

    Stmt* st = new PrintStmt(exp);
    return st;
}

Stmt* parser::expressionSt(){
    cout << "Expression St" << endl;
    Expr* expr = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression");
    return new ExprStmt(expr);
}

Stmt* parser::varDecSt(){
    cout << "Var Dec St" << endl;

    TokenType type;
    if(peek().type == TokenType::KW_BOOL) type = consume(TokenType::KW_BOOL, "Type missing").type;
    else type = consume(TokenType::KW_INT, "Type missing").type;

    string var = consume(TokenType::IDENTIFIER, "Missing Identifier").lexeme;
    
    Expr* exp = nullptr;
    if(match(TokenType::EQUAL)) exp = expression();

    consume(TokenType::SEMICOLON, "Expected ; here");
    return new VarDecStmt(type, var, exp);
}

Stmt* parser::loopSt(){
    cout << "Loop" << endl;
    consume(TokenType::KW_LOOP, "Expected 'loop' keyword here");

    consume(TokenType::LEFT_PAREN, "Expected ( here");
    Expr* con = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ) here");

    Stmt* blk = statement();
    return new LoopStmt(con, blk);
}

Stmt* parser::ifelseSt(){
    cout << "ifelse" << endl;
    consume(TokenType::KW_IF, "Expected 'if' keyword here");

    consume(TokenType::LEFT_PAREN, "Expected ( here");
    Expr* ifCon = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ) here");

    Stmt* ifBlk = statement();
    Stmt* elseBlk = nullptr;
    if(match(TokenType::KW_ELSE)) elseBlk = statement();

    return new IfStmt(ifCon, ifBlk, elseBlk);
}


// -------------------------------------------------
// Expressions 
// expression -> equality (== or !=)
// equality -> comparison (< or >)
// comparison -> term (+ -)
// term -> factor (* /)
// factor -> unary (- !)
// unary -> primary (IDENTIFIER | NUMBER | EXPRESION)

Expr* parser::expression(){
    cout << "Expression" << endl;
    return equality();
}

Expr* parser::equality(){
    cout << "equality" << endl;
    Expr* left = comparison();
    while(match(TokenType::BANG_EQUAL) || match(TokenType::EQUAL_EQUAL)){
        TokenType ops = previous().type;
        Expr* right = comparison();
        left = new BinaryExpr(left, ops, right);
    };
    return left;
}

Expr* parser::comparison(){
    cout << "comparison" << endl;
    Expr* left = term();
    while(match(TokenType::LESS) || match(TokenType::GREATER)){
        TokenType ops = previous().type;
        Expr* right = term();
        left = new BinaryExpr(left, ops, right);
    };
    return left;
}

Expr* parser::term(){
    cout << "term" << endl;
    Expr* left = factor();
    while(match(TokenType::PLUS) || match(TokenType::MINUS)){
        TokenType ops = previous().type;
        Expr* right = factor();
        left = new BinaryExpr(left, ops, right);
    };
    return left;
}

Expr* parser::factor(){
    cout << "factor" << endl;
    Expr* left = unary();
    while(match(TokenType::STAR) || match(TokenType::SLASH)){
        TokenType ops = previous().type;
        Expr* right = unary();
        left = new BinaryExpr(left, ops, right);
    };
    return left;
}

Expr* parser::unary(){
    cout << "unary" << endl;
    if(match(TokenType::MINUS)){
        TokenType ops = previous().type;
        Expr* right = unary();
        return new UnaryExpr(ops, right);
    }
    return primary();
}

Expr* parser::primary(){
    cout << "primary" << endl;
    if(match(TokenType::NUMBER)) return new LiteralExpr(previous().lexeme);
    if(match(TokenType::IDENTIFIER)) return new VariableExpr(previous().lexeme);
    if(match(TokenType::LEFT_PAREN)){
        Expr* exp = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ) here");
        return exp;
    }
    throw runtime_error(errorMsg("Expression error"));
}

// -------------------------------------------------