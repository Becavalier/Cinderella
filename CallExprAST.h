#pragma once

#include "LLVMBinder.h"
#include "ExprAST.h"
#include <string>
#include <memory>
#include <vector>
#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"


class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;
public:
    CallExprAST(const std::string &callee, std::vector<std::unique_ptr<ExprAST>> Args)
        : Callee(callee), Args(std::move(Args)) {}

    // Used for generating LLVM IR;
    llvm::Value *codegen() override;
};