#pragma once

#include "ExprAST.h"

class BinaryExprAST: public ExprAST {
    char Op;
    ExprAST *LHS, *RHS;

public:
    BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs)
            : Op(op), LHS(lhs), RHS(rhs) {}
};