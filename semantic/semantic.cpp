#include <iostream>
#include "semantic.h"
using namespace std;

// -----------------------------------------
// helper functions

void Semantic::enterBlock(){ 
    scopes.push_back({}); 
}
void Semantic::exitBlock(){ 
    scopes.pop_back();
}

void Semantic::enterLoop(){ loopDepth++; }
void Semantic::exitLoop(){ loopDepth--; }

void Semantic::newVar(string name, TokenType type){
    if(scopes.back().count(name)) throw runtime_error("Semantic Error");
    else scopes.back()[name] = type;
}

TokenType Semantic::getType(string name){
    for(int i = scopes.size()-1; i >= 0; i--){
        if(scopes[i].count(name)) return scopes[i][name];
    }
    throw runtime_error("Semantic Error : Variable not Declared");
}

bool Semantic::checkType(TokenType expected, TokenType x){
    return expected == x;
}

// -----------------------------------------

void Semantic::analyzeStmt(Stmt* st){
    if(auto block = dynamic_cast<BlockStmt*>(st)){
        enterBlock();
        for(const auto& b : block->statements) analyzeStmt(b.get());
        exitBlock();
    }
    else if (auto brk = dynamic_cast<BreakStmt*>(st)){
        if(loopDepth == 0) throw runtime_error("Semantic Error");
    }
    else if(auto ifSt = dynamic_cast<IfStmt*>(st)){
        if(!checkType(TokenType::KW_BOOL, analyzeExpr(ifSt->condition.get())))
            throw runtime_error("Semantic Error");
        analyzeStmt(ifSt->thenBranch.get());
        if(ifSt->elseBranch.get()) analyzeStmt(ifSt->elseBranch.get());
    }
    else if(auto loop = dynamic_cast<LoopStmt*>(st)){
        if(!checkType(TokenType::KW_BOOL, analyzeExpr(loop->condition.get())))
            throw runtime_error("Semantic Error");
        enterLoop();
        analyzeStmt(loop->body.get());
        exitLoop();
    }
    else if(auto ex = dynamic_cast<ExprStmt*>(st)){
        analyzeExpr(ex->expression.get());
    }
    else if(auto pr = dynamic_cast<PrintStmt*>(st)){
        analyzeExpr(pr->expression.get());
    }
    else if(auto vr = dynamic_cast<VarDecStmt*>(st)){
        newVar(vr->name, vr->type);
        if(vr->initializer){
            if(!checkType(vr->type, analyzeExpr(vr->initializer.get())))
                throw runtime_error("Semantic Error");
        }
    }
    else throw runtime_error("Semantic Error");
}

// -----------------------------------------

TokenType Semantic::analyzeExpr(Expr* ex){
    if(auto expr = dynamic_cast<VariableExpr*>(ex)){
        return getType(expr->value);
    }
    else if(auto expr = dynamic_cast<LiteralExpr*>(ex)){
        if(expr->value == "true" || expr->value == "false")
            return TokenType::KW_BOOL;
        return TokenType::KW_INT;
    }
    else if(auto expr = dynamic_cast<BinaryExpr*>(ex)){
        TokenType left = analyzeExpr(expr->left.get());
        TokenType right = analyzeExpr(expr->right.get());
        
        if(expr->ops == TokenType::LESS || expr->ops == TokenType::GREATER){
            if(left != TokenType::KW_INT || right != TokenType::KW_INT)
                throw runtime_error("Semantic Error");
            return TokenType::KW_BOOL;
        }
        else if(expr->ops == TokenType::EQUAL_EQUAL || expr->ops == TokenType::BANG_EQUAL ){
            if(left != right)
                throw runtime_error("Semantic Error");
            return TokenType::KW_BOOL;
        }
        else if( expr->ops == TokenType::PLUS ||
                expr->ops == TokenType::MINUS ||
                expr->ops == TokenType::STAR ||
                expr->ops == TokenType::SLASH )
        {
            if(left != TokenType::KW_INT || right != TokenType::KW_INT)
                throw runtime_error("Semantic Error");
            return TokenType::KW_INT;
        }
        else throw runtime_error("Semantic Error");
    }
    else if(auto expr = dynamic_cast<UnaryExpr*>(ex)){
        TokenType t = analyzeExpr(expr->value.get());
        if(expr->ops == TokenType::MINUS){
            if(t != TokenType::KW_INT)
                throw runtime_error("Semantic Error");
            return TokenType::KW_INT;
        }
        throw runtime_error("Semantic Error");
    }
    else throw runtime_error("Semantic Error");
}

// -----------------------------------------