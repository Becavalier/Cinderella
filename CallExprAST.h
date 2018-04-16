#pragma once

#include "ExprAST.h"
#include <string>

using namespace std;

class CallExprAST : public ExprAST {
    string Callee;
    vector<ExprAST*> Args;

public:
    CallExprAST(const string &callee, vector<ExprAST*> &args)
            : Callee(callee), Args(args) {}

};