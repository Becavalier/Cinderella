#pragma once

#include "LLVMBinder.h"
#include "PrototypeAST.h"
#include "ExprAST.h"
#include <memory>
#include <iostream>
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Verifier.h"

class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;
public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}

    // Used for generating LLVM IR;
    llvm::Function *codegen();
};