#include "Parser.h"
#include <iostream>
using namespace std;

// -------------------------------------------------
// helper functions

Token& Parser::peek() { return tokens[cur]; }

Token& Parser::previous() {
    if (cur == 0) return tokens[0];
    return tokens[cur-1];
}

Token& Parser::advance() {
    if(!isAtEnd()) cur++;
    return previous();
}

bool Parser::isAtEnd() { return peek().type == TokenType::END_OF_FILE; }

bool Parser::check(TokenType expected){
    if(isAtEnd()) return false;
    return peek().type == expected;
}

bool Parser::match(TokenType expected){
    if(!check(expected)) return false;
    advance();
    return true;
}

Token& Parser::consume(TokenType expected, string msg){
    if(!check(expected)) throw runtime_error(errorMsg(msg));
    return advance();
}

string Parser::errorMsg(string msg){
    return "Syntax Error : at line " + to_string(peek().line) +  " :: " + msg;
}

// -------------------------------------------------
// Main

unique_ptr<Stmt> Parser::ast(){
    return start();
}

unique_ptr<Stmt> Parser::start(){
    vector<unique_ptr<Stmt>> Statements;
    while(!isAtEnd()) Statements.push_back(statement());
    
    return make_unique<BlockStmt>(move(Statements));
}

// -------------------------------------------------
// Statements

unique_ptr<Stmt> Parser::blockSt(){
    consume(TokenType::LEFT_BRACE, "Expected {");

    vector<unique_ptr<Stmt>> Statements;
    while(!isAtEnd() && !check(TokenType::RIGHT_BRACE)) Statements.push_back(statement());

    consume(TokenType::RIGHT_BRACE, "Expected }");

    return make_unique<BlockStmt>(move(Statements));
}

unique_ptr<Stmt> Parser::statement(){
    if(check(TokenType::LEFT_BRACE)) return blockSt();
    else if(check(TokenType::KW_PRINT)) return printSt();
    else if(check(TokenType::KW_LOOP)) return loopSt();
    else if(check(TokenType::KW_IF)) return ifelseSt();
    else if(check(TokenType::KW_INT) || check(TokenType::KW_BOOL)) return varDecSt();
    else return expressionSt();
}

unique_ptr<Stmt> Parser::printSt(){
    consume(TokenType::KW_PRINT, "Expected 'print' here");
    consume(TokenType::LEFT_PAREN, "Expected ( here");

    unique_ptr<Expr> exp = expression();

    consume(TokenType::RIGHT_PAREN, "Expected ) here");
    consume(TokenType::SEMICOLON, "Expected ; here");

    unique_ptr<Stmt> st = make_unique<PrintStmt>(move(exp));

    return st;
}

unique_ptr<Stmt> Parser::expressionSt(){
    cout << "Expression St" << endl;
    
    unique_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression");

    return make_unique<ExprStmt>(move(expr));
}

unique_ptr<Stmt> Parser::varDecSt(){
    TokenType type;
    if(peek().type == TokenType::KW_BOOL) type = consume(TokenType::KW_BOOL, "Type missing").type;
    else type = consume(TokenType::KW_INT, "Type missing").type;

    string var = consume(TokenType::IDENTIFIER, "Missing Identifier").lexeme;
    
    unique_ptr<Expr> exp = nullptr;
    if(match(TokenType::EQUAL)) exp = expression();

    consume(TokenType::SEMICOLON, "Expected ; here");

    return make_unique<VarDecStmt>(type, var, move(exp));
}

unique_ptr<Stmt> Parser::loopSt(){
    consume(TokenType::KW_LOOP, "Expected 'loop' keyword here");

    consume(TokenType::LEFT_PAREN, "Expected ( here");
    unique_ptr<Expr> con = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ) here");

    unique_ptr<Stmt> blk = statement();

    return make_unique<LoopStmt>(move(con), move(blk));
}

unique_ptr<Stmt> Parser::ifelseSt(){
    consume(TokenType::KW_IF, "Expected 'if' keyword here");

    consume(TokenType::LEFT_PAREN, "Expected ( here");
    unique_ptr<Expr> ifCon = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ) here");

    unique_ptr<Stmt> ifBlk = statement();
    unique_ptr<Stmt> elseBlk = nullptr;
    if(match(TokenType::KW_ELSE)) elseBlk = statement();

    return make_unique<IfStmt>(move(ifCon), move(ifBlk), move(elseBlk));
}


// -------------------------------------------------
// Expressions 
// expression -> equality (== or !=)
// equality -> comparison (< or >)
// comparison -> term (+ -)
// term -> factor (* /)
// factor -> unary (- !)
// unary -> primary (IDENTIFIER | NUMBER | EXPRESION)

unique_ptr<Expr> Parser::expression(){
    return equality();
}

