#pragma once

#include "LLVMBinder.h"
#include "ExprAST.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Value.h"

// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
    double Val;
public:
    NumberExprAST(double val) : Val(val) {}

    // Used for generating LLVM IR;
    llvm::Value *codegen() override;
};

