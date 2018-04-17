#pragma once

#include "ExprAST.h"
#include <string>
#include <memory>

using namespace std;

class CallExprAST : public ExprAST {
    string Callee;
    vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const string &callee, vector<std::unique_ptr<ExprAST>> Args)
            : Callee(callee), Args(std::move(Args)) {}

};