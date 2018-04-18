#pragma once

#include "LLVMBinder.h"
#include "ExprAST.h"
#include <memory>
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"


class BinaryExprAST: public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
            : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    // Used for generating LLVM IR;
    llvm::Value *codegen() override;
};