unique_ptr<Expr> Parser::equality(){
    unique_ptr<Expr> left = comparison();

    while(match(TokenType::BANG_EQUAL) || match(TokenType::EQUAL_EQUAL)){
        TokenType ops = previous().type;
        unique_ptr<Expr> right = comparison();
        left = make_unique<BinaryExpr>(move(left), ops, move(right));
    };

    return left;
}

unique_ptr<Expr> Parser::comparison(){
    unique_ptr<Expr> left = term();

    while(match(TokenType::LESS) || match(TokenType::GREATER)){
        TokenType ops = previous().type;
        unique_ptr<Expr> right = term();
        left = make_unique<BinaryExpr>(move(left), ops, move(right));
    };

    return left;
}

unique_ptr<Expr> Parser::term(){
    unique_ptr<Expr> left = factor();

    while(match(TokenType::PLUS) || match(TokenType::MINUS)){
        TokenType ops = previous().type;
        unique_ptr<Expr> right = factor();
        left = make_unique<BinaryExpr>(move(left), ops, move(right));
    };

    return left;
}

unique_ptr<Expr> Parser::factor(){
    unique_ptr<Expr> left = unary();

    while(match(TokenType::STAR) || match(TokenType::SLASH)){
        TokenType ops = previous().type;
        unique_ptr<Expr> right = unary();
        left = make_unique<BinaryExpr>(move(left), ops, move(right));
    };

    return left;
}

unique_ptr<Expr> Parser::unary(){
    if(match(TokenType::MINUS)){
        TokenType ops = previous().type;
        unique_ptr<Expr> right = unary();
        return make_unique<UnaryExpr>(ops, move(right));
    }

    return primary();
}

unique_ptr<Expr> Parser::primary(){
    if(match(TokenType::NUMBER)) return make_unique<LiteralExpr>(previous().lexeme);

    if(match(TokenType::KW_TRUE)) return make_unique<LiteralExpr>("true");

    if(match(TokenType::KW_FALSE)) return make_unique<LiteralExpr>("false");

    if(match(TokenType::IDENTIFIER)) return make_unique<VariableExpr>(previous().lexeme);

    if(match(TokenType::LEFT_PAREN)){
        unique_ptr<Expr> exp = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ) here");
        return exp;
    }

    throw runtime_error(errorMsg("Expression error"));
}

// -------------------------------------------------
// print functions 

string tokenToString(TokenType t){
    if(t == TokenType::MINUS) return "-";
    else if(t == TokenType::PLUS) return "+";
    else if(t == TokenType::SLASH) return "/";
    else if(t == TokenType::STAR) return "*";
    else if(t == TokenType::EQUAL) return "=";
    else if(t == TokenType::EQUAL_EQUAL) return "==";
    else if(t == TokenType::BANG_EQUAL) return "!=";
    else if(t == TokenType::KW_BOOL) return "bool";
    else if(t == TokenType::KW_INT) return "int";
    else if(t == TokenType::GREATER) return ">";
    else if(t == TokenType::LESS) return "<";

    else return " ";
}

void LiteralExpr::print(int indent) const {
        cout << string(indent, ' ') << "Literal Ex : " << value << endl;
}

void VariableExpr::print(int indent) const {
        cout << string(indent, ' ') << "Variable Ex : " << value << endl;
}

void UnaryExpr::print(int indent) const { 
    cout << string(indent, ' ') << "Unary Ex : " << tokenToString(ops) << endl;
    value->print(indent+2);
}

void BinaryExpr::print(int indent) const {
    cout << string(indent, ' ') << "Binary Ex : " << tokenToString(ops) << endl;
    left->print(indent+2);
    right->print(indent+2);
}

void BreakStmt::print(int indent) const {
    cout << string(indent, ' ') << "Break St" << endl;
}

void ExprStmt::print(int indent) const {
    cout << string(indent, ' ') << "Expr St" << endl;
    expression->print(indent+2);
}

void VarDecStmt::print(int indent) const {
    cout << string(indent, ' ') << "VarDec St : " << tokenToString(type) << " " << name << endl;
    if(initializer) initializer->print(indent+2);
}

void PrintStmt::print(int indent) const {
    cout << string(indent, ' ') << "Print St : " << endl;
    expression->print(indent+2);
}

void BlockStmt::print(int indent) const {
    cout << string(indent, ' ') << "Block St : " << endl;
    for(const auto& st : statements) st->print(indent+2);
}

void IfStmt::print(int indent) const {
    cout << string(indent, ' ') << "IfElse St : " << endl;

    cout << string (indent+2, ' ') << "Condition : " << endl;
    condition->print(indent+4);

    cout << string (indent+2, ' ') << "If part : " << endl;
    thenBranch->print(indent+4);

    if(elseBranch){
        cout << string (indent+2, ' ') << "Else Part : " << endl;
        elseBranch->print(indent+4);
    }
}

void LoopStmt::print(int indent) const {
    cout << string(indent, ' ') << "Loop St : " << endl;

    cout << string (indent+2, ' ') << "Condition : " << endl;
    condition->print(indent+4);

    body->print(indent + 2);
}

// -------------------------------------------------