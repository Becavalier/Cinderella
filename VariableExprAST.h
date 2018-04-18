#pragma once

#include "LLVMBinder.h"
#include <string>
#include "ExprAST.h"
#include "llvm/IR/Value.h"

class VariableExprAST: public ExprAST {
    std::string Name;

public:
    VariableExprAST(const std::string &name)
            : Name(name) {}

    // Used for generating LLVM IR;
    llvm::Value *codegen() override;
};