#pragma once

#include "PrototypeAST.h"
#include "ExprAST.h"

class FunctionAST {
    PrototypeAST *Proto;
    ExprAST *Body;

public:
    FunctionAST(PrototypeAST *proto, ExprAST *body)
            : Proto(proto), Body(body) {}
};