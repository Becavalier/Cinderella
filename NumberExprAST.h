#pragma once

#include "ExprAST.h"
#include "llvm/IR/Constants.h"

// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double val) : Val(val) {}
};

