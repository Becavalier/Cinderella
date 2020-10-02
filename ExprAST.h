#pragma once

#include <cstdio>
#include <memory>
#include "llvm/IR/Value.h"
#include "PrototypeAST.h"


struct ExprAST {
    virtual ~ExprAST() = default;

    // Pure function, used for generating LLVM IR;
    virtual llvm::Value *codegen() = 0;


    // Error handling methods;
    static std::unique_ptr<ExprAST> LogError(const char *Str) {
        fprintf(stderr, "LogError: %s\n", Str);
        return nullptr;
    }

    static llvm::Value *LogErrorV (const char *Str) {
        LogError(Str);
        return nullptr;
    }

    static std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
        LogError(Str);
        return nullptr;
    }
};