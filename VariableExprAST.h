#pragma once

#include <string>
#include "ExprAST.h"


using namespace std;

class VariableExprAST: public  ExprAST {
    string Name;
public:
    VariableExprAST(const string &name): Name(name) {}
};