#include <iostream>
#include "interpreter.h"
using namespace std;

// ---------------------------------
// helper function

void Interpreter::enterBlock(){
    scopes.push_back({});
}

void Interpreter::exitBlock(){
    scopes.pop_back();
}

int Interpreter::find(string name){
    for(int i = scopes.size()-1; i >= 0; i--){
        if(scopes[i].count(name)) return scopes[i][name];
    }

    throw runtime_error("Interpreter Error");
}

void Interpreter::run(Stmt* program){
    execute(program);
}

// ---------------------------------

void Interpreter::execute(Stmt* st){
    if(auto stmt = dynamic_cast<BlockStmt*>(st)){
        enterBlock();
        try {
            for(auto& s : stmt->statements) execute(s.get());
        }
        catch(BreakException&) {
            exitBlock();
            throw;
        }
        exitBlock();
    }
    else if(auto stmt = dynamic_cast<PrintStmt*>(st)){
        cout << evaluate(stmt->expression.get()) << endl;
    }
    else if(auto stmt = dynamic_cast<VarDecStmt*>(st)){
        int val = 0;
        if(stmt->initializer) val = evaluate(stmt->initializer.get());
        scopes.back()[stmt->name] = val;      
    }
    else if(auto stmt = dynamic_cast<LoopStmt*>(st)){
        try{
            while(evaluate(stmt->condition.get())){
                execute(stmt->body.get());
            }
        } catch (BreakException&){
            // Exit loop
        }
    }
    else if(auto stmt = dynamic_cast<IfStmt*>(st)){
        if(evaluate(stmt->condition.get())){
            execute(stmt->thenBranch.get());
        }
        else if (stmt->elseBranch) {
            execute(stmt->elseBranch.get());
        }
    }
    else if(auto stmt = dynamic_cast<ExprStmt*>(st)){
        evaluate(stmt->expression.get());
    }
    else if(auto stmt = dynamic_cast<BreakStmt*>(st)){
        throw BreakException();
    }
}

// ---------------------------------

int Interpreter::evaluate(Expr* ex){
    if(auto expr = dynamic_cast<AssignExpr*>(ex)){
        for(int i = scopes.size()-1; i >= 0; i--){
            if(scopes[i].count(expr->name)){
                int value = evaluate(expr->expression.get());
                scopes[i][expr->name] = value;
                return value;
            }
        }
        throw runtime_error("Interpreter Error");
    }
    else if(auto expr = dynamic_cast<BinaryExpr*>(ex)){
        int left = evaluate(expr->left.get());
        int right = evaluate(expr->right.get());
        if(expr->ops == TokenType::BANG_EQUAL) return left != right; 
        else if(expr->ops == TokenType::EQUAL_EQUAL) return left == right; 
        else if(expr->ops == TokenType::GREATER) return left > right; 
        else if(expr->ops == TokenType::LESS) return left < right; 
        else if(expr->ops == TokenType::PLUS) return left + right; 
        else if(expr->ops == TokenType::MINUS) return left - right; 
        else if(expr->ops == TokenType::STAR) return left * right; 
        else if(expr->ops == TokenType::SLASH){ 
            if(right == 0) throw runtime_error("Division by zero");
            return left / right; 
        }
        else throw runtime_error("Interpreter Error");
    }
    else if(auto expr = dynamic_cast<UnaryExpr*>(ex)){
        return -1 * evaluate(expr->value.get());
    }
    else if(auto expr = dynamic_cast<LiteralExpr*>(ex)){
        if(expr->value == "false") return 0;
        if(expr->value == "true") return 1;
        return stoi(expr->value);
    }
    else if(auto expr = dynamic_cast<VariableExpr*>(ex)){
        return find(expr->value);
    }
    else throw runtime_error("Interpreter error");
}

// ---------------------------------