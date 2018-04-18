#pragma once

#include <map>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"


class LLVMBinder {

public:
    // Symbol table;
    static std::map<std::string, llvm::Value*> NamedValues;

    // Own core LLVM data structures, opaque value;
    static llvm::LLVMContext TheContext;

    // Used for generating LLVM instructions;
    static llvm::IRBuilder<> Builder;

    // Hold global functions and variables;
    static std::unique_ptr<llvm::Module> TheModule;

};